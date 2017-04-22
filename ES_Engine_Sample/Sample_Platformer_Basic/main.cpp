// ES engine - Sample Platformer 1°
//- Création d'un mini jeu de plates formes sans animations avec
//  1 carte et 1 scrolling horizontale (touches <- et ->) avec OpenGL & SDL
//  la carte est crée manuellement via un tableau
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Event/event.h>

using namespace ES;

// Définie la taille de la carte
#define CARTE_LARGEUR 20
#define CARTE_HAUTEUR 4
#define MODE ES_Graphics::ES_OPENGL // changez en ES_SDL pour utiliser SDL

// les données de notre carte sont stocker dans un tableau, 0 = vide, 1 = sol, 2 = angle droit,
// 3 = sol angle gauche, 4 = bloc, 5 = tube, 6 = porte bas, 7 = porte haut
// 20 cases de largeurs et 10 cases de hauteurs, à regarder la tête pencher :p
int carte [CARTE_LARGEUR][CARTE_HAUTEUR] = {0, 0, 0, 3,
											0, 0, 0, 1,
											0, 0, 0, 1,
											0, 0, 0, 1,
											0, 0, 4, 1,
											0, 5, 4, 1,
											0, 0, 4, 1,
											0, 0, 0, 2,
											0, 0, 0, 0,
											0, 0, 0, 3,
											0, 0, 0, 1,
											0, 0, 4, 1,
											0, 4, 4, 1,
											0, 0, 4, 1,
											0, 0, 4, 1,
											0, 0, 0, 2,
											0, 0, 0, 3,
											0, 0, 5, 1,
											0, 7, 6, 1,
											0, 0, 0, 2};

ES_Graphics *graphics;
int i, j;
int decalage = 2, offset_X = 0; // variables liés au scrolling
bool left = false, right = false; // GP2X

void draw_map(int offset_X);

int main(int argc, char **argv)
{
	ES_Resources ressources;
	ES_Event event;

	// ouvre un écran en 240x64 fenêtré sans curseur (dernier paramétre = cache le curseur)
#ifdef GP2X
	graphics = new ES_Graphics("ES engine - Platformer", 320, 240, 16, false, false, true);
#else
    graphics = new ES_Graphics("ES engine - Platformer", 240, 64, 32, false, MODE, true);
#endif

	// initialise OpenGL si le mode est opengl
	if(MODE == ES_Graphics::ES_OPENGL)
	{
		graphics->opengl_init();
		graphics->set_orto_view();
	}

	// chargement des images
	ressources.add_resource("fond", "data/fond/fond.png", ES_Resources::ES_IMG); // le fond
	ressources.add_resource("sol", "data/tiles/sol.png", ES_Resources::ES_IMG); // le tile sol
	ressources.add_resource("porte bas", "data/tiles/porte_bas.png", ES_Resources::ES_IMG);
	ressources.add_resource("porte haut", "data/tiles/porte_haut.png", ES_Resources::ES_IMG);
	// chargement des images avec comme couleur de transparence le bleu clair
	ressources.add_resource("sol rebord droit", "data/tiles/sol_rebordd.png", ES_Resources::ES_IMG, false, NULL, true, 0, 255, 255);
	ressources.add_resource("sol rebord gauche", "data/tiles/sol_rebordg.png", ES_Resources::ES_IMG, false, NULL, true, 0, 255, 255);
	ressources.add_resource("bloc bois", "data/tiles/bloc_bois.png", ES_Resources::ES_IMG, false, NULL, true, 0, 255, 255);
	ressources.add_resource("tube", "data/tiles/tube.png", ES_Resources::ES_IMG, false, NULL, true, 0, 255, 255);

	// répétitions des touches (valeurs très basse pour que le scrolling soit fluide)
	event.set_key_repeat(1, 0);

	while (graphics->run())
	{
		graphics->clear_screen();

		if(offset_X <= 0) // permet de bloquer le scroll pour rester dans le cadre de la map
			offset_X = 0;
		else if(offset_X >= 96)
			offset_X = 96;

		// affiche le fond toujours en rapport avec le scrolling avec un léger décalage (vitesse amoindri)
		// entre les tiles de la carte (avant les maps pour qu'il soit dérriére)
		graphics->draw_image("fond", graphics->rect(0, 0, 512, 128), graphics->rect(0 - offset_X/decalage, 0, 512, 128));

		draw_map(offset_X); // on affiche notre carte

		// partie évenements
		while(event.poll_event())
		{
            switch(event.get_event_type())
            {
                // Pour la GP2X les évenements continues sont un peu spécial
                // nous devons incrémenter le scroll quand l'utilisateur reste appuiyé
                // mais quand il relache nous devons arréter d'incrémenter
                // il n'y à pas de gestion de touches répétitive pour la GP2X
                case SDL_JOYBUTTONDOWN:
                    switch(event.get_sdlevent().jbutton.button)
                    {
                        case GP2X_BUTTON_START:
                            graphics->quit();
                        break;

                        case GP2X_BUTTON_LEFT:
                            left = true;
                        break;

                        case GP2X_BUTTON_RIGHT:
                            right = true;
                        break;
                    }
                break;

                case SDL_JOYBUTTONUP: // Bouton relaché
                    switch(event.get_sdlevent().jbutton.button)
                    {
                        case GP2X_BUTTON_LEFT:
                            left = false;
                        break;

                        case GP2X_BUTTON_RIGHT:
                            right = false;
                        break;
                    }
                break;

                case SDL_QUIT: // ferme la fenêtre ? on quitte
                    graphics->quit();
                    break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        case SDLK_ESCAPE: // appui sur echap ? on quitte
                            graphics->quit();
                        break;

                        // astuce pour la fluidité incrementez/dec offset_X de la taille de vos tiles
                        // ça permettra si vous n'avez pas besoin d'un scroll 'précis' de ne pas voir
                        // l'effet de bave des tiles quand on scroll
                        case SDLK_LEFT: // on appui sur fléche gauche ? on scroll vers la gauche
                            offset_X -= 1;
                        break;

                        case SDLK_RIGHT: // pareil si fléche droite appuyer, on scroll vers la droite
                            offset_X += 1;
                        break;
                    }
                break;
            }
		}

		// Gestion des répétitions pour la GP2X
		#ifdef GP2X
		if(left == true)
            offset_X -= 1;
        else if(right == true)
            offset_X += 1;
        #endif

		// peut jouer sur la fluidité du scrolling (une valeur basse peut amoindrir l'effet de 'bave' des tiles quand on scroll)
		graphics->flip(1);
	}

	ressources.clear_resources(); // détruit toutes les images précédement chargés
	delete graphics;

	return EXIT_SUCCESS;
}

