/*
 * Objectif du fichier :
 *   Refaire simplement la commande mkdir en C.
 *
 * Rôle principal du programme :
 *   Créer un dossier dont le nom est donné en argument.
 *
 * Appels système importants utilisés :
 *   mkdir() : demande au noyau de créer un nouveau répertoire.
 *
 * Ce que je dois dire en soutenance :
 *   "Ce programme montre comment créer un répertoire sans passer par la
 *   commande shell mkdir. Le nom du dossier vient de argv[1] et mkdir()
 *   reçoit aussi les droits initiaux, ici 0755."
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * Fonction principale :
 *   Vérifie l'argument puis crée le dossier demandé.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage : %s <nom_dossier>\n", argv[0]);
        return 1;
    }

    /* mkdir() crée le répertoire avec les droits rwxr-xr-x. */
    if (mkdir(argv[1], 0755) == -1)
    {
        perror("mkdir");
        return 1;
    }

    printf("Dossier '%s' créé avec succès.\n", argv[1]);

    return 0;
}
