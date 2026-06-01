/*
 * Objectif du fichier :
 *   Refaire simplement la suppression d'un fichier.
 *
 * Rôle principal du programme :
 *   Supprimer le nom de fichier donné en argument.
 *
 * Appels système importants utilisés :
 *   unlink() : supprime le lien vers un fichier dans le système de fichiers.
 *
 * Ce que je dois dire en soutenance :
 *   "unlink ne supprime pas directement les données visibles pour nous :
 *   il enlève le nom du fichier. Si aucun autre lien ou processus ne l'utilise,
 *   le système libère ensuite l'espace."
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Fonction principale :
 *   Vérifie l'argument puis appelle unlink sur le fichier demandé.
 */
int main(int argc, char *argv[])
{
    /* Vérification des arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* unlink() demande au noyau de supprimer l'entrée du fichier. */
    if (unlink(argv[1]) == -1) {
        perror("unlink");
        return EXIT_FAILURE;
    }

    printf("Fichier '%s' supprimé avec succès.\n", argv[1]);
    return EXIT_SUCCESS;
}
