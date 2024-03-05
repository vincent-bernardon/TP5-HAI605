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

 char cNomImgLue[250], cNomImgOutY[250],cNomImgOutCb[250],cNomImgOutCr[250]; // input du fichier .pgm
 int nH, nW, nTaille;

 if (argc != 2) 
 {
 printf("Usage: ImageIn.ppm \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgLue) ; // association avec varaibles

 OCTET *ImgIn, *ImgOutY, *ImgOutCb, *ImgOutCr; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
 nTaille = nH * nW;
 int nTaille3= nTaille*3;

 
 
 allocation_tableau(ImgIn, OCTET, nTaille3);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 

 lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
 allocation_tableau(ImgOutY, OCTET, nTaille);
 allocation_tableau(ImgOutCb, OCTET, nTaille);
 allocation_tableau(ImgOutCr, OCTET, nTaille);


 


 for (int i=0; i < nH; i++){
 for (int j=0; j < nW; j++)
 {
    //on prend la valeur de chaque couleur
    int R = ImgIn[getIndice(i,j,nW,nH)*3];
    int G = ImgIn[getIndice(i,j,nW,nH)*3+1];
    int B = ImgIn[getIndice(i,j,nW,nH)*3+2];
    
    //on fait la conversion Y
    int Y = 0.299*R + 0.587*G + 0.114*B;
    //on enregire la valeur dans ImgOutY
    ImgOutY[getIndice(i,j,nW,nH)]=Y;

    //on fait la conversion Cb
    int Cb = -0.1687*R-0.3313*G+0.5*B+128;
    //on enregire la valeur dans ImgOutCb
    ImgOutCb[getIndice(i,j,nW,nH)]=Cb;

    //on fait la conversion Cr
    int Cr = 0.5*R-0.4187*G-0.0813*B+128;
    //on enregire la valeur dans ImgOutCr
    ImgOutCr[getIndice(i,j,nW,nH)]=Cr;
 }
 }
//on enregistre les valeurs dans des fichiers .pgm
//Y
transformeNomImage(cNomImgLue, "pgm", "Y_", "", cNomImgOutY);
ecrire_image_pgm(cNomImgOutY, ImgOutY,  nH, nW);
//Cb
transformeNomImage(cNomImgLue, "pgm", "Cb_", "", cNomImgOutCb);
ecrire_image_pgm(cNomImgOutCb, ImgOutCb,  nH, nW);
//Cr
transformeNomImage(cNomImgLue, "pgm", "Cr_", "", cNomImgOutCr);
ecrire_image_pgm(cNomImgOutCr, ImgOutCr,  nH, nW);

 free(ImgIn); free(ImgOutY); free(ImgOutCb); free(ImgOutCr);
 return 1;
}

/*
rappel : 
YCbCr est un modèle de couleur utilisé en traitement d'image et vidéo. Il sépare une image en trois composantes :

Y est la composante de luminance, qui représente la luminosité dans l'image.
Cb est la composante de chrominance bleue, qui représente la différence entre la luminance et la couleur bleue.
Cr est la composante de chrominance rouge, qui représente la différence entre la luminance et la couleur rouge.
*/