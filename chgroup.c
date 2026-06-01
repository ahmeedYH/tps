/*
 * Objectif du fichier :
 *   Changer le groupe propriétaire d'un fichier.
 *
 * Rôle principal du programme :
 *   Trouver le GID d'un groupe puis l'appliquer au fichier.
 *
 * Appels système importants utilisés :
 *   stat()  : récupère l'utilisateur actuel pour le conserver.
 *   chown() : change le propriétaire utilisateur/groupe d'un fichier.
 *
 * Ce que je dois dire en soutenance :
 *   "Même si la fonction s'appelle chown, elle peut aussi changer le groupe.
 *   Ici je garde l'UID actuel et je remplace seulement le GID."
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Fonction principale :
 *   Vérifie les arguments, cherche le groupe et appelle chown.
 */
int main(int argc, char *argv[])
{
    /* Vérification des arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nom_fichier> <nouveau_groupe>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *fichier        = argv[1];
    const char *nouveau_groupe = argv[2];

    /* Récupérer le GID du nouveau groupe */
    struct group *grp = getgrnam(nouveau_groupe);
    if (grp == NULL) {
        fprintf(stderr, "groupe introuvable: %s\n", nouveau_groupe);
        return EXIT_FAILURE;
    }
    gid_t nouveau_gid = grp->gr_gid;

    /* stat() permet de conserver l'utilisateur actuel du fichier. */
    struct stat buffer;
    if (stat(fichier, &buffer) == -1) {
        perror("stat");
        return EXIT_FAILURE;
    }

    /* chown() reçoit l'UID à garder et le nouveau GID à appliquer. */
    if (chown(fichier, buffer.st_uid, nouveau_gid) == -1) {
        perror("chown");
        return EXIT_FAILURE;
    }

    printf("Groupe de '%s' changé en '%s'\n", fichier, nouveau_groupe);
    return EXIT_SUCCESS;
}
