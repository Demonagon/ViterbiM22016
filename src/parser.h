#ifndef __PARSER__
#define __PARSER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 1024

/** Rédigé par Pacôme Perrotin
*
* Ce module fournit toutes le fonctions nécessaires à la lecture des fichiers
* contenant les corpus étudiés.
* On procédera à découper le fichier d'entrée par mots directement, afin
* d'éviter le stockage d'une phrase éventuellement de très grande taille dans
* une chaîne de caractères.
*/

/**
* Fonction qui renvoie la chaîne de caractère la plus longue ne comportant pas
* de retour à la ligne à partir du début du fichier input. Cette chaîne et le
* premier retour à la ligne rencontrés sont consomés dans le flux.
*/

void get_line(FILE * input, char * line);

/**
* Chacune de ces trois fonction analyse la prochaine ligne de texte dans le
* fichier d'entrée, puis retourne le mot, ou l'étiquette, ou les deux, que cette
* ligne contient.
* Si la ligne est vide, c'est que nous passons à une nouvelle phrase. Dans ce
* cas, ces fonctions retournent toutes 0.
*/
int get_next_word(FILE * input);

int get_next_label(FILE * input);

int get_next_labeled_word(FILE * input, int * label);

/**
* La fonction suivante compte le nombre de lignes d'un fichier.
* Cela nous permet, en comptant le nombre de lignes des fichiers voc_etats.txt
* et voc_observables.txt, d'obtenir automatiquement le nombre d'états et
* d'observables.
*/
int get_line_count(FILE * input);

/* Fonctions de test */

void main_test_parsing(char * file_name);
void main_test_line_count();

#endif
