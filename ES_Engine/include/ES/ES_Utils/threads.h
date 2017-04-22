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

#ifndef _ES_THREADS_H_
#define _ES_THREADS_H_

 #include <sdl/sdl_thread.h>
 #include <string>

namespace ES
{
 /**
 * Gestion des threads avec SDL.
 */
 class ES_Threads
 {
     public:

        /**
        * Constructeur. Cr�e un thread.
        *
        * @param fn fonction � executer
        * @param data donn�es � pass�s � la fonction
        */

        ES_Threads(int (*fn)(void *), void *data);

        /**
        * Destructeur. Attend la fin d'execution du thread.
        */

        ~ES_Threads();

        /**
        * Attend la fin d'execution du thread.
        *
        * @return le retour de la fonction
        */

        int wait();

        /**
        * Termine le thread en cours sans lib�ration m�moires (utilisez @code wait_thread() @endcode plut�t)
        */

        void kill();

        /**
        * @return structure SDL_Thread du thread (NULL si �chou�)
        */
        SDL_Thread *getSDL_Thread();

        /**
        * @return Derni�re erreur SDL
        */
        std::string getError();

    private:
        SDL_Thread *thread;
        int c_err;
 };
}
#endif
