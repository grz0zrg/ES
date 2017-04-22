// ES engine - Exemple 4
//- Ouverture d'une fenêtre SDL, chargement et lecture d'un fichier musical, lecture d'un sample et
//  affichage de textes à l'écran avec différentes polices
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

	// initialisation du gestionnaire sonore (attention doit toujours être appeler après la création
	// d'une fenêtre)
	ES_Sound *sounds = new ES_Sound();

	// initialisation des polices, doit être appeler après la création d'une fenêtre
	ES_Ttf *fonts = new ES_Ttf();

    fonts->set_size(24); // on régle la taille de notre premiére font
    // on charge la premiére police (police simple)
    ressources.add_resource("police basique", "arial.ttf", ES_Resources::ES_TTF_FONT);

    fonts->set_size(48); // on régle la taille de notre deuxiéme font (2x plus grosses que la premiére)
    // on charge notre deuxiéme font (font plus jolie/complexe trouvé sur http://www.dafont.com/fr)
    ressources.add_resource("police starcraft", "starcraft.ttf", ES_Resources::ES_TTF_FONT);

	// indique qu'on souhaite ajouter/charger un fichier musical
	ressources.add_resource("smw intro", "smwintro.mid", ES_Resources::ES_MUSIC);
	// indique que l'on souhaite charger un fichier sample (voir note en bas pour connaître les différence)
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

                        // Pour les fonctions relatives à la lectures des musiques regardez
                        // plus bas le code commenté
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
                        // si F1 est pressé on joue la musique (si déjà joué ça la rejoue)
                        // une fois finis elle s'arréte de joué
                        // si vous voulez qu'elle se joue en boucle faîte :
                        // sounds->play_music("smw intro", -1);
                        case SDLK_F1:
                            sounds->play_music("smw intro");
                        break;

                        // si F2 est pressé joue le sample
                        // il est possible comme pour la musique de joué le sample en boucle
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

        // affiche "Hello World !" à l'écran avec la police arial.ttf (basique) avec une couleur noir
        // le get_screen_surface sert à récupérer là où doit être afficher le texte et les 3 paramétres
        // facultatifs à la fin servent à positionner le texte ici 0, 6 = haut gauche de l'écran (6 pixels pour être bien ne pas dépasser le haut de l'écran)
        // la police va s'afficher en style solide cad qu'elle ne seras pas lissé (par défaut)
		fonts->draw_text("police basique", "Hello World !", 0, 6, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_LEFT);

        // même chose que précédement sauf qu'on change le mode en ES_SHADED, la police est lissé
        // mais le fond n'est plus transparent, le dernier paramétre indique quel couleur de fond on souhaite
        // ici comme le fond de notre programme (vert) ce qui feras comme si c'étaît transparent
		fonts->draw_text("police basique", "Hello World !", 0, 24+6, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_SHADED, graphics->color(0, 125, 0));

        // pareil que précédement sauf qu'on met le texte en qualité max (blended), fond transparent
        // + lissage de la police (à utiliser pour du texte statique sinon c'est assez lent)
		fonts->draw_text("police basique", "Hello World !", 0, 48+6, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

        // affiche "Hello World !" à l'écran avec la police starcraft avec une couleur qui varie
        // au démarrage du noir au blanc et à 24 pixel du bord haut de l'écran (qui équivaut à la taille
        // de la première font), police en haute qualité (ES_BLENDED) et positionner au milieu de l'écran
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

	// on libére la mémoire
	ressources.clear_resources(); // décharge toutes les ressources chargés
	delete fonts; // quitte le gestionnaire d'affichage de textes proprement /!\ à faire toujours après ressources.clear_resources();
	delete sounds; // quitte le gestionnaire sonore proprement
	delete graphics;

	return EXIT_SUCCESS;
}

// Note :
//        Les justifications (ES_TTF_LEFT/ES_TTF_CENTER/ES_TTF_RIGHT) servent à définir l'origine du text,
//        ES_TTF_LEFT: Texte avec origine à 0
//        ES_TTF_CENTER: Texte avec l'origine tailledutexte/2
//        ES_TTF_RIGHT: Texte avec l'origine à taille du texte

//        Les samples sont de petits fichiers souvent en ".wav" très cours qui doivent être joué
//        rapidement à chaques actions d'un personnage par exemple (saut, pièce, ressort etc...)
//        Les samples peuvent être joué plusieurs fois en même temps alors que les musiques non.
//        Pour testé ceci je vous conseille de réappuyer plusieurs fois sur "F2" et vous entendrez
//        plusieurs fois le sample se joué :p
//        Je vous conseille d'utiliser tout le temps les samples pour des sons très court et les
//        musiques pour les musiques de votre jeu.
