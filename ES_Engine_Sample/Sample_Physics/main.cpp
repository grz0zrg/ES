// ES engine - Sample Physics 1°
//- Exemple montrant l'utilisation de la physique avec ES.
//  Affichage de boîtes tombantes via les clics souris et le stylet pour la GP2X.
#include <ES/es.h>
#include <ES/ES_Utils/utils.h>
#include <ES/ES_Utils/timer.h> // pour le timer
#include <ES/ES_Physics/physics.h> // utilise pour les fonctions physiques

using namespace ES;

#define MODE ES_Graphics::ES_SDL // changez en ES_SDL pour utiliser SDL

ES_Graphics *graphics;

int main(int argc, char **argv)
{
    ES_MouseEvent mouse;
    ES_Physics physics;
    ES_Timer *timer; // timer pour gérer le nombres de boîtes par pression

    // crée et défini le monde physique
    // les 4 premiers paramétres sont la zone en rectangle ou toutes les interactions physiques
    // seront faîtes, icile maximum est de la taille de l'écran pour la droite et le haut du
    // rectangle (320*240).
    // Le dernier paramétre indique la gravité horizontale (ici on souhaite une gravité terrestre normal
    // donc on à pas besoin de la gravité horizontal), regardez la définition de setup_world dans la
    // documentation pour plus d'informations.
    physics.setup_world(-100,-1000.0f,320.0f,240.0f,0.0f);

    // Ajoute un plan (sol) en bas de l'écran
    // les deux premiers paramétres sont la largeur du sol et les deux derniers ça position
    // ici on défini ça largeur à 320 (largeur de l'écran), ça hauteur à 1, ça position x à 0 et
    // ça position y à la hauteur de l'écran
    physics.add_ground(320.0f,1.0f,0.0f,240.0f-10.0f);

    int numbox=1; // va définir notre nombres de boîtes par clics
    float y=0.0f;

	ES_Resources ressources;
	ES_Event event;
	ES_Ttf *ttf;

	ttf = new ES_Ttf();
	ttf->set_size(18);

	// ouvre un écran en 320x240 fenêtré sans curseur (dernier paramétre = cache le curseur)
	graphics = new ES_Graphics("ES engine - Platformer", 320, 240, 16, false, MODE, false);

	// initialise OpenGL si le mode est opengl
	if(MODE == ES_Graphics::ES_OPENGL)
	{
		graphics->opengl_init();
		graphics->set_orto_view();
	}

    ressources.add_resource("font", "data/Cicle.ttf", ES_Resources::ES_TTF_FONT);

    // charge notre boîte
	ressources.add_resource("bloc bois", "data/tiles/bloc_bois.png", ES_Resources::ES_IMG, false, NULL, true, 0, 255, 255);

    // initialise l'écran tactile de la GP2X
    mouse.init_touchscreen();

    // On active les hacks pour la GP2X
    // l'utilisation de la physique sur GP2X est un peu lente dû à l'utilisation
    // des nombres flottants (pas d'unités de calculs sur GP2X), en attendant une nouvelle version du
    // moteur & de la lib physique qui vient avec en fixed point utilisez les hacks pour améliorer la
    // vitesse :) (20 boîtes = ~50fps)
    mmu_hack();
    set_cpu_clock(266);
    ram_hack();

    timer = new ES_Timer(); // initialise le timer

	while (graphics->run())
	{
		graphics->clear_screen();

        // récupére le nombres de frames par secondes (fps)
		int fps = graphics->get_fps(1000);

        // met à jour le monde physique
        // premier paramétre = temp entre chaques iterations
        // deuxiéme paramétre = nombres d'iterations
        // modifiez ses valeurs pour obtenir de meilleurs performances ou une meilleur qualité
        // de la physique
        physics.update_world(1.0f/20.0f,10);

        // boucle pour chaques objets boîte dans le monde physique
        for(int i = 0; i < physics.count_object(ES_Physics::ES_BOX);i++)
        {
            // récupére l'angle de rotation de chaques boîtes (la valeur retourné par get_rotation
            // est en radian donc il faut la convertir en degrée via la fonction rad2deg du moteur)
            float rot = rad2deg(physics.get_rotation(i,ES_Physics::ES_BOX));
            // Affiche chaques boîtes avec la bonne orientation & la bonne position
            graphics->draw_image("bloc bois", graphics->rect(0, 0, 16, 16), graphics->rect((int)physics.get_origin_position(i,ES_Physics::ES_BOX).x, (int)physics.get_origin_position(i,ES_Physics::ES_BOX).y, 16, 16), 255, true,rot);
        }

		// partie évenements
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

                        case GP2X_BUTTON_L: // ajoute une bôite par appuie via le stylet
                            numbox++;
                        break;

                        case GP2X_BUTTON_R: // enléve une boîte
                            numbox--;
                            if(numbox <= 0)
                                numbox=0;
                        break;

                        case GP2X_BUTTON_B: // enléve des boîtes (pour pas surcharger le CPU de la GP2X)
                            for(int i = 0; i < physics.count_object(ES_Physics::ES_BOX);i++)
                                physics.delete_object(i,ES_Physics::ES_BOX);
                        break;
                    }
                break;

                case SDL_QUIT: // ferme la fenêtre ? on quitte
                    graphics->quit();
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // ajoute des boîtes par clics
                    if(event.get_sdlevent().button.button == SDL_BUTTON_WHEELDOWN )
                    {
                        numbox--;
                        if(numbox <= 0)
                            numbox=0;
                    }
                    // enléve des boîtes par clics
                    else if(event.get_sdlevent().button.button == SDL_BUTTON_WHEELUP )
                        numbox++;
                    else // ajoute une boîte si un clic à eu lieu
                    {
                        // pour chaques nombres de boîtes par clics défini
                        for(int i = 0; i < numbox;i++)
                        {
                            // ajoute une boîte dans le monde physique à la position de la souris
                            // le 5° paramétre défini la densité de la boîte ici de 10
                            // le 6° défini l'angle de rotation initiale de la boîte
                            // le 7&8° paramétre défini la vélocité linéaire de la boîte en x/y
                            // le 9° paramétre permet de mettre en pause l'objet lorsque qu'il est inactif (permet de pas calculer
                            // inutilement)
                            physics.add_box(mouse.get_pos_x()+y,mouse.get_pos_y(),16.0f/2,16.0f/2,10.0f,0.0,0.0f,0.0f,true,false);
                            y+=16.0f;
                        }
                        y=0;
                    }
                break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        case SDLK_ESCAPE: // appui sur echap ? on quitte
                            graphics->quit();
                        break;
                    }
                break;
            }
		}

        // pression sur l'écran tactile
        if(mouse.get_touchscreen_data() != 0 && mouse.get_touchscreen_data()->Pressed)
        {
            // 1 pression toutes les 1 secondes max (pour gérer le temps entre chaques pressions)
            if(timer->get_elapsed_time() >= 600)
            {
                // ajoute une boîte au monde physique (voir plus haut pour l'explication)
                for(int i = 0; i < numbox;i++)
                {
                    physics.add_box(mouse.get_touchscreen_data()->x+y,mouse.get_touchscreen_data()->y,16.0f/2,16.0f/2,10.0f,0.0,0.0f,0.0f,true,false);
                    y+=16.0f;
                }
                y=0;
                timer->reset_timer(); // reset le timer
            }
        }

        ttf->draw_text("font", "Nombres de boîtes: " + to_string(physics.count_object(ES_Physics::ES_BOX)) + " FPS:" + to_string(fps), 0, 2, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        ttf->draw_text("font", "Boîtes par clics (ou pressions): " + to_string(numbox), 0, 20, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        ttf->draw_text("font", "'B' pour effacer les boîtes (GP2X).", 0, 42, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		graphics->flip(1);
	}

    // libére le monde physique crée ainsi que tous les objets
    physics.free_world();
    mouse.close_touchscreen();
	ressources.clear_resources();
	delete ttf;
	delete timer;
	delete graphics;

	return EXIT_SUCCESS;
}
