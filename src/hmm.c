#include "hmm.h"

void free_hmm(Hmm *h)
{
  int i;
  if(h == NULL) return;
  for(i = 0; i < h->nbe; i++){ 
    free(h->T[i]);
    free(h->E[i]);
  }

  free(h->T);
  free(h->E);
  free(h->PI);

  free(h);
}

Hmm *allocate_hmm(int nbe, int nbo)
{
  Hmm *h;
  int i;

  h = malloc(sizeof(Hmm));
  if(h == NULL){
    fprintf(stderr, "erreur d'allocation\n");
    exit(1);
  }
  h->nbe = nbe;
  h->nbo = nbo;

  /* allocation de la matrice de transition */

  h->T = malloc(nbe * sizeof(double *));
  if(h->T == NULL){
    fprintf(stderr, "erreur d'allocation\n");
    exit(1);
  }
  for(i = 0; i < nbe; i++){
    h->T[i] = malloc(nbe * sizeof(double));
    if(h->T[i] == NULL){
      fprintf(stderr, "erreur d'allocation\n");
      exit(1);
    }
  }
  
  /* allocation de la matrice d'emission */

  h->E = malloc(nbe * sizeof(double *));
  if(h->E == NULL){
    fprintf(stderr, "erreur d'allocation\n");
    exit(1);
  }
  for(i = 0; i < nbe; i++){
    h->E[i] = malloc(nbo * sizeof(double));
    if(h->E[i] == NULL){
      fprintf(stderr, "erreur d'allocation\n");
      exit(1);
    }
  }
  
  /* allocation de la matrice des probabilites initiales */
  h->PI = malloc(nbe * sizeof(double));
  if(h->PI == NULL){
    fprintf(stderr, "erreur d'allocation\n");
    exit(1);
  }

  return h;
}

void initialize_hmm(Hmm *h, GlobalData * data)
{
  int  i,j;

	h->data = data;

  for(i=0; i < h->nbe; i++){
    h->PI[i] = 0;
    for(j=0; j < h->nbe; j++) h->T[i][j] = 0;
    for(j=0; j < h->nbo; j++) h->E[i][j] = 0;
  }
}

void print_hmm(Hmm *h, char *file_name)
{
  int  i,j;
  FILE *f = stdout;

  if(file_name)
    f = fopen(file_name, "w");
  else
    f = stdout;

  fprintf(f, "#nb etats\n");
  fprintf(f, "%d\n", h->nbe);
  fprintf(f, "#nb observables\n");
  fprintf(f, "%d\n", h->nbo);
  fprintf(f, "#probabilites initiales\n");
  for(i=0; i < h->nbe; i++){
    fprintf(f, "%f\n", exp(h->PI[i]));
  }

  fprintf(f, "#probabilites de transition\n");
  for(i=0; i < h->nbe; i++){
    for(j=0; j < h->nbe; j++){
      fprintf(f, "%f\n", exp(h->T[i][j]));
    }
  }

  fprintf(f, "#probabilites d'emission\n");
  for(i=0; i < h->nbe; i++){
    for(j=0; j < h->nbo; j++){
      fprintf(f, "%f\n", exp(h->E[i][j]));
    }
  }
  if(file_name)
    fclose(f);
}

