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

 char cNomImgLue1[250],cNomImgLue2[250]; // input du fichier .pgm
 int nH, nW, nH2,nW2, nTaille;

 if (argc != 3) 
 {
 printf("Usage: ImageIn1.ppm ImageIn2.ppm \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgLue1) ; // association avec varaibles
    sscanf (argv[2],"%s",cNomImgLue2) ; // association avec varaibles

 OCTET *ImgIn1, *ImgIn2; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_ppm(cNomImgLue1, &nH, &nW);
 nTaille = nH * nW;
 lire_nb_lignes_colonnes_image_ppm(cNomImgLue2, &nH2, &nW2);
 if(nH!=nH2 || nW!=nW2){
     printf("Les images n'ont pas la même taille\n");
     exit(1);
 }

    int nTaille3=nTaille*3;
 
 
 allocation_tableau(ImgIn1, OCTET, nTaille3);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 
    allocation_tableau(ImgIn2, OCTET, nTaille3);
 lire_image_ppm(cNomImgLue1, ImgIn1, nH * nW);
 lire_image_ppm(cNomImgLue2, ImgIn2, nH * nW);

 
char cNomFichierEcrite[250];
char lesDeux[500];
char* point = strrchr(cNomImgLue1, '.');
if (point != NULL) {
    *point = '\0'; // Terminer la chaîne à l'emplacement du point
}

char* nomImgLue2 = basename(cNomImgLue2); // Retirer le chemin de cNomImgLue2

sprintf(lesDeux, "%s-%s", cNomImgLue1, nomImgLue2);

transformeNomImage(lesDeux, "txt", "EQM-couleur_", "", cNomFichierEcrite);


FILE* erreur_quadratique_moyen = fopen(cNomFichierEcrite, "w");

int moyenneR = 0, moyenneV = 0, moyenneB = 0;
for (int i = 0; i < nH; i++) {
    for (int j = 0; j < nW; j++) {
        int diffR, diffV, diffB;
        diffR = ImgIn1[getIndice(i, j, nW, nH) * 3] - ImgIn2[getIndice(i, j, nW, nH) * 3];
        diffV = ImgIn1[getIndice(i, j, nW, nH) * 3 + 1] - ImgIn2[getIndice(i, j, nW, nH) * 3 + 1];
        diffB = ImgIn1[getIndice(i, j, nW, nH) * 3 + 2] - ImgIn2[getIndice(i, j, nW, nH) * 3 + 2];
        moyenneB += diffB * diffB;
        moyenneV += diffV * diffV;
        moyenneR += diffR * diffR;
    }
}
moyenneR /= nTaille;
moyenneV /= nTaille;
moyenneB /= nTaille;

//enregister dans un fichier ou la première colone sera rouge la seconde verte et la troisième bleu
fprintf(erreur_quadratique_moyen, "%d %d %d\n", moyenneR, moyenneV, moyenneB);

free(ImgIn1);
free(ImgIn2);
return 1;
}
