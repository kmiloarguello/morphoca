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


graphe * Erosion(unsigned char *X, struct graphe *g1) {

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

        // When the current vertex is 0 AND the successor is 255
        if (g1->v_sommets[i] == OUTSET && 
            g1->v_sommets[j] == INSET ) {
            g->v_sommets[j] = 1;
            AjouteArcValue(g, i, j, OUTSET);
        } else {
          AjouteArcValue(g, i, j, (TYP_VARC)v);
        }
    }
  }


  for (i = 0; i < nsom; i++) {
    for (p = g->gamma[i]; p != NULL; p = p->next) {
        j = p->som;
        v = p->v_arc;

        if (g->v_sommets[i] == 1) g->v_sommets[i] = 0;
        /*
        if(X[i] == INSET && v == INSET) {
          X[i] = OUTSET;
          RetireArc(g, i, j);
        } */
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
  printf("Erosion started! \n");

  readSeList(argv[2], &tab_es_i, &tab_es_j, &n); /* reading of the structuring element */

  printf("L'e.s. contient %d points \n",n);

  G = Image2Graphe(image,SP_AVG, 4); /* Building of the graph */
  
  printf("Le graphe G contient %d sommets et %d arcs \n", G->nsom, G->narc);
  
  X = UCHARDATA(image); 
  
  G = Erosion(X, G);

  SaveGraphe(G, argv[argc-1]);
  
  imageResult = Graphe2Image(G, rowsize(image));
  writeimage(imageResult, argv[argc-1]);
  freeimage(imageResult);

  printf("Erosion done! \n");

  freeimage(image);
  free(tab_es_i);
  free(tab_es_j);
  TermineGraphe(G);

  return 0;
}
