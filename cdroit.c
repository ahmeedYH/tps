/*
 * Objectif du fichier :
 *   Modifier les droits d'accès d'un fichier.
 *
 * Rôle principal du programme :
 *   Lire les droits actuels avec stat, modifier certains bits, puis appliquer
 *   le résultat avec chmod.
 *
 * Appels système importants utilisés :
 *   stat()  : récupère les droits actuels du fichier.
 *   chmod() : applique les nouveaux droits.
 *
 * Ce que je dois dire en soutenance :
 *   "Les droits Unix sont stockés dans st_mode. Je garde les droits existants,
 *   je modifie seulement la partie demandée, puis chmod enregistre le nouveau
 *   mode dans le système de fichiers."
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fonction principale :
 *   Analyse les arguments, calcule le nouveau mode et appelle chmod.
 */
int main(int argc, char *argv[])
{
    /* Vérification des arguments : 3 ou 4 */
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <nom_fichier> <user|group|other> [lecture|ecriture|execution|all]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *fichier        = argv[1];
    const char *type_utilisateur = argv[2];
    const char *droit          = (argc == 4) ? argv[3] : NULL;

    /* stat() récupère le mode actuel pour ne pas repartir de zéro. */
    struct stat buffer;
    if (stat(fichier, &buffer) == -1) {
        perror("stat");
        return EXIT_FAILURE;
    }

    mode_t mode = buffer.st_mode;

    /* Vérifier que type_utilisateur est valide */
    if (strcmp(type_utilisateur, "user")  != 0 &&
        strcmp(type_utilisateur, "group") != 0 &&
        strcmp(type_utilisateur, "other") != 0) {
        fprintf(stderr, "Erreur: type_utilisateur doit être 'user', 'group' ou 'other'\n");
        return EXIT_FAILURE;
    }

    /* Cas 1 : argument [droit] absent → supprimer tous les droits du type */
    if (droit == NULL) {
        if      (strcmp(type_utilisateur, "user")  == 0) mode &= ~(S_IRUSR | S_IWUSR | S_IXUSR);
        else if (strcmp(type_utilisateur, "group") == 0) mode &= ~(S_IRGRP | S_IWGRP | S_IXGRP);
        else                                              mode &= ~(S_IROTH | S_IWOTH | S_IXOTH);
    }
    /* Cas 2 : [droit] == "all" → donner tous les droits */
    else if (strcmp(droit, "all") == 0) {
        if      (strcmp(type_utilisateur, "user")  == 0) mode |= (S_IRUSR | S_IWUSR | S_IXUSR);
        else if (strcmp(type_utilisateur, "group") == 0) mode |= (S_IRGRP | S_IWGRP | S_IXGRP);
        else                                              mode |= (S_IROTH | S_IWOTH | S_IXOTH);
    }
    /* Cas 3 : droit spécifique → ajouter uniquement ce droit */
    else if (strcmp(droit, "lecture") == 0) {
        if      (strcmp(type_utilisateur, "user")  == 0) mode |= S_IRUSR;
        else if (strcmp(type_utilisateur, "group") == 0) mode |= S_IRGRP;
        else                                              mode |= S_IROTH;
    }
    else if (strcmp(droit, "ecriture") == 0) {
        if      (strcmp(type_utilisateur, "user")  == 0) mode |= S_IWUSR;
        else if (strcmp(type_utilisateur, "group") == 0) mode |= S_IWGRP;
        else                                              mode |= S_IWOTH;
    }
    else if (strcmp(droit, "execution") == 0) {
        if      (strcmp(type_utilisateur, "user")  == 0) mode |= S_IXUSR;
        else if (strcmp(type_utilisateur, "group") == 0) mode |= S_IXGRP;
        else                                              mode |= S_IXOTH;
    }
    else {
        fprintf(stderr, "Erreur: droit invalide. Choisir: lecture, ecriture, execution, all\n");
        return EXIT_FAILURE;
    }

    /* chmod() applique réellement les nouveaux droits sur le fichier. */
    if (chmod(fichier, mode) == -1) {
        perror("chmod");
        return EXIT_FAILURE;
    }

    printf("Droits modifiés avec succès sur '%s'\n", fichier);
    return EXIT_SUCCESS;
}
