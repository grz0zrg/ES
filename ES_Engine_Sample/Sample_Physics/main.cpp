// ES engine - Sample Physics 1�
//- Exemple montrant l'utilisation de la physique avec ES.
//  Affichage de bo�tes tombantes via les clics souris et le stylet pour la GP2X.
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
    ES_Timer *timer; // timer pour g�rer le nombres de bo�tes par pression

    // cr�e et d�fini le monde physique
    // les 4 premiers param�tres sont la zone en rectangle ou toutes les interactions physiques
    // seront fa�tes, icile maximum est de la taille de l'�cran pour la droite et le haut du
    // rectangle (320*240).
    // Le dernier param�tre indique la gravit� horizontale (ici on souhaite une gravit� terrestre normal
    // donc on � pas besoin de la gravit� horizontal), regardez la d�finition de setup_world dans la
    // documentation pour plus d'informations.
    physics.setup_world(-100,-1000.0f,320.0f,240.0f,0.0f);

    // Ajoute un plan (sol) en bas de l'�cran
    // les deux premiers param�tres sont la largeur du sol et les deux derniers �a position
    // ici on d�fini �a largeur � 320 (largeur de l'�cran), �a hauteur � 1, �a position x � 0 et
    // �a position y � la hauteur de l'�cran
    physics.add_ground(320.0f,1.0f,0.0f,240.0f-10.0f);

    int numbox=1; // va d�finir notre nombres de bo�tes par clics
    float y=0.0f;

	ES_Resources ressources;
	ES_Event event;
	ES_Ttf *ttf;

	ttf = new ES_Ttf();
	ttf->set_size(18);

	// ouvre un �cran en 320x240 fen�tr� sans curseur (dernier param�tre = cache le curseur)
	graphics = new ES_Graphics("ES engine - Platformer", 320, 240, 16, false, MODE, false);

	// initialise OpenGL si le mode est opengl
	if(MODE == ES_Graphics::ES_OPENGL)
	{
		graphics->opengl_init();
		graphics->set_orto_view();
	}

    ressources.add_resource("font", "data/Cicle.ttf", ES_Resources::ES_TTF_FONT);

    // charge notre bo�te
	ressources.add_resource("bloc bois", "data/tiles/bloc_bois.png", ES_Resources::ES_IMG, false, NULL, true, 0, 255, 255);

    // initialise l'�cran tactile de la GP2X
    mouse.init_touchscreen();

    // On active les hacks pour la GP2X
    // l'utilisation de la physique sur GP2X est un peu lente d� � l'utilisation
    // des nombres flottants (pas d'unit�s de calculs sur GP2X), en attendant une nouvelle version du
    // moteur & de la lib physique qui vient avec en fixed point utilisez les hacks pour am�liorer la
    // vitesse :) (20 bo�tes = ~50fps)
    mmu_hack();
    set_cpu_clock(266);
    ram_hack();

    timer = new ES_Timer(); // initialise le timer

	while (graphics->run())
	{
		graphics->clear_screen();

        // r�cup�re le nombres de frames par secondes (fps)
		int fps = graphics->get_fps(1000);

        // met � jour le monde physique
        // premier param�tre = temp entre chaques iterations
        // deuxi�me param�tre = nombres d'iterations
        // modifiez ses valeurs pour obtenir de meilleurs performances ou une meilleur qualit�
        // de la physique
        physics.update_world(1.0f/20.0f,10);

        // boucle pour chaques objets bo�te dans le monde physique
        for(int i = 0; i < physics.count_object(ES_Physics::ES_BOX);i++)
        {
            // r�cup�re l'angle de rotation de chaques bo�tes (la valeur retourn� par get_rotation
            // est en radian donc il faut la convertir en degr�e via la fonction rad2deg du moteur)
            float rot = rad2deg(physics.get_rotation(i,ES_Physics::ES_BOX));
            // Affiche chaques bo�tes avec la bonne orientation & la bonne position
            graphics->draw_image("bloc bois", graphics->rect(0, 0, 16, 16), graphics->rect((int)physics.get_origin_position(i,ES_Physics::ES_BOX).x, (int)physics.get_origin_position(i,ES_Physics::ES_BOX).y, 16, 16), 255, true,rot);
        }

		// partie �venements
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

                        case GP2X_BUTTON_L: // ajoute une b�ite par appuie via le stylet
                            numbox++;
                        break;

                        case GP2X_BUTTON_R: // enl�ve une bo�te
                            numbox--;
                            if(numbox <= 0)
                                numbox=0;
                        break;

                        case GP2X_BUTTON_B: // enl�ve des bo�tes (pour pas surcharger le CPU de la GP2X)
                            for(int i = 0; i < physics.count_object(ES_Physics::ES_BOX);i++)
                                physics.delete_object(i,ES_Physics::ES_BOX);
                        break;
                    }
                break;

                case SDL_QUIT: // ferme la fen�tre ? on quitte
                    graphics->quit();
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // ajoute des bo�tes par clics
                    if(event.get_sdlevent().button.button == SDL_BUTTON_WHEELDOWN )
                    {
                        numbox--;
                        if(numbox <= 0)
                            numbox=0;
                    }
                    // enl�ve des bo�tes par clics
                    else if(event.get_sdlevent().button.button == SDL_BUTTON_WHEELUP )
                        numbox++;
                    else // ajoute une bo�te si un clic � eu lieu
                    {
                        // pour chaques nombres de bo�tes par clics d�fini
                        for(int i = 0; i < numbox;i++)
                        {
                            // ajoute une bo�te dans le monde physique � la position de la souris
                            // le 5� param�tre d�fini la densit� de la bo�te ici de 10
                            // le 6� d�fini l'angle de rotation initiale de la bo�te
                            // le 7&8� param�tre d�fini la v�locit� lin�aire de la bo�te en x/y
                            // le 9� param�tre permet de mettre en pause l'objet lorsque qu'il est inactif (permet de pas calculer
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

        // pression sur l'�cran tactile
        if(mouse.get_touchscreen_data() != 0 && mouse.get_touchscreen_data()->Pressed)
        {
            // 1 pression toutes les 1 secondes max (pour g�rer le temps entre chaques pressions)
            if(timer->get_elapsed_time() >= 600)
            {
                // ajoute une bo�te au monde physique (voir plus haut pour l'explication)
                for(int i = 0; i < numbox;i++)
                {
                    physics.add_box(mouse.get_touchscreen_data()->x+y,mouse.get_touchscreen_data()->y,16.0f/2,16.0f/2,10.0f,0.0,0.0f,0.0f,true,false);
                    y+=16.0f;
                }
                y=0;
                timer->reset_timer(); // reset le timer
            }
        }

        ttf->draw_text("font", "Nombres de bo�tes: " + to_string(physics.count_object(ES_Physics::ES_BOX)) + " FPS:" + to_string(fps), 0, 2, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        ttf->draw_text("font", "Bo�tes par clics (ou pressions): " + to_string(numbox), 0, 20, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        ttf->draw_text("font", "'B' pour effacer les bo�tes (GP2X).", 0, 42, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		graphics->flip(1);
	}

    // lib�re le monde physique cr�e ainsi que tous les objets
    physics.free_world();
    mouse.close_touchscreen();
	ressources.clear_resources();
	delete ttf;
	delete timer;
	delete graphics;

	return EXIT_SUCCESS;
}
