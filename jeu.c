#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>

#include "constantes.h"
#include "jeu.h"

void creerBloc(int *perdu,int carte[][NB_BLOCS_HAUTEUR],int tableauCoord[][2] , Bloc blocCouleur, int* forme, SDL_Rect *limiteH, SDL_Rect *limiteV, int *sens);
void deplacerBloc(int carte[][NB_BLOCS_HAUTEUR], int tableauCoord[][2], int forme, int direction, Bloc blocCouleur, SDL_Rect *limiteH, int *sens);
void chuteDuBloc(int carte[][NB_BLOCS_HAUTEUR],int tableauCoord[][2] ,Bloc blocCouleur, int forme, SDL_Rect *limiteV, int *formePosee, int *perdu, int *sens);
void rotationBloc(int carte[][NB_BLOCS_HAUTEUR],int tableauCoord[][2] ,Bloc blocCouleur, int forme, SDL_Rect *limiteV, SDL_Rect *limiteH, int *formePosee, int *perdu, int *sens);
void afficherInfoPro(int affBloc[][2], int formePro);
void ligneDetruite(int carte[][NB_BLOCS_HAUTEUR], int ligneDestroy, int nbLigneDetruiteEnUnBloc);

void jouer(SDL_Surface *ecran, int *continuer)
{
    //// DECLARATION DE TOUTES LES VARIABLES ////
    SDL_Surface *fond = NULL, *jaune = NULL, *bleu = NULL, *vert = NULL, *violet = NULL, *rouge = NULL, *blanc = NULL, *particule=NULL, *texte = NULL;
    SDL_Event event;
    SDL_Rect position, limiteH, limiteV, positionTexte;
    int perdu=1, formePosee=1, couleur=0, couleurPro=0, forme=0, formePro=0, direction=0; ////formePosee A 1: signifie que le bloc est pos�e on en fait spawn un autre////
    int tableauCoord[4][2];
    Bloc blocCouleur;
    int i=0,j=0,e=0, disp=0, sens=1, blocDel = 0, aff=0, dispBloc=0, nbLigneDetruite=0, nbLigneDetruiteEnUnBloc=0;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR];
    int affBloc[4][2];
    int tpsActu=0, tpsPre=0, ligneDestroy=0;
    char nbligne[4]="";


    ////// INITIALISATION DES IMAGES /////
    fond = IMG_Load("fond.png");
    jaune = IMG_Load("Bloc_jaune.png");
    bleu = IMG_Load("Bloc_bleu.png");
    vert = IMG_Load("Bloc_vert.png");
    violet = IMG_Load("Bloc_violet.png");
    rouge = IMG_Load("Bloc_rouge.png");
    blanc = IMG_Load("Bloc_blanc.png");
    particule = IMG_Load("Particule_bombe.png");

    ////NETTOYAGE DE LA CARTE DE JEU////
    for(i=0;i<NB_BLOCS_HAUTEUR;i++)
        {
            for(j=0;j<NB_BLOCS_LARGEUR;j++)
            {
                carte[j][i]=VIDE;
            }
        }

    TTF_Font *police = NULL;
    SDL_Color color_noire = {0,0,0};
    police = TTF_OpenFont("PressStart2P.ttf",32);

    sprintf(nbligne, "%d", nbLigneDetruite);
    texte = TTF_RenderText_Solid(police, nbligne, color_noire);

    positionTexte.x=525;
    positionTexte.y=600;

    couleur = (rand() % (5 - 1 + 1)) + 1;   ////CHOIX DE LA COULEUR////
    forme = (rand() % (7 - 1 + 1)) + 1;

    //// BOUCLE PRINCIPALE DU JEU ////
    while(perdu && *continuer)
    {
        position.x = 0;
        position.y = 0;

        //// EFFACEMENT ET INITIALISATION DU PLATEAU DE JEU ////
        SDL_BlitSurface(fond,NULL,ecran,&position);
        SDL_BlitSurface(texte,NULL,ecran, &positionTexte);
        ///// SI ON A DEPOSER LE BLOC ON EN CREER UN AUTRE /////
        if(formePosee==1)
        {
            if(dispBloc==1)
            {
                forme=formePro;
                couleur=couleurPro;
            }
            else
            {
                dispBloc=1;
            }

            sens=1;
            switch(couleur)
            {
                case 1:
                    blocCouleur= JAUNE;
                    formePosee=0;
                    break;

                case 2:
                    blocCouleur= BLEU;
                    formePosee=0;
                    break;

                case 3:
                    blocCouleur= VERT;
                    formePosee=0;
                    break;

                case 4:
                    blocCouleur= VIOLET;
                    formePosee=0;
                    break;

                case 5:
                    blocCouleur= ROUGE;
                    formePosee=0;
                    break;
            }

            creerBloc(&perdu,carte,tableauCoord, blocCouleur,&forme, &limiteH, &limiteV, &sens);      //// CREATION DU BLOC ////

            formePro= (rand() % (7 - 1 + 1)) + 1;
            couleurPro= (rand() % (5 - 1 + 1)) + 1;

            position.x = 0;
            position.y = 0;

            limiteV.x=2;
            disp=1;
        }

        SDL_Delay(85);

        afficherInfoPro(affBloc,formePro);

        SDL_PollEvent(&event); ////SI ON APPUYE SUR UNE TOUCHE ... ////
        switch(event.type)
        {
            case SDL_QUIT:
                *continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        *continuer = 0;
                        break;

                    case SDLK_LEFT:
                        direction = GAUCHE;
                        deplacerBloc(carte,tableauCoord,forme,direction,blocCouleur,&limiteH, &sens);
                        break;

                    case SDLK_a:
                        direction = GAUCHE;
                        deplacerBloc(carte,tableauCoord,forme,direction,blocCouleur,&limiteH, &sens);
                        break;

                    case SDLK_RIGHT:
                        direction = DROITE;
                        deplacerBloc(carte,tableauCoord,forme,direction,blocCouleur, &limiteH, &sens);
                        break;

                    case SDLK_d:
                        direction = DROITE;
                        deplacerBloc(carte,tableauCoord,forme,direction,blocCouleur, &limiteH, &sens);
                        break;

                    case SDLK_DOWN:
                        if(disp == 0)
                        {
                            chuteDuBloc(carte,tableauCoord ,blocCouleur, forme, &limiteV, &formePosee, &perdu, &sens);
                        }
                        break;

                    case SDLK_s:
                        if(disp == 0)
                        {
                            chuteDuBloc(carte,tableauCoord ,blocCouleur, forme, &limiteV, &formePosee, &perdu, &sens);
                        }
                        break;

                    case SDLK_UP:
                        if(disp == 0)
                        {
                            rotationBloc(carte, tableauCoord ,blocCouleur, forme, &limiteV, &limiteH, &formePosee, &perdu, &sens);
                        }
                        break;

                    case SDLK_w:
                        if(disp == 0)
                        {
                            rotationBloc(carte, tableauCoord ,blocCouleur, forme, &limiteV, &limiteH, &formePosee, &perdu, &sens);
                        }
                        break;
                }
        }

        ////FAIRE TOMBER LE BLOC TOUTE LES X SECONDES ////
        tpsActu = SDL_GetTicks();
        if(tpsActu - tpsPre > 500)
        {
            chuteDuBloc(carte,tableauCoord ,blocCouleur, forme, &limiteV, &formePosee, &perdu, &sens);
            tpsPre=tpsActu;
        }

        //// AFFICHAGE DES BLOCS SUR PLATEAU DE JEU /////
        for(i=0;i<NB_BLOCS_HAUTEUR;i++)
        {
            for(j=0;j<NB_BLOCS_LARGEUR;j++)
            {
                if(carte[j][i]==VIDE)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                }
                else if(carte[j][i]==JAUNE)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    SDL_BlitSurface(jaune,NULL,ecran,&position);
                    blocDel ++;
                }
                else if(carte[j][i]==BLEU)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    SDL_BlitSurface(bleu,NULL,ecran,&position);
                    blocDel ++;
                }
                else if(carte[j][i]==VERT)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    SDL_BlitSurface(vert,NULL,ecran,&position);
                    blocDel ++;
                }
                else if(carte[j][i]==VIOLET)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    SDL_BlitSurface(violet,NULL,ecran,&position);
                    blocDel ++;
                }
                else if(carte[j][i]==ROUGE)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    SDL_BlitSurface(rouge,NULL,ecran,&position);
                    blocDel ++;
                }
                else if(carte[j][i]==ROUGE)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    SDL_BlitSurface(rouge,NULL,ecran,&position);
                    blocDel ++;
                }
                else if(carte[j][i]==BOMBE)
                {
                    position.x= j * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    SDL_BlitSurface(particule,NULL,ecran,&position);
                }
            }

            if(blocDel == NB_BLOCS_LARGEUR && formePosee == 1)
            {

                for(e=0;e<NB_BLOCS_LARGEUR;e++)
                {
                    position.x= e * TAILLE_BLOCS;
                    position.y= i * TAILLE_BLOCS;
                    ligneDestroy = i;
                    SDL_BlitSurface(blanc,NULL,ecran,&position);
                }
                for(e=0;e<NB_BLOCS_LARGEUR;e++)
                {
                    carte[e][i]=VIDE;
                }
                aff=1;
                nbLigneDetruite++;
                nbLigneDetruiteEnUnBloc++;
            }
            blocDel =0;
        }

        //// AFFICHAGE DU PROCHAIN BLOC EN HAUT A DROITE ////
        for(i=0;i<4;i++)
        {
            position.x=affBloc[i][0]* TAILLE_BLOCS + 440;
            position.y=affBloc[i][1]* TAILLE_BLOCS;

            switch(couleurPro)
            {
                case 1:
                    SDL_BlitSurface(jaune, NULL, ecran, &position);
                    break;

                case 2:
                    SDL_BlitSurface(bleu, NULL, ecran, &position);
                    break;

                case 3:
                    SDL_BlitSurface(vert, NULL, ecran, &position);
                    break;

                case 4:
                    SDL_BlitSurface(violet, NULL, ecran, &position);
                    break;

                case 5:
                    SDL_BlitSurface(rouge, NULL, ecran, &position);
                    break;
            }
        }

        //// RAFRAICHISSEMENT DE L'ECRAN ////
        SDL_Flip(ecran);

        //// SI ON A DETRUIT UNE LIGNE ON AJOUTE UN DELAY////
        if(aff==1)
        {
            SDL_Delay(550);
            aff=0;
            if(nbLigneDetruite >= 10)
            {
                positionTexte.x=510;
            }
            sprintf(nbligne, "%d", nbLigneDetruite);
            SDL_FreeSurface(texte);
            texte = TTF_RenderText_Solid(police, nbligne, color_noire);
            ligneDetruite(carte, ligneDestroy, nbLigneDetruiteEnUnBloc);
            nbLigneDetruiteEnUnBloc=0;
        }
        disp =0;
    }

    if(perdu ==0)
    {
        SDL_Delay(1000);
    }
    if(*continuer ==0 && event.key.keysym.sym == SDLK_ESCAPE)
    {
        *continuer=1;
    }

    ////LIBERATION DE LA MEMOIRE////
    TTF_CloseFont(police);

    SDL_FreeSurface(fond);
    SDL_FreeSurface(jaune);
    SDL_FreeSurface(bleu);
    SDL_FreeSurface(vert);
    SDL_FreeSurface(violet);
    SDL_FreeSurface(rouge);
    SDL_FreeSurface(blanc);
    SDL_FreeSurface(texte);

}

