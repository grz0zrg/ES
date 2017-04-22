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

#ifndef _ES_EVENT_
#define _ES_EVENT_

 #include <SDL/sdl.h>

 #define GP2X_BUTTON_UP              (0)
 #define GP2X_BUTTON_DOWN            (4)
 #define GP2X_BUTTON_LEFT            (2)
 #define GP2X_BUTTON_RIGHT           (6)
 #define GP2X_BUTTON_UPLEFT          (1)
 #define GP2X_BUTTON_UPRIGHT         (7)
 #define GP2X_BUTTON_DOWNLEFT        (3)
 #define GP2X_BUTTON_DOWNRIGHT       (5)
 #define GP2X_BUTTON_CLICK           (18)
 #define GP2X_BUTTON_A               (12)
 #define GP2X_BUTTON_B               (13)
 #define GP2X_BUTTON_X               (14)
 #define GP2X_BUTTON_Y               (15)
 #define GP2X_BUTTON_L               (10)
 #define GP2X_BUTTON_R               (11)
 #define GP2X_BUTTON_START           (8)
 #define GP2X_BUTTON_SELECT          (9)
 #define GP2X_BUTTON_VOLUP           (16)
 #define GP2X_BUTTON_VOLDOWN         (17)

namespace ES
{
 /**
 * Gestion des évenements de l'application avec SDL. (claviers & fenêtre)
 */
 class ES_Event
 {
 	 public:

        /**
        * Questionne la file d'évenement. (non bloquante contrairement à wait_event)
        *
        * @return 1 si y'a un nouvelle évenement sinon 0
        */

        int pollEvent();

		/**
		* Attend un évenement. (bloquante)
		*
		* @return 0 si il y à eu une erreur lors de l'attente d'un évenement sinon 1
		*/

		int waitEvent();

		/**
		* Définie la répétitions des touches.
		*
		* @param delay temp d'attente avant que la répétition prenne effet
		* @param interval interval d'attente entre chaque répétitions
		*/

		void setKeyRepeat(unsigned short int delay = 300, unsigned short int interval = 60);

		/**
		* @return le type d'évenement reçus par @code poll_event() @endcode où @code wait_event() @endcode
		*/

		int getEventType();

		/**
		* @return le type dévénement clavier reçus par @code poll_event() @endcode où @code wait_event() @endcode
		*/

		int getEventKeyType();

		/**
		* @return structure SDL_Event
		*/

		SDL_Event getEvent();

 	 private:
		SDL_Event event;
 };

 /**
 * Types d'évenements
 */

