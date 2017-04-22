// ES engine - Exemple 3
//- Ouverture d'une fenêtre opengl où sdl en utilisant un curseur personnalisé,
//  démonstration d'un changement de résolution et utilisation des rotations + transparences
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Event/event.h>

// modifiés la valeur pour passer de SDL à OpenGL :
//  - ES_OPENGL = utilise OpenGL (Sur GP2X les fonctions restes portable mais il n'y à pas de support OpenGL)
//  - ES_SDL = utilise SDL (Seul mode sur GP2X)
#define MODE ES_Graphics::ES_SDL

using namespace ES;

int main(int argc, char **argv)
{
    float angle = 0.0f;

	ES_Event event;
	ES_Resources ressources;

	// on indique le mode que l'on souhaite suivant ce qui à étaît définie,
	// et on indique qu'on veut un curseur perso situé dans le répertoire data/
	// le paramètre après MODE indique que l'on cache le curseur par défaut
	// sur gp2x le curseur est affiché mais n'est pas fonctionnelle
#ifdef GP2X
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 3", 320, 240, 16, false,
		MODE, true, "data/curseur.png");
#else
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 3", 800, 600, 32, false,
		MODE, true, "data/curseur.png");
#endif

    ressources.add_resource("image de test", "data/test.png", ES_Resources::ES_IMG, false, NULL, true, 0, 0, 0);

	// si le mode définie est OpenGL
	if(MODE == ES_Graphics::ES_OPENGL)
	{
		graphics->opengl_init(); // on initialise opengl avec les paramétres par défaut
		graphics->set_orto_view(); // on définie qu'on souhaite une vue orthogonale (pratique pour la 2D)
	}

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
                    }
                break;

                case SDL_QUIT:
                    graphics->quit();
                    break;

                case SDL_KEYDOWN: // une touche à étaît pressé ?
                    // permet de savoir quel sorte de touches à étaît pressé
                    switch (event.get_event_key_type())
                    {
                        // si echap est pressé on passe en plein écran 1024x768
                        // si l'utilisateur réappuis sur
                        // la touche on repasse en fenêtre avec la même résolution (1024x768)
                        // si il réappui sur echap la fenêtre repasse en plein écran et ainsi de suite...
                        case SDLK_ESCAPE:
                            graphics->switch_window_mode(1024, 768); // Ne marche pas sur GP2X
                        break;
                    }
                break;
            }
		}

		graphics->clear_screen();

        /* Le dernier paramètre indique l'angle en degrée de rotations de l'image.
        *  La transparence est au maximum.
        *  Note: Avec OpenGL il est impossible d'utiliser une colorkey et la transparence en même temp.
        */
        graphics->draw_image("image de test", graphics->rect(0, 0, 256, 256), graphics->rect(graphics->get_window_width()/2 - 256/2, graphics->get_window_height()/2 - 256/2, 256, 256), 255, true, angle);

		graphics->display_custom_cursor(graphics->rect(0, 0, 24, 24)); // on affiche le curseur personnalisé

		graphics->flip(2);

		angle++; // incrémente l'angle de rotation
	}

    ressources.clear_resources();
	delete graphics;

	return EXIT_SUCCESS;
}

// Notes:
// Comme vous pouvez le voir ce qui change est simplement la façon d'initialiser l'écran ainsi
// les 2 lignes concernants l'initialisation d'OpenGL.
// Le moteur active automatiquement le curseur par défaut de la SDL si il n'est pas arrivé à ouvrir
// le fichier du curseur, il prévient juste en sortie (fichier stderr)
// qu'il n'est pas arrivé à charger le curseur.

// Il est possible de définir la colorkey (couleur de transparence) pour le curseur, par défaut
// la colorkey est de 255 / 0 / 255 (rose) mais vous pouvez la modifier en rajoutant ses options
// à la fin du constructeur ES_Graphics (voir la définition du constructeur
// dans le fichier graphics.h pour plus d'informations)

// Sur GP2X il est presque inutile d'utiliser le curseur.
