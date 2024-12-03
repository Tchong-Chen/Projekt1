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
#define lambda 0.1


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
    T_noeud *tete = NULL;
    T_noeud *nouveau;

    nouveau = ((T_noeud *) malloc(sizeof(T_noeud)));

    nouveau -> data = nv_client;
    nouveau -> suiv = *tete;
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
    int *auvegarde = dernier -> data
    courant -> suiv = NULL;

    free(dernier;)
    return sauvegarde;
}


// comptage du nbr de clients dans la file

int compte_client(T_noeud *tete)
{
    int compteur = 0;
    T_noeud courant = *tete;

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

int main(void)
{
    srand(time(NULL));
    
    T_noeud *file_attente;
    file_attente -> suiv = NULL;

    for(int i = 0; i <= NBR_JOUR; i++)
    {   
        

        for(int j = 0; j <= NBR_HEURES * 3600; i++)
        {
            // creation centre appel
            int simu_centre_appel[NBR_OPS] = {0};

            // tableau contenant l'heure d'arrivee des clients
            int *heure_arrivee = simu_arrivee();

            // regarde si un client est arrive
            if (compare_tps(j, heure_arrivee))
            {
                struct Client client;
                client.h_arrivee = j;
                ajout_client(*file_attente, client);
            }

            // regarde s'il y a des gens dans la file d'attente & si un operateur est libre
            // si oui, enregistre son tps de debut de prise en charge + genere une duree aleatoire de conversation client/ops
            int compteur = compte_client(*file_attente);
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
        }
    }
}