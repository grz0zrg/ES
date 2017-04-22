/*
* Copyright (c) 2006, 2007, 2008 Julien Verneuil
* Julien Verneuil a.k.a Grz-
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of ES or ES Engine nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _ES_TTF_H_
#define _ES_TTF_H_

 #include <SDL/SDL_ttf.h>

 #include "graphics.h"

namespace ES
{
 extern unsigned short int sfont_size;
 extern SDL_Color es_col;

 /**
 * Utilisé pour l'affichage de textes avec les polices true type/freetype etc..
 * Sous OpenGL avec OGLFT où SDL avec SDL_ttf.
 */
 class ES_Ttf
 {
     public:

		/**
		* Constructeur.
		* Initialise la bibliothéque SDL_ttf si on est en mode SDL.
		*/

        ES_Ttf();

        /**
        * Destructeur.
        */

        ~ES_Ttf();

        /**
        * Affiche un texte.
        *
        * @param resource_name nom de la police charger via le systéme de ressources
        * @param text le texte à afficher
        * @param x, y position sur l'écran du texte spécifié
        * @param color couleur du texte
        * @param justification alignement du texte en x
        * @param style style du texte
        * @param color_bg couleur de transparence du fond si le texte est en mode ES_SHADED
        */

        void drawText(const std::string& resource_name, const std::string& text, short int x = 0, short int y = 0, SDL_Color color = es_col, unsigned short int justification = ES_TTF_RIGHT, unsigned short int style = ES_SOLID, SDL_Color color_bg = es_col );

        /**
        * Régle la taille de la police.
        *
        * @param font_size taille de la prochaine police à être chargé
        */

        void setSize(unsigned short int font_size);

		/**
		* Styles et alignements des polices.
		*/
		enum ES_TTF_STYLE{ ES_SOLID = 0, ES_SHADED = 1, ES_BLENDED = 2, ES_TTF_CENTER = 3, ES_TTF_RIGHT = 4, ES_TTF_LEFT = 5 };

    private:
        int font_width, font_height, horizontal_justification;
        SDL_Rect rect;
        int w,h;
        #ifdef ES_USE_OPENGL
        GLuint texture;
        #endif
        SDL_Surface *initial;
        SDL_Surface *intermediary;
        SDL_Surface *txt;
        SDL_Rect pos;
        ES_Resources resources;
 };
}

#endif
