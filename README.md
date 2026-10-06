# Gestion de livraison de colis (C)

Programme en langage C qui gère des colis à livrer à l'aide d'une **file de priorité** : les colis urgents sont livrés avant les autres.

## Fonctionnalités

- Ajouter un colis (destinataire, adresse, priorité)
- Livrer le prochain colis (le plus prioritaire)
- Afficher les colis en attente, dans l'ordre de livraison
- Rechercher un colis par son numéro

## Priorités

| Valeur | Type       |
|--------|------------|
| 1      | Urgent     |
| 2      | Normal     |
| 3      | Économique |

À priorité égale, le colis arrivé en premier est livré en premier.

## Principe technique

La file de priorité est implémentée avec un **tas binaire (min-heap)** stocké dans un tableau :

- `enfiler` ajoute le colis à la fin du tableau, puis le fait remonter à sa place (`remonter`).
- `defiler` retire la racine (colis le plus prioritaire), la remplace par le dernier élément, puis le fait descendre (`descendre`).
- La comparaison des colis se fait dans `passe_avant` (priorité, puis ordre d'arrivée).

Capacité maximale : 100 colis.

## Compiler et lancer

Il faut un compilateur C (par exemple `gcc`).

```bash
gcc -Wall -Wextra -o colis main.c
./colis
```

Sous Windows, lancer `colis.exe` après la compilation.

## Exemple d'utilisation

```
===== Gestion de livraison de colis =====
1. Ajouter un colis
2. Livrer le prochain colis
3. Afficher les colis en attente
4. Rechercher un colis
0. Quitter
```

## Idées d'amélioration

- Sauvegarder les colis dans un fichier
- Utiliser un tableau dynamique à la place d'une capacité fixe
- Ajouter la suppression ou la modification d'un colis

## Auteur

Faniry Santatriniaina ANDRIANALISOA, étudiant en informatique à l'ISPM (Madagascar).
