#include <stdio.h>
#include "image_ppm.h"
#include<iostream>
#include <vector>
#include <libgen.h>
#include <fstream> 
#include <cmath>
#include <algorithm>
using namespace std;
using std::ofstream;

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

 char cNomImgEcrite[750], cNomImgInH[250],cNomImgInS[250],cNomImgInL[250]; // input du fichier .pgm
 int nH, nW, nHS, nWS,nHL, nWL, nTaille;

 if (argc != 4) 
 {
 printf("Usage: ImageInH.pgm ImageInS.pgm ImageInL.pgm \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgInH) ; // association avec varaibles
 sscanf (argv[2],"%s",cNomImgInS) ; // association avec varaibles
 sscanf (argv[3],"%s",cNomImgInL) ; // association avec varaibles


 OCTET *ImgOut, *ImgInH, *ImgInS, *ImgInL; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_pgm(cNomImgInH, &nH, &nW);
 nTaille = nH * nW;
 
lire_nb_lignes_colonnes_image_pgm(cNomImgInS, &nHS, &nWS);
if(nHS != nH || nWS != nW){
    printf("Les images L et S n'ont pas la même taille\n");
    exit(1);
}

lire_nb_lignes_colonnes_image_pgm(cNomImgInL, &nHL, &nWL);
if(nHL != nH || nWL != nW){
    printf("Les images H et L n'ont pas la même taille\n");
    exit(1);
}
if(nHL != nHS || nWL != nWS){
    printf("Les images L et S n'ont pas la même taille\n");
    exit(1);
}
 
 allocation_tableau(ImgInH, OCTET, nTaille);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 
allocation_tableau(ImgInS, OCTET, nTaille);
allocation_tableau(ImgInL, OCTET, nTaille);

lire_image_pgm(cNomImgInH, ImgInH, nH * nW);
lire_image_pgm(cNomImgInS, ImgInS, nH * nW);
lire_image_pgm(cNomImgInL, ImgInL, nH * nW);

allocation_tableau(ImgOut, OCTET, nTaille * 3);


 
 //FILE *histo; 
 //histo = fopen("val.txt", "wb");

 for (int i=0; i < nH; i++){
 for (int j=0; j < nW; j++)
 {
    /*
    les calcule vienne de : https://www.rapidtables.org/fr/convert/color/hsl-to-rgb.html
    */
    //initialisation la teinte ou H
    float H= ImgInH[getIndice(i,j,nW,nH)]/ 255.0 * 360.0;; 
    //initialisation la saturation ou S
    float S= ImgInS[getIndice(i,j,nW,nH)]/255.0; 
    //initialisation la luminance ou L
    float L= ImgInL[getIndice(i,j,nW,nH)]/255.0;

    //écrire H S L 
    //fprintf(histo, "%f %f %f\n", H, S, L);
    

    float C = (1.0 - abs(2.0*L - 1.0)) * S;
    float X = C * (1.0 - abs(fmod(H / 60.0, 2.0) - 1.0));
    float m = L - C / 2.0;
    float Rprime, Gprime, Bprime;
    if (H >= 0 && H < 60) {
        Rprime = C;
        Gprime = X;
        Bprime = 0;
    } else if (H >= 60 && H < 120) {
        Rprime = X;
        Gprime = C;
        Bprime = 0;
    } else if (H >= 120 && H < 180) {
        Rprime = 0;
        Gprime = C;
        Bprime = X;
    } else if (H >= 180 && H < 240) {
        Rprime = 0;
        Gprime = X;
        Bprime = C;
    } else if (H >= 240 && H < 300) {
        Rprime = X;
        Gprime = 0;
        Bprime = C;
    } else if (H >= 300 && H < 360) {
        Rprime = C;
        Gprime = 0;
        Bprime = X;
    }
    ImgOut[getIndice(i,j,nW,nH)*3] = (Rprime + m) * 255;
    ImgOut[getIndice(i,j,nW,nH)*3+1] = (Gprime + m) * 255;
    ImgOut[getIndice(i,j,nW,nH)*3+2] = (Bprime + m) * 255;
 }
 }
//création du nom de l'image de sortie, qui dois prendre le nom des images d'entrée, en les séparent par des -, et les transformer en .ppm
char cNomImgEcriteH[250];
transformeNomImage(cNomImgInH, "pgm", "", "", cNomImgEcriteH);
char* point1 = strrchr(cNomImgEcriteH, '.');
*point1 = '\0';

char cNomImgEcriteS[250];
transformeNomImage(cNomImgInS, "pgm", "", "", cNomImgEcriteS);
char* point2 = strrchr(cNomImgEcriteS, '.');
*point2 = '\0';

char cNomImgEcriteL[250];
transformeNomImage(cNomImgInL, "pgm", "", "", cNomImgEcriteL);
char* point3 = strrchr(cNomImgEcriteL, '.');
*point3 = '\0';

sprintf(cNomImgEcrite, "%s-%s-%s", cNomImgEcriteH, cNomImgEcriteS, cNomImgEcriteL);

char cNomImgEcrite2[250];
transformeNomImage(cNomImgEcrite, "ppm", "", "_toRGB", cNomImgEcrite2);


ecrire_image_ppm(cNomImgEcrite2, ImgOut,  nH, nW);

free(ImgInH);
free(ImgInS);
free(ImgInL);
free(ImgOut);

 return 1;
}
