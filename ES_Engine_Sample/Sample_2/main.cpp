// ES engine - Exemple 2
//- Ouverture d'une fen�tre SDL et chargement/affichage d'une image avec transparence d'une couleur
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Event/event.h>
#include <ES/ES_Display/ttf.h>

using namespace ES;

int main(int argc, char **argv)
{
	ES_Resources ressources; // cr�e le gestionnaire de ressources d'ES
	ES_Event event; // cr�e le gestionnaire d'�venements

	// aucuns changements ici par rapport � l'exemple pr�c�dent
#ifdef GP2X
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 2", 320, 240, 16, false);
#else
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 2", 800, 600, 32, false);
#endif

	// indique qu'on souhaite ajouter/charger l'image "test.png" de type ES_IMG (image) ayant
	// l'identifiant "image de test" au gestionnaire de ressources (le dernier param�tre indique que
	// l'image doit �tre charger avec une couleur de transparence (par d�faut la couleur de transparence
	// est noir, 0, 0, 0)
	ressources.add_resource("image de test", "test.png", ES_Resources::ES_IMG, false, NULL, true, 0, 0, 0);

	while (graphics->run())
	{
	    // rend le programme non bloquant (contraire de wait_event())
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
            }
		}

		// efface l'�cran
		graphics->clear_screen();

		// pareil que pr�c�dement sauf que gr�ce � poll_event() il n'est pas obliger
		// d'y avoir un �venement pour que �a prenne effet
		graphics->screen_fade();

		// affiche notre image pr�c�dement charger au centre de l'�cran avec la colorkey active (dernier
		// param�tre) la m�thode rect sp�cifie ou notre image doit �tre afficher et sur quel surface
		// rect(position_x, position_y, largeur, hauteur) le premier rect correspond � la surface de d�part
		// par exemple si on veut blitter qu'un morceau de l'image (ici on blitte tout donc 256, 256)
		// l'avant dernier param�tre (avec la valeur 255) correspond � la transparence de l'image, ici opaque (0 = compl�tement transparent)
		graphics->draw_image("image de test", graphics->rect(0, 0, 256, 256), graphics->rect(graphics->get_window_width()/2 - 256/2, graphics->get_window_height()/2 - 256/2, 256, 256), 255, true);

		graphics->flip(2); // met � jour l'�cran avec un delay de 2ms pour lib�rer le CPU
	}

	// on lib�re la m�moire
	ressources.clear_resources(); // d�charge les images charg�s
	delete graphics;

	return EXIT_SUCCESS;
}