void creerBloc(int *perdu,int carte[][NB_BLOCS_HAUTEUR],int tableauCoord[][2] , Bloc blocCouleur, int* forme, SDL_Rect *limiteH, SDL_Rect *limiteV, int *sens)
{
    switch(*forme)
    {
        case 1: /////FORME 'T' /////

            tableauCoord[0][0]=4;
            tableauCoord[0][1]=1;
            tableauCoord[1][0]=5;
            tableauCoord[1][1]=1;
            tableauCoord[2][0]=6;
            tableauCoord[2][1]=1;
            tableauCoord[3][0]=5;
            tableauCoord[3][1]=2;

            if(carte[tableauCoord[0][0]][tableauCoord[0][1]]!=VIDE)
            {
                carte[tableauCoord[0][0]][tableauCoord[0][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[4][1]=blocCouleur;
            }
            if(carte[tableauCoord[1][0]][tableauCoord[1][1]]!=VIDE)
            {
                carte[tableauCoord[1][0]][tableauCoord[1][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][1]=blocCouleur;
            }
            if(carte[tableauCoord[2][0]][tableauCoord[2][1]]!=VIDE)
            {
                carte[tableauCoord[2][0]][tableauCoord[2][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[6][1]=blocCouleur;
            }
            if(carte[tableauCoord[3][0]][tableauCoord[3][1]]!=VIDE)
            {
                carte[tableauCoord[3][0]][tableauCoord[3][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][2]=blocCouleur;
            }

            limiteH->x=4;
            limiteH->y=6;

            break;

        case 2: /////FORME 'L1' /////

            tableauCoord[0][0]=6;
            tableauCoord[0][1]=1;
            tableauCoord[1][0]=4;
            tableauCoord[1][1]=2;
            tableauCoord[2][0]=5;
            tableauCoord[2][1]=2;
            tableauCoord[3][0]=6;
            tableauCoord[3][1]=2;

            if(carte[tableauCoord[0][0]][tableauCoord[0][1]]!=VIDE)
            {
                carte[tableauCoord[0][0]][tableauCoord[0][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[6][1]=blocCouleur;
            }
            if(carte[tableauCoord[1][0]][tableauCoord[1][1]]!=VIDE)
            {
                carte[tableauCoord[1][0]][tableauCoord[1][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[4][2]=blocCouleur;
            }
            if(carte[tableauCoord[2][0]][tableauCoord[2][1]]!=VIDE)
            {
                carte[tableauCoord[2][0]][tableauCoord[2][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][2]=blocCouleur;
            }
            if(carte[tableauCoord[3][0]][tableauCoord[3][1]]!=VIDE)
            {
                carte[tableauCoord[3][0]][tableauCoord[3][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[6][2]=blocCouleur;
            }

            limiteH->x=4;
            limiteH->y=6;
            break;

        case 3: /////FORME 'L2' /////

            tableauCoord[0][0]=4;
            tableauCoord[0][1]=1;
            tableauCoord[1][0]=4;
            tableauCoord[1][1]=2;
            tableauCoord[2][0]=6;
            tableauCoord[2][1]=2;
            tableauCoord[3][0]=5;
            tableauCoord[3][1]=2;

            if(carte[tableauCoord[0][0]][tableauCoord[0][1]]!=VIDE)
            {
                carte[tableauCoord[0][0]][tableauCoord[0][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[4][1]=blocCouleur;
            }
            if(carte[tableauCoord[1][0]][tableauCoord[1][1]]!=VIDE)
            {
                carte[tableauCoord[1][0]][tableauCoord[1][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[4][2]=blocCouleur;
            }
            if(carte[tableauCoord[2][0]][tableauCoord[2][1]]!=VIDE)
            {
                carte[tableauCoord[2][0]][tableauCoord[2][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[6][2]=blocCouleur;
            }
            if(carte[tableauCoord[3][0]][tableauCoord[3][1]]!=VIDE)
            {
                carte[tableauCoord[3][0]][tableauCoord[3][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][2]=blocCouleur;
            }

            limiteH->x=4;
            limiteH->y=6;
            break;

        case 4: /////FORME 'Z1' /////

            tableauCoord[0][0]=4;
            tableauCoord[0][1]=1;
            tableauCoord[1][0]=5;
            tableauCoord[1][1]=1;
            tableauCoord[2][0]=5;
            tableauCoord[2][1]=2;
            tableauCoord[3][0]=6;
            tableauCoord[3][1]=2;

            if(carte[tableauCoord[0][0]][tableauCoord[0][1]]!=VIDE)
            {
                carte[tableauCoord[0][0]][tableauCoord[0][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[4][1]=blocCouleur;
            }
            if(carte[tableauCoord[1][0]][tableauCoord[1][1]]!=VIDE)
            {
                carte[tableauCoord[1][0]][tableauCoord[1][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[5][1]=blocCouleur;
            }
            if(carte[tableauCoord[2][0]][tableauCoord[2][1]]!=VIDE)
            {
                carte[tableauCoord[2][0]][tableauCoord[2][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[5][2]=blocCouleur;
            }
            if(carte[tableauCoord[3][0]][tableauCoord[3][1]]!=VIDE)
            {
                carte[tableauCoord[3][0]][tableauCoord[3][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[6][2]=blocCouleur;
            }

            limiteH->x=4;
            limiteH->y=6;
            break;

        case 5: /////FORME 'Z2' /////

            tableauCoord[0][0]=5;
            tableauCoord[0][1]=1;
            tableauCoord[1][0]=6;
            tableauCoord[1][1]=1;
            tableauCoord[2][0]=4;
            tableauCoord[2][1]=2;
            tableauCoord[3][0]=5;
            tableauCoord[3][1]=2;

            if(carte[tableauCoord[0][0]][tableauCoord[0][1]]!=VIDE)
            {
                carte[tableauCoord[0][0]][tableauCoord[0][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][1]=blocCouleur;
            }
            if(carte[tableauCoord[1][0]][tableauCoord[1][1]]!=VIDE)
            {
                carte[tableauCoord[1][0]][tableauCoord[1][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[6][1]=blocCouleur;
            }
            if(carte[tableauCoord[2][0]][tableauCoord[2][1]]!=VIDE)
            {
                carte[tableauCoord[2][0]][tableauCoord[2][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[4][2]=blocCouleur;
            }
            if(carte[tableauCoord[3][0]][tableauCoord[3][1]]!=VIDE)
            {
                carte[tableauCoord[3][0]][tableauCoord[3][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][2]=blocCouleur;
            }

            limiteH->x=4;
            limiteH->y=6;

            break;

        case 6: /////FORME 'CUBE' /////

            tableauCoord[0][0]=4;
            tableauCoord[0][1]=1;
            tableauCoord[1][0]=5;
            tableauCoord[1][1]=1;
            tableauCoord[2][0]=4;
            tableauCoord[2][1]=2;
            tableauCoord[3][0]=5;
            tableauCoord[3][1]=2;

            if(carte[tableauCoord[0][0]][tableauCoord[0][1]]!=VIDE)
            {
                carte[tableauCoord[0][0]][tableauCoord[0][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[4][1]=blocCouleur;
            }
            if(carte[tableauCoord[1][0]][tableauCoord[1][1]]!=VIDE)
            {
                carte[tableauCoord[1][0]][tableauCoord[1][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][1]=blocCouleur;
            }
            if(carte[tableauCoord[2][0]][tableauCoord[2][1]]!=VIDE)
            {
                carte[tableauCoord[2][0]][tableauCoord[2][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[4][2]=blocCouleur;
            }
            if(carte[tableauCoord[3][0]][tableauCoord[3][1]]!=VIDE)
            {
                carte[tableauCoord[3][0]][tableauCoord[3][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                 carte[5][2]=blocCouleur;
            }

            limiteH->x=4;
            limiteH->y=5;
            break;

        case 7: /////FORME 'TRAIT' /////

            tableauCoord[0][0]=4;
            tableauCoord[0][1]=2;
            tableauCoord[1][0]=5;
            tableauCoord[1][1]=2;
            tableauCoord[2][0]=6;
            tableauCoord[2][1]=2;
            tableauCoord[3][0]=7;
            tableauCoord[3][1]=2;

            ////Y a t-il un bloc d�j� existants ?////
            if(carte[tableauCoord[0][0]][tableauCoord[0][1]]!=VIDE)
            {
                carte[tableauCoord[0][0]][tableauCoord[0][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[4][2]=blocCouleur;
            }
            if(carte[tableauCoord[1][0]][tableauCoord[1][1]]!=VIDE)
            {
                carte[tableauCoord[1][0]][tableauCoord[1][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[5][2]=blocCouleur;
            }
            if(carte[tableauCoord[2][0]][tableauCoord[2][1]]!=VIDE)
            {
                carte[tableauCoord[2][0]][tableauCoord[2][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[6][2]=blocCouleur;
            }
            if(carte[tableauCoord[3][0]][tableauCoord[3][1]]!=VIDE)
            {
                carte[tableauCoord[3][0]][tableauCoord[3][1]]=BOMBE;
                *perdu =0;
            }
            else
            {
                carte[7][2]=blocCouleur;
            }

            limiteH->x=4;
            limiteH->y=7;
            break;
    }
}

void deplacerBloc(int carte[][NB_BLOCS_HAUTEUR], int tableauCoord[][2], int forme, int direction, Bloc blocCouleur, SDL_Rect *limiteH, int *sens)
{
    int tourner = 1;

        if(direction == GAUCHE)
        {
            if(limiteH->x-1 >= 0)
            {
                switch(forme)
                {
                    case 1: /////FORME 'T' /////
                         if(*sens == 1)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 2)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 3)
                         {
                            if(carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 4)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=1;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 2: /////FORME 'L1' /////
                        if(*sens == 1)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 2)
                         {
                            if(carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 3)
                         {
                            if(carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 4)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }

                        /*
                        tableauCoord[0][0]=6;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=4;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=5;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=6;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 3: /////FORME 'L2' /////
                        if(*sens == 1)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 2)
                         {
                            if(carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 3)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 4)
                         {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=4;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 4: /////FORME 'Z1' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=5;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=6;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 5: /////FORME 'Z2' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }

                        /*
                        tableauCoord[0][0]=5;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=6;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=4;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 6: /////FORME 'CUBE' /////
                        if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE || carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE)
                        {
                            tourner =0;
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=4;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 7: /////FORME 'TRAIT' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]-1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]-1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]-1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[3][0]-1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=2;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=7;
                        tableauCoord[3][1]=2;
                        */
                        break;
                }

                if(tourner == 1)
                {
                    limiteH->x--;
                    limiteH->y--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]--;
                        tableauCoord[1][0]--;
                        tableauCoord[2][0]--;
                        tableauCoord[3][0]--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
            }
        }
        else if(direction == DROITE)
        {
            if(limiteH->y+1 < NB_BLOCS_LARGEUR)
            {
                switch(forme)
                {
                    case 1: /////FORME 'T' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 3)
                        {
                            if(carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 4)
                        {
                            if(carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=1;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 2: /////FORME 'L1' /////
                        if(*sens == 1)
                         {
                            if(carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 2)
                         {
                            if(carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 3)
                         {
                            if(carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }
                         else if(*sens == 4)
                         {
                            if(carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE)
                            {
                                tourner =0;
                            }
                         }

                        /*
                        tableauCoord[0][0]=6;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=4;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=5;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=6;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 3: /////FORME 'L2' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 3)
                        {
                            if(carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 4)
                        {
                            if(carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE
                            || carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=4;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 4: /////FORME 'Z1' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=5;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=6;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 5: /////FORME 'Z2' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }

                        /*
                        tableauCoord[0][0]=5;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=6;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=4;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 6: /////FORME 'CUBE' /////
                        if(carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                        {
                            tourner =0;
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=4;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 7: /////FORME 'TRAIT' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]+1][tableauCoord[2][1]] != VIDE
                            || carte[tableauCoord[0][0]+1][tableauCoord[0][1]] != VIDE
                            || carte[tableauCoord[1][0]+1][tableauCoord[1][1]] != VIDE
                            || carte[tableauCoord[3][0]+1][tableauCoord[3][1]] != VIDE)
                            {
                                tourner =0;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=2;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=7;
                        tableauCoord[3][1]=2;
                        */
                        break;
                }

                if(tourner == 1)
                {
                    limiteH->x++;
                    limiteH->y++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]++;
                        tableauCoord[1][0]++;
                        tableauCoord[2][0]++;
                        tableauCoord[3][0]++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
            }
            direction=0;
        }
}

void chuteDuBloc(int carte[][NB_BLOCS_HAUTEUR],int tableauCoord[][2] ,Bloc blocCouleur, int forme, SDL_Rect *limiteV, int *formePosee, int *perdu,int *sens)
{
    int tomber=1;

    if(limiteV->x+1 < NB_BLOCS_HAUTEUR)
    {
                switch(forme)
                {
                    case 1: /////FORME 'T' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 3)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 4)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=1;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 2: /////FORME 'L1' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 3)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 4)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }

                        /*
                        tableauCoord[0][0]=6;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=4;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=5;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=6;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 3: /////FORME 'L2' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 3)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 4)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=4;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 4: /////FORME 'Z1' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE
                            || carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=5;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=6;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 5: /////FORME 'Z2' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }

                        /*
                        tableauCoord[0][0]=5;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=6;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=4;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 6: /////FORME 'CUBE' /////
                        if(carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                        || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                        {
                            tomber =0;
                            *formePosee = 1;
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=4;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                        break;

                    case 7: /////FORME 'TRAIT' /////
                        if(*sens == 1)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE
                            || carte[tableauCoord[1][0]][tableauCoord[1][1]+1] != VIDE
                            || carte[tableauCoord[2][0]][tableauCoord[2][1]+1] != VIDE
                            || carte[tableauCoord[3][0]][tableauCoord[3][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }
                        else if(*sens == 2)
                        {
                            if(carte[tableauCoord[0][0]][tableauCoord[0][1]+1] != VIDE)
                            {
                                tomber =0;
                                *formePosee = 1;
                            }
                        }

                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=2;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=7;
                        tableauCoord[3][1]=2;
                        */
                        break;
                }

                if(*formePosee == 1 && limiteV->x == 2)
                {
                    *perdu = 0;
                    return;
                }

                if(tomber == 1)
                {
                    limiteV->x++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][1]++;
                        tableauCoord[1][1]++;
                        tableauCoord[2][1]++;
                        tableauCoord[3][1]++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
    }
    else
    {
        *formePosee = 1;
    }
}

void rotationBloc(int carte[][NB_BLOCS_HAUTEUR],int tableauCoord[][2] ,Bloc blocCouleur, int forme, SDL_Rect *limiteV, SDL_Rect *limiteH, int *formePosee, int *perdu, int *sens)
{
        switch(forme)
        {
            case 1: /////FORME 'T' /////
                if(*sens == 1)
                {
                    *sens = 2;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]++;
                        tableauCoord[0][1]--;
                        tableauCoord[2][0]--;
                        tableauCoord[2][1]++;
                        tableauCoord[3][0]--;
                        tableauCoord[3][1]--;

                        limiteH->y--;


                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                    return;
                }
                else if(*sens == 2)
                {
                    *sens = 3;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]++;
                        tableauCoord[0][1]++;
                        tableauCoord[2][0]--;
                        tableauCoord[2][1]--;
                        tableauCoord[3][0]++;
                        tableauCoord[3][1]--;

                        limiteV->x--;
                        limiteH->y++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                else if(*sens == 3)
                {
                    *sens = 4;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]--;
                        tableauCoord[0][1]++;
                        tableauCoord[2][0]++;
                        tableauCoord[2][1]--;
                        tableauCoord[3][0]++;
                        tableauCoord[3][1]++;

                        limiteV->x++;
                        limiteH->x++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                else if(*sens == 4)
                {
                    *sens = 1;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]--;
                        tableauCoord[0][1]--;
                        tableauCoord[2][0]++;
                        tableauCoord[2][1]++;
                        tableauCoord[3][0]--;
                        tableauCoord[3][1]++;

                        limiteH->x--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                break;

            case 2: /////FORME 'L1' /////
                if(*sens == 1)
                {
                    *sens = 2;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][1]+=2;
                        tableauCoord[3][0]--;
                        tableauCoord[3][1]++;
                        tableauCoord[1][0]++;
                        tableauCoord[1][1]--;

                        limiteV->x++;
                        limiteH->x++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                    return;
                }
                else if(*sens == 2)
                {
                    *sens = 3;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]-=2;
                        tableauCoord[3][0]--;
                        tableauCoord[3][1]--;
                        tableauCoord[1][0]++;
                        tableauCoord[1][1]++;

                        limiteH->x--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                else if(*sens == 3)
                {
                    *sens = 4;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][1]-=2;
                        tableauCoord[3][0]++;
                        tableauCoord[3][1]--;
                        tableauCoord[1][0]--;
                        tableauCoord[1][1]++;

                        limiteH->y--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                else if(*sens == 4)
                {
                    *sens = 1;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]+=2;
                        tableauCoord[3][0]++;
                        tableauCoord[3][1]++;
                        tableauCoord[1][0]--;
                        tableauCoord[1][1]--;

                        limiteV->x--;
                        limiteH->y++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                break;

            case 3: /////FORME 'L2' /////

                if(*sens == 1)
                {
                    *sens = 2;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]+=2;
                        tableauCoord[1][0]++;
                        tableauCoord[1][1]--;
                        tableauCoord[2][0]--;
                        tableauCoord[2][1]++;

                        limiteV->x++;
                        limiteH->x++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                    return;
                }
                else if(*sens == 2)
                {
                    *sens = 3;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][1]+=2;
                        tableauCoord[2][0]--;
                        tableauCoord[2][1]--;
                        tableauCoord[1][0]++;
                        tableauCoord[1][1]++;

                        limiteH->x--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                else if(*sens == 3)
                {
                    *sens = 4;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]-=2;
                        tableauCoord[2][0]++;
                        tableauCoord[2][1]--;
                        tableauCoord[1][0]--;
                        tableauCoord[1][1]++;

                        limiteH->y--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                else if(*sens == 4)
                {
                    *sens = 1;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][1]-=2;
                        tableauCoord[2][0]++;
                        tableauCoord[2][1]++;
                        tableauCoord[1][0]--;
                        tableauCoord[1][1]--;

                        limiteV->x--;
                        limiteH->y++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=4;
                        tableauCoord[1][1]=2;
                        tableauCoord[2][0]=6;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                break;

            case 4: /////FORME 'Z1' /////
                if(*sens == 1)
                {
                    *sens = 2;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]++;
                        tableauCoord[0][1]--;
                        tableauCoord[2][0]--;
                        tableauCoord[2][1]--;
                        tableauCoord[3][0]-=2;

                        limiteH->y--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                    return;
                }
                else if(*sens == 2)
                {
                    *sens = 1;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]--;
                        tableauCoord[0][1]++;
                        tableauCoord[2][0]++;
                        tableauCoord[2][1]++;
                        tableauCoord[3][0]+=2;

                        limiteH->y++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                        /*
                        tableauCoord[0][0]=4;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=5;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=5;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=6;
                        tableauCoord[3][1]=2;
                        */
                break;

            case 5: /////FORME 'Z2' /////
                if(*sens == 1)
                {
                    *sens = 2;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[1][0]--;
                        tableauCoord[1][1]--;
                        tableauCoord[2][0]+=2;
                        tableauCoord[3][1]--;
                        tableauCoord[3][0]++;

                        limiteH->x++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                    return;
                }
                else if(*sens == 2)
                {
                    *sens = 1;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[1][0]++;
                        tableauCoord[1][1]++;
                        tableauCoord[2][0]-=2;
                        tableauCoord[3][1]++;
                        tableauCoord[3][0]--;

                        limiteH->x--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }
                        /*
                        tableauCoord[0][0]=5;
                        tableauCoord[0][1]=1;
                        tableauCoord[1][0]=6;
                        tableauCoord[1][1]=1;
                        tableauCoord[2][0]=4;
                        tableauCoord[2][1]=2;
                        tableauCoord[3][0]=5;
                        tableauCoord[3][1]=2;
                        */
                break;

            case 7: /////FORME 'TRAIT' /////
                if(*sens == 1)
                {
                    *sens = 2;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]++;
                        tableauCoord[0][1]++;
                        tableauCoord[2][0]--;
                        tableauCoord[2][1]--;
                        tableauCoord[3][0]-=2;
                        tableauCoord[3][1]-=2;

                        limiteH->x++;
                        limiteH->y-=2;
                        limiteV->x++;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                    return;
                }
                else if(*sens == 2)
                {
                    *sens = 1;
                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=VIDE;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=VIDE;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=VIDE;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=VIDE;

                        tableauCoord[0][0]--;
                        tableauCoord[0][1]--;
                        tableauCoord[2][0]++;
                        tableauCoord[2][1]++;
                        tableauCoord[3][0]+=2;
                        tableauCoord[3][1]+=2;

                        limiteH->x--;
                        limiteH->y+=2;
                        limiteV->x--;

                    carte[tableauCoord[0][0]][tableauCoord[0][1]]=blocCouleur;
                    carte[tableauCoord[1][0]][tableauCoord[1][1]]=blocCouleur;
                    carte[tableauCoord[2][0]][tableauCoord[2][1]]=blocCouleur;
                    carte[tableauCoord[3][0]][tableauCoord[3][1]]=blocCouleur;
                }

                    /*
                    tableauCoord[0][0]=4;
                    tableauCoord[0][1]=2;
                    tableauCoord[1][0]=5;
                    tableauCoord[1][1]=2;
                    tableauCoord[2][0]=6;
                    tableauCoord[2][1]=2;
                    tableauCoord[3][0]=7;
                    tableauCoord[3][1]=2;
                    */
            break;
    }
}

void afficherInfoPro(int affBloc[][2],int formePro)
{
    switch(formePro)
    {
        case 1:
            //// T ////
            affBloc[0][0]= 2;
            affBloc[0][1]= 1;
            affBloc[1][0]= 3;
            affBloc[1][1]= 1;
            affBloc[2][0]= 4;
            affBloc[2][1]= 1;
            affBloc[3][0]= 3;
            affBloc[3][1]= 2;
            break;

        case 2:
            //// L1 ////
            affBloc[0][0]= 2;
            affBloc[0][1]= 2;
            affBloc[1][0]= 3;
            affBloc[1][1]= 2;
            affBloc[2][0]= 4;
            affBloc[2][1]= 2;
            affBloc[3][0]= 4;
            affBloc[3][1]= 1;
            break;

        case 3:
            //// L2 ////
            affBloc[0][0]= 2;
            affBloc[0][1]= 1;
            affBloc[1][0]= 2;
            affBloc[1][1]= 2;
            affBloc[2][0]= 3;
            affBloc[2][1]= 2;
            affBloc[3][0]= 4;
            affBloc[3][1]= 2;
            break;

        case 4:
            //// Z1 ////
            affBloc[0][0]= 2;
            affBloc[0][1]= 1;
            affBloc[1][0]= 3;
            affBloc[1][1]= 1;
            affBloc[2][0]= 3;
            affBloc[2][1]= 2;
            affBloc[3][0]= 4;
            affBloc[3][1]= 2;
            break;

        case 5:
            //// Z2 ////
            affBloc[0][0]= 2;
            affBloc[0][1]= 2;
            affBloc[1][0]= 3;
            affBloc[1][1]= 2;
            affBloc[2][0]= 3;
            affBloc[2][1]= 1;
            affBloc[3][0]= 4;
            affBloc[3][1]= 1;
            break;

        case 6:
            //// CUBE ////
            affBloc[0][0]= 2;
            affBloc[0][1]= 1;
            affBloc[1][0]= 3;
            affBloc[1][1]= 1;
            affBloc[2][0]= 2;
            affBloc[2][1]= 2;
            affBloc[3][0]= 3;
            affBloc[3][1]= 2;
            break;

        case 7:
            //// TRAIT ////
            affBloc[0][0]= 1;
            affBloc[0][1]= 2;
            affBloc[1][0]= 2;
            affBloc[1][1]= 2;
            affBloc[2][0]= 3;
            affBloc[2][1]= 2;
            affBloc[3][0]= 4;
            affBloc[3][1]= 2;
            break;
    }
}

void ligneDetruite(int carte[][NB_BLOCS_HAUTEUR], int ligneDestroy,int nbLigneDetruiteEnUnBloc)
{
    int i, j, e;
    int bloc[2];

    for(e=0; e<nbLigneDetruiteEnUnBloc; e++)
    {
        for(i=ligneDestroy+e; i >= 0; i--)
        {
            for(j=0; j<NB_BLOCS_LARGEUR; j++)
            {
                bloc[0]= j;
                bloc[1]= i;

                if(carte[bloc[0]][bloc[1]]!=VIDE)
                {
                    carte[bloc[0]][bloc[1]+1]=carte[bloc[0]][bloc[1]];
                    carte[bloc[0]][bloc[1]]=VIDE;
                }
            }
        }
    }
}