void print_hmm_verbose(Hmm *h, char *file_name)
{
  int  i,j;
  FILE *f = stdout;

  if(file_name)
    f = fopen(file_name, "w");
  else
    f = stdout;

  fprintf(f, "#nb etats\n");
  fprintf(f, "%d\n", h->nbe);
  fprintf(f, "#nb observables\n");
  fprintf(f, "%d\n", h->nbo);
  fprintf(f, "#probabilites initiales\n");
  for(i=0; i < h->nbe; i++){
    //fprintf(f, "pi[%d] = %f\n", i, exp(h->PI[i]));
   fprintf(f, "pi[%d] = %f\n", i, h->PI[i]);
  }

  fprintf(f, "#probabilites de transition\n");
  for(i=0; i < h->nbe; i++){
    for(j=0; j < h->nbe; j++){
      //fprintf(f, "T[%d][%d] = %f\n", i, j, exp(h->T[i][j]));
      fprintf(f, "T[%d][%d] = %f\n", i, j, h->T[i][j]);
    }
  }

  fprintf(f, "#probabilites d'emission\n");
  for(i=0; i < h->nbe; i++){
    for(j=0; j < h->nbo; j++){
      //fprintf(f, "E[%d][%d] = %f\n", i, j, exp(h->E[i][j]));
      fprintf(f, "E[%d][%d] = %f\n", i, j, h->E[i][j]);
    }
  }
  if(file_name)
    fclose(f);
}


void print_hmm2(Hmm *h, char *file_name)
{
  int  i,j;
  FILE *f = stdout;

  for(i=0; i < h->nbe; i++){
    fprintf(f, "%f ", exp(h->PI[i]));
  }

  for(i=0; i < h->nbe; i++){
    for(j=0; j < h->nbe; j++){
      fprintf(f, "%f ", exp(h->T[i][j]));
    }
  }
  for(i=0; i < h->nbe; i++){
    for(j=0; j < h->nbo; j++){
      fprintf(f, "%f ", exp(h->E[i][j]));
    }
  }
  printf("\n");
}

void ligne_suivante(FILE *f, char *buff)
{
  do{
    fgets (buff, LONG_LIGNE, f);
  } while(buff[0] == '#');
}

Hmm *load_hmm(char *file_name)
{
  char buff[LONG_LIGNE];
  FILE *f = fopen(file_name, "r");
  Hmm *h;
  int nbe, nbo;
  int i,j;

  if(f == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", file_name);
    exit(1);
  }

  /* lecture du nombre d'etats */
  ligne_suivante(f, buff);
  sscanf(buff, "%d", &nbe);

  /* lecture du nombre de symboles d'emission */
  ligne_suivante(f, buff);
  sscanf(buff, "%d", &nbo);
  
  /* allocation du Hmm */
  h = allocate_hmm(nbe, nbo);

  /* lecture des proba initiales */
  i = 0;
  while(i < nbe){
    ligne_suivante(f, buff);
    sscanf(buff, "%lf", &(h->PI[i]));
    h->PI[i] = log(h->PI[i]);
    i++;
  }

  /* lecture des proba de transition */
  i = 0;
  while(i < nbe){
    j = 0;
    while(j < nbe){
      ligne_suivante(f, buff);
      sscanf(buff, "%lf", &(h->T[i][j]));
      h->T[i][j] = log(h->T[i][j]);
      j++;
    }
    i++;
  }
  

  /* lecture des proba d'emssion */
  i = 0;
  while(i < nbe){
    j = 0;
    while(j < nbo){
      ligne_suivante(f, buff);
      sscanf(buff, "%lf", &(h->E[i][j]));
      h->E[i][j] = log(h->E[i][j]);
      j++;
    }
    i++;
  }

  fclose(f);
  return h;
}


int *charge_observables(char *file_name, int T)
{
  FILE *f = fopen(file_name, "r");
  char buff[LONG_LIGNE];
  int t = 0;
  int *o = malloc(T * sizeof(int));
  
  if(o == NULL){
    fprintf(stderr, "erreur d'allocation\n");
    exit(1);
  }

  if(f == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", file_name);
    exit(1);
  }
  
  while(!feof(f) && t < T){
    fgets (buff, LONG_LIGNE, f);
    sscanf(buff, "%d", &o[t]);
    t++;
  }
  if(t != T){
    fprintf(stderr, "ATTENTION, le fichier \"%s\" contient moins de %d observables !\n", file_name, T);
	for(; t < T; t++)
		o[t] = 0;
  }

  return o;

}
