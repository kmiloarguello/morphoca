#include<stdlib.h>
#include<stdio.h>
#include "graphes.h"
#include "mcimage.h"
#include "mccodimage.h"

#define INSET 255
#define OUTSET 0
#define SP_AVG 0
#define SP_MIN 1
#define SP_MAX 2

/* ====================================================================== */
struct graphe *initImageGraph(struct xvimage *image, 
			      int *tab_es_i, int *tab_es_j, 
			      /* list of successeurs points (coordinates) of the point (0,0) */
			      int n_es){
  
  struct graphe * G;
  uint8_t *F;
  uint8_t sup;
  F = UCHARDATA(image);

  int rs = rowsize(image);     /* row size */
  int cs = colsize(image);     /* column size */
  int n = rs * cs;             /* image size = |E| */
  int m = n * (n_es);          /* |\Gamma| without taking into account the borders */
  
  int x, y;                    /* two points */
  int x_i, x_j, y_i, y_j;      /* coordinates of x and y */

  int k,l;                       /* an index */
  //register index_t k, l;           /* index muet */
  uint8_t *H;

  H = (uint8_t *)calloc(1,n*sizeof(char));
  if (H == NULL)
  {
     fprintf(stderr, "Malloc failed for H\n");
     return(0);
  }

  G = InitGraphe(n, m);

  for(x_j = 0; x_j < cs; x_j++ )
    for(x_i = 0; x_i < rs; x_i++ ){ /* for any vertex (x_i, x_j) de E */

      int p = x_j*rs + x_i;

      G->x[p] = (double)x_i; 
      G->y[p] = (double)x_j; 	// coord sommet
      G->v_sommets[p] = OUTSET;

      int current = p;
      int next = p + 1;

      printf("%lf\n",G->x[p]);

      /*
      if ( p < (n -1) ) {
        printf("%d \n",p);
        AjouteArcValue(G, current, next, OUTSET);
        AjouteArcValue(G, next, current, OUTSET);
      }*/

      if (F[p]) {
        G->v_sommets[p] = 1;

        if ((x_i < (rs-1)) && F[x_j*rs+x_i+1])
        {
          printf("A: %d\n",p);
          AjouteArcValue(G, p, p+1, INSET);
          AjouteArcValue(G, p+1, p, INSET);
        }
        if ((x_j < (cs-1)) && F[(x_j+1)*rs+x_i])
        {
          printf("B: %d\n",p);
          AjouteArcValue(G, p, p+rs, INSET);
          AjouteArcValue(G, p+rs, p, INSET);
        }
      }
    }
  
  free(H);

  return G;
} /* initImageGraph() */


