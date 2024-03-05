#include <stdio.h>
#include "image_ppm.h"
#include<iostream>
#include <vector>
#include <libgen.h>
#include <fstream>
#include <cmath>
#include <algorithm>
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

 char cNomImgLue[250], cNomImgOutL[250],cNomImgOutS[250],cNomImgOutH[250]; // input du fichier .pgm
 int nH, nW, nTaille;

 if (argc != 2) 
 {
 printf("Usage: ImageIn.ppm \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgLue) ; // association avec varaibles

 OCTET *ImgIn, *ImgOutL, *ImgOutS, *ImgOutH; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
 nTaille = nH * nW;
 int nTaille3= nTaille*3;

 
 
 allocation_tableau(ImgIn, OCTET, nTaille3);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 

 lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
 allocation_tableau(ImgOutL, OCTET, nTaille);
 allocation_tableau(ImgOutS, OCTET, nTaille);
 allocation_tableau(ImgOutH, OCTET, nTaille);


 


 for (int i=0; i < nH; i++){
 for (int j=0; j < nW; j++)
 {
    //initialisation la teinte ou H
    float H=0;
    //initialisation la luminance ou L
    float L=0;
    //initialisation la saturation ou S
    float S=0;

    /*
    Les calculs effectués dans cette partie du code sont basés sur la formule de conversion RGB vers HSL, 
    telle qu'expliquée sur le site https://www.rapidtables.org/fr/convert/color/rgb-to-hsl.html 
    Cette formule permet de convertir les valeurs de couleur RGB en valeurs de teinte (H), saturation (S) et luminance (L) correspondantes.
    */

    //on prend la valeur de chaque couleur
    float R = (ImgIn[getIndice(i,j,nW,nH)*3])/255.0;
    float G = (ImgIn[getIndice(i,j,nW,nH)*3+1])/255.0;
    float B = (ImgIn[getIndice(i,j,nW,nH)*3+2])/255.0;

    //on calcule la valeur maximale et minimale et le delta
    float Cmax = max(max(R,G),B);
    float Cmin = min(min(R,G),B);
    float delta = Cmax-Cmin;
    //on calcule la teinte
    if(delta==0){ //si delta est nul alors la teinte est nulle
        H=0;
    }
    else if(Cmax==R){ //sinon on calcule la teinte
        H=60*std::fmod((G - B) / delta, 6);
    }
    else if(Cmax==G){
        H=60*((B-R)/delta+2);
    }
    else if(Cmax==B){
        H=60*((R-G)/delta+4);
    }
    //on calcule la luminance
    L=(Cmax+Cmin)/2; //mettre avant car on utilise L dans S
    //on calcule la saturation
    if(delta==0){
        S=0;
    }
    else{
        S = delta / (1 - std::fabs(2*L - 1));
    }

    // Ramenez h, l et s dans leurs plages respectives
    H = std::max(0.0f, std::min(360.0f, H));
    L = std::max(0.0f, std::min(1.0f, L));
    S = std::max(0.0f, std::min(1.0f, S));

    //on enregire la valeur dans ImgOutL
    ImgOutL[getIndice(i,j,nW,nH)] = L * 255;
    ImgOutS[getIndice(i,j,nW,nH)] = S * 255;
    ImgOutH[getIndice(i,j,nW,nH)] = H; 


 }
 }
//on enregistre les valeurs dans des fichiers .pgm
//L
transformeNomImage(cNomImgLue, "pgm", "L_", "", cNomImgOutL);
ecrire_image_pgm(cNomImgOutL, ImgOutL,  nH, nW);
//S
transformeNomImage(cNomImgLue, "pgm", "S_", "", cNomImgOutS);
ecrire_image_pgm(cNomImgOutS, ImgOutS,  nH, nW);
//H
transformeNomImage(cNomImgLue, "pgm", "H_", "", cNomImgOutH);
ecrire_image_pgm(cNomImgOutH, ImgOutH,  nH, nW);

 free(ImgIn); free(ImgOutL); free(ImgOutS); free(ImgOutH);
 return 1;
}