/* -
   - ES engine - Exemple
   - Affichage d'une carte .esm (provenant de l'éditeur d'ES) et scripting d'évenements.
*/
#include <ES/es.h>
#include <ES/ES_Resources/resources.h>
#include <ES/ES_Map/map.h> // gére les cartes .esm
#include <ES/ES_Utils/configuration.h>
#include <ES/ES_Physics/physics.h>
#include <ES/ES_Scripts/lua.h> // es scripts

using namespace ES; // évite de rajouter le préfixe ES

ES_Graphics *graphics;
ES_Map *map;
ES_Ttf *font;
ES_Scripts *scripts;
ES_Physics phys;

int x_anim, y_anim, pos_x = 30, pos_y = 30, pnj_x_tmp, pnj_y_tmp, pos_txt_y = 16+18, maperr;
int r, g ,b, a, fps;
long time_night, time_npc_txt;
bool mode, fullscreen, night = false, night_begin = false, bdraw_text = false, other_map = false;

// structure qui défini nos PNJ
struct npc
{
	std::string name;
	std::string text;
	int x, y;
}n;

// structure qui défini les endroits de téléportations
struct teleport
{
	std::string map;
	int x_src, y_src;
	int x_dest, y_dest;
}t;

std::vector<npc> pnj; // tableau de structures pour les pnj
std::vector<teleport> e_teleport; // tableau de structures pour les teleports
std::string text_tmp;

// ajoute un pnj à une position avec un message et un nom
int add_npc(lua_State *ls)
{
	// si il y à bien 4 arguments (nom du npc, message, position x, position y)
	if(scripts->get_arg() == 4)
	{
		// récupére les valeurs en paramétres
		n.name = scripts->get_value_isstr(1, false); // le false est ici pour dire qu'on ne veut pas que la pile reprenne ça position initial (pour les autres valeurs)
		n.text = scripts->get_value_isstr(2, false);
		n.x = scripts->get_value_isint(3, false);
		n.y = scripts->get_value_isint(4, false);

		// ajoute le pnj avec les autres
		pnj.push_back(n);
	}
	return 0;
}

// ajoute un événement de téléportation qui prend en paramétre ça position, la position d'arrivé du perso
// et la map d'arrivé
int add_teleport(lua_State *ls)
{
	// si il y à bien 5 arguments (nom de la carte, position x du teleport, position y du teleport,
	// position x d'arrivé, position y d'arrivé)
	if(scripts->get_arg() == 5)
	{
		t.map = scripts->get_value_isstr(1, false);
		t.x_src = scripts->get_value_isint(2, false);
		t.y_src = scripts->get_value_isint(3, false);
		t.x_dest = scripts->get_value_isint(4, false);
		t.y_dest = scripts->get_value_isint(5, false);

		// ajoute le point de teleportation
		e_teleport.push_back(t);
	}
	return 0;
}