// Fonction d'affichage de notre carte
void draw_map(int offset_X)
{
	// parcoure le tableau et place les tiles
	for (i = 0; i < CARTE_LARGEUR; i++){
		for (j = 0; j < CARTE_HAUTEUR; j++){
			if(carte[i][j] == 1) // si c'est un bloc sol on affiche le sprite sol
				graphics->draw_image("sol", graphics->rect(0, 0, 16, 16), graphics->rect(i * 16 - offset_X, j * 16, 16, 16));
			else if(carte[i][j] == 2) // si c'est un bloc sol angle droit on affiche avec la transparence du bleu clair
				graphics->draw_image("sol rebord droit", graphics->rect(0, 0, 16, 16), graphics->rect(i * 16 - offset_X, j * 16, 16, 16), 255, true);
			else if(carte[i][j] == 3) // etc...
				graphics->draw_image("sol rebord gauche", graphics->rect(0, 0, 16, 16), graphics->rect(i * 16 - offset_X, j * 16, 16, 16), 255, true);
			else if(carte[i][j] == 4)
				graphics->draw_image("bloc bois", graphics->rect(0, 0, 16, 16), graphics->rect(i * 16 - offset_X, j * 16, 16, 16), 255, true);
			else if(carte[i][j] == 5)
				graphics->draw_image("tube", graphics->rect(0, 0, 16, 16), graphics->rect(i * 16 - offset_X, j * 16, 16, 16), 255, true);
			else if(carte[i][j] == 6)
				graphics->draw_image("porte bas", graphics->rect(0, 0, 16, 16), graphics->rect(i * 16 - offset_X, j * 16, 16, 16));
			else if(carte[i][j] == 7)
				graphics->draw_image("porte haut", graphics->rect(0, 0, 16, 16), graphics->rect(i * 16 - offset_X, j * 16, 16, 16));
		}
	}
}

// Note : Avec OpenGL le mieux étant de charger des textures ^2 (64x64, 256x256, 1024x512 etc...)
//        sinon des bugs d'affichages peuvent apparaître et la fluidité du jeu seras réduit

// En mode OpenGL le scrolling seras très (trop) rapide c'est dû au faît que OpenGL gére les coordonées
// en nombres à virgules (float) beaucoups plus prècis que SDL, si vous voulez un scrolling moins rapide
// avec OpenGL il faut diviser offset_X exemple : (offset_X/4) comme quand on affiche le fond ou utiliser
// la gestion du framerate d'ES.
