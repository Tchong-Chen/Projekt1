#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


// def constantes modifiable

#define NBR_OPS 5                  // Nombre d'operateur
#define NBR_HEURES 11              // Duree de la journee
#define NBR_JOUR 1                 // Nombre de jour de la simulation
#define minsrv 100                 // Duree min du service
#define maxsrv 600                 // Duree max du service
#define lambda 0.06                // lambda
#define NOM_FIC_DONNES "Data.txt"  // Nom du fichier


// Structure et liste chainee

struct Client {
    int h_arrivee;
    int debut_prise_en_charge;
    int fin_prise_en_charge;
    int duree_attente;
    int jour;
} ;

typedef struct T_noeud {
    struct Client data;
    struct T_noeud *suiv;
} T_noeud;


//Fonctions utilise dans le programme  et leur role:

double intervalle_tps(void);                                // Renvoie la duree entre l'appel de 2 clients
int* simu_arrivee(void);                                    // Cree et renvoie un tableau contenant les heures d'arrive des differents clients
bool compare_tps(int j, int *tab);                          // comparaison entre tableau de simu_arrivee et l'heure actuelle
int tps_prise_en_charge(void);                              // Renvoie la duree de prise en charge, (choisit au hasard entre minsrv et maxsrv)
void ajout_client(T_noeud *file, struct Client nv_client);  // Ajoute un client a la liste chainee
T_noeud pop_client(T_noeud *tete);                           // Retire un client et renvoie les donnes du clients (Heure_prise en charge,Duree attente ....)
int compte_client(T_noeud *tete);                           // Compte le nombre de clients present dans la file ??? File d'attente 
int* ops_libre(int* tableau);                               // Prend un tableau contenant les operateurs, et determine si ils sont libres ou pas
int convertisseur_tps(int n);                               // Transforme un entier en seconde et renvoie , l'heure, la minute et la seconde.
void ecrireFicClients(T_noeud *Donnes, FILE *fp);           // Ecrit les donnes dans un fichier


// Intervalle de temps separant deux clients

double intervalle_tps(void)
{
    double U = (double) rand() / RAND_MAX;
    double X = -logf(1.0 - U) / lambda;

    return ceil(X);
}


// Simulation tps d'arrivee des clients

int* simu_arrivee(void)
{
    int i = 0;
    int* tableau_tps_arrivee = malloc(10000 * sizeof(int));
    tableau_tps_arrivee[i] = intervalle_tps();

    while(tableau_tps_arrivee[i] < 24 * 3600)
    {
        i++;
        int tps = intervalle_tps();
        tableau_tps_arrivee[i] = tableau_tps_arrivee[i - 1] + tps;
    }
    return tableau_tps_arrivee;
}

// Comparaison entre tableau de simu_arrivee et l'heure actuelle

bool compare_tps(int j, int *tab)
{
    int i = 0;
    while( tab[i] < j)
        i++;
    return (tab[i] == j);
}


// tps de prise en charge client

int tps_prise_en_charge(void)
{
    int tps_avec_client = rand() % maxsrv + minsrv;
    return tps_avec_client;
}



// ajout d'un client dans la file

void ajout_client(T_noeud *file, struct Client nv_client)
{
    T_noeud *nouveau;

    nouveau = ((T_noeud *) malloc(sizeof(T_noeud)));

    nouveau -> data = nv_client;
    nouveau -> suiv = file;
    file = nouveau;
}


// retirer un client de la file

Client pop_client(T_noeud *tete) 
{
    if ( tete == NULL)
        return;

    if ((tete) -> suiv == NULL) 
    {
        int *sauvegarde = (tete) -> data;
        free(tete);
        tete = NULL;
        return sauvegarde;
    }

    T_noeud *courant = tete;
    while (courant -> (suiv -> suiv) != NULL) 
        courant = courant -> suiv;

    T_noeud *dernier = courant -> suiv ;
    int *sauvegarde = dernier -> data ;
    courant -> suiv = NULL;

    free(dernier);
    return sauvegarde->data ;
}


// comptage du nbr de clients dans la file

int compte_client(T_noeud *tete)
{
    int compteur = 0;
    T_noeud *courant = tete;

    while(courant -> suiv != NULL)
    {
        compteur++;
        courant = courant -> suiv;
    }
    return compteur;
}


// Verifie si un operateur est libre

int* ops_libre(int* tableau)
{
    int* libre = malloc(NBR_OPS * sizeof(int));
    
    for(int i = 0; i < NBR_OPS; i++)
    {
        if (tableau[i] > 0)
            libre[i] = 1;
        else 
            libre[i] = 0;
    }
    return libre;
}

// Converti un temp en seconde en heure/minute/seconde
int convertisseur_tps(int n)
{
    int heures, minutes, secondes;
    heures = n / 3600;
    n %= 3600;
    minutes = n / 60;
    secondes = n % 60;
    return heures, minutes, secondes;
}


