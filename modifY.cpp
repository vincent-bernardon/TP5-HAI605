#include <stdio.h>
#include "image_ppm.h"
#include<iostream>
#include <vector>
#include <libgen.h>
using namespace std;

void transformeNomImage(char* cheminImage, const char* nouvelleExtension, const char* prefixe, const char* suffixe, char* resultat) {
    // Extraire uniquement le nom du fichier de cheminImage
    char* base = basename(cheminImage);

    // Copier le préfixe dans le résultat
    strcpy(resultat, prefixe);

    // Ajouter le nom du fichier (sans extension) au résultat
    char* point = strrchr(base, '.');
    if (point != NULL) {
        *point = '\0'; // Terminer la chaîne à l'emplacement du point
    }
    strcat(resultat, base);

    // Ajouter le suffixe au résultat
    strcat(resultat, suffixe);

    // Ajouter la nouvelle extension au résultat
    strcat(resultat, ".");
    strcat(resultat, nouvelleExtension);
}

int getIndice(int i, int j, int nW, int nH){
    i=std::min(std::max(i,0),nH-1);
    j=std::min(std::max(j,0),nW-1);
    return i*nW+j;
}

int main(int argc, char* argv[])
{

 char cNomImgLue[250]; // input du fichier .pgm
 int nH, nW, nTaille,valeur_actuelle_gris,k;

 if (argc != 3) 
 {
 printf("Usage: ImageIn.pgm -128<k<128\n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgLue) ; // association avec varaibles
 k=atoi(argv[2]);

 OCTET *ImgIn, *ImgOut; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
 nTaille = nH * nW;

 
 
 allocation_tableau(ImgIn, OCTET, nTaille);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 

 lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
 allocation_tableau(ImgOut, OCTET, nTaille);
 


 for (int i=0; i < nH; i++){
 for (int j=0; j < nW; j++)
 {
    //on fait la conversion
    int formuleY = ImgIn[getIndice(i,j,nW,nH)]+k;
    int Y=std::min(std::max(formuleY,0),255);
    //on enregire la valeur dans ImgOut
    ImgOut[getIndice(i,j,nW,nH)]=Y;
 }
 }
char cNomImgEcrite[250];
char suffixe[10];
sprintf(suffixe, "_%d", k);
transformeNomImage(cNomImgLue, "pgm", "modifY_", suffixe, cNomImgEcrite);
//affiche cNomImgEcrite
printf("%s\n",cNomImgEcrite);
ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);


 free(ImgIn); free(ImgOut);
 return 1;
}

