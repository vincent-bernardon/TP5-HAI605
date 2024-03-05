#include <stdio.h>
#include "image_ppm.h"
#include<iostream>
#include <vector>
using namespace std;
int main(int argc, char* argv[])
{

 char cNomImgLue[250]; // input du fichier .pgm
 int nH, nW, nTaille,valeur_actuelle_gris;

//int iteration[255]; // tableau qui va contenir tout les itération des valeurs de gris
std::vector<int> iteration;

//on rempli notre tableau de 0
for (int i = 0; i < 256; i++){
 iteration.push_back(0);

}
//verification du remplissage
for (int i = 0; i < 256; i++){
 cout<<iteration.at(i);

}




 if (argc != 2) 
 {
 printf("Usage: ImageIn.pgm \n"); 
 exit (1) ;
 }
 
 sscanf (argv[1],"%s",cNomImgLue) ; // association avec varaibles

 OCTET *ImgIn, *ImgOut; // tableau d'octet contenant les couleurs de chaque pixel
 
 lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
 nTaille = nH * nW;

 
 
 allocation_tableau(ImgIn, OCTET, nTaille);// calcul qui permet de représenter les différents pixel de l'imag dans un tableau 1D 


 // ex 0 1 2
 // 3 4 5 ==> 1 2 3 4 5 6 7 8 
 // 6 7 8 


 lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
 allocation_tableau(ImgOut, OCTET, nTaille);
 


 for (int i=0; i < nH; i++)
 for (int j=0; j < nW; j++)
 {
 
 //on parcours notre tableau representant notre image


 valeur_actuelle_gris = ImgIn[i*nW+j];

 iteration.at(valeur_actuelle_gris) = iteration.at(valeur_actuelle_gris)+1;


 

 
 }


 //affichage tableau final

 for (int i = 0; i < 256; i++)
 {
 cout<< i << " " << iteration.at(i) << '\n';

 }

 FILE *histo; 
 histo = fopen("histogram.dat", "wb");
for (int i = 0; i < 256; i++)
 {
    fprintf(histo, "%d %d\n",i,iteration.at(i));


 }



 free(ImgIn); free(ImgOut);
 return 1;
}

