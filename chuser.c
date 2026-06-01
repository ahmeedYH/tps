/*
 * Objectif du fichier :
 *   Changer le propriétaire utilisateur d'un fichier.
 *
 * Rôle principal du programme :
 *   Trouver l'UID d'un nom d'utilisateur puis l'appliquer au fichier.
 *
 * Appels système importants utilisés :
 *   stat()  : récupère le groupe actuel pour le conserver.
 *   chown() : change le propriétaire du fichier.
 *
 * Ce que je dois dire en soutenance :
 *   "Je transforme le nom d'utilisateur en UID avec getpwnam. Ensuite chown
 *   change seulement l'utilisateur, car je garde le GID déjà présent dans
 *   le fichier."
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Fonction principale :
 *   Vérifie les arguments, cherche l'utilisateur et appelle chown.
 */
int main(int argc, char *argv[])
{
    /* Vérification des arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nom_fichier> <nouveau_user>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *fichier      = argv[1];
    const char *nouveau_user = argv[2];

    /* Récupérer l'UID du nouvel utilisateur */
    struct passwd *pw = getpwnam(nouveau_user);
    if (pw == NULL) {
        fprintf(stderr, "utilisateur introuvable: %s\n", nouveau_user);
        return EXIT_FAILURE;
    }
    uid_t nouveau_uid = pw->pw_uid;

    /* stat() permet de conserver le groupe actuel du fichier. */
    struct stat buffer;
    if (stat(fichier, &buffer) == -1) {
        perror("stat");
        return EXIT_FAILURE;
    }

    /* chown() change le propriétaire ; sur Linux il faut souvent être root. */
    if (chown(fichier, nouveau_uid, buffer.st_gid) == -1) {
        perror("chown");
        return EXIT_FAILURE;
    }

    printf("Propriétaire de '%s' changé en '%s'\n", fichier, nouveau_user);
    return EXIT_SUCCESS;
}
