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

 char cNomImgEcrite[750], cNomImgInY[250],cNomImgInCb[250],cNomImgInCr[250]; // input du fichier .pgm
 int nH, nW, nHCb, nWCb,nHCr, nWCr, nTaille;

 if (argc != 4) 
 {
 printf("Usage: ImageInY.pgm ImageInCb.pgm ImageInCr.pgm \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgInY) ; // association avec varaibles
 sscanf (argv[2],"%s",cNomImgInCb) ; // association avec varaibles
 sscanf (argv[3],"%s",cNomImgInCr) ; // association avec varaibles


 OCTET *ImgOut, *ImgInY, *ImgInCb, *ImgInCr; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_pgm(cNomImgInY, &nH, &nW);
 nTaille = nH * nW;
 
lire_nb_lignes_colonnes_image_pgm(cNomImgInCb, &nHCb, &nWCb);
if(nHCb != nH || nWCb != nW){
    printf("Les images Y et Cb n'ont pas la même taille\n");
    exit(1);
}

lire_nb_lignes_colonnes_image_pgm(cNomImgInCr, &nHCr, &nWCr);
if(nHCr != nH || nWCr != nW){
    printf("Les images Y et Cr n'ont pas la même taille\n");
    exit(1);
}
if(nHCr != nHCb || nWCr != nWCb){
    printf("Les images Cb et Cr n'ont pas la même taille\n");
    exit(1);
}
 
 allocation_tableau(ImgInY, OCTET, nTaille);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 
allocation_tableau(ImgInCb, OCTET, nTaille);
allocation_tableau(ImgInCr, OCTET, nTaille);

lire_image_pgm(cNomImgInY, ImgInY, nH * nW);
lire_image_pgm(cNomImgInCb, ImgInCb, nH * nW);
lire_image_pgm(cNomImgInCr, ImgInCr, nH * nW);

allocation_tableau(ImgOut, OCTET, nTaille * 3);

//création du nom de l'image de sortie, qui dois prendre le nom des images d'entrée, en les séparent par des -, et les transformer en .ppm
char cNomImgEcriteY[250];
transformeNomImage(cNomImgInY, "pgm", "", "", cNomImgEcriteY);
char* point1 = strrchr(cNomImgEcriteY, '.');
*point1 = '\0';

char cNomImgEcriteCb[250];
transformeNomImage(cNomImgInCb, "pgm", "", "", cNomImgEcriteCb);
char* point2 = strrchr(cNomImgEcriteCb, '.');
*point2 = '\0';

char cNomImgEcriteCr[250];
transformeNomImage(cNomImgInCr, "pgm", "", "", cNomImgEcriteCr);
char* point3 = strrchr(cNomImgEcriteCr, '.');
*point3 = '\0';

sprintf(cNomImgEcrite, "%s-%s-%s", cNomImgEcriteY, cNomImgEcriteCb, cNomImgEcriteCr);
 



// Les noms pour R, V et B
const char* noms[3] = {"R", "V", "B"};

for (int r = 0; r < 3; r++) {
    for (int v = 0; v < 3; v++) {
        for (int b = 0; b < 3; b++) {
            // Assurez-vous que R, V et B sont tous différents
            if (r != v && r != b && v != b) {
                // Parcourez tous les pixels de l'image
                for (int i = 0; i < nH; i++) {
                    for (int j = 0; j < nW; j++) {
                        int Y = ImgInY[getIndice(i,j,nW,nH)]; 
                        int Cb = ImgInCb[getIndice(i,j,nW,nH)]; 
                        int Cr = ImgInCr[getIndice(i,j,nW,nH)];

                        int formuleR= Y + 1.402 * (Cr - 128);
                        int formuleV= Y -  0.34414 * (Cb - 128) - 0.714414  * (Cr - 128);
                        int formuleB= Y + 1.772 * (Cb - 128);
                        // Les formules pour R, V et B
                        int formules[3];
                        for(int k = 0; k < 3; k++){
                            if(k == 0){
                                formules[k] = formuleR;
                            } else if(k == 1){
                                formules[k] = formuleV;
                            } else if(k == 2){
                                formules[k] = formuleB;
                            }
                        }
                        // Utilisez formules[r], formules[v] et formules[b] pour R, V et B respectivement
                        ImgOut[getIndice(i,j,nW,nH)*3] = std::min(std::max(formules[r],0),255);
                        ImgOut[getIndice(i,j,nW,nH)*3+1] = std::min(std::max(formules[v],0),255);
                        ImgOut[getIndice(i,j,nW,nH)*3+2] = std::min(std::max(formules[b],0),255);
                    }
                }
                // Créez le suffixe basé sur les noms[r], noms[v] et noms[b]
                char suffixe[10];
                sprintf(suffixe, "_%s%s%s", noms[r], noms[v], noms[b]);

                // Transformez le nom de l'image
                char cNomImgEcrite2[250];
                transformeNomImage(cNomImgEcrite, "ppm", "", suffixe, cNomImgEcrite2);

                // Écrivez l'image
                ecrire_image_ppm(cNomImgEcrite2, ImgOut,  nH, nW);
            }
        }
    }
}





free(ImgInY);
free(ImgInCb);
free(ImgInCr);
free(ImgOut);

 return 1;
}
