#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


// def constantes modifiable

#define NBR_OPS 5                  // Nombre d'operateur
#define NBR_HEURES 11              // Duree de la journee
#define NBR_JOUR 2                 // Nombre de jour de la simulation
#define minsrv 100                 // Duree min du service
#define maxsrv 600                 // Duree max du service
#define lambda 0.001                // lambda
#define NOM_FIC_DONNES "Data.txt"  // Nom du fichier
#define HEURE_DEB 8
#define HEURE_FIN 19

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

double intervalle_tps(void);                                                // Renvoie la duree entre l'appel de 2 clients successifs
int* simu_arrivee(void);                                                    // Cree et renvoie un tableau contenant les heures d'arrivee des differents clients
bool compare_tps(int j, int *tab);                                          // Comparaison entre tableau de simu_arrivee et l'heure actuelle
int tps_prise_en_charge(void);                                              // Renvoie la duree de prise en charge, (choisit au hasard entre minsrv et maxsrv)
void ajout_client(T_noeud **file, struct Client nv_client);                 // Ajoute un client a la liste chainee
struct Client pop_client(T_noeud **tete);                                   // Retire un client et renvoie les donnees du clients (Heure_prise en charge,Duree attente ....)
int compte_client(T_noeud *tete);                                           // Compte le nombre de clients presents dans la file d'attente 
int* ops_libre(int* tableau);                                               // Prend un tableau contenant les operateurs, et determine s'ils sont libres ou pas
void convertisseur_tps(int n, int *heures, int *minutes, int *secondes);    // Transforme un entier en seconde et renvoie , l'heure, la minute et la seconde.
void ecrireFicClients(T_noeud *Donnes, FILE *fp);                           // Ecrit les donnes dans un fichier


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
    free(tableau_tps_arrivee);
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
    int tps_avec_client = rand() % (maxsrv - minsrv + 1) + minsrv;
    return tps_avec_client;
}



// ajout d'un client dans la file

void ajout_client(T_noeud **file, struct Client nv_client) 
{
    T_noeud *nouveau = (T_noeud *) malloc(sizeof(T_noeud));

    nouveau -> data = nv_client;
    nouveau -> suiv = NULL;

    if (*file == NULL) 
        *file = nouveau;
    else 
    {
        T_noeud *courant = *file;
        while (courant -> suiv != NULL)
            courant = courant -> suiv;
        courant -> suiv = nouveau;
    }
}

// retirer un client de la file

struct Client pop_client(T_noeud **tete) 
{
    T_noeud *courant = *tete;
    
    if (courant == NULL) 
    {
        struct Client sauvegarde;
        sauvegarde.h_arrivee = -1;  // Valeur d'indication d'erreur
        return sauvegarde;
    }

    struct Client sauvegarde = courant -> data; 
    *tete = courant -> suiv;
    free(courant);
    return sauvegarde;

}


// comptage du nbr de clients dans la file