// fonction d'affichage du texte des PNJ
void draw_text()
{
	if(bdraw_text == true)
	{
		// affiche le texte du PNJ
		font->draw_text("font", text_tmp, pnj_x_tmp+32/2-map->get_offset_x(), pnj_y_tmp-pos_txt_y-map->get_offset_y(), graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
		pos_txt_y++; // fait remonter le texte
		// si il s'est écoulé 2 secondes depuis qu'on à parlé au pnj
		if(graphics->get_time() - time_npc_txt > 2000)
		{
			// on désaffiche le texte, remet le texte à ça position normale
			bdraw_text = false;
			time_npc_txt = graphics->get_time();
			pos_txt_y = 16+18;
		}
	}
}

int main(int argc, char **argv)
{
    bool up = false, down = false, right = false, left = false;

	ES_Resources ressources;
	ES_Event event;

    // lit le fichier de configuration
    ES_Configuration *conf = new ES_Configuration();
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

    // initialise ce qu'il faut pour les cartes (à faire après l'initialisation d'une fenêtre)
	map = new ES_Map();

    // charge la carte "map.esm" dans le répertoire data
	map->load_map("data/map.esm");

	// vu que les couleurs différes suivant le mode (opengl où sdl) on défini 2 couleurs pour les 2 modes
	if(mode == true) // opengl
	{
		map->set_color(255, 255, 255, 255); // définie une couleur de départ pour la map
		r = 255; g = 255; b = 255; a = 255;
	}
	else // sdl
	{
		map->set_color(0, 0, 0, 0);
		r = 0; g = 0; b = 0; a = 0;
	}

	// initialisation des scripts
	scripts = new ES_Scripts();
	// permet de laisser géré les erreurs des scripts/fonctions par le moteur
	scripts->set_default_error();
	// charge notre script
	scripts->load_file("data/script.lua");
	// ajoute les fonctions (ajouter un npc et téléportation)
	scripts->add_function("add_npc", add_npc);
	scripts->add_function("add_teleport", add_teleport);
	// lit le script général
	scripts->read_script();

    font = new ES_Ttf();

    // charge le charset du voleur
	ressources.add_resource("voleur", "data/rogue.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);
	// charge le charset du pnj
	ressources.add_resource("npc", "data/npc.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);

	//charge la font pour afficher le texte
	font->set_size(18);
	ressources.add_resource("font", "data/arial.ttf", ES_Resources::ES_TTF_FONT);

    // opengl
    if(mode == true)
    {
        graphics->opengl_init();
        graphics->set_orto_view();
    }

    // définie un taux d'affichages de 60 images par secondes
	graphics->set_framerate();

	// pour le timer de l'affichage des textes pnj
	time_npc_txt = graphics->get_time();
	// pour le timer temps (nuit/jour)
	time_night = graphics->get_time();

	// répétitions des touches
	event.set_key_repeat(80, 1);

	// récupére le temps au début du programme (pour géré l'animation ensuite)
    long frame = graphics->get_time();
    // temp entre chaques images de l'animation (ms)
    unsigned long time_frame = 90;

    int last_pos_x, last_pos_y;

    /* Optimisation GP2X - Active le hack mmu, active le hack ram et overclock le cpu à 266mhz */
    set_cpu_clock(266);
    mmu_hack();
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
							// seulement si un texte n'est pas affiché
							if(bdraw_text == false)
							{
								// on vérifie que le perso est près d'un personnage
								for(size_t i = 0; i < pnj.size(); i++)
								{
									// si il est prêt alors on affiche le message du pnj
									if(phys.rect_collision(pos_x, pos_y, 32, 48, pnj[i].x, pnj[i].y, 32, 48))
									{
										// on met le compteur au temp actuel
										time_npc_txt = graphics->get_time();
										bdraw_text = true;
										// on sauve la position du pnj à qui on à voulu parler
										pnj_x_tmp = pnj[i].x;
										pnj_y_tmp = pnj[i].y;
										// ainsi que son texte
										text_tmp = pnj[i].text;
										break; // on continue pas (plus besoin)
									}
								}
							}
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

                        case SDLK_SPACE:
							// seulement si un texte n'est pas affiché
							if(bdraw_text == false)
							{
								// on vérifie que le perso est près d'un personnage
								for(size_t i = 0; i < pnj.size(); i++)
								{
									// si il est prêt alors on affiche le message du pnj
									if(phys.rect_collision(pos_x, pos_y, 32, 48, pnj[i].x, pnj[i].y, 32, 48))
									{
										// on met le compteur au temp actuel
										time_npc_txt = graphics->get_time();
										bdraw_text = true;
										// on sauve la position du pnj à qui on à voulu parler
										pnj_x_tmp = pnj[i].x;
										pnj_y_tmp = pnj[i].y;
										// ainsi que son texte
										text_tmp = pnj[i].text;
										break; // on continue pas (plus besoin)
									}
								}
							}
                        break;
                    }
                break;
            }
		}

        /* Gestion évenements pad GP2X (voir les évenements SDL normals pour comprendre) */
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

		fps = graphics->get_fps(1000); // prend les fps

        if(x_anim >= 32*4) // si on est sur la derniére frame du charset on revient au début
            x_anim = 0;

		// collisions tiles + bord de l'écran
        if(map->tiles_collision(pos_x, pos_y, 0, 8, 48/2+14, 24) == true || pos_x >= graphics->get_window_width()+map->get_offset_x()-32 || pos_x <= -6 || pos_y >= graphics->get_window_height()+map->get_offset_y()-48 || pos_y <= -6)
        {
            // on repositionne le sprite à la derniére position valide
            pos_x = last_pos_x;
            pos_y = last_pos_y;
            x_anim = 0; // revient à la première frame (qui correspond au persos à l'arrêt)
        }

		// fondus jour -> nuit
        if(night == true && night_begin == true)
        {
            map->set_color(r, g, b, a);
            if(mode == true) // opengl
            {
				r--; g--; b--;
				if(r < 100 && g < 100) // si on à atteind une valeur correct d'ambiance on arréte
				{
					r = 100; g = 100;
					night_begin = false;
				}
				if(b < 200)
					b = 200;
            }
            else // sdl
            {
            	r++; g++; b++; a+=2;
				if(r > 15 && g > 15)
				{
					r = 15; g = 15;
				}
				if(b > 50)
					b = 50;
				if(a >= 150)
				{
					a = 150;
					night_begin = false;
				}
            }
        }
        // fondus nuit -> jour
        else if(night == false && night_begin == false)
        {
            if(mode == true) // opengl
            {
				map->set_color(r, g, b, a);
				r++; g++; b++;
				if(r >= 255 && g >= 255)
				{
					r = 255; g = 255;
					night_begin = true;
				}
				if(b >= 255)
					b = 255;
            }
            else // sdl
            {
				map->set_color(r, g, b, a);
				r--; g--; b--; a-=2;
				if(r <= 0 && g <= 0)
				{
					r = 0; g = 0;
				}
				if(b <= 0)
					b = 0;
				if(a <= 0)
				{
					a = 0;
					night_begin = true;
				}
            }
        }

		// si il s'est écoulé 10 secondes
		if(graphics->get_time() - time_night > 10000)
		{
			// on switch le mode
			if(night == true)
			{
				night = false;
				night_begin = false;
			}
			else
			{
				night = true;
				night_begin = true;
			}
			// on remet le compteur au temp actuel
			time_night = graphics->get_time();
		}

        map->draw_bg(0, 0, 0, graphics->get_window_width(), graphics->get_window_height());

		if(other_map == false)
		{
			// vérifie qu'on est pas sur un endroit de téléportation
			for(size_t i = 0; i < e_teleport.size(); i++)
			{
				// si y'a collision avec un point de teleportation
				if(phys.rect_collision(pos_x, pos_y, 32, 48, e_teleport[i].x_src, e_teleport[i].y_src, 32*3, 0)) // 32*3 = taille d'une zone de changement
				{
					maperr = map->load_map(e_teleport[i].map);

					// vérifie que la carte est correctement chargé
					if(maperr == 1 || maperr == 2 || maperr == 3)
						std::cout << "Carte où tileset non trouvé, aucunes possibilités de téléportations.";
					else
					{
						// si la carte est bien chargé, on place le personnage à la position d'arrivé
						pos_x = e_teleport[i].x_dest-map->get_offset_x();
						pos_y = e_teleport[i].y_dest-map->get_offset_y();
						other_map = true; // pour pas que les évenements de la première soit utilisable dans la deuxiéme
					}
					break;
				}
			}
		}

		// affiche le bas des pnj (avant le perso car il passe au dessus) sur la première map
		if(other_map == false)
		{
			for(size_t i = 0; i < pnj.size(); i++)
			{
				graphics->draw_image("npc", graphics->rect(0, 48/2, 32, 48/2), graphics->rect(pnj[i].x-map->get_offset_x(), pnj[i].y+48/2-map->get_offset_y(), 32, 48/2), 255, true);

				// collisions avec les pnj
				if(phys.rect_collision(pos_x, pos_y, 32, 48, pnj[i].x+12, pnj[i].y+24, 10, -5))
				{
					pos_x = last_pos_x;
					pos_y = last_pos_y;
					x_anim = 0;
				}
			}
		}

		graphics->draw_image("voleur", graphics->rect(x_anim, y_anim, 32, 48), graphics->rect(pos_x-map->get_offset_x(), pos_y-map->get_offset_y(), 32, 48), 255, true);

		if(other_map == false)
		{
			// affiche le haut des pnj (affiché après le perso pour que notre perso puisse passé dérriére)
			for(size_t i = 0; i < pnj.size(); i++)
				graphics->draw_image("npc", graphics->rect(0, 0, 32, 48/2), graphics->rect(pnj[i].x-map->get_offset_x(), pnj[i].y-map->get_offset_y(), 32, 48/2), 255, true);
		}

		// affiche le reste de la map
		map->draw_mlayer(0, 0, 0, graphics->get_window_width(), graphics->get_window_height());

		if(other_map == false)
		{
			// affiche le nom de tous les pnj
			for(size_t i = 0; i < pnj.size(); i++)
				font->draw_text("font", pnj[i].name, pnj[i].x+32/2-map->get_offset_x(), pnj[i].y-18-map->get_offset_y(), graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

			// affiche les textes
			#ifndef GP2X
			font->draw_text("font", "Espace pour discuter avec un PNJ. "+to_string(fps), 0, 4, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
            #else // pour GP2X c'est pas la même touche :)
            font->draw_text("font", "'B' = Discuter avec un PNJ. FPS:"+to_string(fps), 0, 4, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
            #endif

			draw_text();
		}

        // on centre la carte (où plutôt le "scroll") sur la position du personnage
        map->center_scroll_map(pos_x, pos_y);

		graphics->flip();

        // permet de maintenir le taux d'affichage définie au début (dans notre cas 60 fps)
		graphics->framerate_delay();
	}

	ressources.clear_resources();
	delete scripts;
    delete map; // on oublie surtout pas de libérer la map
    delete conf;
    delete font;
	delete graphics;

	return EXIT_SUCCESS;
}

// Note: Voilà un exemple d'utilisation de scripts Lua dans les jeux. Bien sûr ceci n'est qu'un exemple
//       et les fonctions sont assez basiques mais on pourrait l'améliorer pour en faire un vrai RPG avec
//       des cartes ayants ses propres scripts etc... car là les scripts ne sont qu'effectif que sur la première carte
//       chargé.
