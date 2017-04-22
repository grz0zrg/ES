// ES engine - Exemple 1
//- Premier programme utilisant SDL pour l'affichage
#include <ES/ES_Display/graphics.h> // Pour utiliser les fonctions graphiques du moteur
#include <ES/ES_Event/event.h> // Pour utiliser la gestion des �venements du moteur

using namespace ES; // permet de ne pas rajouter le pr�fixe ES:: � chaques d�but de fonctions

int main(int argc, char **argv)
{
	// Cr�e le gestionnaire d'�venement
	ES_Event event;

// Permet le portage sur GP2X & autres plateformes
#ifdef GP2X
	// Ouvre une fen�tre de taille 320x240 en couleurs 16 bits
	// avec le titre "ES engine - Sample 1" (inutile sur GP2X)
	// Note: Sur GP2X il faut tous le temps entrer une r�solution
	//       de 320x240 car c'est la r�solution de l'�cran
    ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 1", 320, 240, 16, false);
#else
	// Ouvre une fen�tre en mode fen�tr� et de taille 800x600 en couleurs 32 bits
	// avec le titre "ES engine - Sample 1"
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 1", 800, 600, 32, false);
#endif

	while (graphics->run()) // boucle principale
	{
		event.wait_event(); // attend un �v�nement
		switch(event.get_event_type()) // un �v�nement � eu lieu ? r�cup�re le type
		{
		    // Utile seulement sur GP2X (r�cup�re les �venements GP2X quand un bouton est appuy�)
            case SDL_JOYBUTTONDOWN:
                switch(event.get_sdlevent().jbutton.button)
                {
                    case GP2X_BUTTON_START: // si l'utilisateur appuie sur le bouton Start
                        graphics->quit(); // on quitte notre boucle
                    break;
                }
            break;

			case SDL_QUIT: // si l'utilisateur ferme la fen�tre manuellement (type SDL_QUIT)
				graphics->quit(); // on quitte notre boucle
				break;
		}

		graphics->clear_screen(); // efface l'�cran avec une couleur noir

        // cr�e un fade (fondu progressif) du fond (noir -> blanc -> noir) etc...
        graphics->screen_fade();

		graphics->flip(2); // met � jour l'�cran avec un delay de 2ms pour lib�rer le CPU
	}

	// on lib�re la m�moire
	delete graphics;

	return EXIT_SUCCESS;
}

//- Note : Bougez votre souris dans la fen�tre pour qu'un fade du fond se forme.
//         Pourquoi �tre obliger de bougez la souris ?
//          - Car event.wait_event() bloque le programme jusqu'a temps qu'un �venement � lieu.
//         Voir les prochains exemples pour une autre fonction non bloquante. ;)
//  GP2X : Cet exemple fonctionne sur GP2X mais ne cr�e aucun fade. (voir dans les autres exemples
//         pour l'utilisation du pad GP2X ou stylet)
