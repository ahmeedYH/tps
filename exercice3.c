/*
 * Objectif du fichier :
 *   Faire communiquer un processus père et un processus fils.
 *
 * Rôle principal du programme :
 *   Le père envoie des entiers au fils, le fils calcule le double et renvoie
 *   le résultat au père.
 *
 * Appels système importants utilisés :
 *   pipe()  : crée un canal de communication.
 *   fork()  : crée le processus fils.
 *   read()  : lit une valeur dans un tube.
 *   write() : écrit une valeur dans un tube.
 *   wait()  : attend la fin du processus fils.
 *
 * Ce que je dois dire en soutenance :
 *   "Un tube Unix est unidirectionnel. Pour communiquer dans les deux sens,
 *   j'utilise donc deux tubes : un pour père vers fils et un pour fils vers
 *   père."
 *
 * Schéma de communication :
 *
 *   PÈRE  ──[p1: écriture]──►  FILS  (père envoie 5 entiers)
 *   PÈRE  ◄──[p2: lecture]──   FILS  (fils renvoie les doubles)
 *
 *   - p1[1] : père écrit   → p1[0] : fils lit
 *   - p2[1] : fils écrit   → p2[0] : père lit
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NB_ENTIERS 5

/*
 * Fonction principale :
 *   Crée les deux tubes, lance le fils avec fork, puis sépare le code du père
 *   et celui du fils selon la valeur de pid.
 */
int main(void) {
    int p1[2]; /* tube père → fils */
    int p2[2]; /* tube fils → père */
    pid_t pid;

    /* --- Création des deux tubes --- */
    if (pipe(p1) == -1) {
        perror("pipe p1");
        exit(EXIT_FAILURE);
    }
    if (pipe(p2) == -1) {
        perror("pipe p2");
        exit(EXIT_FAILURE);
    }

    /* fork() duplique le processus : pid vaut 0 dans le fils. */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /* ============================================================
     *  PROCESSUS FILS
     * ============================================================ */
    if (pid == 0) {
        /* Fermer les extrémités inutilisées par le fils */
        close(p1[1]); /* le fils ne écrit pas dans p1 */
        close(p2[0]); /* le fils ne lit pas dans p2  */

        int valeur, double_val;

        for (int i = 0; i < NB_ENTIERS; i++) {
            /* read() bloque jusqu'à recevoir un entier envoyé par le père. */
            if (read(p1[0], &valeur, sizeof(int)) == -1) {
                perror("fils: read p1");
                exit(EXIT_FAILURE);
            }

            printf("[FILS]  Reçu : %d\n", valeur);

            /* write() envoie le double dans le tube de retour. */
            double_val = valeur * 2;
            if (write(p2[1], &double_val, sizeof(int)) == -1) {
                perror("fils: write p2");
                exit(EXIT_FAILURE);
            }
        }

        /* Fermer les extrémités restantes */
        close(p1[0]);
        close(p2[1]);

        exit(EXIT_SUCCESS);
    }

    /* ============================================================
     *  PROCESSUS PÈRE
     * ============================================================ */
    else {
        /* Fermer les extrémités inutilisées par le père */
        close(p1[0]); /* le père ne lit pas dans p1  */
        close(p2[1]); /* le père n'écrit pas dans p2 */

        int entiers[NB_ENTIERS] = {3, 7, 12, 25, 40};
        int resultat;

        for (int i = 0; i < NB_ENTIERS; i++) {
            printf("[PÈRE]  Envoi de : %d\n", entiers[i]);

            /* write() envoie un entier au processus fils. */
            if (write(p1[1], &entiers[i], sizeof(int)) == -1) {
                perror("père: write p1");
                exit(EXIT_FAILURE);
            }

            /* read() récupère la réponse calculée par le fils. */
            if (read(p2[0], &resultat, sizeof(int)) == -1) {
                perror("père: read p2");
                exit(EXIT_FAILURE);
            }

            printf("[PÈRE]  Double reçu : %d\n\n", resultat);
        }

        /* Fermer les extrémités restantes */
        close(p1[1]);
        close(p2[0]);

        /* wait() évite de laisser un processus fils zombie. */
        wait(NULL);
        printf("[PÈRE]  Fils terminé. Fin du programme.\n");
    }

    return EXIT_SUCCESS;
}
