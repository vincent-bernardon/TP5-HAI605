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
 int nH, nW, nTaille,valeur_actuelle_gris;

 if (argc != 2) 
 {
 printf("Usage: ImageIn.ppm \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgLue) ; // association avec varaibles

 OCTET *ImgIn, *ImgOut; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
 nTaille = nH * nW;
 int nTaille3= nTaille*3;

 
 
 allocation_tableau(ImgIn, OCTET, nTaille3);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 

 lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
 allocation_tableau(ImgOut, OCTET, nTaille);
 


 for (int i=0; i < nH; i++){
 for (int j=0; j < nW; j++)
 {
    //on prend la valeur de chaque couleur
    int R = ImgIn[getIndice(i,j,nW,nH)*3];
    int G = ImgIn[getIndice(i,j,nW,nH)*3+1];
    int B = ImgIn[getIndice(i,j,nW,nH)*3+2];
    //on fait la conversion
    int Y = 0.299*R + 0.587*G + 0.114*B;
    //on enregire la valeur dans ImgOut
    ImgOut[getIndice(i,j,nW,nH)]=Y;
 }
 }
char cNomImgEcrite[250];
transformeNomImage(cNomImgLue, "pgm", "Y_", "", cNomImgEcrite);
//affiche cNomImgEcrite
printf("%s\n",cNomImgEcrite);
ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);


 free(ImgIn); free(ImgOut);
 return 1;
}

