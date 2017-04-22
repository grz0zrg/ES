// ES engine - Exemple 1
//- Premier programme utilisant SDL pour l'affichage
#include <ES/ES_Display/graphics.h> // Pour utiliser les fonctions graphiques du moteur
#include <ES/ES_Event/event.h> // Pour utiliser la gestion des évenements du moteur

using namespace ES; // permet de ne pas rajouter le préfixe ES:: à chaques début de fonctions

int main(int argc, char **argv)
{
	// Crée le gestionnaire d'évenement
	ES_Event event;

// Permet le portage sur GP2X & autres plateformes
#ifdef GP2X
	// Ouvre une fenêtre de taille 320x240 en couleurs 16 bits
	// avec le titre "ES engine - Sample 1" (inutile sur GP2X)
	// Note: Sur GP2X il faut tous le temps entrer une résolution
	//       de 320x240 car c'est la résolution de l'écran
    ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 1", 320, 240, 16, false);
#else
	// Ouvre une fenêtre en mode fenêtré et de taille 800x600 en couleurs 32 bits
	// avec le titre "ES engine - Sample 1"
	ES_Graphics *graphics = new ES_Graphics("ES engine - Sample 1", 800, 600, 32, false);
#endif

	while (graphics->run()) // boucle principale
	{
		event.wait_event(); // attend un événement
		switch(event.get_event_type()) // un événement à eu lieu ? récupére le type
		{
		    // Utile seulement sur GP2X (récupére les évenements GP2X quand un bouton est appuyé)
            case SDL_JOYBUTTONDOWN:
                switch(event.get_sdlevent().jbutton.button)
                {
                    case GP2X_BUTTON_START: // si l'utilisateur appuie sur le bouton Start
                        graphics->quit(); // on quitte notre boucle
                    break;
                }
            break;

			case SDL_QUIT: // si l'utilisateur ferme la fenêtre manuellement (type SDL_QUIT)
				graphics->quit(); // on quitte notre boucle
				break;
		}

		graphics->clear_screen(); // efface l'écran avec une couleur noir

        // crée un fade (fondu progressif) du fond (noir -> blanc -> noir) etc...
        graphics->screen_fade();

		graphics->flip(2); // met à jour l'écran avec un delay de 2ms pour libérer le CPU
	}

	// on libére la mémoire
	delete graphics;

	return EXIT_SUCCESS;
}

//- Note : Bougez votre souris dans la fenêtre pour qu'un fade du fond se forme.
//         Pourquoi être obliger de bougez la souris ?
//          - Car event.wait_event() bloque le programme jusqu'a temps qu'un évenement à lieu.
//         Voir les prochains exemples pour une autre fonction non bloquante. ;)
//  GP2X : Cet exemple fonctionne sur GP2X mais ne crée aucun fade. (voir dans les autres exemples
//         pour l'utilisation du pad GP2X ou stylet)
