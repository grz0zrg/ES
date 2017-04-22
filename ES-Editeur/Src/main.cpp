/*
    Editeur conçus pour ES - Crée par Grz- aka Julien Verneuil
    et publié gratuitement sous license BSD version 2 (voir ci-dessous) :

    - Utilise des classes provenants d'ES : http://www.garzul.tonsite.biz/wiki/
    - Utilise la structure de l'éditeur -> http://www.games-creators.org/wiki/Réaliser_un_RPG_en_C_avec_SDL_et_FMOD/L'éditeur_de_carte
*/

/*
* Copyright (c) 2006, 2007 Julien Verneuil
* Julien Verneuil a.k.a Grz-
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of ES or ES Engine nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "editor.h"

// Initialisation Guichan stuff
//- "menu" + init guichan stuff
gcn::SDLInput* g_input;
gcn::SDLGraphics* g_graphics;
gcn::SDLImageLoader* g_imageloader;
gcn::Gui* gui;
gcn::Container* c_top;
gcn::Container* c_menu;
gcn::Container* c_nmap;
//gcn::ImageFont* font;
gcn::SDLTrueTypeFont *font;
gcn::Button* b_load_map;
gcn::Button* b_new_map;
gcn::Button* save_map;
gcn::Button* map_option;
gcn::Button* editor_help;
gcn::Button* hide_menu;
gcn::Button* map_layer1;
gcn::Button* map_layer2;
gcn::Button* map_layer3;
gcn::Button* map_layer4;
//- labels infos
gcn::Container* c_infos;
gcn::Label* l_fps;
gcn::Label* l_layer;
//- nouvelle carte
gcn::Label* l_map_size;
gcn::Label* l_tile_size;
gcn::Label* l_tileset;
gcn::Label* l_colorkey;
gcn::TextField* map_sizew;
gcn::TextField* map_sizeh;
gcn::TextField* tile_sizew;
gcn::TextField* tile_sizeh;
gcn::TextField* tileset;
gcn::TextField* cr_new_map;
gcn::TextField* cv_new_map;
gcn::TextField* cb_new_map;
gcn::Button* create_map;
//- message box
gcn::Container* c_msgbox;
gcn::Label* l_text;
gcn::Label* l_text2;
gcn::Button* close;
//- sauver map
gcn::Container* c_smap;
gcn::Label* l_smap;
gcn::TextField* t_smap;
gcn::Button* b_smap;
//- charger map
gcn::Container* c_lmap;
gcn::Label* l_lmap;
gcn::TextField* t_lmap;
gcn::Button* b_lmap;

SDL_Event event;
SDL_Surface *screen = NULL, *surf_temp;
SDL_Rect rect;
ES_Configuration *config;

// Variables concernants les cartes/tilesets
std::string tileset_file, map_file;
int map_w, map_h, tile_w, tile_h, tile_r, tile_v, tile_b, layer = 0, lmap_err;
unsigned int layer2_alpha_value = 128, layer3_alpha_value = 128, layer4_alpha_value = 128, alpha_bg_layer = 255;

bool loop = true, d_new_map = false;

struct es_map map;

// compteur fps
unsigned int check_time = 0, fps = 0;

void message_box(const std::string& text, const std::string& text2);

// actions des widgets
class GUIActionListener : public gcn::ActionListener
{
    void action(const gcn::ActionEvent& actionEvent)
    {
        if(actionEvent.getId() == "hide menu")
            c_menu->setVisible(false);
        if(actionEvent.getId() == "new map")
        {
            if(c_nmap->isVisible())
            {
                map_sizew->setText("60");
                map_sizeh->setText("60");
                c_nmap->setVisible(false);
            }
            else if(c_smap->isVisible() || c_lmap->isVisible())
            {
                c_smap->setVisible(false);
                c_lmap->setVisible(false);
                c_nmap->setVisible(true);
            }
            else
                c_nmap->setVisible(true);
        }
        // crée une nouvelle carte
        if(actionEvent.getId() == "create map")
        {
            tileset_file = tileset->getText();
            // vérifie si y'a que des chiffres qui sont entrées pour les coords maps/tiles/colorkey
            if(tile_sizew->getText().find_first_not_of("0123456789") != std::string::npos ||
               tile_sizeh->getText().find_first_not_of("0123456789") != std::string::npos ||
               map_sizew->getText().find_first_not_of("0123456789") != std::string::npos ||
               cr_new_map->getText().find_first_not_of("0123456789") != std::string::npos ||
               cv_new_map->getText().find_first_not_of("0123456789") != std::string::npos ||
               cb_new_map->getText().find_first_not_of("0123456789") != std::string::npos ||
               map_sizeh->getText().find_first_not_of("0123456789") != std::string::npos)
            {
                message_box("Les largeurs/hauteurs doivent êtres numériques.", "C'est aussi valable pour la colorkey.");
            }
            else
            {
                convert_from_string(tile_sizew->getText(), tile_w);
                convert_from_string(tile_sizeh->getText(), tile_h);
                convert_from_string(map_sizew->getText(), map_w);
                convert_from_string(map_sizeh->getText(), map_h);
                convert_from_string(cr_new_map->getText(), tile_r);
                convert_from_string(cv_new_map->getText(), tile_v);
                convert_from_string(cb_new_map->getText(), tile_b);

                // on vérifie les valeurs des tiles/colorkey/taille map voir si elles sont valides
                if(tile_r > 255 || tile_v > 255 || tile_b > 255)
                    message_box("Les valeurs de la colorkey ne sont pas correctes.", "Valeurs comprises entre 0 et 255.");
                else
                {
                    // vérifie l'extension (voir si c'est un format connu)
                    if(tileset_file.substr(tileset_file.length() - 4) == ".bmp" ||
                       tileset_file.substr(tileset_file.length() - 4) == ".jpg" ||
                       tileset_file.substr(tileset_file.length() - 4) == ".png" ||
                       tileset_file.substr(tileset_file.length() - 4) == ".gif")
                    {
                        // vérifie si l'image peut être ouverte
                        surf_temp = IMG_Load(tileset_file.c_str());
                        if(surf_temp != NULL)
                        {
                            SDL_FreeSurface(surf_temp);
                            if(&map!=NULL) // libére la map ouverte si y'en à déjà une
                                free_map(&map);
                            // crée la nouvelle carte avec les paramétres
                            new_map(&map, map_w, map_h, tile_w, tile_h, tileset_file, tile_r, tile_v, tile_b);

                            // active les boutons inactifs
                            layer = 1;
                            if(d_new_map == false)
                            {
                                save_map->setEnabled(true);
                                map_layer1->setEnabled(true);
                                map_layer2->setEnabled(true);
                                map_layer3->setEnabled(true);
                                map_layer4->setEnabled(true);
                                save_map->setBaseColor(b_load_map->getBaseColor());
                                map_layer1->setBaseColor(b_load_map->getBaseColor());
                                map_layer2->setBaseColor(b_load_map->getBaseColor());
                                map_layer3->setBaseColor(b_load_map->getBaseColor());
                                map_layer4->setBaseColor(b_load_map->getBaseColor());

                                // on affiche la nouvelle carte avec les données
                                d_new_map = true;
                            }
                            c_nmap->setVisible(false);
                        }
                        else
                        {
                            message_box("Impossible de charger l'image spécifié.", "Vérifier que le chemin est correct.");
                        }
                    }
                    else
                    {
                        message_box("Format d'images incorrecte.", "(.bmp, .jpg, .png, .gif seulement)");
                    }
                }
            }
        }
        if(actionEvent.getId() == "close msgbox")
            c_msgbox->setVisible(false);
        if(actionEvent.getId() == "layer 1")
            layer = 1;
        if(actionEvent.getId() == "layer 2")
            layer = 2;
        if(actionEvent.getId() == "layer 3")
            layer = 3;
        if(actionEvent.getId() == "layer 4")
            layer = 4;
        // sauvegarde carte
        if(actionEvent.getId() == "save map_b")
        {
            if(c_smap->isVisible())
                c_smap->setVisible(false);
            else if(c_nmap->isVisible() || c_lmap->isVisible())
            {
                c_lmap->setVisible(false);
                c_nmap->setVisible(false);
                c_smap->setVisible(true);
            }
            else
                c_smap->setVisible(true);
        }
        if(actionEvent.getId() == "save map")
        {
            map_file = t_smap->getText();
            if(t_smap->getText().find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-/.\\_[]") != std::string::npos)
            {
                message_box("Nom spécifié incorrecte.", "Le nom ne peut pas contenir des accents.");
            }
            else
            {
                map_file = map_file + ".esm";
                tileset_file = map.tileset_file;
                save_esmap(&map, map_file, tileset_file);
                c_smap->setVisible(false);
            }
        }
        // chargement carte
        if(actionEvent.getId() == "load map_b")
        {
            if(c_lmap->isVisible())
                c_lmap->setVisible(false);
            else if(c_nmap->isVisible() || c_smap->isVisible())
            {
                c_smap->setVisible(false);
                c_nmap->setVisible(false);
                c_lmap->setVisible(true);
            }
            else
                c_lmap->setVisible(true);
        }
        if(actionEvent.getId() == "load map")
        {
            map_file = t_lmap->getText();
            if(t_lmap->getText().find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-/.\\_[]") != std::string::npos)
            {
                message_box("Nom spécifié incorrecte.", "Le nom ne peut pas contenir des accents.");
            }
            else
            {
                if(&map!=NULL) // libére la map ouverte si y'en à déjà une
                    free_map(&map);

                map_file = map_file + ".esm";
                lmap_err = load_map(&map, map_file);
                if(lmap_err == 1)
                    message_box("Impossible d'ouvrir la map spécifié.", "Vérifié qu'elle existe.");
                else if(lmap_err == 3)
                    message_box("Impossible d'ouvrir la map spécifié.", "Le tileset ne peut pas être chargé.");
                else if(lmap_err == 2)
                    message_box("Carte corrompu.", "Un problème à dû survenir lors de la sauvegarde.");
                else
                {
                    map_w = map.width;
                    map_h = map.height;
                    tile_w = map.tile_w;
                    tile_h = map.tile_h;

                    if(d_new_map == false)
                    {
                        save_map->setEnabled(true);
                        map_layer1->setEnabled(true);
                        map_layer2->setEnabled(true);
                        map_layer3->setEnabled(true);
                        map_layer4->setEnabled(true);
                        save_map->setBaseColor(b_load_map->getBaseColor());
                        map_layer1->setBaseColor(b_load_map->getBaseColor());
                        map_layer2->setBaseColor(b_load_map->getBaseColor());
                        map_layer3->setBaseColor(b_load_map->getBaseColor());
                        map_layer4->setBaseColor(b_load_map->getBaseColor());

                        // on affiche la nouvelle carte avec les données
                        d_new_map = true;
                    }

                    layer = 1;
                    c_lmap->setVisible(false);
                }
            }
        }
    }
};

GUIActionListener guiActionListener;

int main(int argc, char *argv[])
{
    argc = 0; argv = NULL; // vire les warnings du compilo

    config = new ES_Configuration();
    config->check_configuration_file("config.xml");

    if(SDL_Init(SDL_INIT_VIDEO) == 1)
    {
        std::cerr << "- Erreur d'initialisation SDL." << SDL_GetError() << std::endl;
        delete config;
        return EXIT_FAILURE;
    }

    // crée le contexte graphique
    if(config->get_key_value("fullscreen", 0) == 0 && config->get_key_value("hwsurface", 0) == 0)
        screen = SDL_SetVideoMode(config->get_key_value("window_w", 640), config->get_key_value("window_h", 480), 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    else if(config->get_key_value("fullscreen", 0) == 0 && config->get_key_value("hwsurface", 0) == 1)
        screen = SDL_SetVideoMode(config->get_key_value("window_w", 640), config->get_key_value("window_h", 480), 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    else if(config->get_key_value("fullscreen", 0) == 1 && config->get_key_value("hwsurface", 0) == 0)
        screen = SDL_SetVideoMode(config->get_key_value("window_w", 640), config->get_key_value("window_h", 480), 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    else if(config->get_key_value("fullscreen", 0) == 1 && config->get_key_value("hwsurface", 0) == 1)
        screen = SDL_SetVideoMode(config->get_key_value("window_w", 640), config->get_key_value("window_h", 480), 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

    if(screen == NULL)
    {
        std::cerr << "- Erreur d'ouverture de fenêtre SDL." << SDL_GetError() << std::endl;
        delete config;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_WM_SetCaption("ES - Editeur", NULL);

    /*
        Initialisation GUI Guichan
    */
    try
    {
        g_imageloader = new gcn::SDLImageLoader();
        gcn::Image::setImageLoader(g_imageloader);
        g_graphics = new gcn::SDLGraphics();
        g_graphics->setTarget(screen);
        g_input = new gcn::SDLInput();

        // création du conteneur globale & initialisation GUI
        c_top = new gcn::Container();
        c_top->setDimension(gcn::Rectangle(0, 0, screen->w, screen->h));
        c_top->setBaseColor(gcn::Color(0, 0, 0, 0));
        gui = new gcn::Gui();
        gui->setGraphics(g_graphics);
        gui->setInput(g_input);
        gui->setTop(c_top);

        //font = new gcn::ImageFont("data/font.png", " abcdefghijklmnopqrstuvwxyz"
        //                                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567"
        //                                           "89:@!\"$%&/=?^+*#[]{}()<>_;'.,\\|-~`"
        //                                           "øåáÁéÉíÍóÓúÚçë¥£¢¡¿àãõêñÑöüäÖÜÄßèÈÅ");
        TTF_Init();
        font = new gcn::SDLTrueTypeFont("data/arial.ttf", 12);
        gcn::Widget::setGlobalFont(font);

        // création du "menu"
        c_menu = new gcn::Container();
        c_menu->setDimension(gcn::Rectangle(0, 0, 226+3*113+5, 36));
        c_menu->setBaseColor(gcn::Color(0, 0, 0));
        b_load_map = new gcn::Button("Charger une carte");
        b_load_map->setSize(112, 16);
        b_load_map->setFocusable(false);
        b_load_map->setActionEventId("load map_b");
        b_load_map->addActionListener(&guiActionListener);
        b_new_map = new gcn::Button("Crée une carte");
        b_new_map->setSize(112, 16);
        b_new_map->setFocusable(false);
        b_new_map->setActionEventId("new map");
        b_new_map->addActionListener(&guiActionListener);
        save_map = new gcn::Button("Sauver la carte");
        save_map->setSize(112, 16);
        save_map->setFocusable(false);
        save_map->setEnabled(false); // on active seulement si une carte est crée où chargé
        save_map->setBaseColor(gcn::Color(100, 100, 100));
        save_map->setActionEventId("save map_b");
        save_map->addActionListener(&guiActionListener);
        map_option = new gcn::Button("Options");
        map_option->setSize(112, 16);
        map_option->setFocusable(false);
        editor_help = new gcn::Button("Aide éditeur");
        editor_help->setSize(112, 16);
        editor_help->setFocusable(false);
        hide_menu = new gcn::Button("Cacher le menu");
        hide_menu->setSize(112, 16);
        hide_menu->setFocusable(false);
        hide_menu->setActionEventId("hide menu");
        hide_menu->addActionListener(&guiActionListener);
        // Boutons des layers
        map_layer1 = new gcn::Button("Couche 1"); // bouton layer 1
        map_layer1->setSize(112, 16);
        map_layer1->setFocusable(false);
        map_layer1->setEnabled(false); // on active seulement si une carte est crée où chargé
        map_layer1->setBaseColor(gcn::Color(100, 100, 100));
        map_layer1->setActionEventId("layer 1");
        map_layer1->addActionListener(&guiActionListener);
        map_layer2 = new gcn::Button("Couche 2"); // bouton layer 2
        map_layer2->setSize(112, 16);
        map_layer2->setFocusable(false);
        map_layer2->setEnabled(false);
        map_layer2->setBaseColor(gcn::Color(100, 100, 100));
        map_layer2->setActionEventId("layer 2");
        map_layer2->addActionListener(&guiActionListener);
        map_layer3 = new gcn::Button("Couche 3"); // bouton layer 3
        map_layer3->setSize(112, 16);
        map_layer3->setFocusable(false);
        map_layer3->setEnabled(false);
        map_layer3->setBaseColor(gcn::Color(100, 100, 100));
        map_layer3->setActionEventId("layer 3");
        map_layer3->addActionListener(&guiActionListener);
        map_layer4 = new gcn::Button("Couche 4"); // bouton layer 4
        map_layer4->setSize(112, 16);
        map_layer4->setFocusable(false);
        map_layer4->setEnabled(false);
        map_layer4->setBaseColor(gcn::Color(100, 100, 100));
        map_layer4->setActionEventId("layer 4");
        map_layer4->addActionListener(&guiActionListener);
        // ajoute les widgets au conteneur ("menu")
        c_menu->add(b_load_map, 1, 0);
        c_menu->add(b_new_map, 1, 18);
        c_menu->add(save_map, 113, 0);
        c_menu->add(map_option, 113, 18);
        c_menu->add(editor_help, 226, 0);
        c_menu->add(hide_menu, 226, 18);
        c_menu->add(map_layer1, 226+113+4, 0);
        c_menu->add(map_layer2, 226+113+4, 18);
        c_menu->add(map_layer3, 226+2*113+4, 0);
        c_menu->add(map_layer4, 226+2*113+4, 18);

        // création du rectangle informations
        c_infos = new gcn::Container();
        c_infos->setDimension(gcn::Rectangle(screen->w-100, 0, 100, 60));
        c_infos->setBaseColor(gcn::Color(0, 0, 0, 0)); // transparent
        l_fps = new gcn::Label("FPS: "); // label FPS
        l_layer = new gcn::Label("Couche: "); // label layer
        c_infos->add(l_fps, 100-l_fps->getWidth()-14, 1);
        c_infos->add(l_layer, 100-l_layer->getWidth()-14, l_fps->getHeight()+2);

        // Création du conteneur nouvelle carte
        c_nmap = new gcn::Container();
        c_nmap->setDimension(gcn::Rectangle(0, 36, 260, 180));
        c_nmap->setBaseColor(gcn::Color(100, 100, 100));
        c_nmap->setVisible(false);
        map_sizew = new gcn::TextField("60"); // taille map
        map_sizew->setSize(55, font->getHeight()+2);
        map_sizew->setFrameSize(0);
        map_sizeh = new gcn::TextField("60");
        map_sizeh->setSize(55, font->getHeight()+2);
        map_sizeh->setFrameSize(0);
        l_map_size = new gcn::Label("Largeur / Hauteur de la carte (en tiles): ");
        l_map_size->setHeight(font->getHeight());
        tile_sizew = new gcn::TextField("32"); // taille tiles
        tile_sizew->setSize(55, font->getHeight()+2);
        tile_sizew->setFrameSize(0);
        tile_sizeh = new gcn::TextField("32");
        tile_sizeh->setSize(55, font->getHeight()+2);
        tile_sizeh->setFrameSize(0);
        l_tile_size = new gcn::Label("Largeur / Hauteur des tiles du tileset: ");
        l_tile_size->setHeight(font->getHeight());
        tileset = new gcn::TextField("data/tileset.png"); // chemin tileset
        tileset->setSize(55*2+5, font->getHeight()+2);
        tileset->setFrameSize(0);
        l_tileset = new gcn::Label("Chemin du tileset: ");
        l_tileset->setHeight(font->getHeight());
        create_map = new gcn::Button("Crée une nouvelle carte"); // bouton crée carte
        create_map->setSize(200, 26);
        create_map->setFocusable(false);
        create_map->setActionEventId("create map");
        create_map->addActionListener(&guiActionListener);
        l_colorkey = new gcn::Label("Couleur de transparence du tileset (RVB): "); // colorkey
        l_colorkey->setHeight(font->getHeight());
        cr_new_map = new gcn::TextField("255");
        cr_new_map->setSize(55, font->getHeight()+2);
        cr_new_map->setFrameSize(0);
        cv_new_map = new gcn::TextField("0");
        cv_new_map->setSize(55, font->getHeight()+2);
        cv_new_map->setFrameSize(0);
        cb_new_map = new gcn::TextField("255");
        cb_new_map->setSize(55, font->getHeight()+2);
        cb_new_map->setFrameSize(0);
        // ajout des widgets nouvelle carte au conteneur nouvelle carte
        c_nmap->add(map_sizew, 5, 18);
        c_nmap->add(map_sizeh, 65, 18);
        c_nmap->add(l_map_size, 5, 2);
        c_nmap->add(tile_sizew, 5, 18+16*2);
        c_nmap->add(tile_sizeh, 65, 18+16*2);
        c_nmap->add(l_tile_size, 5, 18+16);
        c_nmap->add(tileset, 5, 18+16*4);
        c_nmap->add(l_tileset, 5, 18+16*3);
        c_nmap->add(l_colorkey, 5, 18+16*5);
        c_nmap->add(cr_new_map, 5, 18+16*6);
        c_nmap->add(cv_new_map, 65, 18+16*6);
        c_nmap->add(cb_new_map, 65*2, 18+16*6);
        c_nmap->add(create_map, 260/2-create_map->getWidth()/2, 18+16*8);

        // message box
        c_msgbox = new gcn::Container();
        c_msgbox->setDimension(gcn::Rectangle(screen->w/2-300/2, screen->h/2-60/2, 300, 60));
        c_msgbox->setBaseColor(gcn::Color(100, 100, 100));
        c_msgbox->setVisible(false);
        l_text = new gcn::Label("");
        l_text->setHeight(font->getHeight());
        l_text2 = new gcn::Label("");
        l_text2->setHeight(font->getHeight());
        close = new gcn::Button("Fermer"); // bouton fermer
        close->setSize(112, 16);
        close->setFocusable(false);
        close->setBaseColor(gcn::Color(100, 100, 100));
        close->setActionEventId("close msgbox");
        close->addActionListener(&guiActionListener);
        c_msgbox->add(l_text);
        c_msgbox->add(l_text2);
        c_msgbox->add(close, c_msgbox->getWidth()/2-112/2, c_msgbox->getHeight()-20);

        // sauver carte
        c_smap = new gcn::Container();
        c_smap->setDimension(gcn::Rectangle(0, 36, 240, 80));
        c_smap->setBaseColor(gcn::Color(100, 100, 100));
        c_smap->setVisible(false);
        l_smap = new gcn::Label("Répertoire/nom de la carte: ");
        l_smap->setHeight(font->getHeight());
        t_smap = new gcn::TextField("data/map"); // textfield filename
        t_smap->setSize(c_smap->getWidth() - 10, font->getHeight()+2);
        t_smap->setFrameSize(0);
        b_smap = new gcn::Button("Sauvegarder la carte"); // bouton sauver
        b_smap->setSize(130, 26);
        b_smap->setFocusable(false);
        b_smap->setActionEventId("save map");
        b_smap->addActionListener(&guiActionListener);
        c_smap->add(l_smap, 5, 0);
        c_smap->add(b_smap, c_smap->getWidth()/2 - b_smap->getWidth()/2, c_smap->getHeight()-30);
        c_smap->add(t_smap, 5, 18);

        // charger carte
        c_lmap = new gcn::Container();
        c_lmap->setDimension(gcn::Rectangle(0, 36, 240, 80));
        c_lmap->setBaseColor(gcn::Color(100, 100, 100));
        c_lmap->setVisible(false);
        l_lmap = new gcn::Label("Répertoire/nom de la carte: ");
        l_lmap->setHeight(font->getHeight());
        t_lmap = new gcn::TextField("data/map"); // textfield filename
        t_lmap->setSize(c_smap->getWidth() - 10, font->getHeight()+2);
        t_lmap->setFrameSize(0);
        b_lmap = new gcn::Button("Charger la carte"); // bouton charger
        b_lmap->setSize(130, 26);
        b_lmap->setFocusable(false);
        b_lmap->setActionEventId("load map");
        b_lmap->addActionListener(&guiActionListener);
        c_lmap->add(l_lmap, 5, 0);
        c_lmap->add(b_lmap, c_smap->getWidth()/2 - b_smap->getWidth()/2, c_smap->getHeight()-30);
        c_lmap->add(t_lmap, 5, 18);

        // ajouts des autres conteneurs sur l'écran
        c_top->add(c_menu);
        c_top->add(c_infos);
        c_top->add(c_nmap);
        c_top->add(c_msgbox);
        c_top->add(c_smap);
        c_top->add(c_lmap);
    }
    catch(gcn::Exception e) // erreur
    {
        std::cerr << e.getMessage() << std::endl;
        delete config;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_EnableKeyRepeat(100, 50);
    SDL_EnableUNICODE(1);

    // timers
    long time = SDL_GetTicks();

    while (loop)
    {
        check_time = SDL_GetTicks();

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 60, 60, 60));

        gui->logic();

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    loop = false;
                break;

                // redimenssione la fenêtre
                case SDL_VIDEORESIZE:
                    if(config->get_key_value("fullscreen", 0) == 0 && config->get_key_value("hwsurface", 0) == 0)
                        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
                    else if(config->get_key_value("fullscreen", 0) == 0 && config->get_key_value("hwsurface", 0) == 1)
                        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
                    else if(config->get_key_value("fullscreen", 0) == 1 && config->get_key_value("hwsurface", 0) == 0)
                        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                    else if(config->get_key_value("fullscreen", 0) == 1 && config->get_key_value("hwsurface", 0) == 1)
                        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                    // Repositionne les widgets correctements
                    c_top->setDimension(gcn::Rectangle(0, 0, event.resize.w, event.resize.h));
                    c_infos->setPosition(event.resize.w-100, 0);
                    // on met à jour la position de la message box si la fenêtre c'est redimenssioner
                    if(c_msgbox->isVisible())
                        c_msgbox->setPosition(screen->w/2-c_msgbox->getWidth()/2, screen->h/2-c_msgbox->getHeight()/2);
                break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        // cache/affiche le menu
                        case SDLK_h:
                            if(c_menu->isVisible())
                            {
                                c_menu->setVisible(false);
                                message_box("Le menu est maintenant invisible.", "Appuyez sur h pour le réafficher");
                            }
                            else
                                c_menu->setVisible(true);
                        break;

                        // chois des layers (f1, f2, f3, f4)
                        case SDLK_F1:
                            layer = 1;
                            break;
                        case SDLK_F2:
                            layer = 2;
                            break;
                        case SDLK_F3:
                            layer = 3;
                            break;
                        case SDLK_F4:
                            layer = 4;
                            break;

                        default:
                        break;
                    }
                    break;

                default:
                break;
            }

            // evénements GUI Guichan
            g_input->pushInput(event);

            // évenements éditeur (tileset/carte)
            if(!c_nmap->isVisible() && !c_smap->isVisible() && !c_msgbox->isVisible() && !c_lmap->isVisible() && d_new_map == true)
                map_event(&event, &map, layer, map.s_tileset->w, 36, tile_w, tile_h, screen->w, screen->h);
        }

        // affiche la carte
        if(d_new_map == true)
        {
            // affiche tous les layers avant le premier layer si on est sur le premier layer
            if(layer == 1)
            {
                // affiche le layer de base (le sol en principe)
                draw_layer(&map, 2, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 3, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 4, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_bg_map(&map, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 200);
            }
            // affiche le layer 2 si on travaille dessus et les autres layers en transparent dérriére
            if(layer == 2)
            {
                draw_bg_map(&map, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 3, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 4, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 2, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 200);
            }
            // affiche le layer 3 si on travaille dessus et les autres layers en transparents
            if(layer == 3)
            {
                draw_bg_map(&map, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 2, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 4, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 3, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 200);
            }
            // affiche le layer 4 si on travaille dessus et les autres layers en transparents
            if(layer == 4)
            {
                draw_bg_map(&map, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 2, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 3, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 128);
                draw_layer(&map, 4, map.s_tileset->w+tile_w, 0, tile_w, tile_h, screen->w, screen->h, 200);
            }

            /* fond du tileset (qui recouvre aussi 3/4 pix de la map) */
            rect.x = 0;
            rect.y = 0;
            rect.w = map.s_tileset->w+7;
            rect.h = screen->h;
            SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 0, 0));
            // affiche le tileset
            draw_tileset(&map, 0, 36, tile_w, tile_h);
        }

        // affiche GUI Guichan
        gui->draw();

        // calcul fps toutes les 150ms
        if(SDL_GetTicks()-time>=150)
        {
            fps = 1000/(SDL_GetTicks()-check_time);
            l_fps->setCaption("FPS: " + to_string(fps)); // affichage fps
            l_fps->setSize(l_fps->getWidth()+2, font->getHeight());
            time = SDL_GetTicks();
        }

        l_layer->setCaption("Couche: " + to_string(layer));
        l_layer->setSize(l_layer->getWidth()+2, font->getHeight());

        SDL_Flip(screen);
        SDL_Delay(5);
    }

    // libération mémoire GUI Guichan
    // widgets
    delete b_load_map;
    delete b_new_map;
    delete save_map;
    delete map_option;
    delete editor_help;
    delete hide_menu;
    delete map_layer1;
    delete map_layer2;
    delete map_layer3;
    delete map_layer4;
    delete map_sizew;
    delete map_sizeh;
    delete l_map_size;
    delete l_tileset;
    delete l_tile_size;
    delete l_text;
    delete l_text2;
    delete l_fps;
    delete l_layer;
    delete l_colorkey;
    delete tile_sizew;
    delete tile_sizeh;
    delete tileset;
    delete font;
    delete create_map;
    delete close;
    delete cr_new_map;
    delete cv_new_map;
    delete cb_new_map;
    delete l_smap;
    delete t_smap;
    delete b_smap;
    delete l_lmap;
    delete t_lmap;
    delete b_lmap;
    // conteneurs
    delete c_top;
    delete c_menu;
    delete c_infos;
    delete c_nmap;
    delete c_msgbox;
    delete c_smap;
    delete c_lmap;
    // guichan
    delete gui;
    delete g_input;
    delete g_graphics;
    delete g_imageloader;
    if(&map!=NULL) // libére la map ouverte si y'en à déjà une
        free_map(&map);
    // libération mémoire SDL & autres
    TTF_Quit();
    SDL_Quit();
    delete config;

    return EXIT_SUCCESS;
}

// message box avec Guichan
void message_box(const std::string& text, const std::string& text2)
{
    l_text->setCaption(text);
    l_text->setAlignment(gcn::Graphics::CENTER);
    l_text->setSize(300, font->getHeight());
    l_text->setPosition(0, c_msgbox->getHeight()/2-l_text->getHeight()-5);
    l_text2->setCaption(text2);
    l_text2->setAlignment(gcn::Graphics::CENTER);
    l_text2->setSize(300, font->getHeight());
    l_text2->setPosition(0, c_msgbox->getHeight()/2-l_text2->getHeight()/2);
    c_msgbox->setPosition(screen->w/2-c_msgbox->getWidth()/2, screen->h/2-c_msgbox->getHeight()/2);
    c_msgbox->setVisible(true);
}
