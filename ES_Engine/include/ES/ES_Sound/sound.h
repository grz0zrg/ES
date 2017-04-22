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

#ifndef _ES_SOUND_H_
#define _ES_SOUND_H_

 #include <SDL/SDL_mixer.h>
 #include "../ES_Resources/resources.h"

 #define ES_MAX_VOLUME MIX_MAX_VOLUME

namespace ES
{
 /**
 * Gestion des musiques, samples.
 */
 class ES_Sound
 {
	 public:

		 /**
		 * Constructeur. Initialise le module sonore.
		 *
		 * @param frequency fr�quence sonore
		 * @param format formats sonores
		 * @param channels nombres de channels
		 * @param chunksize :O
		 */

	     ES_Sound(int frequency = MIX_DEFAULT_FREQUENCY, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 1024);

	     /**
	     * Destructeur.
	     */

	     ~ES_Sound();

		 /**
		 * Joue une musique.
		 *
		 * @param resource_name nom de la ressource
		 * @param loop -1 = lecture en boucle
		 */

	     void playMusic(const std::string& resource_name, short int loop = 0);

	     /**
	     * Bascule la musique jou� en mode pause/lecture. (fonctionne pas avec certains formats)
	     */

	     void switchMusicMode();

	     /**
	     * Joue un �chantillon sonore.
	     *
	     * @param resource_name nom de la ressource
	     * @param loop -1 = lecture en boucle
	     * @param channel channel o� jouer l'�chantillon
	     */

	     void playSample(const std::string& resource_name, short int loop = 0, short int channel = -1);

	     /**
	     * R�gle le volume d'un �chantillon sonores.
	     *
	     * @param resource_name nom de la ressource
	     * @param volume nouveau volume de l'�chantillon
	     * @return le volume actuel si volume = -1
	     */

	     unsigned short int sampleVol(const std::string& resource_name, short int volume = ES_MAX_VOLUME);

	     /**
	     * R�gle le volume pour diff�rents canals.
	     *
	     * @param channel channel o� r�gler le volume (-1 = tous les channels)
	     * @param volume nouveau volume du channel
	     * @return volume actuel du canal sp�cifi� si volume = -1
	     */

	     unsigned short int channelVol(short int channel = -1, short int volume = ES_MAX_VOLUME);

	     /**
	     * R�gle le volume de la musique jou�.
	     *
	     * @param volume nouveau volume de la musique
	     * @return le volume de la musique actuel si volume = -1
	     */

	     unsigned short int musicVol(short int volume = ES_MAX_VOLUME);

	     /**
	     * Joue une musique avec un effet de fondus d'entr�e.
	     *
	     * @param resource_name nom de la ressource
	     * @param ms duration de l'effet en millisecondes
	     * @param loop -1 = joue la musique en boucle
	     */

	     void musicFadeIn(const std::string& resource_name, unsigned short int ms = 5000, short int loop = 0);

	     /**
	     * Cr�e un effet de fondus de sortis sur la musique actuel. (stoppe la musique une fois termin�)
	     *
	     * @param duration dur�e de l'effet
	     */

	     void musicFadeOut(unsigned short int duration = 3000);

	     /**
	     * Stoppe la musique acutellement jou�.
	     */

	     void musicStop();

	     /**
	     * @return true si une muisique est jou� sinon false
	     */

	     bool isPlayed();

	     /**
	     * @return true si une musique est en pause sinon false
	     */

	     bool isPaused();

	     /**
	     * @return true si le fondus de sortie de la musique est termin�
	     */

         bool getFadeState();

	 private:
		 ES_Resources resources;
	     //- Permet de savoir quand un fade out est finis
         static void setFadeFinished();
         static void ess_bla();
 };
}
#endif
