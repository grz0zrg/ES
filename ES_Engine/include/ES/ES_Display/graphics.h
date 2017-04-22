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

#ifndef _ES_GRAPHICS_H_
#define _ES_GRAPHICS_H_

 #include <SDL/sdl.h>
 #include <SDL/SDL_image.h>
 #ifdef GP2X
  #include <SDL/SDL_Rotozoom.h>
 #else
  #include <SDL_GFX/SDL_Rotozoom.h>
 #endif

 #ifdef ES_USE_OPENGL
  #include <GL/gl.h>
 #endif

 #include "../ES_Event/mouse_event.h"
 #include "../ES_Resources/resources.h"

namespace ES
{

 extern bool ES_OpenGL_Mode; // pas très propre mais permet de savoir si le moteur tourne sous opengl où pas

 /**
 * Utilisé pour l'affichage avec SDL où OpenGL
 * des élements graphiques,
 * gère la boucle du programme, intégre une gestion
 * du taux d'affichage ainsi que des méthodes qui
 * agisses sur le comportement de l'affichage.
 */
 class ES_Graphics
 {
 	public:

		/**
		* Constructeur.
		*
		* @param title titre de la fenêtre
		* @param w_screen largeur de la fenêtre
		* @param h_screen hauteur de la fenêtre
		* @param bpp nombre de bits par pixels (8, 16, 24, 32)
		* @param fullscreen true = plein écran / false = fenêtré
		* @param r_mode mode de rendu (ES_SDL où ES_OPENGL)
		* @param hide_cursor true = affiche le curseur / false = cache le curseur
		* @param cursor_path chemin du fichier du curseur image
		* @param r, g, b couleur de transparence du curseur image
        */

        ES_Graphics(const std::string& title, unsigned short int w_screen, unsigned short int h_screen, unsigned short int bpp, bool fullscreen, bool r_mode = ES_SDL, bool hide_cursor = false, const std::string& cursor_path = "", unsigned short int r = 255, unsigned short int g = 0, unsigned short int b = 255);

		/** Destructeur. */

		~ES_Graphics();

		/**
		* Initialise Opengl.
		*
		* @param r, g ,b, a couleur du fond
		* @param target détermine la qualité d'échantillonnage pour les points GL_POINT_SMOOTH_HINT, lignes LINE_SMOOTH_HINT et polygones GL_POLYGON_SMOOTH_HINT
		* @param mode peut prendre la valeur GL_FASTEST, pour privilégier la vitesse, où GL_NICEST pour privilégier la qualité, où GL_DONT_CARE pour l'absence de préférences de rendus
		*/

		void openGL_Init(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f, unsigned int target = 0x0C50, unsigned int mode = 0x1102);

#ifdef ES_USE_OPENGL
		/**
		* Commence l'affichage d'une image avec OpenGL.
		*
		* @param resource_name nom de l'image définie via le gestionnaire de ressources
		* @param blend true = couleur de transparence de l'image activé / false = désactiver
		* @param sfactor paramétre source pour la transparence
		* @param dfactor paramétre de destination pour la transparence
		*/

		void openGL_BeginIMG(const std::string& resource_name, bool blend = false, GLenum sfactor= GL_ONE_MINUS_SRC_ALPHA, GLenum dfactor = GL_SRC_ALPHA);

		/**
		* Dessine un rectangle OpenGL. (appel à begin_opengl_draw_image nécéssaire avant)
		*
		* @param dest rectangle de destination
		*/

		void drawQuad(SDL_Rect dest);

		/**
		* Termine l'affichage d'une image avec OpenGL.
		*
		* @param blend true = couleur de transparence de l'image activé / false = désactiver
		*/

		void openGL_EndIMG(bool blend = false);
#endif
		/**
		* Définie une vue orthogonale avec OpenGL.
		*
		* @param orto_default régle automatiquement la vue sur la surface de l'écran = true sinon false
		* @param w largeur ou doit être appliquer la vue orthogonale
		* @param h hauteur ou doit être appliquer la vue orthogonale
		*/

		void setOrthoView(bool orto_default = true, unsigned short int w = 0, unsigned short int h = 0);

		/**
		* @return true si le mode de rendu actuel est OpenGL
		*/

		bool getUseOpenGL();

        /**
        * Définie un taux d'affichage. (FPS)
        *
        * @param rate nombres d'images par secondes (hz) ES_FPS_DEFAULT, ES_FPS_LOWER_LIMIT, ES_FPS_UPPER_LIMIT
        */

		int setFPS(int rate = ES_FPS_DEFAULT);

        /**
        * @return taux d'affichage actuel
        */

		int getFrameRate();

		/**
		* Met en pause le programme un temp donnée
		*
		* @param ms délai à attendre
		*/

		void delay(unsigned int ms);

