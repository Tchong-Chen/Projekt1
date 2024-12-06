Table des matières

    Présentation du programme
    Prérequis
    Compilation
    Exécution
    Résultats attendus
    Découpage en modules et structures de données
    Choix techniques

1. Présentation du programme

Ce programme simule un centre d'appel avec plusieurs opérateurs et clients. Il modélise l'arrivée des clients, leur prise en charge par les opérateurs, et génère des statistiques sur le débit du centre, les temps d'attente, et d'autres métriques de performance.
Objectifs :

    Simuler l'arrivée des clients à des horaires aléatoires dans un centre d'appel.
    Chaque client est pris en charge par un opérateur libre, avec une durée de traitement aléatoire.
    Enregistrer les informations concernant l'attente et le traitement des clients.
    Calculer des statistiques sur le centre d'appel, telles que :
        Le débit journalier moyen.
        Le taux de clients pris en charge.
        La taille moyenne de la file d'attente.
        Le temps moyen d'attente.
        Le temps de réponse moyen.

2. Prérequis

Le programme est écrit en C et utilise les bibliothèques standards suivantes :

    stdio.h
    stdlib.h
    math.h
    time.h
    stdbool.h

Il nécessite un compilateur C compatible avec le standard C99, et l'option -lm est requise pour lier la bibliothèque mathématique.
3. Compilation

Pour compiler le programme, il faut exécuter la commande suivante dans un terminal :

gcc -o centre_appel centre_appel.c -lm

Explications :

    gcc : Le compilateur C.
    -o centre_appel : Cette option spécifie le nom de l'exécutable à générer (centre_appel).
    centre_appel.c : Le fichier source contenant le code du programme.
    -lm : Cette option lie la bibliothèque mathématique (nécessaire pour les fonctions comme logf() et ceil() utilisées dans le programme).

4. Exécution

Une fois le programme compilé, vous pouvez l'exécuter avec la commande suivante :

./centre_appel

Entrées :

Le programme génère les entrées de manière aléatoire, notamment les horaires d'arrivée des clients et les durées de traitement. Il ne nécessite donc pas d'entrées manuelles.
Sortie :

Les résultats sont affichés à l'écran et également écrits dans un fichier texte (Data.txt) sous le format suivant :

jour h_arrivee duree_attente debut_prise_en_charge fin_prise_en_charge

Les résultats à l'écran incluent des informations telles que :

    Le débit journalier moyen.
    Le taux de clients pris en charge.
    La taille de la file d'attente (min, max, moyenne).
    L'heure de fin de service.
    Les temps d'attente (min, max, moyen).
    Le temps de réponse moyen.

5. Résultats attendus

Voici un exemple de sortie à l'écran :

Pour le jour : 0 , On a : 

Débit journalier moyen : 0.0546 
Taux client pris en charge : 0.95 
Taille de la file d'attente : 
min : 0  max = 3  moy = 1
Heure de fin de service :  23 : 59 : 59 
Temps moyen d'attente (en seconde)  :
min : 12  max = 120  moy = 60 
Temps de réponse moyen en seconde : 180

Les résultats sont aussi sauvegardés dans le fichier Data.txt, avec les informations détaillées sur chaque client, telles que l'heure d'arrivée, le temps d'attente, le début et la fin de la prise en charge.
6. Découpage en modules et structures de données
Modules

Le programme est découpé en plusieurs fonctions/modules qui sont responsables de tâches spécifiques. Voici une vue d'ensemble des modules :

    intervalle_tps :
        Génère un intervalle de temps aléatoire entre l'arrivée de deux clients, basé sur une distribution exponentielle avec un taux (lambda).

    simu_arrivee :
        Génère les horaires d'arrivée des clients sur une journée de 24 heures, en utilisant intervalle_tps.

    compare_tps :
        Compare l'heure actuelle avec les horaires d'arrivée des clients pour déterminer si un client arrive à ce moment.

    tps_prise_en_charge :
        Génère une durée de prise en charge aléatoire pour chaque client, entre un minimum (minsrv) et un maximum (maxsrv).

    ajout_client :
        Ajoute un client à la fin de la liste chaînée file.

    pop_client :
        Retire un client de la file d'attente et renvoie ses informations.

    compte_client :
        Compte le nombre de clients dans la file d'attente.

    ops_libre :
        Vérifie quels opérateurs sont libres.

    convertisseur_tps :
        Convertit un temps en secondes en heures, minutes, et secondes.

    ecrireFicClients :
        Écrit les informations des clients dans un fichier texte (Data.txt).

    tps_attente :
        Calcule les statistiques sur les temps d'attente des clients.

Structures de données

    Client :
        Cette structure contient les informations d'un client, y compris l'heure d'arrivée, les temps de début et fin de prise en charge, la durée d'attente et le jour.

struct Client {
    int h_arrivee;
    int debut_prise_en_charge;
    int fin_prise_en_charge;
    int duree_attente;
    int jour;
};

T_noeud :

    Il s'agit d'un noeud dans une liste chaînée qui contient un client et un pointeur vers le noeud suivant.

    typedef struct T_noeud {
        struct Client data;
        struct T_noeud *suiv;
    } T_noeud;

    Liste chaînée :
        La liste chaînée file stocke les clients en attente, et la liste Donnees enregistre les clients pris en charge.

7. Choix techniques
Utilisation de la distribution exponentielle

    La génération des horaires d'arrivée des clients est basée sur une distribution exponentielle, simulant des arrivées aléatoires dans le temps avec un taux (lambda). Cela est typique pour des modèles de simulation de files d'attente.

Liste chaînée

    Une liste chaînée est utilisée pour modéliser la file d'attente des clients, car elle permet une gestion dynamique de la taille de la file (ajout et retrait de clients) sans avoir à redimensionner un tableau.

Structures de données simples

    Le programme utilise des structures simples pour stocker les informations des clients et des opérateurs, ce qui facilite la gestion et l'accès aux données.

Simplicité de l'implémentation

    Le choix d'un modèle de simulation basé sur des boucles temporelles simples (secondes, heures) permet une simulation claire et compréhensible tout en restant flexible pour des améliorations futures (ajout de plus d'opérateurs, changements dans les règles d'arrivée des clients, etc.).
