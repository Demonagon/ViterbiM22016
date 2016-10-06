#ifndef __HMM__
#define __HMM__

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
/*#include"matrices.h"*/
/*#include"somme_log.h"*/

#define LONG_LIGNE 100
#define MINUS_INF -999999999.0


typedef struct
{
  int nbe; /* nombre d'etats */
  int nbo; /* nombre d'observables */
  
  double **T; /* matrice de transition T[i][j] = P(j | i) */
              /* probabilite d'aller de l'etat j depuis l'etat i*/
  double **E; /* matrice d'emission E[i][j] = P(j | i)*/
	      /* probabilite d'emettre le symbole j depuis l'etat i*/
  double *PI; /* matrice des probabilites initiales PI[i] = P(X0 = i)*/
              /* probabilite que le Hmm se trouve dans l'etat i au debut */ 
              /* de la generation */
} Hmm;


Hmm *allocate_hmm(int nbe, int nbo);
void initialize_hmm(Hmm *h);
void print_hmm(Hmm *h, char *file_name);
void print_hmm_verbose(Hmm *h, char *file_name);
Hmm *load_hmm(char *file_name);
int *charge_observables(char *file_name, int T);
void free_hmm(Hmm *h);

#endif

