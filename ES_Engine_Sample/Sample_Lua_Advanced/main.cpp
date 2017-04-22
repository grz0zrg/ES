// ES engine - Exemple d'utilisation des scripts Lua advanced
//- Deuxiéme exemple d'utilisation, appels de fonctions Lua, tableaux et autres
#include <ES/es.h>
#include <ES/ES_Scripts/lua.h> // es scripts

using namespace ES; // évite d'écrire ES:: à chaques utilisations des méthodes du moteur

ES_Graphics *graphics;
ES_Ttf *font;
ES_Scripts *scripts;

std::vector<int> tab;
int count = 0, pos_y = 12*3+5, addition = 0;

int main(int argc, char **argv)
{
	ES_Resources ressources;
	ES_Event event;

#ifdef GP2X
	graphics = new ES_Graphics("ES engine - Lua Sample 2", 320, 240, 16, false);
#else
	graphics = new ES_Graphics("ES engine - Lua Sample 2", 640, 480, 32, false);
#endif

    // initialisation des fonts
	font = new ES_Ttf();
	font->set_size(12);

    // charge notre font
    ressources.add_resource("font", "data/arial.ttf", ES_Resources::ES_TTF_FONT);

	// initialisation des scripts
	scripts = new ES_Scripts();
	// /!\ on ne laisse plus gérer les erreurs avec le moteur dans cette exemple (juste pour montrer qu'il est possible de gérer soit même les erreurs) !
	// charge notre script
	if(scripts->load_file("data/script.lua") == -1)
	{
		// erreur de chargement du script ? on quitte proprement et on indique le problème
		std::cerr << scripts->get_error() << std::endl;
		delete font;
		delete scripts;
		delete graphics;
		exit(EXIT_FAILURE);
	}

	// lit le script "basic.lua"
	if(scripts->read_script() != 0)
	{
		// erreur de lecture du script ? on quitte proprement et on indique le problème
		// scripts->get_error() renvoi la derniére erreur
		std::cerr << scripts->get_error() << std::endl;
		delete font;
		delete scripts;
		delete graphics;
		exit(EXIT_FAILURE);
	}

	// on se prépare à récupérer les valeurs du tableau "array" dans le script Lua
	scripts->get_var("array");
	// sert pour parcourir et récupérer les valeurs du tableau (ne pas le mettre = plantage)
	scripts->push_nil();
	// on parcours le tableau tant qu'il reste des élements
	while(scripts->next_value())
	{
		// on récupére ses valeurs dans notre tableau C++
		// la paramètre en plus du get_value_isint sert à dire où dans la pile Lua il doit lire (normalement pour un tableau c'est tout le temps -1)
		tab.push_back(scripts->get_value_isint(-1));
		count++;
	}

	// on remet l'état de la pile au départ (pour le push_nil())
	scripts->pop_state();

	// on définie une fonction Lua qui pourra être appellé
	scripts->add_lua_function("HelloWorld");

	// on vérifie que la fonction existe dans le script
	if(scripts->get_isfnc() != 0)
		std::cerr << "Fonction HelloWorld inexistante dans le script." << std::endl;
	else // si elle existe
		// on appel la fonction lua simple (aucunes valeurs de retours et aucun arguments)
		scripts->call_lua_function();

	// on définie une fonction Lua qui pourra être appellé
	scripts->add_lua_function("print_");

	// on vérifie que la fonction existe dans le script
	if(scripts->get_isfnc() != 0)
		std::cerr << "Fonction print_ inexistante dans le script." << std::endl;
	else // si elle existe
	{
		// on ajoute l'argument message à lui passé
		scripts->add_arg_str("Salut !");
		// on appel la fonction Lua print_ qui à 1 paramétre
		scripts->call_lua_function(1);
	}

	// on définie une autre fonction Lua
	scripts->add_lua_function("add");

	// on vérifie que la fonction existe dans le script
	if(scripts->get_isfnc() != 0)
		std::cerr << "Fonction add inexistante dans le script." << std::endl;
	else // si elle existe
	{
		// on ajoute les arguments à lui passer pour qu'elle retourne le résultat
		scripts->add_arg_number(20);
		scripts->add_arg_number(15);

		// enfin, on appel la fonction lua qui à 2 arguments et 1 valeur de retours
		scripts->call_lua_function(2, 1);
		// on récupére la valeur de retour (donc le résultat de l'addition des 2 nombres passé en paramétre)
		addition = scripts->get_value_isint();
	}


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
                    }
                break;
            }
		}

		// affiche le résultat de l'addition
		font->draw_text("font", "20 + 15 = " + to_string(addition), 0, 5, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		// affiche combien d'élément contient le tableau lua array
		font->draw_text("font", "Il y à " + to_string(count) + " éléments dans la variable array.", 0, 12+5, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		// affiche le contenu du tableau array
		for(int i = 0; i < count; i++)
		{
		    if(tab.empty() == false)
		    {
                font->draw_text("font", to_string(i+1) + "° élément du tableau = " + to_string(tab[i]), 0, pos_y, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
                pos_y +=12;
		    }
		}

		pos_y = 12*3+5; // repositionne le texte correctement

		graphics->flip(2);
	}

	// on libére la mémoire
	tab.clear(); // libére le tableau
	ressources.clear_resources(); // décharge toutes les ressources chargés
	// ferme la librairie de scripts
	delete scripts;
	delete font;
	delete graphics;

	return EXIT_SUCCESS;
}

// Note : Je vous conseille de regarder le script dans le dossier data. ;)