void ecrireFicClients(T_noeud *Donnes, FILE *fp)
{
  T_noeud *Nouveau;

  fp = fopen(NOM_FIC_DONNES, "a");

  Nouveau = Donnes->suiv;
  while (Nouveau != NULL)
  {
    fprintf(fp, "%d %d %d %d %d\n", Nouveau->data.jour, Nouveau->data.h_arrivee, Nouveau->data.duree_attente, Nouveau->data.debut_prise_en_charge, Nouveau->data.fin_prise_en_charge );
    Nouveau = Nouveau->suiv;
  }
}



int main(void)
{
    srand(time(NULL));  // initialise pour l'utilisation de random
    
    T_noeud *file_attente;
    file_attente -> suiv = NULL;    //Creation de la file d'attente sous forme de liste chainee


    FILE *fp;
    fp = fopen(NOM_FIC_DONNES, "w");  //On initialise le fichier

    // Initialisation des tableaux & variables pour etude de performance 
    int* compteur_file_attente[NBR_HEURES * 3600] = {0};
    int max_file_attente ;
    int min_file_attente ;
    float moy_file_attente = 0.0 ;

    float debit_journalier_moyen ;          // Nombre moyen d'appel pris en charge par 
    float nbr_client_par_jour = 0.0 ;       // On utilise un float pour eviter probleme de type lors d'un calcul plus tard
    float taux_client_pris_en_charge ;
    int h_fin_de_service ;

    // creation centre appel ; un tableau contenant les operateur, >= 1 si ils sont occupe, 0 si ils sont libres

    int simu_centre_appel[NBR_OPS] = {0};
// Simulation
    for(int i = 0; i <= NBR_JOUR; i++)
    {   
        
        // tableau contenant l'heure d'arrivee des clients
        int *heure_arrivee = simu_arrivee();

        for(int j = 0; j <= 24 * 3600; i++)
        {
            // regarde si un client est arrive
            if (compare_tps(j, heure_arrivee) && j <= NBR_HEURES * 3600)   // Faut que le client arrive dans une des heure travailles
            {
                struct Client client;
                client.h_arrivee = j;
                ajout_client(file_attente, client);
            }

            // regarde s'il y a des gens dans la file d'attente & si un operateur est libre
            // si oui, enregistre son tps de debut de prise en charge + genere une duree aleatoire de conversation client/ops
            int compteur = compte_client(file_attente);
            int *libre = ops_libre(simu_centre_appel); // renvoie un tableau avec 0 si l'operateur est libre
            for(int k = 0; k < NBR_OPS; k++)
            {
                if ((libre[k] == 0) && (compteur >= 1))
                {
                    compteur = compteur - 1;
                    T_noeud client = pop_client(*file_attente);
                    //Donnes a rajouter
                    client.debut_prise_en_charge = j;
                    simu_centre_appel[k] = tps_prise_en_charge();
                }
            }
            
            // simule la seconde passee
            for(int l = 0; l < NBR_OPS; l++)  //Si 
            {
                if (simu_centre_appel[l] > 0)
                    simu_centre_appel[l] = simu_centre_appel[l] - 1;
            }

            compteur_file_attente[i] = compte_client(file_attente);

            if (compte_client(file_attente) == 0)
            {
                int ok = 0;
                while(simu_centre_appel[ok] == 0)
                    ok++;
                if (ok == NBR_OPS)
                    h_fin_de_service = j;
            }
        }
//Traitement des donnes
        for(int i = 0; i < NBR_HEURES * 3600; i++)
        {
            moy_file_attente = moy_file_attente + compteur_file_attente[i];
            if (compteur_file_attente[i] < min_file_attente)
                min_file_attente = &compteur_file_attente[i];
            else if (compteur_file_attente[i] > max_file_attente)
                max_file_attente = &compteur_file_attente[i];
        }
        moy_file_attente = moy_file_attente / (NBR_HEURES * 3600.0);
        moy_file_attente = ceil(moy_file_attente); //arrondi la moyenne

        for(int i = 0; i < 10000; i++)
        {
            if (heure_arrivee[i] <= 24 * 3600)
                nbr_client_par_jour++;
                if (heure_arrivee[i] <= NBR_HEURES * 3600)
                    debit_journalier_moyen++;
                    taux_client_pris_en_charge++;
            
        }
        debit_journalier_moyen = debit_journalier_moyen / (NBR_HEURES * 3600.0);
        taux_client_pris_en_charge = taux_client_pris_en_charge / nbr_client_par_jour;
        int heures, minutes, secondes = convertisseur_tps(h_fin_de_service);

        // Affichage des performances de la journee
        printf("Debit journalier moyen : %d \n", debit_journalier_moyen);
        printf("Taux client pris en charge : %3f \n", taux_client_pris_en_charge);
        printf("Taille de la file d'attente : \n min : %d  max = %d  moy = %1f \n", min_file_attente, max_file_attente, moy_file_attente);
        printf("Heure de fin de servide :     %d : %d : %d \n", heures, minutes, secondes);

    }
    fclose(fp); //On ferme le fichier
    return 0;
}