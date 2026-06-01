/*
 * Objectif du fichier :
 *   Lancer plusieurs programmes plusieurs fois.
 *
 * Rôle principal du programme :
 *   Recevoir un nombre N puis une liste de programmes, et créer N processus
 *   pour chaque programme.
 *
 * Appels système importants utilisés :
 *   fork() : crée un processus fils.
 *   exec() : remplace le fils par le programme demandé.
 *
 * Ce que je dois dire en soutenance :
 *   "Le père parcourt les programmes et crée des fils avec fork. Dans chaque
 *   fils, execlp charge le vrai programme à exécuter."
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Fonction principale :
 *   Analyse les arguments puis lance chaque programme N fois.
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nombre> <prog1> [prog2 ...]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Nombre invalide: %s\n", argv[1]);
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        for (int j = 0; j < n; j++) {
            /* fork() crée un fils pour ne pas remplacer le lanceur lui-même. */
            pid_t pid = fork();
            if (pid == 0) {
                /* execlp() remplace le code du fils par argv[i]. */
                execlp(argv[i], argv[i], NULL);
                perror("execlp");
                exit(1);
            } else if (pid < 0) {
                perror("fork");
            }
        }
    }

    return 0;
}
