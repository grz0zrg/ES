#ifndef _ess_map_H_
#define _ess_map_H_

 #include <sdl/sdl.h>
 #include <sdl/sdl_image.h>
 #include <string>
 #include "../ES_Display/graphics.h"
 #include "../ES_Utils/utils.h"

 #define GET_TILE(x,y,map) map->data[x+(y)*map->width]
 #define GET_TILEl2(x,y,map) map->data_layer2[x+(y)*map->width]
 #define GET_TILEl3(x,y,map) map->data_layer3[x+(y)*map->width]
 #define GET_TILEl4(x,y,map) map->data_layer4[x+(y)*map->width]

 #define GET_TILEp(x,y,map) map.data[x+(y)*map.width]
 #define GET_TILEpl2(x,y,map) map.data_layer2[x+(y)*map.width]
 #define GET_TILEpl3(x,y,map) map.data_layer3[x+(y)*map.width]
 #define GET_TILEpl4(x,y,map) map.data_layer4[x+(y)*map.width]

 struct es_tile
 {
    unsigned int x;
    unsigned int y;

    unsigned char col;

    unsigned int next_tile;
 };

 struct ess_map
 {
     int n_tile;
     struct es_tile *tileset;
     SDL_Surface *s_tileset;

     int width;
     int height;
     int tileset_w;
     int tileset_h;
     int tile_w;
     int tile_h;
     unsigned int n_layer;
     int *data;
     int *data_layer2;
     int *data_layer3;
     int *data_layer4;

     unsigned int r;
     unsigned int v;
     unsigned int b;

     int offset_y;
     int offset_x;

     unsigned int offset_y_tileset;

     unsigned int tile_sel;
 };

 void init_map(struct ess_map *map);

 #ifdef ES_USE_OPENGL
 void draw_bg_map(struct ess_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
 #else
 void draw_bg_map(struct ess_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen);
 #endif
 void draw_layer(struct ess_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int r, unsigned int g, unsigned int b, unsigned int a);
 void draw_tileset(struct ess_map *map, int width, int pos_x, int pos_y, int w_tile, int h_tile);
 void map_event(SDL_Event *event, struct ess_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen);
 void free_map(struct ess_map *map);
 void scroll(struct ess_map *map, int x, int y, int screen_w, int screen_h);
 #ifdef ES_USE_OPENGL
 int load_esmap(struct ess_map *map, const std::string& filename, short int alpha, unsigned short int r, unsigned short int g, unsigned short int b);
 #else
 int load_esmap(struct ess_map *map, const std::string& filename, short int alpha);
 #endif
#endif
