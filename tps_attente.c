#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


// def constantes modifiable

#define NBR_OPS 5       // Nombre d'operateur
#define NBR_HEURES 11   // Duree de la journee
#define NBR_JOUR 1      // Nombre de jour de la simulation
#define minsrv 100      // Duree min du service
#define maxsrv 600      // Duree max du service
#define lambda 0.06     // lambda


// Structure et liste chainee

struct Client {
    int h_arrivee;
    int debut_prise_en_charge;
    int fin_prise_en_charge;
    int duree_attente;
} ; 

typedef struct T_noeud {
    struct Client data;
    struct T_noeud *suiv;
} T_noeud;


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
    return min_attente, max_attente, (moy_attente / compteur), (tps_rep_moy / compteur);
}


int main(void)
{
    return 0;
}