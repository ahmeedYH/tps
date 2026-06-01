/*
 * Objectif du fichier :
 *   Afficher les attributs importants d'un fichier.
 *
 * Rôle principal du programme :
 *   Lire les métadonnées d'un fichier et les afficher sous une forme simple.
 *
 * Appels système importants utilisés :
 *   stat() : récupère le type, les droits, la taille, le propriétaire, etc.
 *
 * Ce que je dois dire en soutenance :
 *   "Ce programme interroge le système de fichiers avec stat. La structure
 *   stat contient les informations que ls -l affiche normalement."
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Fonction principale :
 *   Appelle stat puis affiche les champs les plus utiles pour expliquer
 *   les attributs d'un fichier Unix.
 */
int main(int argc, char *argv[])
{
    /* Vérification des arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct stat buffer;

    /* stat() remplit la structure buffer avec les métadonnées du fichier. */
    if (stat(argv[1], &buffer) == -1) {
        perror("stat");
        return EXIT_FAILURE;
    }

    /* Type de fichier */
    printf("Type        : ");
    if      (S_ISREG(buffer.st_mode))  printf("Fichier régulier\n");
    else if (S_ISDIR(buffer.st_mode))  printf("Répertoire\n");
    else if (S_ISLNK(buffer.st_mode))  printf("Lien symbolique\n");
    else if (S_ISCHR(buffer.st_mode))  printf("Périphérique caractère\n");
    else if (S_ISBLK(buffer.st_mode))  printf("Périphérique bloc\n");
    else if (S_ISFIFO(buffer.st_mode)) printf("FIFO (pipe nommé)\n");
    else if (S_ISSOCK(buffer.st_mode)) printf("Socket\n");
    else                               printf("Inconnu\n");

    /* Droits d'accès sous forme rwxrwxrwx */
    printf("Droits      : ");
    putchar(buffer.st_mode & S_IRUSR ? 'r' : '-');
    putchar(buffer.st_mode & S_IWUSR ? 'w' : '-');
    putchar(buffer.st_mode & S_IXUSR ? 'x' : '-');
    putchar(buffer.st_mode & S_IRGRP ? 'r' : '-');
    putchar(buffer.st_mode & S_IWGRP ? 'w' : '-');
    putchar(buffer.st_mode & S_IXGRP ? 'x' : '-');
    putchar(buffer.st_mode & S_IROTH ? 'r' : '-');
    putchar(buffer.st_mode & S_IWOTH ? 'w' : '-');
    putchar(buffer.st_mode & S_IXOTH ? 'x' : '-');
    putchar('\n');

    /* Date de dernière modification */
    printf("Modifié le  : %s", ctime(&buffer.st_mtime));

    /* Propriétaire via getpwuid() */
    struct passwd *pw = getpwuid(buffer.st_uid);
    if (pw)
        printf("Propriétaire: %s\n", pw->pw_name);
    else
        printf("Propriétaire: UID %d\n", buffer.st_uid);

    /* Taille en octets */
    printf("Taille      : %lld octets\n", (long long)buffer.st_size);

    /* Nombre de liens physiques */
    printf("Liens       : %lld\n", (long long)buffer.st_nlink);

    return EXIT_SUCCESS;
}
