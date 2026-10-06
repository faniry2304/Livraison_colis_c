/*
 * Gestion de livraison de colis - file de priorité (tas binaire min)
 *
 * Priorité : 1 = urgent, 2 = normal, 3 = économique
 * Le colis avec la plus petite priorité est livré en premier.
 * À priorité égale, le colis arrivé le premier est livré le premier (FIFO).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITE 100
#define TAILLE_NOM 50
#define TAILLE_ADRESSE 80

typedef struct {
    int id;
    char destinataire[TAILLE_NOM];
    char adresse[TAILLE_ADRESSE];
    int priorite;   /* 1, 2 ou 3 */
    int ordre;      /* numéro d'arrivée, pour départager les égalités */
} Colis;

typedef struct {
    Colis donnees[CAPACITE];
    int taille;
} FilePriorite;

/* ---------- Outils de la file de priorité ---------- */

/* Retourne 1 si le colis a doit passer avant le colis b */
int passe_avant(Colis a, Colis b) {
    if (a.priorite != b.priorite)
        return a.priorite < b.priorite;
    return a.ordre < b.ordre;
}

void echanger(Colis *a, Colis *b) {
    Colis tmp = *a;
    *a = *b;
    *b = tmp;
}

void initialiser(FilePriorite *f) {
    f->taille = 0;
}

/* Remonte le colis à l'indice i tant qu'il est plus prioritaire que son parent */
void remonter(FilePriorite *f, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (passe_avant(f->donnees[i], f->donnees[parent])) {
            echanger(&f->donnees[i], &f->donnees[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

/* Descend le colis à l'indice i tant qu'un enfant est plus prioritaire */
void descendre(FilePriorite *f, int i) {
    while (1) {
        int gauche = 2 * i + 1;
        int droite = 2 * i + 2;
        int premier = i;

        if (gauche < f->taille && passe_avant(f->donnees[gauche], f->donnees[premier]))
            premier = gauche;
        if (droite < f->taille && passe_avant(f->donnees[droite], f->donnees[premier]))
            premier = droite;

        if (premier == i)
            break;
        echanger(&f->donnees[i], &f->donnees[premier]);
        i = premier;
    }
}

/* Ajoute un colis. Retourne 1 si réussi, 0 si la file est pleine. */
int enfiler(FilePriorite *f, Colis c) {
    if (f->taille >= CAPACITE)
        return 0;
    f->donnees[f->taille] = c;
    remonter(f, f->taille);
    f->taille++;
    return 1;
}

/* Retire le colis le plus prioritaire. Retourne 1 si réussi, 0 si la file est vide. */
int defiler(FilePriorite *f, Colis *resultat) {
    if (f->taille == 0)
        return 0;
    *resultat = f->donnees[0];
    f->taille--;
    f->donnees[0] = f->donnees[f->taille];
    descendre(f, 0);
    return 1;
}

/* ---------- Affichage ---------- */

const char *nom_priorite(int p) {
    switch (p) {
        case 1: return "Urgent";
        case 2: return "Normal";
        default: return "Economique";
    }
}

void afficher_colis(Colis c) {
    printf("  #%d | %-15s | %-25s | %s\n",
           c.id, c.destinataire, c.adresse, nom_priorite(c.priorite));
}

/* Affiche tous les colis dans l'ordre de livraison, sans modifier la file */
void afficher_file(const FilePriorite *f) {
    if (f->taille == 0) {
        printf("Aucun colis en attente.\n");
        return;
    }
    FilePriorite copie = *f;
    Colis c;
    int rang = 1;
    printf("Colis en attente (%d), dans l'ordre de livraison :\n", f->taille);
    while (defiler(&copie, &c)) {
        printf("%2d.", rang++);
        afficher_colis(c);
    }
}

/* ---------- Saisie sécurisée ---------- */

void lire_texte(const char *message, char *dest, int taille) {
    printf("%s", message);
    if (fgets(dest, taille, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    dest[strcspn(dest, "\n")] = '\0';
}

int lire_entier(const char *message) {
    char ligne[32];
    int valeur;
    while (1) {
        printf("%s", message);
        if (fgets(ligne, sizeof(ligne), stdin) == NULL)
            return 0;
        if (sscanf(ligne, "%d", &valeur) == 1)
            return valeur;
        printf("Entrée invalide, réessayez.\n");
    }
}

/* ---------- Actions du menu ---------- */

void action_ajouter(FilePriorite *f, int *prochain_id, int *prochain_ordre) {
    Colis c;
    c.id = (*prochain_id)++;
    c.ordre = (*prochain_ordre)++;

    lire_texte("Destinataire : ", c.destinataire, TAILLE_NOM);
    lire_texte("Adresse : ", c.adresse, TAILLE_ADRESSE);

    do {
        c.priorite = lire_entier("Priorité (1=urgent, 2=normal, 3=économique) : ");
    } while (c.priorite < 1 || c.priorite > 3);

    if (enfiler(f, c))
        printf("Colis #%d ajouté.\n", c.id);
    else
        printf("Erreur : la file est pleine.\n");
}

void action_livrer(FilePriorite *f) {
    Colis c;
    if (defiler(f, &c)) {
        printf("Colis livré :\n");
        afficher_colis(c);
    } else {
        printf("Aucun colis à livrer.\n");
    }
}

void action_rechercher(const FilePriorite *f) {
    int id = lire_entier("Numéro du colis : ");
    for (int i = 0; i < f->taille; i++) {
        if (f->donnees[i].id == id) {
            printf("Colis trouvé :\n");
            afficher_colis(f->donnees[i]);
            return;
        }
    }
    printf("Colis #%d introuvable (inexistant ou déjà livré).\n", id);
}

/* ---------- Programme principal ---------- */

int main(void) {
    FilePriorite file;
    int prochain_id = 1;
    int prochain_ordre = 1;
    int choix;

    initialiser(&file);

    do {
        printf("\n===== Gestion de livraison de colis =====\n");
        printf("1. Ajouter un colis\n");
        printf("2. Livrer le prochain colis\n");
        printf("3. Afficher les colis en attente\n");
        printf("4. Rechercher un colis\n");
        printf("0. Quitter\n");
        choix = lire_entier("Votre choix : ");

        switch (choix) {
            case 1: action_ajouter(&file, &prochain_id, &prochain_ordre); break;
            case 2: action_livrer(&file); break;
            case 3: afficher_file(&file); break;
            case 4: action_rechercher(&file); break;
            case 0: printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
