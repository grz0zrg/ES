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

#ifndef _ES_PHYSICS_H_
#define _ES_PHYSICS_H_

 #include <SDL/sdl.h>
 #include <math.h>
 #include <assert.h>
 #include <Box2D/Box2d.h>
 #include <vector>

namespace ES
{
 /**
 * Gestion de la physique. (collisions, gravités etc...)
 */
 class ES_Physics
 {
	public:

		/**
		* Collision cercle/cercle.
		*
		* @param cx1, cy1 position du premier cercle
		* @param r1 rayon du premier cercle
		* @param cx2, cy2 position du deuxiéme cercle
		* @param r2 rayon du deuxiéme cercle
		* @return true si il y à eu collision
		*/

        bool circleCollision(float cx1, float cy1, float r1, float cx2, float cy2, float r2);

		/**
		* Collision rectangle/rectangle.
		*
		* @param x1, y1 position du premier rectangle
		* @param w1, h1 dimensions du premier rectangle
		* @param x2, y2 position du deuxiéme rectangle
		* @param w2, h2 dimensions du deuxiéme rectangle
		* @return true si il y à eu collision
		*/

        bool rectCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

        /**
        * Crée un "monde" physique.
        *
        * @param min_x position gauche du rectangle
        * @param min_y position bas du rectangle
        * @param max_x position droite du rectangle
        * @param max_y position haute du rectangle
        * @param grav_x force de gravité x
        * @param grav_y force de gravité y (par défaut celle de la terre)
        * @param dosleep permet au monde de "dormir"
        */

        void setupWorld(float min_x, float min_y, float max_x, float max_y, float gravx, float gravy = 8.81f, bool dosleep = true);

        /**
        * Ajoute un sol au monde physique. (setup_world doit être appelé avant)
        *
        * @param size_x largeur du sol
        * @param size_y hauteur du sol
        * @param pos_x position x du sol
        * @param pos_y position y du sol
        */

        void addGround(float size_x, float size_y, float pos_x, float pos_y);

        /**
        * Ajoute une boîte rectangulaire au monde physique. (setup_world doit être appelé avant)
        *
        * @param pos_x position x de la box
        * @param pos_y position y de la box
        * @param size_x largeur de la box
        * @param size_y hauteur de la box
        * @param density densité de la box
        * @param angle angle de rotation
        * @param linear_vel_x velocité linéaire x de la box
        * @param linear_vel_y velocité linéaire y de la box
        * @param sleep permet à l'objet de se mettre en pause lorsqu'il n'y à plus rien à faire (défaut : pas de pause)
        * @param enable_rot permet d'activer la rotation (défaut : rotation activé)
        */

        void addBox(float pos_x, float pos_y, float size_x, float size_y, float density, float angle, float friction, float linear_vel_x, float linear_vel_y, bool sleep = false, bool enable_rot = true);

        /**
        * Met à jour le monde physique. (setup_world doit être appelé avant)
        *
        * @param time_step temp entre chaques iterations
        * @param iterations nombres d'iterations
        */

        void updateWorld(float time_step = 1.0/60.0, int iterations = 10);

        /**
        * Récupére la position origine d'un objet. (setup_world doit être appelé avant)
        *
        * @param i numéro de l'objet
        * @param type type de l'objet
        * @return position
        */

        b2Vec2 getPosition(unsigned int i, unsigned short int type);

        /**
        * Récupére l'angle rotation d'un objet. (setup_world doit être appelé avant)
        *
        * @param i numéro de l'objet
        * @param type type de l'objet
        * @return rotation en radian
        */

        float getRotation(unsigned int i, unsigned short int type);

        /**
        * Applique une force sur un objet. (setup_world doit être appelé avant)
        *
        * @param i numéro de l'objet
        * @param type type de l'objet
        * @param x position x de l'objet
        * @param y position y de l'objet
        * @param f_x force sur l'objet appliqué en x
        * @param f_y force sur l'objet appliqué en y
        */

        void applyForce(unsigned int i, unsigned short int type, float x, float y, float f_x, float f_y);

        /**
        * Compte le nombre d'objet du type spécifié. (setup_world doit être appelé avant)
        *
        * @param type type de l'objet
        * @return nombres d'objets (-1 si le type n'a pas étaît trouvé)
        */

        int countObject(unsigned short int type);

        /**
        * Supprime un objet du monde. (setup_world doit être appelé avant)
        *
        * @param i numéro de l'objet à supprimer
        * @param type type de l'objet à supprimer
        */

        void delObject(unsigned int i, unsigned short int type);

        /**
        * Supprime le monde et les objets crées. (setup_world doit être appelé avant)
        */

        void freeWorld();

        /**
        * Types physique d'objets.
        */
        enum ES_BODY_TYPE{ES_BOX, ES_GROUND};

	private:
        b2World *world;
        std::vector<b2Body*> ground_list;
        std::vector<b2Body*> box_list;
        std::vector<b2Vec2> ground_list_size;
        std::vector<b2Vec2> box_list_size;
        double d;
 };
}
#endif
