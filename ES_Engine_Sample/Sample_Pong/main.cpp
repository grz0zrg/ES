/* -----------------------------------------------------
   - ES_Pong / Pong utilisant les fonctionnalités d'ES -
   -----------------------------------------------------*/

#include <ES/ES_Display/graphics.h>
#include <ES/ES_Display/ttf.h>
#include <ES/ES_Event/event.h>
#include <ES/ES_Sound/sound.h>
#include <ES/ES_Physics/physics.h> // gestion des collisions
#include <ES/ES_Utils/configuration.h> // sert à géré le fichier de configuration

using namespace ES; // évite de rajouter les préfixes aux fonctions du moteur

enum state{ MENU = 0, GAME = 1, QUIT = 2 }; // les différents états du jeu
unsigned short int game_state = MENU; // variable qui stocke l'état du jeu

// variables pour d'autres états annexes (intro/quitte/début du jeu)
bool intro = true, help_state = false, game_begin = true, game_solo_begin = true, game_solo = false, game_over = false, player_win = false, pbegin = true, ball_init = false;
// positionne les options du menu
int y_title = -80, x_play = -48, x_help = 848, choix = 176, i = 0;

struct Raquette{ // structure raquette
    int x, y, w, h, score; // coordonnées rectangle + score du joueur qui posséde la raquette (et vélocité cpu)
};

struct Balle{ // structure balle
    int x, y, vx, vy, w, h, dir;
};

Raquette player_solo, player_1, player_2; // 3 raquettes, 1 solo horizontal et les 2 autres verticales
Balle balle, balle_solo; // les balles

ES_Graphics *graphics;
ES_Sound *sounds;
ES_Ttf *fonts;
ES_Event event;
ES_MouseEvent mouse;
ES_Physics physics;

// déclaration des fonctions
void menu();
void game();
void help();

