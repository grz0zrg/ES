#ifndef _ES_MAP_H_
#define _ES_MAP_H_

 #include <sdl/sdl.h>
 #include <sdl/sdl_image.h>
 #include <string>

 #define GET_TILE(x,y,map) map->data[x+(y)*map->width]
 #define SUP_TILE(x,y,map) map->data[x*(y)*map->width]

 #define GET_TILEl2(x,y,map) map->data_layer2[x+(y)*map->width]
 #define SUP_TILEl2(x,y,map) map->data_layer2[x*(y)*map->width]

 #define GET_TILEl3(x,y,map) map->data_layer3[x+(y)*map->width]
 #define SUP_TILEl3(x,y,map) map->data_layer3[x*(y)*map->width]

 #define GET_TILEl4(x,y,map) map->data_layer4[x+(y)*map->width]
 #define SUP_TILEl4(x,y,map) map->data_layer4[x*(y)*map->width]

 struct es_tile
 {
    unsigned int x;
    unsigned int y;

    unsigned char col;

    unsigned int next_tile;
 };

 struct es_map
 {
     int n_tile;
     struct es_tile *tileset;
     SDL_Surface *s_tileset;
     SDL_Surface *tiles_list;

	 std::string tileset_file;
     int width;
     int height;
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

     int offset_y_tileset;
     int offset_x_tileset;

     unsigned int tile_sel;
 };

 void init_map(struct es_map *map);
 void new_map(struct es_map *map, int width, int height, int w_tile, int h_tile, const std::string& tileset, int r, int v, int b);
 void draw_bg_map(struct es_map *map, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int alpha_value);
 void draw_layer(struct es_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int alpha_value);
 void draw_tileset(struct es_map *map, int pos_x, int pos_y, int w_tile, int h_tile);
 void map_event(SDL_Event *event, struct es_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen);
 void free_map(struct es_map *map);
 void save_esmap(struct es_map *map, const std::string& filename, const std::string& chipset);
 int load_map(struct es_map *map, const std::string& filename);

#endif
