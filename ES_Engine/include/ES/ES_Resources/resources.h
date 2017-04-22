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

#ifndef _ES_RESOURCES_H_
#define _ES_RESOURCES_H_

 #include <SDL/sdl.h>
 #include <SDL/sdl_image.h>
 #include <SDL/SDL_mixer.h>
 #include <SDL/SDL_ttf.h>
 #ifdef ES_USE_OPENGL
  #include <GL/gl.h>
  #include <GL/glext.h>
 #endif
 #include <iostream>
 #include <map>
 #include <vector>
 #include "../ES_Pack/pack.h"
 #include "../ES_Utils/utils.h"

namespace ES
{
 /**
 * Gestion des ressources. (chargement/gestion des fichiers images/sonores/polices etc...)
 */
 class ES_Resources
 {
	 public:

		 /**
		 * Ajoute une ressource au gestionnaire de ressources.
		 *
		 * @param resource_name nom de la ressource (identifiant)
		 * @param resource_filename chemin où charger la ressource
		 * @param resource_type type de ressources à charger
		 * @param esd true = la ressource provient d'un fichier de données ES false = mode normal
		 * @param pack si esd est égale à true indique l'objet ES_Pack pour allez chercher la ressource
		 * @param colorkey dans le cas des images true = activer une couleur de transparence
		 * @param r, g, b couleur de transparence
		 */

		 void add(const std::string& resource_name, const std::string& resource_filename, unsigned short int resource_type, bool esd = false, ES_Pack *pack = NULL, bool colorkey = false, unsigned short int r = 0, unsigned short int g = 0, unsigned short int b = 0);

         /**
         * Vérifie qu'une ressource est chargé.
         *
         * @param resource_name nom de la ressource à vérifier
         * @param resource_type type de ressource à vérifer
         * @param pack objet ES_Pack si la ressource provient d'un fichier de données ES
         */

         bool isLoaded(const std::string& resource_name, unsigned short int resource_type, ES_Pack *pack);

		 /**
		 * Libére une ressource.
		 *
		 * @param resource_name nom de la ressource à libérer
		 * @param resource_type type de ressource à libérer
		 * @param pack objet ES_Pack si la ressource provient d'un fichier de données ES
		 * @param resource_filename le fichier de la ressource chargé en mémoire (à spécifier si chargé depuis la mémoire)
		 */

		 void del(const std::string& resource_name, unsigned short int resource_type, ES_Pack *pack = NULL, const std::string& resource_filename = "");

		 /**
		 * Libére toutes les ressources quelquesoit le type proprement.
		 */

		 void free();

		 /**
		 * Active le mode OpenGL pour les ressources. (utilisé pour les images)
		 */

		 void set_opengl_mode();

		 /**
		 * @param resource_name nom de la ressource
		 * @return retourne le SDL_Surface de la ressource où 0 si la ressource n'existe pas (images SDL)
		 */

		 SDL_Surface *getSDL_IMG(const std::string& resource_name);
		 #ifdef ES_USE_OPENGL

		 /**
		 * @param resource_name nom de la ressource
		 * @return retourne l'identifiant OpenGL de la ressource où 0 si la ressource n'existe pas (images OpenGL)
		 */

		 GLuint getOpenGL_IMG(const std::string& resource_name);

		 /**
		 * @param resource_name nom de la ressource
		 * @return retourne la largeur du texte de la ressource où 0 si la ressource n'existe pas (polices)
		 */

		 int getTextWidth(const std::string& resource_name);

		 /**
		 * @param resource_name nom de la ressource
		 * @return retourne le hauteur du texte de la ressource où 0 si la ressource n'existe pas (polices)
		 */

		 int getTextHeight(const std::string& resource_name);

		 #endif

		 /**
		 * @param resource_name nom de la ressource
		 * @return retourne l'identifiant SDL_ttf de la ressource où 0 si la ressource n'existe pas (polices SDL)
		 */

		 TTF_Font *getTTF(const std::string& resource_name);

		 /**
		 * @param resource_name nom de la ressource
		 * @return retourne l'identifiant SDL_mixer de la ressource où 0 si la ressource n'existe pas (musiques)
		 */

		 Mix_Music *getMusic(const std::string& resource_name);

		 /**
		 * @param resource_name nom de la ressource
		 * @return retourne l'identifiant SDL_mixer de la ressource où 0 si la ressource n'existe pas (samples)
		 */

		 Mix_Chunk *getSample(const std::string& resource_name);

		 #ifdef ES_USE_OPENGL
		 /**
		 * Charge une texture OpenGL.
		 *
		 * @param filename chemin de l'image à charger
		 * @param colorkey true pour activer la couleur de transparence
		 * @param r, g, b couleur de transparence
		 * @param esd true = la ressource provient d'un fichier de données ES false = mode normal
		 * @param pack si esd est égale à true indique l'objet ES_Pack pour allez chercher la ressource
		 * @return l'identifiant OpenGL de la texture chargé où 0 si erreur
		 */
		 GLuint load_texture(const std::string& filename, bool colorkey, unsigned short int r, unsigned short int g, unsigned short int b, bool esd = false, ES_Pack *pack = NULL);
		 #endif

         /**
         * Types de ressources.
         */

         enum { ES_IMG = 1, ES_MUSIC = 2, ES_SAMPLE = 3, ES_TTF_FONT = 4, ES_PACK = true };

 	 private:
 	    #ifdef ES_USE_OPENGL
		GLuint id_img, pixels_format, rmask, gmask, bmask, amask;
		#endif
		SDL_Surface *image_temp, *imageo_temp, *surface_temp, *surf_pot;
		SDL_Rect area;
		Mix_Music *music_temp;
		Mix_Chunk *sample_temp;
		TTF_Font *sfont_temp;
		unsigned char *pixels, *pixels_temp;
		int i, j, cnt, p_cnt, w, h, ft_error;
 };
}
#endif