        /**
        * Maintien le taux d'affichage à une valeur constante. (définie via @code set_framerate(); @endcode )
        */

		void fpsDelay();

		/**
		* Affiche le curseur choisi lors de la création de la fenêtre.
		*
		* @param dest rectangle de destination (quand ES est compilé en mode OPENGL)
		*/

		void displayCustomCursor(SDL_Rect dest);

		/**
		* Définie un rectangle.
		*
		* @param x, y position du rectangle
		* @param w, h dimension du rectangle
		* @return rectangle
		*/

		SDL_Rect rect(short int x, short int y, short int w, short int h);

		/**
		* Définie une couleur.
		*
		* @param r, g, b taux de rouge, vert, bleu
		* @param a niveau de transparence
		* @return couleur
		*/

		SDL_Color color(short r, short g, short b, short a = 0);

        /**
        * Blit une Surface SDL sur l'écran.
        *
        * @param surf surface SDL
        * @param src rectangle source
        * @param dest rectangle de destinations
        * @param zoom zoom
        */

        void blitSDLSurface(SDL_Surface *surf, SDL_Rect src, SDL_Rect dest, double zoom = 1.0);

		/**
		* Affiche une image suivant le mode définie lors de la création de la fenêtre.
		*
		* @param resource_name nom de la ressource
		* @param src rectangle source
		* @param dest rectangle de destinations
		* @param alpha valeur de la transparence
		* @param blend true = transparence sinon false (quand ES est compilé en mode OPENGL)
		* @param angle angle de rotation en degrée
		*/

		void drawIMG(const std::string& resource_name, SDL_Rect src, SDL_Rect dest, short int alpha = 255, bool blend = false, double angle = 0.0f);

        /**
        * Récupére la couleur d'un pixel d'une surface.
        *
        * @param surf surface
        * @param x,y position
        * @return couleur RGB
        */

        SDL_Color getPixelRGB(SDL_Surface *surf, int x, int y);

		/**
		* Régle un nouveau mode vidéo pour la fenêtre actuel.
		*
		* @param w_screen largeur de la fenêtre
		* @param h_screen hauteur de la fenêtre
		* @param fullscreen true = plein écran sinon false
		*/

		void set_video_mode(unsigned short int w_screen , unsigned short int h_screen, bool fullscreen);

        /**
        * Affiche un pixel.
        *
        * @param x position x
        * @param y position y
        * @param r,g,b couleur du pixel
        * @param verif true = vérification si le pixel sort de l'écran / false = pas de vérification
        */

        void setPixel(unsigned short int x, unsigned short int y, unsigned short int r = 255, unsigned short int g = 255, unsigned short int b = 255, bool verif = true);

        /**
        * Affiche un pixel.
        *
        * @param surf surface ou écrire
        * @param x position x
        * @param y position y
        * @param r,g,b couleur du pixel
        * @param verif true = vérification si le pixel sort de l'écran / false = pas de vérification
        */

        void setPixel(SDL_Surface *surf,unsigned short int x, unsigned short int y, unsigned short int r = 255, unsigned short int g = 255, unsigned short int b = 255, bool verif = true);

        /**
        * Affiche un cercle plein ou non.
        *
        * @param x,y position
        * @param ry rayon
        * @param r,g,b couleur
        * @param fill plein ou non
        */

        void circle(int x, int y, int ry, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255, bool fill = true);

        /**
        * Affiche un cercle plein ou non.
        *
        * àparam surf surface ou écrire
        * @param x,y position
        * @param ry rayon
        * @param r,g,b couleur
        * @param fill plein ou non
        */

        void circle(SDL_Surface *surf,int x, int y, int ry, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255, bool fill = true);

        /**
        * Affiche une ligne horizontale.
        *
        * @param x,y position
        * @param w largeur
        * @param r,g,b couleur
        */

        void hBar(int x, int y, int w, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255);

        /**
        * Affiche une ligne horizontale.
        *
        * @param surf surface ou écrire
        * @param x,y position
        * @param w largeur
        * @param r,g,b couleur
        */

        void hBar(SDL_Surface *surf, int x, int y, int h, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255);

        /**
        * Affiche une ligne vertical.
        *
        * @param x,y position
        * @param h hauteur
        * @param r,g,b couleur
        */

        void vBar(int x, int y, int h, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255);

        /**
        * Affiche une ligne vertical.
        *
        * @param surf surface ou écrire
        * @param x,y position
        * @param h hauteur
        * @param r,g,b couleur
        */

        void vBar(SDL_Surface *surf, int x, int y, int h, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255);

        /**
        * Affiche un rectangle.
        *
        * @param rect rectangle
        * @param r,g,b couleur
        * @param fill plein ou non
        */

        void drawBox(SDL_Rect rect, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255, bool fill = true);

