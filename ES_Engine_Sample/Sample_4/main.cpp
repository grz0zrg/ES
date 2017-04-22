// ES engine - Exemple 4
//- Ouverture d'une fen�tre SDL, chargement et lecture d'un fichier musical, lecture d'un sample et
//  affichage de textes � l'�cran avec diff�rentes polices
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Display/ttf.h> // pour l'affichage de textes
#include <ES/ES_Event/event.h>
#include <ES/ES_Sound/sound.h> // pour jouer les musiques

using namespace ES;

int main(int argc, char **argv)
{
    int rouge = 0, vert = 0, bleu = 0;

	ES_Resources ressources;
	ES_Event event;

#ifdef GP2X
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 4", 320, 240, 16, false);
#else
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 4", 450, 200, 32, false);
#endif

	// initialisation du gestionnaire sonore (attention doit toujours �tre appeler apr�s la cr�ation
	// d'une fen�tre)
	ES_Sound *sounds = new ES_Sound();

	// initialisation des polices, doit �tre appeler apr�s la cr�ation d'une fen�tre
	ES_Ttf *fonts = new ES_Ttf();

    fonts->set_size(24); // on r�gle la taille de notre premi�re font
    // on charge la premi�re police (police simple)
    ressources.add_resource("police basique", "arial.ttf", ES_Resources::ES_TTF_FONT);

    fonts->set_size(48); // on r�gle la taille de notre deuxi�me font (2x plus grosses que la premi�re)
    // on charge notre deuxi�me font (font plus jolie/complexe trouv� sur http://www.dafont.com/fr)
    ressources.add_resource("police starcraft", "starcraft.ttf", ES_Resources::ES_TTF_FONT);

	// indique qu'on souhaite ajouter/charger un fichier musical
	ressources.add_resource("smw intro", "smwintro.mid", ES_Resources::ES_MUSIC);
	// indique que l'on souhaite charger un fichier sample (voir note en bas pour conna�tre les diff�rence)
	ressources.add_resource("smw secret", "smw_key.wav", ES_Resources::ES_SAMPLE);

	while (graphics->run())
	{
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

                        // Pour les fonctions relatives � la lectures des musiques regardez
                        // plus bas le code comment�
                        case GP2X_BUTTON_B: // Bouton B de la GP2X
                            sounds->play_music("smw intro");
                        break;

                        case GP2X_BUTTON_X: // Bouton X de la GP2X
                            sounds->play_sample("smw secret");
                        break;
                    }
                break;

                case SDL_QUIT:
                    graphics->quit();
                    break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        // si F1 est press� on joue la musique (si d�j� jou� �a la rejoue)
                        // une fois finis elle s'arr�te de jou�
                        // si vous voulez qu'elle se joue en boucle fa�te :
                        // sounds->play_music("smw intro", -1);
                        case SDLK_F1:
                            sounds->play_music("smw intro");
                        break;

                        // si F2 est press� joue le sample
                        // il est possible comme pour la musique de jou� le sample en boucle
                        // il suffit de faire :
                        // sounds->play_sample("smw secret", -1);
                        case SDLK_F2:
                            sounds->play_sample("smw secret");
                        break;

                        // touche echap appuyer, on quitte
                        case SDLK_ESCAPE:
                            graphics->quit();
                        break;
                    }
                break;
            }
		}

		graphics->clear_screen(0, 125);

        // affiche "Hello World !" � l'�cran avec la police arial.ttf (basique) avec une couleur noir
        // le get_screen_surface sert � r�cup�rer l� o� doit �tre afficher le texte et les 3 param�tres
        // facultatifs � la fin servent � positionner le texte ici 0, 6 = haut gauche de l'�cran (6 pixels pour �tre bien ne pas d�passer le haut de l'�cran)
        // la police va s'afficher en style solide cad qu'elle ne seras pas liss� (par d�faut)
		fonts->draw_text("police basique", "Hello World !", 0, 6, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_LEFT);

        // m�me chose que pr�c�dement sauf qu'on change le mode en ES_SHADED, la police est liss�
        // mais le fond n'est plus transparent, le dernier param�tre indique quel couleur de fond on souhaite
        // ici comme le fond de notre programme (vert) ce qui feras comme si c'�ta�t transparent
		fonts->draw_text("police basique", "Hello World !", 0, 24+6, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_SHADED, graphics->color(0, 125, 0));

        // pareil que pr�c�dement sauf qu'on met le texte en qualit� max (blended), fond transparent
        // + lissage de la police (� utiliser pour du texte statique sinon c'est assez lent)
		fonts->draw_text("police basique", "Hello World !", 0, 48+6, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

        // affiche "Hello World !" � l'�cran avec la police starcraft avec une couleur qui varie
        // au d�marrage du noir au blanc et � 24 pixel du bord haut de l'�cran (qui �quivaut � la taille
        // de la premi�re font), police en haute qualit� (ES_BLENDED) et positionner au milieu de l'�cran
        fonts->draw_text("police starcraft", "Starcraft !", graphics->get_window_width()/2, 72+6, graphics->color(rouge, vert, bleu), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

        // change la couleur du texte hello world avec la police starcraft
        if(rouge < 255 || vert < 255 || bleu < 255)
        {
            rouge++;
            vert++;
            bleu++;
        }

		graphics->flip(2);
	}

	// on lib�re la m�moire
	ressources.clear_resources(); // d�charge toutes les ressources charg�s
	delete fonts; // quitte le gestionnaire d'affichage de textes proprement /!\ � faire toujours apr�s ressources.clear_resources();
	delete sounds; // quitte le gestionnaire sonore proprement
	delete graphics;

	return EXIT_SUCCESS;
}

// Note :
//        Les justifications (ES_TTF_LEFT/ES_TTF_CENTER/ES_TTF_RIGHT) servent � d�finir l'origine du text,
//        ES_TTF_LEFT: Texte avec origine � 0
//        ES_TTF_CENTER: Texte avec l'origine tailledutexte/2
//        ES_TTF_RIGHT: Texte avec l'origine � taille du texte

//        Les samples sont de petits fichiers souvent en ".wav" tr�s cours qui doivent �tre jou�
//        rapidement � chaques actions d'un personnage par exemple (saut, pi�ce, ressort etc...)
//        Les samples peuvent �tre jou� plusieurs fois en m�me temps alors que les musiques non.
//        Pour test� ceci je vous conseille de r�appuyer plusieurs fois sur "F2" et vous entendrez
//        plusieurs fois le sample se jou� :p
//        Je vous conseille d'utiliser tout le temps les samples pour des sons tr�s court et les
//        musiques pour les musiques de votre jeu.