 enum {ES_QUIT = SDL_QUIT, ES_ACTIVEEVENT = SDL_ACTIVEEVENT, ES_KEYDOWN = SDL_KEYDOWN, ES_KEYUP = SDL_KEYUP,
       ES_MOUSEMOTION = SDL_MOUSEMOTION, ES_MOUSEBUTTONUP = SDL_MOUSEBUTTONUP, ES_MOUSEBUTTONDOWN = SDL_MOUSEBUTTONDOWN,
       ES_BUTTON_WHEELDOWN = SDL_BUTTON_WHEELDOWN, ES_BUTTON_WHEELUP = SDL_BUTTON_WHEELUP, ES_BUTTON_RIGHT = SDL_BUTTON_RIGHT, ES_BUTTON_LEFT = SDL_BUTTON_LEFT,
       ES_JOYAXISMOTION = SDL_JOYAXISMOTION, ES_JOYBALLMOTION = SDL_JOYBALLMOTION, ES_JOYHATMOTION = SDL_JOYHATMOTION,
       ES_JOYBUTTONDOWN = SDL_JOYBUTTONDOWN, ES_JOYBUTTONUP = SDL_JOYBUTTONUP, ES_SYSWMEVENT = SDL_SYSWMEVENT,
       ES_VIDEORESIZE = SDL_VIDEORESIZE, ES_VIDEOEXPOSE = SDL_VIDEOEXPOSE, ES_USEREVENT = SDL_USEREVENT,
       ESK_BACKSPACE = SDLK_BACKSPACE, ESK_TAB = SDLK_TAB, ESK_CLEAR = SDLK_CLEAR, ESK_RETURN = SDLK_RETURN,
       ESK_PAUSE = SDLK_PAUSE, ESK_ESCAPE = SDLK_ESCAPE, ESK_SPACE = SDLK_SPACE, ESK_EXCLAIM = SDLK_EXCLAIM,
       ESK_QUOTEDBL = SDLK_QUOTEDBL, ESK_HASH = SDLK_HASH, ESK_DOLLAR = SDLK_DOLLAR, ESK_AMPERSAND = SDLK_AMPERSAND,
       ESK_QUOTE = SDLK_QUOTE, ESK_LEFTPAREN = SDLK_LEFTPAREN, ESK_RIGHTPAREN = SDLK_RIGHTPAREN, ESK_ASTERISK = SDLK_ASTERISK,
       ESK_PLUS = SDLK_PLUS, ESK_COMMA = SDLK_COMMA, ESK_MINUS = SDLK_MINUS, ESK_PERIOD = SDLK_PERIOD,
       ESK_SLASH = SDLK_SLASH, ESK_0 = SDLK_0, ESK_1 = SDLK_1, ESK_2 = SDLK_2, ESK_3 = SDLK_3, ESK_4 = SDLK_4,
       ESK_5 = SDLK_5, ESK_6 = SDLK_6, ESK_7 = SDLK_7, ESK_8 = SDLK_8, ESK_9 = SDLK_9, ESK_COLON = SDLK_COLON,
       ESK_SEMICOLON = SDLK_SEMICOLON, ESK_LESS = SDLK_LESS, ESK_EQUALS = SDLK_EQUALS, ESK_GREATER = SDLK_GREATER,
       ESK_QUESTION = SDLK_QUESTION, ESK_AT = SDLK_AT, ESK_LEFTBRACKET = SDLK_LEFTBRACKET, ESK_BACKSLASH = SDLK_BACKSLASH,
       ESK_RIGHTBRACKET = SDLK_RIGHTBRACKET, ESK_CARET = SDLK_CARET, ESK_UNDERSCORE = SDLK_UNDERSCORE, ESK_BACKQUOTE = SDLK_BACKQUOTE,
       ESK_a = SDLK_a, ESK_b = SDLK_b, ESK_c = SDLK_c, ESK_d = SDLK_d, ESK_e = SDLK_e, ESK_f = SDLK_f, ESK_g = SDLK_g,
       ESK_h = SDLK_h, ESK_i = SDLK_i, ESK_j = SDLK_j, ESK_k = SDLK_k, ESK_l = SDLK_l, ESK_m = SDLK_m, ESK_n = SDLK_n,
       ESK_o = SDLK_o, ESK_p = SDLK_p, ESK_q = SDLK_q, ESK_r = SDLK_r, ESK_s = SDLK_s, ESK_t = SDLK_t, ESK_u = SDLK_u,
       ESK_v = SDLK_v, ESK_w = SDLK_w, ESK_x = SDLK_x, ESK_y = SDLK_y, ESK_z = SDLK_z, ESK_DELETE = SDLK_DELETE,
       ESK_KP0 = SDLK_KP0, ESK_KP1 = SDLK_KP1, ESK_KP2 = SDLK_KP2, ESK_KP3 = SDLK_KP3, ESK_KP4 = SDLK_KP4, ESK_KP5 = SDLK_KP5,
       ESK_KP6 = SDLK_KP6, ESK_KP7 = SDLK_KP7, ESK_KP8 = SDLK_KP8, ESK_KP9 = SDLK_KP9, ESK_KP_PERIOD = SDLK_KP_PERIOD,
       ESK_KP_DIVIDE = SDLK_KP_DIVIDE, ESK_KP_MULTIPLY = SDLK_KP_MULTIPLY, ESK_KP_MINUS = SDLK_KP_MINUS, ESK_KP_PLUS = SDLK_KP_PLUS,
       ESK_KP_ENTER = SDLK_KP_ENTER, ESK_KP_EQUALS = SDLK_KP_EQUALS, ESK_UP = SDLK_UP, ESK_DOWN = SDLK_DOWN, ESK_RIGHT = SDLK_RIGHT,
       ESK_LEFT = SDLK_LEFT, ESK_INSERT = SDLK_INSERT, ESK_HOME = SDLK_HOME, ESK_END = SDLK_END, ESK_PAGEUP = SDLK_PAGEUP,
       ESK_PAGEDOWN = SDLK_PAGEDOWN, ESK_F1 = SDLK_F1, ESK_F2 = SDLK_F2, ESK_F3 = SDLK_F3, ESK_F4 = SDLK_F4, ESK_F5 = SDLK_F5,
       ESK_F6 = SDLK_F6, ESK_F7 = SDLK_F7, ESK_F8 = SDLK_F8, ESK_F9 = SDLK_F9, ESK_F10 = SDLK_F10, ESK_F11 = SDLK_F11,
       ESK_F12 = SDLK_F12, ESK_F13 = SDLK_F13, ESK_F14 = SDLK_F14, ESK_F15 = SDLK_F15, ESK_NUMLOCK = SDLK_NUMLOCK,
       ESK_CAPSLOCK = SDLK_CAPSLOCK, ESK_SCROLLOCK = SDLK_SCROLLOCK, ESK_RSHIFT = SDLK_RSHIFT, ESK_LSHIFT = SDLK_LSHIFT,
       ESK_RCTRL = SDLK_RCTRL, ESK_LCTRL = SDLK_LCTRL, ESK_RALT = SDLK_RALT, ESK_LALT = SDLK_LALT, ESK_RMETA = SDLK_RMETA,
       ESK_LMETA = SDLK_LMETA, ESK_LSUPER = SDLK_LSUPER, ESK_RSUPER = SDLK_RSUPER, ESK_MODE = SDLK_MODE,
       ESK_HELP = SDLK_HELP, ESK_PRINT = SDLK_PRINT, ESK_SYSREQ = SDLK_SYSREQ, ESK_BREAK = SDLK_BREAK,
       ESK_MENU = SDLK_MENU, ESK_POWER = SDLK_POWER, ESK_EURO = SDLK_EURO};
}
#endif
