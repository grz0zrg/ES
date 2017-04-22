// ES engine - Exemple 5
//- Gestion sonore avanc�s (effets de fondus/volumes/autres)
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Display/ttf.h>
#include <ES/ES_Event/event.h>
#include <ES/ES_Sound/sound.h>
#include <sstream>

using namespace ES;

int main(int argc, char **argv)
{
    // variable qui nous permet de savoir si un fade in (fondu d'entr�e) est en cours (voir plus loin)
    bool fade = false;

	ES_Resources ressources;
	ES_Event event;

#ifdef GP2X
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 5", 320, 240, 16, false);
#else
    ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 5", 400, 300, 32, false);
#endif

    // initialisation des fonts
	ES_Ttf *fonts = new ES_Ttf();
	fonts->set_size(16);

	// initialisation du gestionnaire sonore (attention doit toujours �tre appeler apr�s la cr�ation
	// d'une fen�tre)
	ES_Sound *sounds = new ES_Sound();

    // charge notre font
    ressources.add_resource("police", "arial.ttf", ES_Resources::ES_TTF_FONT);
	// indique qu'on souhaite ajouter/charger un fichier musical
	ressources.add_resource("tetris", "data/tetris.ogg", ES_Resources::ES_MUSIC);
	ressources.add_resource("musique", "data/f-lagoon.mod", ES_Resources::ES_MUSIC);
	// indique que l'on souhaite charger nos fichiers d'effets sonores (samples)
	ressources.add_resource("rebond", "data/boing_x.wav", ES_Resources::ES_SAMPLE);
	ressources.add_resource("splash", "data/splash1.wav", ES_Resources::ES_SAMPLE);
	ressources.add_resource("splash2", "data/splash2.wav", ES_Resources::ES_SAMPLE);

	while (graphics->run())
	{
	    graphics->clear_screen(0, 125);

		while(event.poll_event())
		{
            switch(event.get_event_type())
            {
                case SDL_JOYBUTTONDOWN:
                    switch(event.get_sdlevent().jbutton.button)
                    {
                        case GP2X_BUTTON_START:
                            graphics->quit();
                        break;

                        // Pour les fonctions regardez plus bas :)
                        case GP2X_BUTTON_B: // bouton B du pad
                            sounds->music_fade_in("musique", 3000, 0);
                        break;

                        case GP2X_BUTTON_X: // Bouton X du pad
                            sounds->play_music("tetris");
                        break;

                        case GP2X_BUTTON_A: // Bouton A du pad
                            sounds->play_sample("rebond");
                        break;

                        case GP2X_BUTTON_Y: // Bouton Y du pad
                            sounds->play_sample("splash");
                        break;

                        case GP2X_BUTTON_L: // Bouton L du pad
                            sounds->play_sample("splash2");
                        break;

                        case GP2X_BUTTON_R: // Bouton R du pad
                            fade = false;
                            sounds->music_fade_out();
                        break;

                        case GP2X_BUTTON_SELECT: // Bouton Select
                            sounds->switch_music_mode();
                        break;

                        case GP2X_BUTTON_VOLUP: // Bouton Volume +
                            sounds->music_volume(0);
                        break;

                        case GP2X_BUTTON_VOLDOWN: // Bouton Volume -
                            sounds->music_volume(ES_MAX_VOLUME);
                        break;

                        case GP2X_BUTTON_UP: // Bouton du pad haut
                            sounds->music_stop();
                        break;

                        case GP2X_BUTTON_DOWN: // Bouton du pad bas
                            sounds->sample_volume("rebond", ES_MAX_VOLUME/2);
                            sounds->sample_volume("splash", ES_MAX_VOLUME/2);
                            sounds->sample_volume("splash2", ES_MAX_VOLUME/2);
                        break;

                        case GP2X_BUTTON_LEFT: // Bouton du pad gauche
                            sounds->sample_volume("rebond", ES_MAX_VOLUME);
                            sounds->sample_volume("splash", ES_MAX_VOLUME);
                            sounds->sample_volume("splash2", ES_MAX_VOLUME);
                        break;
                    }
                break;

                case SDL_QUIT:
                    graphics->quit();
                    break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        /* Ok, maintenant c'est ici que �a deviens int�r�ssant, tout d'abord la premi�re
                           musique d�marrera lorqu'on appui sur la touche F1 avec un effet de fondu d'entr�e
                           (fade in) et se jouera normalement jusqu'a la fin, sauf si on l'arr�te o� que l'on joue
                           une autre musique, le deuxi�me param�tre (3000) indique la dur�e de l'effet en
                           millisecondes ce qui �quivaut ici � un fondu d'entr�e de 3secondes. */
                        case SDLK_F1:
                            sounds->music_fade_in("musique", 3000, 0);
                        break;

                        // La deuxi�me musique d�marre en appuyant sur la touche F2 (pas d'effets particuliers)
                        case SDLK_F2:
                            sounds->play_music("tetris");
                        break;

                        // F3 ? joue le sample "boing"
                        case SDLK_F3:
                            sounds->play_sample("rebond");
                        break;

                        // F4 ? joue le sample splash
                        case SDLK_F4:
                            sounds->play_sample("splash");
                        break;

                        // F5 ? joue le sample spalsh2
                        case SDLK_F5:
                            sounds->play_sample("splash2");
                        break;

                        // effectue un fade out sur la musique actuellement jou�, et joue une autre en fade in
                        // le fondu de sortie est un effet qui peut �tre utile quand on veut glisser
                        // d'une musique � une autre sans que le changement soit brusque
                        // param�tre par d�faut = 3sec de dur�e si on veut changer �a donnera pour 4sec :
                        // sounds->music_fade_out(4000);
                        case SDLK_f:
                            fade = false;
                            sounds->music_fade_out(); // cr�e le fade out d'une dur�e de 3sec sur la musique actuel
                        break;

                        /* La touche p � �ta�t appuyer ? on switch le mode de la musique actuel (pause/reprise)
                           si elle est jou� elle est mise en pause sinon elle est reprise. */
                        case SDLK_p:
                            sounds->switch_music_mode();
                        break;

                        // si touche m ("," sur clavier QWERTY) on mute la musique actuel (volume = 0 mais la musique continue d'�tre jou�)
                        case SDLK_m:
                            sounds->music_volume(0);
                        break;

                        // si touche r on reprend la musique qui � �ta�t muter (r�gle le volume � fond)
                        case SDLK_r:
                            sounds->music_volume(ES_MAX_VOLUME);
                        break;

                        // stop la musique en cours
                        case SDLK_s:
                            sounds->music_stop();
                        break;

                        // r�gle le volume des 3 samples � la moiti�
                        case SDLK_h:
                            sounds->sample_volume("rebond", ES_MAX_VOLUME/2);
                            sounds->sample_volume("splash", ES_MAX_VOLUME/2);
                            sounds->sample_volume("splash2", ES_MAX_VOLUME/2);
                        break;

                        // remet le volume des samples en mode normal (� fond)
                        case SDLK_g:
                            sounds->sample_volume("rebond", ES_MAX_VOLUME);
                            sounds->sample_volume("splash", ES_MAX_VOLUME);
                            sounds->sample_volume("splash2", ES_MAX_VOLUME);
                        break;

                        case SDLK_ESCAPE:
                            graphics->quit(); // quitte si echap
                        break;
                    }
                break;
            }
		}

        if(sounds->get_fade_out_state() == true && fade == false) // si le fade out est terminer
        {
            sounds->music_fade_in("tetris", 3000, 0); // on joue la musique tetris avec un fade in
            fade = true;
        }

        // on v�rifie si la musique est en pause, si oui on affiche les infos
        if(sounds->music_is_paused() == true)
            fonts->draw_text("police", "La musique est en pause!", 0, 20+6, graphics->color(0,0,0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

        // on v�rifie si une musique est en train d'�tre jou�
        if(sounds->music_is_played() == true)
            fonts->draw_text("police", "La musique est jou�!", 0, 6, graphics->color(0,0,0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        else
            fonts->draw_text("police", "Musique arr�t�!", 0, 6, graphics->color(0,0,0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		graphics->flip(2);
	}

	// on lib�re la m�moire
	ressources.clear_resources(); // d�charge toutes les ressources charg�s
	delete fonts;
	delete sounds; // quitte le gestionnaire sonore proprement
	delete graphics;

	return EXIT_SUCCESS;
}

//- Note :
//        Des probl�mes peuvent survenir lors de la tentative de lecture de fichiers MP3, cela provient
//        de bugs r�cent dans la librarie SDL_Mixer.
//        La seul r�solution possible est de mettre ses fichiers en .ogg (ogg vorbis, meilleur compression/qualit�)
//        et c'est un format gratuit et LIBRE o� d'attendre une nouvelle version de SDL_Mixer qui corrige
//        ce bug.

//        Les sons proviennent du FPS multijoueur libre Sauerbraten : http://www.sauerbraten.org/
//        Tout est libre et gratuit � utiliser.
