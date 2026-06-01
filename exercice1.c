/*
 * Objectif du fichier :
 *   Sauvegarder des données quand l'utilisateur interrompt le programme
 *   avec Ctrl-C.
 *
 * Rôle principal du programme :
 *   Installer un gestionnaire de signal SIGINT, puis écrire les données dans
 *   sauvegarde.txt avant de quitter.
 *
 * Appels système importants utilisés :
 *   signal() : associe Ctrl-C à une fonction de traitement.
 *   sleep()  : ralentit la boucle pour simuler un traitement.
 *
 * Ce que je dois dire en soutenance :
 *   "Le programme ne s'arrête pas brutalement sur Ctrl-C. Le signal SIGINT
 *   est intercepté, puis le gestionnaire sauvegarde les données avant
 *   d'appeler exit."
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/* Données globales à sauvegarder */
int donnees[] = {10, 20, 30, 40, 50};
int nb_donnees = 5;

/*
 * Gestionnaire du signal SIGINT :
 *   Cette fonction est appelée automatiquement quand l'utilisateur fait Ctrl-C.
 */
void gestionnaire_sigint(int sig) {
    (void)sig;
    printf("\n[SIGINT reçu] Sauvegarde des données en cours...\n");

    /* Ouvre (ou crée) le fichier en écriture, remplace s'il existe */
    FILE *f = fopen("sauvegarde.txt", "w");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nb_donnees; i++) {
        fprintf(f, "%d\n", donnees[i]);
    }

    fclose(f);
    printf("Données sauvegardées dans 'sauvegarde.txt'.\n");
    exit(EXIT_SUCCESS);
}

/*
 * Fonction principale :
 *   Installe le gestionnaire SIGINT puis lance une boucle de travail.
 */
int main(void) {
    /* signal() remplace le comportement par défaut de Ctrl-C. */
    if (signal(SIGINT, gestionnaire_sigint) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    printf("Programme en cours d'exécution. Appuyez sur Ctrl-C pour sauvegarder et quitter.\n");

    /* Boucle infinie simulant un traitement */
    while (1) {
        printf("Traitement en cours...\n");
        sleep(2);
    }

    return EXIT_SUCCESS;
}
