#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "constantes.h"
#include "jeu.h"

void help(SDL_Surface *ecran, int *continuer);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    srand(time(NULL));

    SDL_Surface *ecran = NULL, *menu = NULL, *cursor = NULL, *texte = NULL;
    SDL_Event event;
    SDL_Rect position, positionEcran, positionTexte;

    TTF_Init();

    TTF_Font *police = NULL;
    SDL_Color color_noire = {0,0,0};
    police = TTF_OpenFont("PressStart2P.ttf",32);

    int continuer = 1, full=0;

        positionEcran.x=0;
        positionEcran.y=0;

    SDL_WM_SetIcon(IMG_Load("icone.png"),NULL);
    ecran = SDL_SetVideoMode(642, TAILLE_FENETRE_HAUTEUR_TOT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    SDL_WM_SetCaption("Tetris",NULL);

    menu = IMG_Load("menu.png");
    cursor = IMG_Load("Curseur_selection.png");

    positionTexte.x=230;
    positionTexte.y=580;

    SDL_BlitSurface(menu,NULL,ecran,&position);

    SDL_Flip(ecran);

    while(continuer)
    {
        SDL_ShowCursor(1);
        SDL_BlitSurface(menu,NULL,ecran,&positionEcran);
        SDL_BlitSurface(texte,NULL,ecran, &positionTexte);

        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                if(event.button.x > 183 && event.button.x < 474 && event.button.y > 188 && event.button.y < 258)
                {
                    position.x=183;
                    position.y=188;
                    SDL_BlitSurface(cursor, NULL, ecran, &position);
                }
                else if(event.button.x > 183 && event.button.x < 474 && event.button.y > 271 && event.button.y < 341)
                {
                    position.x=183;
                    position.y=271;
                    SDL_BlitSurface(cursor, NULL, ecran, &position);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.x > 183 && event.button.x < 474 && event.button.y > 188 && event.button.y < 258)
                {
                    SDL_ShowCursor(0);
                    jouer(ecran, &continuer);
                }
                else if(event.button.x > 183 && event.button.x < 474 && event.button.y > 271 && event.button.y < 341)
                {
                    help(ecran,&continuer);
                }
                break;
        }

        SDL_Flip(ecran);
    }

    TTF_CloseFont(police);
    TTF_Quit();

    SDL_FreeSurface(menu);
    SDL_FreeSurface(cursor);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void help(SDL_Surface *ecran, int *continuer)
{

    SDL_Surface *help=NULL, *cursor = NULL;
    SDL_Rect pos, position;
    SDL_Event event;
    int retour=1;

    pos.x=0;
    pos.y=0;

    help = IMG_Load("help.png");
    cursor = IMG_Load("Curseur_selection.png");

    SDL_BlitSurface(help,NULL,ecran,&pos);
    SDL_Flip(ecran);

    while(retour && *continuer)
    {
        SDL_BlitSurface(help,NULL,ecran,&pos);

        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                *continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        retour = 0;
                        break;
                }

            case SDL_MOUSEMOTION:
                if(event.button.x > 173 && event.button.x < 464 && event.button.y > 633 && event.button.y < 703)
                {
                    position.x=173;
                    position.y=633;
                    SDL_BlitSurface(cursor, NULL, ecran, &position);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.x > 173 && event.button.x < 464 && event.button.y > 633 && event.button.y < 703)
                {
                    retour = 0;
                }
                break;
        }

        SDL_Flip(ecran);
    }

    SDL_FreeSurface(help);
    SDL_FreeSurface(cursor);
}
