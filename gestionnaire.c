/*
 * Objectif du fichier :
 *   Lire une liste d'applications puis les lancer.
 *
 * Rôle principal du programme :
 *   Stocker les noms dans une liste chaînée simple, lancer chaque application
 *   dans un processus fils, puis attendre leur fin.
 *
 * Appels système importants utilisés :
 *   fork() : crée un fils pour chaque application.
 *   exec() : lance l'application dans le fils.
 *   wait() : attend que les fils se terminent.
 *
 * Ce que je dois dire en soutenance :
 *   "La liste chaînée sert seulement à mémoriser les commandes saisies.
 *   Ensuite le programme crée un fils par commande et utilise execlp pour
 *   lancer l'application."
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct Noeud {
    char *app;
    struct Noeud *suivant;
} Noeud;

/*
 * ajouter :
 *   Ajoute une application au début de la liste chaînée.
 */
Noeud *ajouter(Noeud *tete, const char *app) {
    Noeud *n = malloc(sizeof(Noeud));
    if (n == NULL) {
        perror("malloc");
        exit(1);
    }
    n->app = strdup(app);
    if (n->app == NULL) {
        perror("strdup");
        free(n);
        exit(1);
    }
    n->suivant = tete;
    return n;
}

/*
 * lancer :
 *   Parcourt la liste et lance chaque application dans un processus fils.
 */
void lancer(Noeud *tete) {
    for (Noeud *c = tete; c; c = c->suivant) {
        /* fork() crée un fils dédié à cette application. */
        pid_t pid = fork();
        if (pid == 0) {
            /* execlp() remplace le fils par l'application demandée. */
            execlp(c->app, c->app, NULL);
            perror("execlp");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
        }
    }
    /* wait() récupère la fin de tous les fils lancés. */
    while (wait(NULL) > 0);
}

/*
 * liberer :
 *   Libère la mémoire utilisée par la liste chaînée.
 */
void liberer(Noeud *tete) {
    while (tete) {
        Noeud *tmp = tete;
        tete = tete->suivant;
        free(tmp->app);
        free(tmp);
    }
}

/*
 * Fonction principale :
 *   Lit les noms d'applications jusqu'à une ligne vide.
 */
int main() {
    Noeud *liste = NULL;
    char buf[256];

    printf("Entrez les applications (ligne vide pour terminer) :\n");
    while (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        if (buf[0] == '\0') break;
        liste = ajouter(liste, buf);
    }

    lancer(liste);
    liberer(liste);
    return 0;
}
