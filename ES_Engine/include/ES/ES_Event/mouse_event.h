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

#ifndef _ES_MOUSE_EVENT_H_
#define _ES_MOUSE_EVENT_H_

 #include <SDL/sdl.h>
#ifdef GP2X
 #include <fcntl.h>
 #include <stdlib.h>
 #include <stdint.h>
 #include <unistd.h>
 #define SAMPLES_TOUCHSCREEN 4
#endif

namespace ES
{
    typedef struct {
        uint16_t x;			// 0 <= x <= 319
        uint16_t y;			// 0 <= y <= 239
        uint16_t Pressed;
        uint16_t NewPress;
    } TouchScreen;

 /**
 * G�re les �venements concernant la souris.
 */
 class ES_MouseEvent
 {
	 public:

		/**
		* initialise l'�cran tactile de la GP2X
		*
		* @return true si correctement initialis�
		*/
        bool initTouchscreen();

		/**
		* @return position du curseur sur l'�cran dans une structure de type rectangle
		*/

		SDL_Rect *getCursorRect();

		/**
		* @return position du curseur en x
		*/

		int getX();

		/**
		* Permet de placer le pointeur de la souris sur l'�cran
		*
		* @param x position x
		* @param y position y
		*/

        void setCursorPosition(int x, int y);

		/**
		* @return position du curseur en y
		*/

		int getY();

		/**
		* Donn�es de l'�cran tactile de la GP2X
		*/

		TouchScreen *getTouchscreenData();

		/**
		* Lib�re l'�cran tactile de la GP2X
		*/

		void closeTouchscreen();

	private:
		SDL_Rect cursor_position;
		int x_mouse, y_mouse, fd;
 };
}
#endif