/* ====================================================================== */
/*! \fn graphe * Image2Graphe(struct xvimage *image, int32_t mode, int32_t connex)
    \param image (entr�e) : structure image
    \param mode (entr�e) : mode de valuation des arcs
    \param connex (entr�e) : relation d'adjacence (4 ou 8 en 2D, 6 ou 26 en 3D)
    \return un graphe.
    \brief Lit les donn�es d'une image dans la structure image, 
    retourne un pointeur sur la structure graphe construite. 
*/
graphe * Image2Graphe(struct xvimage *image, int32_t mode, int32_t connex)
/* ====================================================================== */
/*
A weighted graph G = (V,E) is built with V = set of pixels of \b image
and E = {(P,Q) in VxV ; P and Q are adjacent}. 
Let F(P) be the value of pixel P in \b image. 
A weight W(P,Q) is assigned to each edge, according to the value of \b mode:

\li max : W(P,Q) = max{F(P),F(Q)} 
\li min : W(P,Q) = min{F(P),F(Q)} 
\li avg : W(P,Q) = (F(P) + F(Q)) / 2 
*/
#undef F_NAME
#define F_NAME "Image2Graphe"
{
  int32_t rs, cs, ds, N, M;
  uint8_t *F;
  graphe * g;
  int32_t i, j, p;
  TYP_VARC v;

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: bad data type (only byte is available)\n", F_NAME);
    return NULL;
  }
  
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  F = UCHARDATA(image);

  if ((ds == 1) && (connex == 4))
  {
    N = rs * cs;
    M = 2 * ((rs-1) * cs + (cs-1) * rs); 
    g = InitGraphe(N, M);

    for (j = 0; j < cs; j++)  
      for (i = 0; i < rs; i++)  
      {
        p = j*rs + i;
        g->x[p] = (double)i; 
        g->y[p] = (double)j; 	// coord sommet
        g->v_sommets[p] = F[p];

        if (i < (rs-1)) 
          {
          switch(mode)
          {
            case SP_MIN: v = mcmin((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
            case SP_MAX: v = mcmax((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
            case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[j*rs+i+1]) / 2; break;
          }

          if (v==127){
            v = 0;
          }
          
          AjouteArcValue(g, p, p+1, v);
          AjouteArcValue(g, p+1, p, v);
        }
        if (j < (cs-1))
        {
          switch(mode)
          {
            case SP_MIN: v = mcmin((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
            case SP_MAX: v = mcmax((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
            case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j+1)*rs+i]) / 2; break;
          }
          if (v==127){
            v = 0;
          }
          
          AjouteArcValue(g, p, p+rs, v);
          AjouteArcValue(g, p+rs, p, v);
        }
      }
  }
  else if ((ds == 1) && (connex == 8))
  {
    N = rs * cs;
    M = 4 * ((rs-1) * cs + (cs-1) * rs); 
    g = InitGraphe(N, M);
    for (j = 0; j < cs; j++)  
      for (i = 0; i < rs; i++)  
      {
	p = j*rs + i;
        g->x[p] = (double)i; 
        g->y[p] = (double)j; 	// coord sommet
	if (i < (rs-1)) 
	{
	  switch(mode)
	  {
	  case SP_MIN: v = mcmin((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
	  case SP_MAX: v = mcmax((TYP_VARC)F[j*rs+i],(TYP_VARC)F[j*rs+i+1]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[j*rs+i+1]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+1, v);
	  AjouteArcValue(g, p+1, p, v);
	}
	if (j < (cs-1))
	{
	  switch(mode)
	  {
	  case SP_MIN: v = mcmin((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
	  case SP_MAX: v = mcmax((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j+1)*rs+i]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+rs, v);
	  AjouteArcValue(g, p+rs, p, v);
	}
	if ((i < (rs-1)) && (j < (cs-1)))
	{
	  switch(mode)
	  {
	  case SP_MIN: v = mcmin((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i+1]); break;
	  case SP_MAX: v = mcmax((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j+1)*rs+i+1]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j+1)*rs+i+1]) / 2; break;
	  }
	  AjouteArcValue(g, p, p+rs+1, v);
	  AjouteArcValue(g, p+rs+1, p, v);
	}
	if ((i < (rs-1)) && (j > 0))
	{
	  switch(mode)
	  {
	  case SP_MIN: v = mcmin((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j-1)*rs+i+1]); break;
	  case SP_MAX: v = mcmax((TYP_VARC)F[j*rs+i],(TYP_VARC)F[(j-1)*rs+i+1]); break;
	  case SP_AVG: v = ((TYP_VARC)F[j*rs+i]+(TYP_VARC)F[(j-1)*rs+i+1]) / 2; break;
	  }
	  AjouteArcValue(g, p, p-rs+1, v);
	  AjouteArcValue(g, p-rs+1, p, v);
	}
      }
  }
  else
  {
    fprintf(stderr, "%s: 3D not yet implemented\n", F_NAME);
    return NULL;
  }

  return g;
} /* Image2Graphe() */


graphe * Dilation(unsigned char *X, struct graphe *g1) {

  graphe * g;
  int32_t i, j;
  TYP_VARC v;
  int32_t nsom = g1->nsom;
  int32_t nmaxarc = g1->nmaxarc;
  pcell p;

  g = InitGraphe(nsom, nmaxarc);

  if (g1->v_sommets)
    for (i = 0; i < nsom; i++) 
      g->v_sommets[i] = g1->v_sommets[i];

  if (g1->x)
    for (i = 0; i < nsom; i++) 
    {
      g->x[i] = g1->x[i];
      g->y[i] = g1->y[i];
      g->z[i] = g1->z[i];
    }

  for (i = 0; i < nsom; i++) {

    for (p = g1->gamma[i]; p != NULL; p = p->next) {
        j = p->som;
        v = p->v_arc;
        
        // When the current vertex is 255 AND the successor is 0
        if (X[i] == INSET && v != INSET) {
          AjouteArcValue(g, i, j, INSET);
          g->v_sommets[j] = INSET;
        } else { // Otherwise add the same value
          AjouteArcValue(g, i, j, (TYP_VARC)v);
        }
      }
  }
  return g;


}


/* ====================================================================== */
/*! \fn struct xvimage *Graphe2Image(graphe * g, int32_t rs)
    \param g (entr�e) : un graphe
    \return une structure image
    \brief g�n�re une image � partir du graphe g 
    (d'apr�s les valeurs associ�es aux sommets) 
    et retourne un pointeur sur la structure image construite. 
*/
struct xvimage *Graphe2Image(graphe * g, int32_t rs)
/* ====================================================================== */
/*

*/
#undef F_NAME
#define F_NAME "Graphe2Image"
{
  int32_t N;
  struct xvimage *image;
  uint8_t *F;
  int32_t i;

  N = g->nsom;
  if (N % rs != 0)
  {
    fprintf(stderr, "%s: rs and g->nsom must be congruent\n", F_NAME);
    return NULL;
  }

  image = allocimage(NULL, rs, N / rs, 1, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    exit(1);
  }
  F = UCHARDATA(image);
  memset(F, 0, N);
  
  for (i = 0; i < N; i++) F[i] = (uint8_t)g->v_sommets[i]; 

  return image;
} /* Graphe2Image() */



unsigned char *dilat(unsigned char *X, struct graphe *G){
  unsigned char *Y;
  int x, y;         /* two vertices */
  pcell p;
  int n = G->nsom;  /* number of elements of the space E */

  Y = calloc(1, n * sizeof(unsigned char));
  if(Y == NULL){
    fprintf(stderr, "Erreur d'allocation mémoire dans la fonction dilat\n");
    exit(0);
  }
    
  memset(Y, OUTSET ,G->nsom * sizeof(unsigned char)); /* Y is initialized to the emptyset */
    
  for(x = 0; x < n; x++){
    if(X[x] == INSET){ /* the vertex x belongs to X */
      for(p = G->gamma[x]; p != NULL; p = p->next){
        y = p->som;
        Y[y] = INSET;
      }
    }
  }

  return Y;
}


int main(int argc, char ** argv){
  struct xvimage * image;
  struct xvimage *imageResult;

  int *tab_es_i;               /* list of the first coordinates of the points in the s.e. */
  int *tab_es_j;               /* list of the second coordinates of the points in the s.e. */
  int n;                       /* Number of vertices in the structuring element */
  
  unsigned char *X;                      /* the subset of the space (the image domain) that we want to dilate */
  unsigned char *Y;                      /* the dilation of X */

  struct graphe * G;                     /* the s.e. represented as a graph */

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s im.pgm el.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]); /* reading of the image */
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  readSeList(argv[2], &tab_es_i, &tab_es_j, &n); /* reading of the structuring element */

  printf("L'e.s. contient %d points \n",n);

  // G = initImageGraph(image, tab_es_i, tab_es_j, n); /* Building of the graph */
  G = Image2Graphe(image,SP_AVG, 4); 
  
  printf("Le graphe G contient %d sommets et %d arcs \n", G->nsom, G->narc);
  
  X = UCHARDATA(image); 
  
  G = Dilation(X, G);

  SaveGraphe(G, argv[argc-1]);
  
  imageResult = Graphe2Image(G, rowsize(image));
  writeimage(imageResult, argv[argc-1]);
  freeimage(imageResult);

  freeimage(image);
  free(tab_es_i);
  free(tab_es_j);
  TermineGraphe(G);

  return 0;
}



// sup = NDG_MIN;

// printf("--------------------------------------------------------------------------------");

// // Dilation
// for(k = 0; k < n_es; k++) { /* for any successor (y_i,y_j) according to the se */
//   y_j = x_j + tab_es_j[k];
//   y_i = x_i + tab_es_i[k];

//   printf("x_i %d \n", x_i);
//   printf("x_i %d \n", x_i);
//   printf("y_i %d \n", y_i);
//   printf("y_j %d \n", y_j);
//   printf("H %d \n", H[y_j * rs + y_i]);

//   if ((y_j >= 0) && (y_j < cs) && (y_i >= 0) && (y_i < rs) && (H[y_j * rs + y_i] > sup)) {
//     sup = H[y_j * rs + y_i];
//     printf("Sup %d \n", sup);
//   }    
// }

// F[p] = sup;