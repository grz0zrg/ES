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

#ifndef _ES_MAP_H_
#define _ES_MAP_H_


 #include "esmap.h"

namespace ES
{
 /**
 * G�re les cartes .esm cr�e avec l'�diteur d'ES.
 */
 class ES_Map
 {
	public:

		/**
		* Constructeur.
		*/

		ES_Map();

		/**
		* Destructeur.
		*/

		~ES_Map();

		/**
		* Charge une carte. (lib�re et charge si il y en � d�j� une d'affich�/charg�)
		*
		* @param esm_file chemin de la carte � charger
		* @param alpha -1 si vous ne voulez pas activer de post-traitement au tileset(pour g�n�r� des ombres transparentes par exemple) >= 0 valeur de la transparence
		* @param r, v, b couleur de l'image � rendre transparente
		* @return 1 si la carte ne peut pas �tre charg�, 2 si la carte est corrompu, 3 si le tileset ne peut pas �tre charg�
		*/

		int loadMap(const std::string& esm_file, short int alpha = -1, unsigned short int r = 0, unsigned short int v = 0, unsigned short int b = 0);

		/**
		* Affiche la premi�re couche & la deuxi�me. (correspond au sol normalement)
		*
		* @param n_layer num�ro de la couche � afficher (0 = les 2, 1 = couche 1 o� 2 = couche 2)
		* @param x, y position de la carte sur l'�cran
		* @param w, h dimension de la carte sur l'�cran � partir de la position
		* @param tile_w, tile_h largeur/hauteur des tiles (permet de faire un effet de zoom)
		*/

		void drawBG(short int n_layer, int x, int y, int w, int h, int tile_w = 0, int tile_h = 0);

		/**
		* Affiche les autres couches.
		*
		* @param n_layer num�ro de la couche � afficher (0 = les 2, 3 = couche 3 o� 4 = couche 4)
		* @param x, y position de la couche sur l'�cran
		* @param w, h dimension de la couche sur l'�cran � partir de la position
		* @param tile_w, tile_h largeur/hauteur des tiles (permet de faire un effet de zoom)
		*/

        void drawLayer(short int n_layer, int x, int y, int w, int h, int tile_w = 0, int tile_h = 0);

		/**
		* Centre la carte sur un objet d�finie par �a position.
		*
		* @param x, y position de l'objet
		*/

        void centerScroll(int x, int y);

        /**
        * D�place le scrolling de la map.
        *
        * @param x, y position du scrolling
        */

        void move(int x, int y);

        /**
        * G�re les collisions avec la carte.
        *
        * @param x, y position de l'objet
        * @param w, h dimension de l'objet
        * @param offsetl, offsetr d�calage du sprite sur la gauche et droite (permet de faire des collisions plus pr�cise suivant la taille du sprite)
        * @param offsetu, offsetd d�calage du sprite sur le haut et bas (permet de faire des collisions plus pr�cise suivant la taille du sprite)
        * @return true si il y � eu collision sinon false
        */

        bool tilesCollide(int x, int y, int offsetl = 0, int offsetr = 0, int offsetu = 0, int offsetd = 0);

        /**
        * D�finie une couleur pour la carte afficher. (permet de faire des effets, donner une ambiance)
        *
        * @param r, g, b couleur (attention la couleur diff�re avec OpenGL car c'est g�r� diff�rement que SDL)
        * @param a niveau de transparence
        */

        void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);

        /**
        * @return position du scrolling actuel en x
        */

        int getOffX();

        /**
        * @return position du scrolling actuel en y
        */

        int getOffY();

        /**
        * @return largeur de la carte en tiles (cases)
        */

        unsigned int getWidth();

        /**
        * @return hauteur de la carte en tiles (cases)
        */

        unsigned int getHeight();

    private:
        struct ess_map map;
        int z_tile_w, z_tile_h, lz_tile_w, lz_tile_h;
        int hgx, hgy, hdx, hdy, bgx, bgy, bdx, bdy;
        unsigned int r, g, b, a;
 };
}
#endif
