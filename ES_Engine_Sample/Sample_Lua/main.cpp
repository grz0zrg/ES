// ES engine - Exemple d'utilisation des scripts Lua
//- Premier exemple d'utilisation des scripts Lua (variables et fonctions)
#include <ES/es.h>
#include <ES/ES_Scripts/lua.h> // es scripts

using namespace ES; // �vite d'�crire ES:: � chaques utilisations des m�thodes du moteur

ES_Graphics *graphics;
ES_Ttf *font;
ES_Scripts *scripts;

int nombre, count = 0, count2 = 0, pos_y = 5;
float flottant;
std::string message;
std::vector<std::string> text; // un tableau de type vector (stl) pour stocker les diff�rents textes des fonctions du script

// la fonction C qui peut �tre appel� depuis le script
int affiche(lua_State *ls)
{
	// si la fonction � un argument c'est qu'il souhaite afficher un message
	if(scripts->get_arg() == 1)
	{
		text.push_back(scripts->get_value_isstr()); // on r�cup�re le message dans notre cas en le pla�ant dans notre tableau
		count++; // sert � conna�tre le nombres de fois que la fonction et que la condition � �ta�t appell� dans le script

		return 0;
	}
	// sinon, si il appel simplement la fonction sans arguments, on affiche le texte par d�faut
	else
	{
		count2++; // sert � conna�tre le nombres de fois que la fonction et que cette condition � �ta�t appell� dans le script
		return 0;
	}
}

int main(int argc, char **argv)
{
	ES_Resources ressources;
	ES_Event event;

	graphics = new ES_Graphics("ES engine - Lua Sample", 320, 240, 16, false);

    // initialisation des fonts
	font = new ES_Ttf();
	font->set_size(12);

    // charge notre font
    ressources.add_resource("font", "data/arial.ttf", ES_Resources::ES_TTF_FONT);

	// initialisation des scripts
	scripts = new ES_Scripts();
	// permet de laisser g�r� les erreurs des scripts/fonctions par le moteur
	scripts->set_default_error();
	// charge notre script
	scripts->load_file("data/basic.lua");
	// ajoute une fonction C qui seras reconnu dans le script (premier param�tre = nom de la fonction dans le script)
	scripts->add_function("affiche", affiche);
	// lit le script "basic.lua"
	scripts->read_script();

	// pr�pare � r�cup�rer la valeur de la variable du script "nombre"
	scripts->get_var("nombre");
	// r�cup�re la valeur dans la variable nombre de type int
	nombre = scripts->get_value_isint();

	// se pr�pare � r�cup�rer la valeur de la variable "flottant" du script
	scripts->get_var("flottant");
	// r�cup�re la valeur dans la variable flottant de type float
	flottant = scripts->get_value_isflt();

	// se pr�pare � r�cup�rer la valeur de la variable "flottant" du script
	scripts->get_var("message");
	// r�cup�re la valeur dans la variable message de type string
	message = scripts->get_value_isstr();


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

		// pour toutes les fois o� la fonction affiche("truc") � �ta�t appell� avec un argument
		for(int i = 0; i < count; i++)
		{
			// on affiche le texte perso !
			font->draw_text("font", text[i], 0, pos_y, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
			pos_y += 12; // on augmente la position de 24 pixels (taille de notre font) pour que les messages s'affiches � la suite
		}

		pos_y += 12; // on laisse un espace entre les deux

		// pour toutes les fois o� la fonction affiche() � �ta�t appell� sans arguments
		for(int j = 0; j < count2; j++)
		{
			// on affiche le message par d�faut
			font->draw_text("font", "Message par d�faut !", 0, pos_y, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
			pos_y += 12;
		}

		pos_y = 5; // on remet la position des textes en haut

		// affiche la valeur des variables lus
		font->draw_text("font", "Variables du script -> nombre = " + to_string(nombre) + " flottant = " + to_string(flottant), 0, 240-12*3, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		// affiche combien de fonctions avec param�tres ont �ta�t appell�s
		font->draw_text("font", "Il y � " + to_string(count) + " appels de fonctions avec params dans le script", 0, 240-12*2, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
		// affiche combien de fonctions sans param�tres ont �ta�t appell�s
		font->draw_text("font", "Il y � " + to_string(count2) + " appels de fonctions sans params dans le script", 0, 240-12, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		graphics->flip(2);
	}

	// on lib�re la m�moire
	text.clear(); // le tableau
	ressources.clear_resources(); // d�charge toutes les ressources charg�s
	// ferme la librairie de scripts
	delete scripts;
	delete font;
	delete graphics;

	return EXIT_SUCCESS;
}

// Note : Je vous conseille de regarder le script basic.lua dans le dossier data. ;)