int compte_client(T_noeud *tete)
{
    int compteur = 0;
    T_noeud *courant = tete;

    while(courant != NULL)
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

// Convertit un temps en secondes en heure/minute/seconde
void convertisseur_tps(int n, int *heures, int *minutes, int *secondes)
{
    *heures = n / 3600 ;
    n %= 3600 ;
    *minutes = n / 60 ;
    *secondes = n % 60 ;
}

void ecrireFicClients(T_noeud *Donnees, FILE *fp)
{
  T_noeud *Nouveau;

  fp = fopen(NOM_FIC_DONNES, "a");

  Nouveau = Donnees -> suiv;
  while (Nouveau != NULL)
  {
    fprintf(fp, "%d %d %d %d %d \n", Nouveau -> data.jour, Nouveau -> data.h_arrivee, Nouveau -> data.duree_attente, Nouveau -> data.debut_prise_en_charge, Nouveau -> data.fin_prise_en_charge );
    Nouveau = Nouveau -> suiv;
  }
}


int main(void)
{
    srand(time(NULL));
    
    T_noeud *file_attente = malloc(sizeof(T_noeud)) ;
    file_attente -> suiv = NULL;    // Creation de la file d'attente sous forme de liste chainee

    T_noeud *Donnees = malloc(sizeof(T_noeud));
    Donnees -> suiv = NULL;         // Creation d'une file ou seront stockes les clients

    FILE *fp;
    fp = fopen(NOM_FIC_DONNES, "w");  //Initialisation du fichier

    // Initialisation des tableaux & variables pour etude de performance 
    int moy_file_attente;
    int max_file_attente = 0; // On initialise a des valeurs random afin de pouvoir les comparer
    int min_file_attente = 5;

    float debit_journalier_moyen;
    int nbr_client_par_jour;       // On utilise un float pour eviter probleme de type lors d'un calcul plus tard
    float taux_client_pris_en_charge;
    int h_fin_de_service;

    int min_attente = 10;
    int max_attente = 0;
    int moy_attente;
    int tps_rep_moy;
    float nbr_client = 0.0;

    // Creation centre appel ; un tableau contenant les operateur, >= 1 s'ils sont occupes, 0 s'ils sont libres
    int simu_centre_appel[NBR_OPS] = {0};

    // Simulation
    for(int i = 1; i <= NBR_JOUR; i++)
    {   
        
        // tableau contenant l'heure d'arrivee des clients
        int *heure_arrivee = simu_arrivee();

        for(int j = 0; j < 24 * 3600; j++)
        {
            if (HEURE_DEB *3600 <= j && j <= HEURE_FIN * 3600) 
            {
                // regarde si un client est arrive
                if (compare_tps(j, heure_arrivee))
                {
                    struct Client client;
                    client.h_arrivee = j;
                    ajout_client(&file_attente, client);
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
                        struct Client client = pop_client(&file_attente);
                        //Donnees a ajouter
                        client.debut_prise_en_charge = j;
                        int tps_conversation = tps_prise_en_charge();
                        simu_centre_appel[k] = tps_conversation;
                        client.duree_attente = j - client.h_arrivee;
                        client.fin_prise_en_charge = j + tps_conversation;
                        client.jour = i;

                        moy_attente = moy_attente + client.duree_attente;
                        tps_rep_moy += client.fin_prise_en_charge - client.h_arrivee;
                        if (client.duree_attente < min_attente)
                            min_attente = client.duree_attente;
                        if (client.duree_attente > max_attente)
                            max_attente = client.duree_attente;
                        nbr_client++;
                        h_fin_de_service = client.fin_prise_en_charge;
                        if (h_fin_de_service < HEURE_FIN * 3600)
                            h_fin_de_service = HEURE_FIN * 3600;

                        ajout_client(&Donnees, client);                              // On enregistre les differentes donnes dans client puis on l'ajoute a la liste des donnes.
                    }
                }
                // simule la seconde passee
                for(int l = 0; l < NBR_OPS; l++)
                {
                    if (simu_centre_appel[l] > 0)
                        simu_centre_appel[l] = simu_centre_appel[l] - 1;
                }
                moy_file_attente = moy_file_attente + compteur;
                if (compteur < min_file_attente)
                    min_file_attente = compteur;
                if (compteur > max_file_attente)
                    max_file_attente = compteur;
            }
        }
// Traitement des donnes
        moy_file_attente = moy_file_attente / (NBR_HEURES * 3600.0);
        moy_file_attente = ceil(moy_file_attente); //arrondit a l'entier superieur

        for(int n = 0; n < 10000; n++)
        {
            if (heure_arrivee[n] > 0 && heure_arrivee[n] <= 24 * 3600)
                {
                    nbr_client_par_jour++;
                    if ((heure_arrivee[n] >= HEURE_DEB * 3600) && (heure_arrivee[n] <= HEURE_FIN * 3600))
                    {
                        debit_journalier_moyen++;
                        taux_client_pris_en_charge++;
                    }
                }
            
        }

        taux_client_pris_en_charge = taux_client_pris_en_charge / nbr_client_par_jour;
        debit_journalier_moyen = debit_journalier_moyen / (NBR_HEURES * 3600.0);
        moy_attente = ceil(moy_attente / nbr_client);
        tps_rep_moy = ceil(tps_rep_moy / nbr_client);

        int heures, minutes, secondes;
        convertisseur_tps(h_fin_de_service, &heures, &minutes, &secondes);

        // Affichage des performances de la journee
        printf("Pour le jour : %d , On a : \n\n", i);
        printf("Debit journalier moyen : %1f \n", debit_journalier_moyen);
        printf("Taux client pris en charge : %3f \n", taux_client_pris_en_charge);
        printf("Taille de la file d'attente :      min : %d  max = %d  moy = %d \n", min_file_attente, max_file_attente, moy_file_attente);
        printf("Heure de fin de service :     %d h %d min %d sec \n", heures, minutes, secondes);
        printf("Temps moyen d'attente (en seconde)  :     min : %d  max = %d  moy = %d \n", min_attente, max_attente, moy_attente);
        printf("Temps de reponse moyen en seconde : %d \n", tps_rep_moy);


    }
    ecrireFicClients(Donnees, fp);
    fclose(fp);
    return 0;
}