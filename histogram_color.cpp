// plot 'data.txt' using 2 with lines lt rgb 'red' title 'Rouge', '' using 3 with lines lt rgb 'green' title 'Vert', '' using 4 with lines lt rgb 'blue' title 'Bleu'"


#include <stdio.h>
#include "image_ppm.h"
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
void ecrire_histo(int occurence[256], char* nomImage)
{
    FILE *f_image;
    char nom[250];
    transformeNomImage(nomImage, "dat", "", "", nom);
    if ((f_image = fopen(nom, "wb")) == NULL)
    {
        printf("\nPas d'acces en ecriture sur l'image %s \n", nom);
        exit(EXIT_FAILURE);
    }
    else
    {

        for (int i = 0; i < 256; i++)
        {
            printf("%d %d %d %d\n", i, occurence[i], occurence[i + 256], occurence[i + 256 * 2]);
            fprintf(f_image, "%d %d %d %d\n", i, occurence[i], occurence[i + 256], occurence[i + 256 * 2]);
        }

        fclose(f_image);
    }
}

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nbOccurence[256 * 3]; // nombre d'occurence d'un certain pixel
    int nH, nW, nTaille, S1, S2;

    if (argc != 2)
    {
        printf("Usage: ImageIn.ppm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);

    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, 3 * nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    for (int i = 0; i < 256; i++)
    {
        //couleur
        nbOccurence[i] = 0; // couleur rouge
        nbOccurence[i + 255] = 0; // couleur vert
        nbOccurence[i + 256 * 2] = 0; // couleur bleu
    }

    for (int i = 0; i < nTaille * 3; i += 3)
    {

        nbOccurence[ImgIn[i]]++;
        nbOccurence[ImgIn[i + 1] + 256]++;
        nbOccurence[ImgIn[i + 2] + 256 * 2]++;
    }

    ecrire_histo(nbOccurence, cNomImgLue);
    free(ImgIn);

    return 1;
}