int main(int argc, char **argv)
{
    // Ses 2 lignes ne sont pas obligatoires, c'est simplement pour virés les warnings du compilo en
    // mode strict ISO C/C++
    argc = 0;
    argv = NULL;

    // initialise les variables de configurations
    std::string smode;
    unsigned short int ifullscreen, bpp;
    bool mode, fullscreen;

	// initialise et lit le fichier de configuration XML
	ES_Configuration *pconfig = new ES_Configuration();
	pconfig->check_configuration_file("config.xml");

	// récupére les valeurs depuis le fichier de configuration
	smode = pconfig->get_key_value("mode", "sdl");
	ifullscreen = pconfig->get_key_value("fullscreen", 0);
	bpp = pconfig->get_key_value("bpp", 32);

	if(smode == "sdl")
        mode = ES_Graphics::ES_SDL;
    else if(smode == "opengl")
        mode = ES_Graphics::ES_OPENGL;
    else
        mode = ES_Graphics::ES_SDL;

    if(ifullscreen == 0)
        fullscreen = false;
    else
        fullscreen = true;

	ES_Resources ressources; // crée le gestionnaire de ressources

    // set le mode vidéo
	graphics = new ES_Graphics("ES Pong - ES Engine", 800, 600, bpp, fullscreen, mode, true);

	// définie un taux d'affichages de 60 fps (par défaut)
	graphics->set_framerate();

    srand(time(NULL)); // mélange

    // initialise les coordonées de départs raquettes/balles (mini jeu à l'écran du menu)
    player_solo.w = 64;
    player_solo.h = 16;
    player_solo.x = 800/2-43/2; // positionne la raquette solo au milieu de l'écran
    player_solo.y = 600-11; // positionne la raquette solo en bas de l'écran
    player_solo.score = 0; // pas de scores pour lui de toutes façon

    balle_solo.x = 800/2;
    balle_solo.y = 600-128;
    balle_solo.w = 8;
    balle_solo.h = 8;
    balle_solo.vx = 1;
    balle_solo.vy = 1;
    balle_solo.dir = 0;

    // initialise les coordonées de départs raquettes/balles du jeu
    player_1.x = 10;
    player_1.y = 600/2-43/2;
    player_1.w = 16;
    player_1.h = 64;
    player_1.score = 0;

    player_2.x = 800-43/2;
    player_2.y = 600/2-43/2;
    player_2.w = 16;
    player_2.h = 64;
    player_2.score = 0;

    balle.x = 800/2-4;
    balle.y = 600/2-4;
    balle.vx = 1;
    balle.vy = 1;
    balle.dir = 0;
    balle.w = 8;
    balle.h = 8;

    // on initialise opengl
    if(mode == ES_Graphics::ES_OPENGL)
    {
        graphics->opengl_init();
        graphics->set_orto_view();
    }

	// initialisation du gestionnaire sonore
	sounds = new ES_Sound();
	// initialisation des polices
	fonts = new ES_Ttf();

    // on charge nos polices
    fonts->set_size(18);
    ressources.add_resource("police general", "data/fonts/arial.ttf", ES_Resources::ES_TTF_FONT);
    fonts->set_size(12);
    ressources.add_resource("police pong small", "data/fonts/space_age.ttf", ES_Resources::ES_TTF_FONT);
    fonts->set_size(48);
    ressources.add_resource("police pong", "data/fonts/space_age.ttf", ES_Resources::ES_TTF_FONT);
    fonts->set_size(72);
    ressources.add_resource("police pong énorme", "data/fonts/space_age.ttf", ES_Resources::ES_TTF_FONT);

	// on charge nos samples
	ressources.add_resource("rebond", "data/sfx/3.wav", ES_Resources::ES_SAMPLE);
	sounds->sample_volume("rebond", 50); // baisse un peu le volume de ce sample
	ressources.add_resource("debut partie", "data/sfx/5.wav", ES_Resources::ES_SAMPLE);
    ressources.add_resource("effet menu", "data/sfx/beep1.wav", ES_Resources::ES_SAMPLE);
    // on charge nos musiques
    ressources.add_resource("menu", "data/sfx/GODZILL2.MOD", ES_Resources::ES_MUSIC);
    ressources.add_resource("jeu", "data/sfx/f-lagoon.mod", ES_Resources::ES_MUSIC);
    ressources.add_resource("perdu", "data/sfx/gameover.mod", ES_Resources::ES_MUSIC);
    ressources.add_resource("gagner", "data/sfx/wind_blown_leavz.mod", ES_Resources::ES_MUSIC);
    // chargement des images
    ressources.add_resource("arene", "data/gfx/arene.png", ES_Resources::ES_IMG, false, NULL, true);
    ressources.add_resource("raquette", "data/gfx/raquette.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);
    ressources.add_resource("raquette1", "data/gfx/raquette1.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);
    ressources.add_resource("raquette2", "data/gfx/raquette2.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);
    ressources.add_resource("balle", "data/gfx/ball.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);

	while (graphics->run())
	{
		graphics->clear_screen();

		while(event.poll_event())
		{
            switch(event.get_event_type())
            {
                case SDL_QUIT:
                    // on quitte directe si c'est pendant les phases d'intros sinon fade in
                    if(game_state == MENU && intro == false)
                        game_state = QUIT;
                    else if(game_state == GAME && game_begin == false)
                        game_state = QUIT;
                    else
                        graphics->quit();
                    break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        // touche echap, on quitte avec fade si possible (si on est dans une partie on retourne au menu)
                        case SDLK_ESCAPE:
                            if(game_state == MENU && intro == false)
                                game_state = QUIT;
                            // on est en jeu ? on revient au menu
                            else if(game_state == GAME && game_begin == false)
                            {
                                // réinitialise les options/choix du menu
                                y_title = -80;
                                x_play = -48;
                                x_help = 848;
                                choix = 176;
                                game_solo_begin = true;
                                help_state = false;
                                game_begin = true;
                                pbegin = true;
                                game_over = false;
                                player_win = false;
                                player_1.y = 600/2-43/2;
                                player_2.y = 600/2-43/2;
                                player_1.score = 0;
                                player_2.score = 0;
                                i = 0;
                                // rejoue la musique du menu en fade in
                                sounds->music_fade_in("menu", 3000, -1);
                                game_state = MENU;
                            }
                            else
                                graphics->quit();
                        break;

                        // switch entre le mode plein écran et fenêtré
                        // seulement si on est en mode SDL (bug avec OpenGL)
                        case SDLK_F1:
                            if(graphics->get_use_opengl() == false)
                                graphics->switch_window_mode();
                        break;

                        case SDLK_F2:
                            if(sounds->music_volume(-1) == ES_MAX_VOLUME)
                                sounds->music_volume(0);
                            else
                                sounds->music_volume(ES_MAX_VOLUME);
                        break;

                        case SDLK_F3:
                            if(sounds->channels_volume(-1, -1) == ES_MAX_VOLUME)
                                sounds->channels_volume(-1, 0);
                            else
                                sounds->channels_volume(-1, ES_MAX_VOLUME);
                        break;

                        case SDLK_UP:
                            // si on est dans le menu et que les anims sont finis
                            if(game_state == MENU && intro == false && x_play == x_help)
                            {
                                if(choix != 176) // permet de bloquer le curseur à la premiére option
                                {
                                    sounds->play_sample("effet menu"); // joue le bip
                                    choix -= 48; // on déplace le curseur à l'option précédente
                                }
                            }
                        break;

                        case SDLK_DOWN:
                            if(game_state == MENU && intro == false && x_play == x_help)
                            {
                                if(choix != 272)
                                {
                                    sounds->play_sample("effet menu");
                                    choix += 48; // on déplace le curseur à l'option suivante
                                }
                            }
                        break;

                        case SDLK_RETURN:
                            if(game_state == MENU && intro == false && x_play == x_help)
                            {
                                switch(choix)
                                {
                                    case 176: // jouer
                                        // fade out de la musique du menu
                                        sounds->music_fade_out(4000);
                                        game_state = GAME;
                                    break;

                                    case 224: // affiche/désaffiche aide
                                        if(help_state == true)
                                            help_state = false;
                                        else
                                            help_state = true;
                                    break;

                                    case 272: // quitter
                                        game_state = QUIT;
                                    break;

                                    default:
                                    break;
                                }
                            }
                            // début de partie
                            else if(game_state == GAME && game_begin == false && pbegin == true)
                            {
                                pbegin = false;
                                sounds->play_sample("debut partie");
                                ball_init = true;
                            }
                            // perdu ? si entrée on rejoue
                            else if(game_state == GAME && game_begin == false && game_over == true)
                            {
                                sounds->music_fade_in("jeu", 3000, -1);
                                ball_init = true;
                                game_over = false;
                                player_1.y = 600/2-43/2;
                                player_2.y = 600/2-43/2;
                                player_1.score = 0;
                                player_2.score = 0;
                                i = 0;
                            }
                            // gagner ? on rejoue si entrée
                            else if(game_state == GAME && game_begin == false && player_win == true)
                            {
                                sounds->music_fade_in("jeu", 3000, -1);
                                ball_init = true;
                                player_win = false;
                                player_1.y = 600/2-43/2;
                                player_2.y = 600/2-43/2;
                                player_1.score = 0;
                                player_2.score = 0;
                                i = 0;
                            }
                        break;
                    }
                break;
            }
		}

        switch(game_state)
        {
            case MENU: // menu
                menu();
                if(help_state == true)
                    help();
            break;

            case GAME: // jeu
                game();
            break;

            case QUIT:
                // musique actuel en fade out
                sounds->music_fade_out(5000);
                if(graphics->screen_fade() == 1) // joue un fade in de l'arriére plan
                    graphics->quit(); // quitte quand l'écran est blanc
            break;

            default:
            break;
        }

		graphics->flip();

		graphics->framerate_delay();
	}

	// on libére la mémoire
	ressources.clear_resources(); // décharge toutes les ressources
	delete fonts;
	delete sounds;
	delete pconfig;
	delete graphics;

	return EXIT_SUCCESS;
}

// fonction qui affiche l'aide
void help()
{
    fonts->draw_text("police general", "- F1: Active/désactive le mode plein écran.",
                     60, 72, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

    fonts->draw_text("police general", "- F2: Active/désactive la musique.",
                     60, 90, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

    fonts->draw_text("police general", "- F3: Active/désactive les effets sonores.",
                     60, 108, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
}

// le jeu en lui même
void game()
{
    if(game_begin == true)
    {
        if(graphics->screen_fade() == 2) // on passe au jeu en douceur (fade du menu)
        {
            if(sounds->get_fade_out_state() == true) // si le fade out de la musique du menu est finis
                sounds->music_fade_in("jeu", 3000, -1); // on joue/fade in la musique du jeu en boucle
            game_begin = false;
        }
    }
    else
    {
        graphics->draw_image("raquette1", graphics->rect(0, 0, player_1.w, player_1.h), graphics->rect(player_1.x, player_1.y, player_1.w, player_1.h), 255, true);

        graphics->draw_image("raquette2", graphics->rect(0, 0, player_2.w, player_2.h), graphics->rect(player_2.x, player_2.y, player_2.w, player_2.h), 255, true);

        // si la partie n'est pas encore commencer
        if(pbegin == true)
        {
            // affichage du texte d'infos de début de parties
            fonts->draw_text("police general", "Entrée = Débuter la partie",
                            800/2, 600/2, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

            fonts->draw_text("police general", "Echap = Revenir au menu",
                            800/2, 600/2+18, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

            fonts->draw_text("police pong small", "Prêt ?",
                            800/2, 600/2-18, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
        }
        // initialise les coordoonées de départs de la balle et ça direction
        else if(ball_init == true)
        {
            // choisis une direction de départ au hasard entre 4 directions
            balle.dir = rand()%3+1;

            // met en place la balle au centre de l'aréne
            balle.x = 800/2-4;
            balle.y = 600/2-4;
            balle.vx = 1;

            // set la direction de départ suivant la direction qui à étaît généré
            switch(balle.dir)
            {
                case 0: // haut/droit
                    balle.vx = balle.vx;
                    balle.vy = balle.vy;
                break;

                case 1: // bas/droit
                    balle.vx = balle.vx;
                    balle.vy = -balle.vy;
                break;

                case 2: // bas/gauche
                    balle.vx = -balle.vx;
                    balle.vy = balle.vy;
                break;

                case 3: // haut/gauche
                    balle.vx = -balle.vx;
                    balle.vy = -balle.vy;
                break;
            }

            ball_init = false;
        }
        else // démarre la partie
        {
            // affiche la balle
            graphics->draw_image("balle", graphics->rect(0, 0, balle.w, balle.h), graphics->rect(balle.x, balle.y, balle.w, balle.h), 255, true);

            // affichage des scores, to_string est une fonction du moteur qui permet de transformer n'importe
            // quel type en une chaîne de caractére
            fonts->draw_text("police general", "Score: " + to_string(player_1.score) + "/" + to_string(player_2.score),
                            800/2, 5, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

            if(game_over == false && player_win == false)
            {
                balle.x += balle.vx;
                balle.y += balle.vy;

                if(balle.y <= 0 || balle.y >= 600-balle.h/2)
                {
                    balle.vy = -balle.vy;
                    sounds->play_sample("rebond"); // joue le sons "rebond"
                }
                else if(physics.rect_collision(balle.x, balle.y, balle.w, balle.h, player_1.x, player_1.y, player_1.w, player_1.h) == true)
                {
                    sounds->play_sample("rebond");
                    balle.vx = -balle.vx;
                    balle.dir = 0;
                    // rebond sur la raquette ? augmente la vitesse de la balle aléatoirement
                    if(balle.vx == -balle.vx)
                        balle.vx-=rand()%4;
                    else if(balle.vx > 5)
                        balle.vx+=rand()%4;
                }
                // collisions sur la raquete de droite
                else if(physics.rect_collision(balle.x, balle.y, balle.w, balle.h, player_2.x, player_2.y, player_2.w, player_2.h) == true)
                {
                    sounds->play_sample("rebond");
                    balle.vx = -balle.vx;
                    balle.dir = 2;
                    // rebond ? vitesse de la balle modifié aléatoirement
                    if(balle.vx == -balle.vx)
                        balle.vx+=rand()%12; // cpu donc un peu plus chaud :P
                    else if(balle.vx < 5)
                        balle.vx-=rand()%12;
                }
                else if(balle.x < 0)
                {
                    player_2.score++;
                    ball_init = true;
                }
                else if(balle.x > 800)
                {
                    player_1.score++;
                    ball_init = true;
                }

                // petite ia de la raquette adverse :) (en gros si la balle vient vers la raquette
                // et qu'elle est d'un coté ou l'autre de la raquette la raquette bouge en conséquence)
                if(balle.dir == 0 || balle.dir == 1)
                {
                    if(balle.y > player_2.y + rand()%18) // le rand est faît pour obtenir un rebond et une position toujours différentes
                        player_2.y++;
                    else if(balle.y < player_2.y + rand()%18)
                        player_2.y--;
                }

                // bloque la raquette 2 si elle dépasse les coords de l'écran
                if(player_2.y > 600 - 43)
                    player_2.y = 600-43;
                else if(player_2.y < 0)
                    player_2.y = 0;

                if(player_2.score == 21) // joue le game over si le score du cpu est égale à 21
                {
                    sounds->music_fade_out(2000);
                    game_over = true;
                }
                else if(player_1.score == 21) // le joueur gagne
                {
                    sounds->music_fade_out(2000);
                    player_win = true;
                }

                // collisions de la raquette sur les bords haut et bas de l'écran
                if(mouse.get_pos_y() < 0)
                    player_1.y = 0;
                else if(mouse.get_pos_y() > 600-43)
                    player_1.y = 600-43;
                else
                    player_1.y = mouse.get_pos_y();
            }
            // perdu
            else if(game_over == true)
            {
                // attend que le fade out de l'autre musique soit finis et joue la musique du game over
                if(sounds->get_fade_out_state() == true && i == 0)
                {
                    sounds->music_fade_in("perdu", 2000, -1);
                    i++;
                }

                fonts->draw_text("police pong", "Vous avez perdu!",
                                800/2, 600/2, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

                fonts->draw_text("police general", "Entrée = Rejouer",
                                800/2, 600/2+48, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
                fonts->draw_text("police general", "Echap = Revenir au menu",
                                800/2, 600/2+48+18, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
            }
            // gagner !
            else if(player_win == true)
            {
                // joue la musique de la partie gagnez
                if(sounds->get_fade_out_state() == true && i == 0)
                {
                    sounds->music_fade_in("gagner", 2000, -1);
                    i++;
                }

                fonts->draw_text("police pong", "Vous avez gagnez!",
                                800/2, 600/2, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

                fonts->draw_text("police general", "Entrée = Rejouer",
                                800/2, 600/2+48, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
                fonts->draw_text("police general", "Echap = Revenir au menu",
                                800/2, 600/2+48+18, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
            }
        }
    }
}

// tout ce qui concerne l'écran du menu avec le pong solo et l'intro du début
void menu()
{
    if(intro == true) // on éxecute l'intro si on vient juste de démarer l'app
    {
        if(graphics->screen_fade() == 2) // execute le fade du fond et attend qu'il soit finis (2 = fade complet terminé)
        {
            intro = false; // si le fade est terminer on n'as plus besoin de réexecuter l'intro
            sounds->music_fade_in("menu", 3000, -1); // fade in de la musique du menu (commence à la joué progressivement)
        }
        else // si le fade n'est pas toujours terminer on affiche les textes d'intro au centre
        {
            fonts->draw_text("police pong énorme", "- ES Pong v1.0 -",
                             800/2, 600/2-72, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
            fonts->draw_text("police pong", "...ES Engine sample...",
                             800/2, 600/2, graphics->color(0, 0, 0), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);
        }
    }
    else // si l'intro est terminer on passe au menu
    {
        if(y_title != 10) // fais arriver le titre du haut, l'arréte à 10 pixels du haut
            y_title++;

        if(x_play != x_help) // fais bouger les 3 options du menus jusqu'a temps qu'il soit à la même position par rapport au menu qui arrive de la droite
        {
            x_play++; // le menu jouer et quitter avance de gauche à droite
            x_help--; // le menu aide arrive de la droite
        }
        else // si l'anim des menus est finis on affiche le "curseur" de selection à coté du menu jouer
        {
            fonts->draw_text("police pong", ">>",
                             x_play - 60, choix, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);
        }

        // affiche tout le reste
        fonts->draw_text("police pong énorme", ". ..ES Pong.. .",
                         800/2, y_title, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

        fonts->draw_text("police pong", "Jouer",
                         x_play, 176, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

        fonts->draw_text("police pong", "Aide",
                         x_help, 224, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

        fonts->draw_text("police pong", "Quitter",
                         x_play, 272, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

        fonts->draw_text("police pong small", "ES Pong - Demonstration",
                         800/2, 600-280, graphics->color(0, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);

        fonts->draw_text("police pong small", "ES Engine - 2007",
                         800/2, 600-268, graphics->color(255, 255, 255), ES_Ttf::ES_TTF_CENTER, ES_Ttf::ES_BLENDED);


        //------------------------- Partie jeu solo (intégré au menu) -------------------------
        // affiche les images de la raquette solo et de "l'aréne" solo
        graphics->draw_image("arene", graphics->rect(0, 0, 512, 256), graphics->rect(800/2-512/2, 600-256, 512, 256), 255, true);

        graphics->draw_image("raquette", graphics->rect(0, 0, player_solo.w, player_solo.h), graphics->rect(player_solo.x-43/2, player_solo.y, player_solo.w, player_solo.h), 255, true);

        // bloque la raquette si elle se barre sur les bords définis (bors de l'aréne gauche/droite)
        if(mouse.get_pos_x() < 800/2-512/2+26) // 26 = taille de la barre sans le "vide"
            player_solo.x = 800/2-512/2+26;
        else if(mouse.get_pos_x() > 800/2+512/2-27)
            player_solo.x = 800/2+512/2-27;
        else // si la raquette est entre les deux on peut l'afficher aux coordonées de la souris
            player_solo.x = mouse.get_pos_x();

        // affiche la balle au milieu de l'aréne
        graphics->draw_image("balle", graphics->rect(0, 0, balle_solo.w, balle_solo.h), graphics->rect(balle_solo.x, balle_solo.y, balle_solo.w, balle_solo.h), 255, true);

        // initialise l'angle de départ de la balle si le jeu solo commence
        if(game_solo_begin == true)
        {
            // choisis une direction de départ au hasard entre 4 directions
            balle_solo.dir = rand()%3+1;

            // met en place la balle au centre de l'aréne
            balle_solo.x = 800/2;
            balle_solo.y = 600-128;
            balle_solo.vy = 1;

            // set la direction de départ suivant la direction qui à étaît généré
            switch(balle_solo.dir)
            {
                case 0: // haut/droit
                    balle_solo.vx = balle_solo.vx;
                    balle_solo.vy = balle_solo.vy;
                break;

                case 1: // bas/droit
                    balle_solo.vx = balle_solo.vx;
                    balle_solo.vy = -balle_solo.vy;
                break;

                case 2: // bas/gauche
                    balle_solo.vx = -balle_solo.vx;
                    balle_solo.vy = balle_solo.vy;
                break;

                case 3: // haut/gauche
                    balle_solo.vx = -balle_solo.vx;
                    balle_solo.vy = -balle_solo.vy;
                break;
            }

            // le jeu solo peut vraiment commencer
            game_solo_begin = false;
            game_solo = true;
        }
        else if(game_solo == true)// lance la balle
        {
            balle_solo.x += balle_solo.vx;
            balle_solo.y += balle_solo.vy;

            // collision bord droit de l'aréne
            if(balle_solo.x >= 800/2+512/2-12) // 12 = taille de la balle + taille du bord
            {
                balle_solo.vx = -balle_solo.vx;
                sounds->play_sample("rebond"); // joue le sons "rebond"
            }
            else if(balle_solo.x <= 800/2-512/2+6) // collision bord gauche de l'aréne
            {
                balle_solo.vx = -balle_solo.vx;
                sounds->play_sample("rebond");
            }
            else if(balle_solo.y <= 600-256+6) // collision bord du haut de l'aréne
            {
                balle_solo.vy = -balle_solo.vy;
                sounds->play_sample("rebond");
            } // collision raquette du joueur
            else if(physics.rect_collision(balle_solo.x, balle_solo.y, balle_solo.w, balle_solo.h, player_solo.x, player_solo.y, player_solo.w, player_solo.h) == true)
            {
                sounds->play_sample("rebond");
                balle_solo.vy = -balle_solo.vy;
                // rebond sur la raquette ? augmente aléatoirement la vitesse de la balle
                if(balle_solo.vy == -balle_solo.vy && balle_solo.vy > rand()%6)
                    balle_solo.vy++;
                else if(balle.vy < rand()%6)
                    balle_solo.vy--;
            }
            else if(balle_solo.y >= 600) // la balle est perdu ? (sort de l'écran)
            {
                // on la replace au centre avec direction aléatoire
                game_solo_begin = true;
                game_solo = false;
            }
        }
    }
}

/* Ce petit Pong montre qu'il est assez simple de crée un jeu avec le moteur et
   que le jeu peut géré indifférament le rendu avec SDL où OpenGL sans pratiquement rien modifier
   (juste 2 fonctions à rajouter), les parties comme les effets sonores/effets de fonds sont elles
   aussi faciliter par le moteur ainsi que les collisions. Tout le reste c'est plus de la logique et du "design" qu'autres choses.

   Le jeu à une IA basique mais qui fonctionne assez bien. Libre à vous de l'améliorez. (faudrais ajouter une vélocité de la raquette adverse par exemple et la faire augmenter pour que ce soit plus réaliste)
*/
