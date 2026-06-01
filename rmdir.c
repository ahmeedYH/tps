/*
 * Objectif du fichier :
 *   Supprimer un répertoire vide avec un appel système bas niveau.
 *
 * Rôle principal du programme :
 *   Recevoir le nom d'un dossier et demander sa suppression au noyau.
 *
 * Appels système importants utilisés :
 *   rmdir() via syscall(SYS_rmdir) : supprime un répertoire vide.
 *   write() : affiche les messages sans printf.
 *
 * Ce que je dois dire en soutenance :
 *   "Ici je montre l'appel système rmdir sous une forme très directe :
 *   syscall(SYS_rmdir, chemin). Le dossier doit être vide, sinon le noyau
 *   refuse la suppression."
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

/*
 * Fonction principale :
 *   Vérifie le chemin reçu puis appelle rmdir via syscall.
 */
int main(int argc, char *argv[])
{
    long ret;

    if (argc != 2)
    {
        write(1, "Usage: ./mon_rmdir <dossier>\n", 30);
        return 1;
    }

    /* rmdir() supprime uniquement un dossier vide. */
    ret = syscall(SYS_rmdir, argv[1]);

    if (ret < 0)
    {
        perror("rmdir");
        return 1;
    }

    /* write() écrit directement sur la sortie standard. */
    write(1, "Dossier supprime\n", 18);

    return 0;
}
