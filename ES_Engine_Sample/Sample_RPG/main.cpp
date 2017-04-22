/* -
   - ES engine - Exemple
   - Affichage d'une carte .esm (provenant de l'éditeur d'ES), d'un personnage, aperçus
   - des collisions/scrolling avec la carte et effets pouvant être appliquer.
   -
*/
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Display/ttf.h>
#include <ES/ES_Event/event.h>
#include <ES/ES_Map/map.h> // gére les cartes .esm
#include <ES/ES_Utils/configuration.h>

using namespace ES;

ES_Graphics *graphics;
ES_Map *map;
ES_Configuration *conf;
ES_Ttf *font;

int x_anim, y_anim, pos_x = 30, pos_y = 30;
int layer2 = 1, layer3 = 1, layer4 = 1, col = 1, n_map = 0, change_char = 0; // toutes les couches activer au début et collisions
bool mode = false, fullscreen, night = false;
bool up = false, down = false, right = false, left = false;

int main(int argc, char **argv)
{
	ES_Resources ressources;
	ES_Event event;

    // lit le fichier de configuration
    conf = new ES_Configuration();
    conf->check_configuration_file("config.xml");

    if(conf->get_key_value("mode", "sdl") == "sdl")
        mode = false;
    else
        mode = true;

    int w = conf->get_key_value("window_w", 320);
    int h = conf->get_key_value("window_h", 240);
    int bpp = conf->get_key_value("bpp", 16);
    bool fullscreen = conf->get_key_value("fullscreen", 0);

	graphics = new ES_Graphics("ES engine - RPG Sample", w, h, bpp,
                                fullscreen, mode, true);

    // initialise ce qui faut pour les cartes (à faire après l'initialisation d'une fenêtre)
	map = new ES_Map();

    // charge la carte "map.esm" dans le répertoire data
	map->load_map("data/map.esm");

	if(mode == true) // opengl
		map->set_color(255, 255, 255, 255); // définie une couleur de départ pour la map
	else // sdl
		map->set_color(0, 0, 0, 0);

    font = new ES_Ttf();

    // charge le charset du voleur
	ressources.add_resource("voleur", "data/rogue.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);
	// charge l'autre personnage
	ressources.add_resource("perso2", "data/player.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);

	//charge la font pour afficher le texte
	font->set_size(18);
	ressources.add_resource("font", "data/Cicle.ttf", ES_Resources::ES_TTF_FONT);

    // opengl
    if(mode == true)
    {
        graphics->opengl_init();
        graphics->set_orto_view();
    }

    // définie un taux d'affichages de 60 images par secondes
	graphics->set_framerate();

	// répétitions des touches
	event.set_key_repeat(80, 1);

	// récupére le temps au début du programme (pour géré l'animation ensuite)
    long frame = graphics->get_time();
    // temp entre chaques images de l'animation (ms)
    unsigned long time_frame = 90;

    int last_pos_x, last_pos_y;

    // Sur GP2X l'utilisation des cartes ES sur 4 couches est très lente sans hacks/overclock
    // ceci est dû au blit de + de 250 sprites avec les 4 couches qui plombe largement la console
    // sachant que la SDL n'utilise pas complétment le hardware de la console,
    // avec les hacks complets + cpu overclocké à 266mhz ça reste très jouable mais il faut plutôt
    // seulement utiliser 2 ou 3 couches pour que ce soit vraiment jouable (40 fps+)
    //
    // Note: L'utilisation des effets de couleurs (pour la nuit par exemple) avec une carte ES de 4
    //       couches est très lent... car pas accéléré sur la console, pour éviter cela utilisez plutôt
    //       des cartes à 2 couches ou n'utilisez pas de transparences.
    mmu_hack();
    set_cpu_clock(266);
    ram_hack();

	while (graphics->run())
	{
		graphics->clear_screen();

	    // stocke la derniére position du personnage (avant de l'avoir bouger)
	    last_pos_x = pos_x;
	    last_pos_y = pos_y;

		// partie évenements
		while(event.poll_event())
		{
            switch(event.get_event_type())
            {
                /* Evenements pour la portabilité GP2X (pour utiliser le pad) */
                case SDL_JOYBUTTONDOWN:
                    switch(event.get_sdlevent().jbutton.button)
                    {
                        case GP2X_BUTTON_START:
                            graphics->quit();
                        break;

                        case GP2X_BUTTON_UP:
                            up = true;
                        break;

                        case GP2X_BUTTON_DOWN:
                            down = true;
                        break;

                        case GP2X_BUTTON_LEFT:
                            left = true;
                        break;

                        case GP2X_BUTTON_RIGHT:
                            right = true;
                        break;

                        case GP2X_BUTTON_B:

                        break;

                        case GP2X_BUTTON_L:
                            if(night == true)
                                night = false;
                            else
                                night = true;
                        break;

                        case GP2X_BUTTON_R:
                            if(layer2 != 1)
                                layer2 = 1;
                            else
                                layer2 = 0;
                        break;

                        case GP2X_BUTTON_A:
                            if(layer3 != 1)
                                layer3 = 1;
                            else
                                layer3 = 0;
                        break;

                        case GP2X_BUTTON_X:
                            if(layer4 != 1)
                                layer4 = 1;
                            else
                                layer4 = 0;
                        break;

                        case GP2X_BUTTON_Y:
                            if(col != 1)
                                col = 1;
                            else
                                col = 0;
                        break;

                        case GP2X_BUTTON_VOLDOWN:
                            if(n_map != 1)
                            {
                                n_map = 1;
                                map->load_map("data/map2.esm"); // charge la deuxiéme carte
                            }
                            else
                            {
                                n_map = 0;
                                map->load_map("data/map.esm"); // charge la première
                            }
                        break;

                        case GP2X_BUTTON_VOLUP:
                            if(change_char != 1)
                                change_char = 1;
                            else
                                change_char = 0;
                        break;
                    }
                break;

                case SDL_JOYBUTTONUP: // Bouton relaché
                    switch(event.get_sdlevent().jbutton.button)
                    {
                        case GP2X_BUTTON_UP:
                            up = false;
                        break;

                        case GP2X_BUTTON_DOWN:
                            down = false;
                        break;

                        case GP2X_BUTTON_LEFT:
                            left = false;
                        break;

                        case GP2X_BUTTON_RIGHT:
                            right = false;
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
                            graphics->quit();
                        break;

                        /* Faît avancer le personnage de 2 pixels puis, si il s'est écoulé 90 millisecondes
                           depuis la derniére animation change la position de l'image
                        */
                        case SDLK_LEFT:
                            pos_x-=2;
                            if(graphics->get_time() > frame + time_frame)
                            {
                                x_anim+=32; // image suivante
                                frame = SDL_GetTicks();
                            }
                            y_anim = 48; // change l'image en y pour qu'elle coresponde à quelqu'un qui marche à gauche
                        break;

                        case SDLK_RIGHT:
                            pos_x+=2;
                            if(graphics->get_time() > frame + time_frame)
                            {
                                x_anim+=32;
                                frame = graphics->get_time();
                            }
                            y_anim = 48*2; // positionne l'image correctement en y pour la droite
                        break;

                        case SDLK_UP:
                            pos_y-=2;
                            if(graphics->get_time() > frame + time_frame)
                            {
                                x_anim+=32;
                                frame = graphics->get_time();
                            }
                            y_anim = 48*3;
                        break;

                        case SDLK_DOWN:
                            pos_y+=2;
                            if(graphics->get_time() > frame + time_frame)
                            {
                                x_anim+=32;
                                frame = graphics->get_time();
                            }
                            y_anim = 0;
                        break;

                        case SDLK_F1:
                            if(night == true)
                                night = false;
                            else
                                night = true;
                        break;

                        case SDLK_F2:
                            if(layer2 != 1)
                                layer2 = 1;
                            else
                                layer2 = 0;
                        break;

                        case SDLK_F3:
                            if(layer3 != 1)
                                layer3 = 1;
                            else
                                layer3 = 0;
                        break;

                        case SDLK_F4:
                            if(layer4 != 1)
                                layer4 = 1;
                            else
                                layer4 = 0;
                        break;

                        case SDLK_F5:
                            if(col != 1)
                                col = 1;
                            else
                                col = 0;
                        break;

                        case SDLK_F6:
                            if(n_map != 1)
                            {
                                n_map = 1;
                                map->load_map("data/map2.esm"); // charge la deuxiéme carte
                            }
                            else
                            {
                                n_map = 0;
                                map->load_map("data/map.esm"); // charge la première
                            }
                        break;

                        // change le personnage
                        case SDLK_F7:
                            if(change_char != 1)
                                change_char = 1;
                            else
                                change_char = 0;
                        break;
                    }
                break;
            }
		}

        if(x_anim >= 32*4) // si on est sur la derniére frame du charset on revient au début
            x_anim = 0;

        // Gestion évenements pad GP2X (voir les évenements SDL normals pour comprendre)
		if(up == true)
		{
            pos_y-=2;
            if(graphics->get_time() > frame + time_frame)
            {
                x_anim+=32;
                frame = graphics->get_time();
            }
            y_anim = 48*3;
		}
		else if(down == true)
		{
            pos_y+=2;
            if(graphics->get_time() > frame + time_frame)
            {
                x_anim+=32;
                frame = graphics->get_time();
            }
            y_anim = 0;
		}
		else if(left == true)
		{
            pos_x-=2;
            if(graphics->get_time() > frame + time_frame)
            {
                x_anim+=32; // image suivante
                frame = SDL_GetTicks();
            }
            y_anim = 48;
		}
		else if(right == true)
		{
            pos_x+=2;
            if(graphics->get_time() > frame + time_frame)
            {
                x_anim+=32;
                frame = graphics->get_time();
            }
            y_anim = 48*2; // positionne l'image correctement en y pour la droite
		}

        int fps = graphics->get_fps(1000); // récupére les fps

        //
        //    gére les collisions avec les tiles de la carte
        //    map->tiles_collision retourne true si une collision est survenu
        //    les 4 derniers paramétres servent à définir précisément où le sprite doit toucher
        //   (par exemple si vous voulez qu'une partie soit au dessus du tile etc...)
        if(map->tiles_collision(pos_x, pos_y, 0, 8, 48/2+14, 24) == true && col == 1)
        {
            // on repositionne le sprite à la derniére position valide
            pos_x = last_pos_x;
            pos_y = last_pos_y;
            x_anim = 0; // revient à la première frame (qui correspond au persos à l'arrêt)
        }

        // collisions bors de l'écran
        if(pos_x >= graphics->get_window_width()+map->get_offset_x()-32 || pos_x <= -6 || pos_y >= graphics->get_window_height()+map->get_offset_y()-48 || pos_y <= -6)
        {
            pos_x = last_pos_x;
            pos_y = last_pos_y;
            x_anim = 0;
        }

        // on centre la carte (où plutôt le "scroll") sur la position du personnage
        map->center_scroll_map(pos_x, pos_y);

        // définie une couleur d'avant plan pour la scéne (pour crée des effets de nuits / brumes etc)
        // le dernier paramétre correspond au niveau de transparence, le rendu est différent avec opengl
        // car celui-ci gére différement l'application des couleurs/transparences (avec sdl c'est simplement
        // un filtre avec un niveau de transparence blitter sur l'écran)
        if(night == true && mode == true)
            map->set_color(100, 100, 200, 255);
        else if(night == false && mode == true)
            map->set_color(255, 255, 255, 255); // mode normal opengl
        else if(night == true && mode == false)
            map->set_color(15, 15, 50, 150);
        else if(night == false && mode == false)
            map->set_color(0, 0, 0, 0); // mode normal sdl

        // affiche le premier layer (fond) en 0, 0 et aux dimensions de la fenêtre
        map->draw_bg(1, 0, 0, graphics->get_window_width(), graphics->get_window_height());
        // affiche le deuxiéme layer
        if(layer2 == 1)
            map->draw_bg(2, 0, 0, graphics->get_window_width(), graphics->get_window_height());

        // affiche l'image du voleur où du perso 2 (en fonction des coordonées du "scroll" -> pos_x-map->get_offset_x())
        if(change_char == 0)
            graphics->draw_image("voleur", graphics->rect(x_anim, y_anim, 32, 48), graphics->rect(pos_x-map->get_offset_x(), pos_y-map->get_offset_y(), 32, 48), 255, true);
        else
            graphics->draw_image("perso2", graphics->rect(x_anim, y_anim, 32, 48), graphics->rect(pos_x-map->get_offset_x(), pos_y-map->get_offset_y(), 32, 48), 255, true);

        // affiche ensuite les 2 autres layers par dessus le sprite voleur (pour passé dérriére les arbres etc..)
        if(layer3 == 1)
            map->draw_mlayer(3, 0, 0, graphics->get_window_width(), graphics->get_window_height());
        if(layer4 == 1)
            map->draw_mlayer(4, 0, 0, graphics->get_window_width(), graphics->get_window_height());

        // affiche les textes
        font->draw_text("font", "F1(R): Jour/Nuit", 0, 4, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        font->draw_text("font", "F2(L)/F3(A)/F4(X): Couches 2/3/4", 0, 4+16, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        font->draw_text("font", "F5(Y): Activer/désactiver les collisions", 0, 4+16*2, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        font->draw_text("font", "F6(vol-): Charge une autre carte", 0, 4+16*3, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        font->draw_text("font", "F7(vol+): Change le personnage", 0, 4+16*4, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        font->draw_text("font", "FPS: "+to_string(fps), 0, 4+16*5, graphics->color(255, 255, 0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		graphics->flip(1);

        // permet de maintenir le taux d'affichage définie au début (dans notre cas 60 fps)
		graphics->framerate_delay();
	}

	ressources.clear_resources();
    delete map; // on oublie surtout pas de libérer la map
    delete conf;
    delete font;
	delete graphics;

	return EXIT_SUCCESS;
}
