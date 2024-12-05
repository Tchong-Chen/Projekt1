#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


// def constantes

#define NBR_OPS 5
#define NBR_HEURES 11
#define NBR_JOUR 1
#define minsrv 100 // en secondes
#define maxsrv 600
#define lambda 0.06


// Intervalle de temps separant deux clients

double intervalle_tps(void)
{
    double U = (double) rand() / RAND_MAX;
    double X = -logf(1.0 - U) / lambda;

    return ceil(X);
}


// Simulation tps d'arrivee des clients

int* simu_arrivee()
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

// comparaison entre tableau de simu_arrivee et l'heure actuelle

bool compare_tps(int j, int *tab)
{
    int i = 0;
    while( tab[i] < j)
        i++;
    return (tab[i] == j);
}


// tps de prise en charge client

int tps_prise_en_charge()
{
    int tps_avec_client = rand() % maxsrv + minsrv;
    return tps_avec_client;
}


// creation d'une file sous forme d'une liste chainee

struct Client {
    int h_arrivee;
    int debut_prise_en_charge;
    int fin_prise_en_charge;
} ;

typedef struct Noeud {
    struct Client data;
    struct noeud *suiv;
} T_noeud;


// ajout d'un client dans la file

void ajout_client(T_noeud *file, struct Client nv_client)
{
    T_noeud *tete = file;
    T_noeud *nouveau;

    nouveau = ((T_noeud *) malloc(sizeof(T_noeud)));

    nouveau -> data = nv_client;
    nouveau -> suiv = tete;
    *tete = nouveau;
}


// retirer un client de la file

struct pop_client(T_noeud *tete) 
{
    if (*tete == NULL)
        return;

    if ((*tete) -> suiv == NULL) 
    {
        int *sauvegarde = (*tete) -> data;
        free(*tete);
        *tete = NULL;
        return sauvegarde;
    }

    T_noeud *courant = *tete;
    while (courant -> (suiv -> suiv) != NULL) 
        courant = courant -> suiv;

    T_noeud *dernier = courant -> suiv;
    int *sauvegarde = dernier -> data
    courant -> suiv = NULL;

    free(dernier;)
    return sauvegarde;
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

int main(void)
{
    srand(time(NULL));
    
    T_noeud *file_attente;
    file_attente -> suiv = NULL;

    // Initialisation des tableaux & variables pour etude de performance 
    int* compteur_file_attente[NBR_HEURES * 3600] = {0};
    int max_file_attente = 0;
    int min_file_attente = 5;
    float moy_file_attente = 0.0;
    int debit_journalier_moyen;
    float nbr_client_par_jour = 0.0;
    float taux_client_pris_en_charge;

    // creation centre appel
    int simu_centre_appel[NBR_OPS] = {0};

    for(int i = 0; i <= NBR_JOUR; i++)
    {   
        
        // tableau contenant l'heure d'arrivee des clients
        int *heure_arrivee = simu_arrivee();

        for(int j = 0; j <= 24 * 3600; i++)
        {
            // creation centre appel
            int simu_centre_appel[NBR_OPS] = {0};

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
            int *libre = ops_libre(simu_centre_appel);
            for(int k = 0; k < NBR_OPS; k++)
            {
                if ((libre[k] == 0) && (compteur >= 1))
                {
                    compteur = compteur - 1;
                    struct Client client = pop_client(*file_attente);
                    client.debut_prise_en_charge = j;
                    simu_centre_appel[k] = tps_prise_en_charge();
                }
            }
            
            // simule la seconde passee
            for(int l = 0; l < NBR_OPS; l++)
            {
                if (simu_centre_appel[l] > 0)
                    simu_centre_appel[l] = simu_centre_appel[l] - 1;
            }

            compteur_file_attente[i] = compte_client(file_attente);
        }

        for(int i = 0; i < NBR_HEURES * 3600; i++)
        {
            moy_file_attente = moy_file_attente + compteur_file_attente[i];
            if (compteur_file_attente[i] < min_file_attente)
                min_file_attente = compteur_file_attente[i];
            else if (compteur_file_attente[i] > max_file_attente)
                max_file_attente = compteur_file_attente[i];
        }
        moy_file_attente = moy_file_attente / (NBR_HEURES * 3600.0);
        moy_file_attente = ceil(moy_file_attente);

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

        // Affichage des performances de la journee
        printf("Debit journalier moyen : %d \n", debit_journalier_moyen);
        printf("Taux client pris en charge : %3f \n", taux_client_pris_en_charge);
        printf("Taille de la file d'attente : \n min : %d  max = %d  moy = %1f \n", min_file_attente, max_file_attente, moy_file_attente);
    }
    return 0;
}