        /**
        * Affiche un rectangle.
        *
        * @param surf surface ou écrire
        * @param rect rectangle
        * @param r,g,b couleur
        * @param fill plein ou non
        */

        void drawBox(SDL_Surface *surf,SDL_Rect rect, unsigned short int r = 255,  unsigned short int g = 255, unsigned short int b = 255, bool fill = true);

        /**
        * Affiche une ligne partant de XY1 et finissant en XY2.
        *
        * @param x départ x
        * @param y départ y
        * @param x2 fin x
        * @param y2 fin y
        * @param r, g, b couleur
        */

        void lineXY(int x, int y, int x2, int y2, unsigned short int r = 255, unsigned short int g = 255, unsigned short int b = 255);

		/**
		* Définie une icône pour le programme.
		*
		* @param filename chemin de l'icône
		*/

		void setIcon(const std::string& filename);

		/**
		* Redéfinie un titre pour la fenêtre.
		*
		* @param title titre de la fenêtre
		*/

		void setTitle(const std::string& title);

		/**
		* Définie le mode Unicode pour les entrées.
		*
		* @param unicode 1 = activé sinon 0
		*/

		void setUnicode(unsigned short int unicode = 1);

		/**
		* Boucle moteur.
		*
		* @return true / false
		*/

		bool run();

		/**
		* Quitte la boucle moteur.
		*/

		void quit();

		/**
		* Met à jour l'écran. (V-Sync automatiquement activé en plein écran)
		*
		* @param delay temps d'attente en ms (utilisé pour libérer le CPU)
		*/

		void flip(unsigned short int delay = 0);

		/**
		* Efface l'écran.
		*
		* @param r, g, b couleur du fond
		*/

		void cls(unsigned int r = 0, unsigned int g = 0, unsigned int b = 0);

		/**
		* Effectue un fondu de la couleur noir/blanche du fond de l'écran.
		*
		* @return 1 si le fondu d'entrée est terminé, 2 si le fondu d'entrée/sortie est terminé
		*/

		unsigned short int screenFade();

		/**
		* Bascule plein écran/fenêtré de la fenêtre crée.
		*
		* @param w largeur de la fenêtre
		* @param h hauteur de la fenêtre
		* @param bpp nombres de bits par pixels de la fenêtre (8, 16, 24, 32)
		*/

		void switchWindowMode(unsigned int w = 0, unsigned int h = 0, unsigned int bpp = 0);

		/**
		* Renvoie le temps écoulé depuis la création de la fenêtre.
		*
		* @return temps écoulé depuis la cration de la fenêtre en ms
		*/

		unsigned long getTime();

		/**
		* Renvoie le temp écoulé depuis la derniére frame. (utilie pour les timers)
		*
		* @return temps écoulé depuis la derniére frame
		*/

		unsigned long getLastFrame(unsigned long dv = 1000);

		/**
		* Renvoie la largeur de la fenêtre.
		*
		* @return largeur de la fenêtre
		*/

		unsigned short int getWindowWidth();

		/**
		* Renvoie la hauteur de la fenêtre.
		*
		* @return hauteur de la fenêtre
		*/

		unsigned short int getWindowHeight();

		/**
		* Renvoie le nombre de bits par pixels de la fenêtre.
		*
		* @return nombre de bits par pixels de la fenêtre
		*/

		unsigned short int getWindowBPP();

		/**
		* Renvoie la surface de l'écran.
		*
		* @return surface de l'écran SDL_Surface
		*/

		SDL_Surface *getScreen();

		/**
		* Renvoie le taux d'affichages en MS.
		*
		* @param delay temps en ms avant que les FPs se mettent à jour
		* @return FPS en ms
		*/

		unsigned short int getFPS(unsigned int delay);

        /**
        * Valeurs défaut/min/max pour le taux d'affichage
        */

		enum { ES_FPS_DEFAULT = 60, ES_FPS_LOWER_LIMIT = 1, ES_FPS_UPPER_LIMIT = 200 };

        /**
        * Mode de rendus.
        */

		enum { ES_OPENGL = true, ES_SDL = false };

	private:
		ES_Resources resources;
		ES_MouseEvent mouse;
		#ifdef ES_USE_OPENGL
		GLuint id_cursor;
		#endif
		SDL_Surface *screen, *cursor, *cursor_temp, *rot;
		bool opengl_mode, loop, fullscreen, check_custom_cursor, orto_view, fade_in, fade_out, fade;
		unsigned short int fps, lasttime, frames, actual_bpp, r_screen, v_screen, b_screen, r_max, v_max, b_max, r_min, v_min, b_min;
		unsigned long es_timer;
		Uint32 framecount, lastticks, rate, current_ticks, target_ticks, the_delay;
		float rateticks;
 };
}

#endif
