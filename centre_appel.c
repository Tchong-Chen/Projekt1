#include <stdio.lib>
#include <stdlib.h>
#include <math.h>
#include <time.h>


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

int simu_arrivee()
{
    int i = 0;
    int tableau_tps_arrivee = (int *) malloc(size_of(int));

    while(tableau_tps_arrivee[i] < NBR_HEURES * 3600)
    {
        int tps = intervalle_tps(void);
        
    }
}


// tps de prise en charge client

int tps_prise_en_charge(struct T_noeud)
{
    int tps_avec_client = rand()
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

void ajout_client(struct T_noeud *tete, struct Client nv_client)
{
    T_noeud *tete = NULL;
    T_noeud *nouveau;

    nouveau = ((T_noeud *) malloc(size_of(T_noeud)));

    nouveau -> data = nv_client;
    nouveau -> suiv = *tete;
    *tete = nouveau;
}


// retirer un client de la file

void pop_client(struct T_noeud *tete, struct Client *sauvegarde) 
{
    if (*tete == NULL)
        return;

    if ((*tete)->suiv == NULL) 
    {
        *sauvegarde = (*tete)->data;
        free(*tete);
        *tete = NULL;
        return;
    }

    T_noeud *current = *tete;
    while (current->suiv != NULL) 
        current = current->suiv;

    T_noeud *temp = current->suiv;  // Le dernier client

    // Sauvegarder les données du dernier client
    *sauvegarde = temp->data;

    current->suiv = NULL;  // Le nouvel avant-dernier élément pointe vers NULL
    free(temp);  // Libère la mémoire du dernier client
}



void pop_client(struct T_noeud *tete)
{
    if ((*tete) -> suiv == NULL)
    {
        int client = (*tete) -> 
    }  
}

int main(void)
{
    srand(time(NULL));
    
    for(i = 0; i <= NBR_JOUR; i++)
    {   
        

        for(j = 0; j <= NBR_HEURES * 3600; i++)
        {
            // creation centre appel
            int simu_centre_appel[NBR_OPS] = {0};


        }
    }
}