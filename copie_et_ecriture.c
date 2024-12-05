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
#define NOM_FIC_DONNES "Data.txt"


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


void ecrireFicClients(T_noeud *Donnees, FILE *fp)
{
    T_noeud *nouveau = Donnees;
    fp = fopen(NOM_FIC_DONNES, "a");

    while (nouveau -> suiv != NULL)
    {
        fprintf(fp, "%d %d %d %d %d \n", nouveau -> data.jour, nouveau -> data.h_arrivee, nouveau -> data.duree_attente, nouveau -> data.debut_prise_en_charge, nouveau -> data.fin_prise_en_charge );
        nouveau = nouveau-> suiv;
    }
}

int main(void)
{
    return 0;
}
