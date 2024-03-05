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
 int nH, nW,nH2, nW2, nTaille,k,division;

 if (argc != 4) 
 {
 printf("Usage: ImageIn.ppm multiplication?(0 oui 1 == division) k \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgLue) ; // association avec varaibles
 k=atoi(argv[3]);
 division=atoi(argv[2]);

 OCTET *ImgIn, *ImgOut; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    if(division==1){
        nH2=nH/k;
        nW2=nW/k;
    }else{
        nH2=nH*k;
        nW2=nW*k;
    }
 nTaille = nH * nW;
 int nTaille3= nTaille*3;
 int nTaille2= nH2 * nW2;
    int nTaille32= nTaille2*3;

    printf("nH : %d, nW : %d, nH2 : %d, nW2 : %d\n",nH,nW,nH2,nW2);
    printf("nTaille : %d, nTaille3 : %d, nTaille2 : %d, nTaille32 : %d\n",nTaille,nTaille3,nTaille2,nTaille32);


 
 
   allocation_tableau(ImgIn, OCTET, nTaille3); // Multiplié par 3 car chaque pixel a 3 canaux
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille32);
 
if(division){                
    for (int x = 0; x < nW2; x++) {
        for (int y = 0; y < nH2; y++) {
            //on divise la taille de l'image par deux
            //algorithme sous-échantillonnage
            // Calculer la position correspondante dans l'image d'origine
            int origX = x * k;
            int origY = y * k;

            // Copier le pixel
            ImgOut[3*x*nW2+3*y] = ImgIn[3*origX*nW+3*origY];
            ImgOut[3*x*nW2+3*y+1] = ImgIn[3*origX*nW+3*origY+1];
            ImgOut[3*x*nW2+3*y+2] = ImgIn[3*origX*nW+3*origY+2];
        }
    }
}else{
 for (int x=0; x < nH; x++){
 for (int y=0; y < nW; y++)
 {
        for(int i=0;i<k;i++){
            for(int j=0;j<k;j++){
                
                    //on multiplie la taille de l'image par deux
                    ImgOut[3*(k*x+i)*nW2+3*(k*y+j)]=ImgIn[3*x*nW+3*y];
                    ImgOut[3*(k*x+i)*nW2+3*(k*y+j)+1]=ImgIn[3*x*nW+3*y+1];
                    ImgOut[3*(k*x+i)*nW2+3*(k*y+j)+2]=ImgIn[3*x*nW+3*y+2];
                }
            }
        }
 }
    
 }

char cNomImgEcrite[250];
char temp[250];
//faire une chaine de caractère : _nWx_nH
sprintf(temp, "_%dx%d", nW2, nH2);
transformeNomImage(cNomImgLue, "ppm", "resizec_", temp, cNomImgEcrite);
ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH2, nW2);


 free(ImgOut);free(ImgIn);
 return 1;
}

