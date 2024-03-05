#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
using namespace std;

int getIndice(int i, int j, int nW, int nH){
    i=std::min(std::max(i,0),nH-1);
    j=std::min(std::max(j,0),nW-1);
    return 3*(i*nW+j); // Multiplié par 3 car chaque pixel a 3 canaux
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.ppm ImageOut.ppm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, 3*nTaille); // Multiplié par 3 car chaque pixel a 3 canaux
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, 3*nTaille);

 for (int i=0; i < nH; i++)
    {
   for (int j=0; j < nW; j++)
     {
       for(int c=0; c<3; c++){ // Boucle sur les canaux de couleur
        int somme=0;
        for(int dx=-1; dx<=1;dx++){
          for(int dy=-1; dy<=1; dy++){
            somme +=ImgIn[getIndice(i+dx, j+dy, nW, nH)+c]; // Ajoute le canal c du pixel voisin
          }
        }
        somme=((somme)/(9));
        ImgOut[getIndice(i,j,nW,nH)+c]=somme; // Affecte la moyenne au canal c du pixel de sortie
       }
     }
    }
   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn); free(ImgOut);

   return 1;
}