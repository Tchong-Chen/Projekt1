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
struct Client pop_client(T_noeud *tete);                           // Retire un client et renvoie les donnes du clients (Heure_prise en charge,Duree attente ....)
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

struct Client pop_client(T_noeud *tete) 
{
    T_noeud *nouveau = tete;

    if (nouveau == NULL)
    {
        struct Client sauvegarde;
        sauvegarde.h_arrivee = -1;
        return sauvegarde;
    }

    if (nouveau -> suiv == NULL) 
    {
        struct Client sauvegarde = (tete) -> data;
        free(tete);
        tete = NULL;
        return sauvegarde;
    }

    T_noeud *courant = tete;
    while (courant -> suiv -> suiv != NULL) 
        courant = courant -> suiv;

    T_noeud *dernier = courant -> suiv ;
    struct Client sauvegarde = dernier -> data ;
    courant -> suiv = NULL;

    free(dernier);
    return sauvegarde ;
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


void ecrireFicClients(T_noeud *Donnees, FILE *fp)
{
  T_noeud *Nouveau;

  fp = fopen(NOM_FIC_DONNES, "a");

  Nouveau = Donnees->suiv;
  while (Nouveau != NULL)
  {
    fprintf(fp, "%d %d %d %d %d\n", Nouveau->data.jour, Nouveau->data.h_arrivee, Nouveau->data.duree_attente, Nouveau->data.debut_prise_en_charge, Nouveau->data.fin_prise_en_charge );
    Nouveau = Nouveau->suiv;
  }
}


int tps_attente(T_noeud *Donnees)
{
    T_noeud *courant = Donnees;
    int min_attente = courant -> data.duree_attente;
    int max_attente = courant -> data.duree_attente;
    int moy_attente;
    int tps_rep_moy;
    float compteur = 0.0;

    while(courant -> suiv != NULL)
    {
        moy_attente = moy_attente + courant -> data.duree_attente;
        tps_rep_moy = tps_rep_moy + (courant -> data.fin_prise_en_charge - courant -> data.h_arrivee);

        if (courant -> data.duree_attente < min_attente)
            min_attente = courant -> data.duree_attente;
        if (courant -> data.duree_attente < max_attente)
            max_attente = courant -> data.duree_attente;
        
        compteur++;
        courant = courant -> suiv;
    }
    return min_attente, max_attente, ceil(moy_attente / compteur), ceil(tps_rep_moy / compteur);
}


int main(void)
{
    srand(time(NULL));
    
    T_noeud *file_attente;
    file_attente -> suiv = NULL;    // Creation de la file d'attente sous forme de liste chainee

    T_noeud *Donnees;
    Donnees -> suiv = NULL;         // Creation d'une file ou seront stockes les clients

    FILE *fp;
    fp = fopen(NOM_FIC_DONNES, "w");  //Initialisation du fichier

    // Initialisation des tableaux & variables pour etude de performance 
    int moy_file_attente;
    int max_file_attente = 0; // On initialise a des valeurs random afin de pouvoir les comparer
    int min_file_attente = 5;

    float debit_journalier_moyen;
    float nbr_client_par_jour = 0.0;       // On utilise un float pour eviter probleme de type lors d'un calcul plus tard
    float taux_client_pris_en_charge;
    int h_fin_de_service;

    // Creation centre appel ; un tableau contenant les operateur, >= 1 s'ils sont occupes, 0 s'ils sont libres
    int simu_centre_appel[NBR_OPS] = {0};

    // Simulation
    for(int i = 0; i <= NBR_JOUR; i++)
    {   
        
        // tableau contenant l'heure d'arrivee des clients
        int *heure_arrivee = simu_arrivee();

        for(int j = 0; j <= 24 * 3600; i++)
        {
            // regarde si un client est arrive
            if (compare_tps(j, heure_arrivee) && j <= NBR_HEURES * 3600)
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
                    struct Client client = pop_client(file_attente);

                    //Donnees a ajouter
                    client.debut_prise_en_charge = j;
                    int tps_conversation = tps_prise_en_charge();
                    simu_centre_appel[k] = tps_conversation;
                    client.duree_attente = j - client.h_arrivee;
                    client.fin_prise_en_charge = j + tps_conversation;
                    client.jour = i;
                    ajout_client(Donnees, client);                              // On enregistre les differentes donnes dans client puis on l'ajoute a la liste des donnes.
                }
            }
            
            // simule la seconde passee
            for(int l = 0; l < NBR_OPS; l++)  //Si 
            {
                if (simu_centre_appel[l] > 0)
                    simu_centre_appel[l] = simu_centre_appel[l] - 1;
            }

            moy_file_attente = moy_file_attente + compteur;
            if (compteur < min_file_attente)
                min_file_attente = compteur;
            if (compteur > max_file_attente)
                max_file_attente = compteur;

            if (compte_client(file_attente) == 0)
            {
                int ok = 0;
                while(simu_centre_appel[ok] == 0)
                    ok++;
                if (ok == NBR_OPS)
                    if (j < NBR_HEURES * 3600)
                        h_fin_de_service = NBR_HEURES * 3600;
                    else 
                        h_fin_de_service = j;
            }
        }

// Traitement des donnes
        moy_file_attente = moy_file_attente / (NBR_HEURES * 3600.0);
        moy_file_attente = ceil(moy_file_attente); //arrondit a l'entier superieur

        for(int n = 0; n < 10000; n++)
        {
            if (heure_arrivee[n] <= 24 * 3600)
                nbr_client_par_jour++;
                if (heure_arrivee[n] <= NBR_HEURES * 3600)
                    debit_journalier_moyen++;
                    taux_client_pris_en_charge++;
            
        }

        debit_journalier_moyen = debit_journalier_moyen / (NBR_HEURES * 3600.0);
        taux_client_pris_en_charge = taux_client_pris_en_charge / nbr_client_par_jour;
        int heures, minutes, secondes = convertisseur_tps(h_fin_de_service);
        int min_attente, max_attente, moy_attente, tps_rep_moy = tps_attente(Donnees);

        // Affichage des performances de la journee
        printf("Pour le jour : %d , On a : \n\n", i);
        printf("Debit journalier moyen : %1f \n", debit_journalier_moyen);
        printf("Taux client pris en charge : %3f \n", taux_client_pris_en_charge);
        printf("Taille de la file d'attente : \n min : %d  max = %d  moy = %d \n", min_file_attente, max_file_attente, moy_file_attente);
        printf("Heure de fin de servide :     %d : %d : %d \n", heures, minutes, secondes);
        printf("Temps moyen d'attente (en seconde)  :\n min : %d  max = %d  moy = %d \n", min_attente, max_attente, moy_attente);
        printf("Temps de reponse moyen en seconde : %d \n", tps_rep_moy);


    }
    ecrireFicClients(Donnees, fp);
    fclose(fp);
    return 0;
}