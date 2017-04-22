// ES engine - Exemple 3
//- Ouverture d'une fen�tre opengl o� sdl en utilisant un curseur personnalis�,
//  d�monstration d'un changement de r�solution et utilisation des rotations + transparences
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Event/event.h>

// modifi�s la valeur pour passer de SDL � OpenGL :
//  - ES_OPENGL = utilise OpenGL (Sur GP2X les fonctions restes portable mais il n'y � pas de support OpenGL)
//  - ES_SDL = utilise SDL (Seul mode sur GP2X)
#define MODE ES_Graphics::ES_SDL

using namespace ES;

int main(int argc, char **argv)
{
    float angle = 0.0f;

	ES_Event event;
	ES_Resources ressources;

	// on indique le mode que l'on souhaite suivant ce qui � �ta�t d�finie,
	// et on indique qu'on veut un curseur perso situ� dans le r�pertoire data/
	// le param�tre apr�s MODE indique que l'on cache le curseur par d�faut
	// sur gp2x le curseur est affich� mais n'est pas fonctionnelle
#ifdef GP2X
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 3", 320, 240, 16, false,
		MODE, true, "data/curseur.png");
#else
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 3", 800, 600, 32, false,
		MODE, true, "data/curseur.png");
#endif

    ressources.add_resource("image de test", "data/test.png", ES_Resources::ES_IMG, false, NULL, true, 0, 0, 0);

	// si le mode d�finie est OpenGL
	if(MODE == ES_Graphics::ES_OPENGL)
	{
		graphics->opengl_init(); // on initialise opengl avec les param�tres par d�faut
		graphics->set_orto_view(); // on d�finie qu'on souhaite une vue orthogonale (pratique pour la 2D)
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

                case SDL_KEYDOWN: // une touche � �ta�t press� ?
                    // permet de savoir quel sorte de touches � �ta�t press�
                    switch (event.get_event_key_type())
                    {
                        // si echap est press� on passe en plein �cran 1024x768
                        // si l'utilisateur r�appuis sur
                        // la touche on repasse en fen�tre avec la m�me r�solution (1024x768)
                        // si il r�appui sur echap la fen�tre repasse en plein �cran et ainsi de suite...
                        case SDLK_ESCAPE:
                            graphics->switch_window_mode(1024, 768); // Ne marche pas sur GP2X
                        break;
                    }
                break;
            }
		}

		graphics->clear_screen();

        /* Le dernier param�tre indique l'angle en degr�e de rotations de l'image.
        *  La transparence est au maximum.
        *  Note: Avec OpenGL il est impossible d'utiliser une colorkey et la transparence en m�me temp.
        */
        graphics->draw_image("image de test", graphics->rect(0, 0, 256, 256), graphics->rect(graphics->get_window_width()/2 - 256/2, graphics->get_window_height()/2 - 256/2, 256, 256), 255, true, angle);

		graphics->display_custom_cursor(graphics->rect(0, 0, 24, 24)); // on affiche le curseur personnalis�

		graphics->flip(2);

		angle++; // incr�mente l'angle de rotation
	}

    ressources.clear_resources();
	delete graphics;

	return EXIT_SUCCESS;
}

// Notes:
// Comme vous pouvez le voir ce qui change est simplement la fa�on d'initialiser l'�cran ainsi
// les 2 lignes concernants l'initialisation d'OpenGL.
// Le moteur active automatiquement le curseur par d�faut de la SDL si il n'est pas arriv� � ouvrir
// le fichier du curseur, il pr�vient juste en sortie (fichier stderr)
// qu'il n'est pas arriv� � charger le curseur.

// Il est possible de d�finir la colorkey (couleur de transparence) pour le curseur, par d�faut
// la colorkey est de 255 / 0 / 255 (rose) mais vous pouvez la modifier en rajoutant ses options
// � la fin du constructeur ES_Graphics (voir la d�finition du constructeur
// dans le fichier graphics.h pour plus d'informations)

// Sur GP2X il est presque inutile d'utiliser le curseur.
