// ES engine - Exemple d'utilisation des fichiers archives d'ES
#include <ES/es.h>
#include <ES/ES_Scripts/lua.h> // es scripts

using namespace ES; // évite d'écrire ES:: à chaques utilisations des méthodes du moteur

ES_Graphics *graphics;
ES_Pack *pack;
ES_Sound *sound;
ES_Scripts *scripts;

int main(int argc, char **argv)
{
	ES_Resources ressources;
	ES_Event event;
	bool count = true;
	int nombre = 0;

#ifdef GP2X
	graphics = new ES_Graphics("ES engine - Pack Sample", 320, 240, 16, false, false);
#else
	graphics = new ES_Graphics("ES engine - Pack Sample", 640, 480, 32, false, false);
#endif

	pack = new ES_Pack();
	// charge la liste des fichiers en mémoires pour la recherche rapide (à effectuer à chaques fois qu'on utilise le systéme d'archive avec les ressources)
	pack->init_list("data.esd");
	// si on veut ouvrir une autre archive avec le même objet il faut utiliser set_archive, charger les ressources de l'archives et réapeller set_archive pour une autre
	//pack->set_archive("data.esd");

	sound = new ES_Sound();

    graphics->opengl_init();
    graphics->set_orto_view();

    // charge les 2 images depuis l'archive
    ressources.add_resource("img", "test.png", ES_Resources::ES_IMG, ES_Resources::ES_PACK, pack);
    ressources.add_resource("img2", "test2.png", ES_Resources::ES_IMG, ES_Resources::ES_PACK, pack);
    // charge le mod depuis l'archive (sur GP2X il est impossible de charger une musique depuis les espack)
#ifndef GP2X
    ressources.add_resource("module", "f-lagoon.mod", ES_Resources::ES_MUSIC, ES_Resources::ES_PACK, pack);

    // joue le mod
    sound->play_music("module");
#endif

	// initialisation des scripts
	scripts = new ES_Scripts();
	// permet de laisser géré les erreurs des scripts/fonctions par le moteur
	scripts->set_default_error();
	// charge notre script depuis l'archive
	scripts->load_file("basic.lua", pack);
	// lit le script "basic.lua"
	scripts->read_script();
	// prépare à récupérer la valeur de la variable du script "nombre"
	scripts->get_var("nombre");
	// récupére la valeur dans la variable nombre de type int
	nombre = scripts->get_value_isint();

	// affiche le nombre lu dans la sortie
	std::cout << nombre << std::endl;

	while (graphics->run())
	{
	    graphics->clear_screen();

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

                        case GP2X_BUTTON_SELECT:
                            if(count == true)
                            {
                                ressources.del_resource("img", ES_Resources::ES_IMG, pack, "test.png");
                                ressources.add_resource("img", "test3.png", ES_Resources::ES_IMG, ES_Resources::ES_PACK, pack);
                                count = false;
                            }
                            else if(count == false)
                            {
                                ressources.del_resource("img", ES_Resources::ES_IMG, pack, "test3.png");
                                ressources.add_resource("img", "test.png", ES_Resources::ES_IMG, ES_Resources::ES_PACK, pack);
                                count = true;
                            }
                        break;
                    }
                break;

                case SDL_QUIT:
                    graphics->quit();
                    break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        case SDLK_ESCAPE:
                            graphics->quit(); // quitte si echap
                        break;

                        // décharge / charge une ressource dynamiquement depuis l'archive
                        case SDLK_F1:
                            if(count == true)
                            {
                                ressources.del_resource("img", ES_Resources::ES_IMG, pack, "test.png");
                                ressources.add_resource("img", "test3.png", ES_Resources::ES_IMG, ES_Resources::ES_PACK, pack);
                                count = false;
                            }
                            else if(count == false)
                            {
                                ressources.del_resource("img", ES_Resources::ES_IMG, pack, "test3.png");
                                ressources.add_resource("img", "test.png", ES_Resources::ES_IMG, ES_Resources::ES_PACK, pack);
                                count = true;
                            }
                        break;
                    }
                break;
            }
		}

        // affiche les 2 images
		graphics->draw_image("img", graphics->rect(0, 0, 32, 48), graphics->rect(0, 0, 32 ,48));
        graphics->draw_image("img2", graphics->rect(0, 0, 32 ,48), graphics->rect(32 ,48, 32 ,48));

		graphics->flip(2);
	}

	ressources.clear_resources(); // décharge toutes les ressources chargés
	delete sound;
	delete scripts;
	delete pack; // libére la mémoire alloué pour les ressources chargés depuis l'archive
	delete graphics;

	return EXIT_SUCCESS;
}

// Note : Toutes les ressources peuvent être chargés depuis une archive ES EXCEPTE les fichiers polices (qui pourront l'être plus tard)
//        Sur GP2X comme dit plus haut il est impossible de charger des musiques depuis la mémoire.
