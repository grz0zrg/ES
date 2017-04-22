#include "map.h"

int x, y, tx, ty, i, j, ntileparcol, spacing, old_w_screen, old_h_screen;
unsigned int old_alpha_value = 300;
SDL_Rect src, dest;
SDL_Surface *temp_surf;

void init_map(struct es_map *map)
{
     map->n_tile = map->width = map->height = map->offset_x = map->offset_y = map->offset_y_tileset = map->offset_x_tileset = 0;
     map->tile_sel = 0;
     map->tileset = NULL;
     map->s_tileset = NULL;
}

void new_map(struct es_map *map, int width, int height, int w_tile, int h_tile, const std::string& tileset, int r, int v, int b)
{
     init_map(map);

     temp_surf = IMG_Load(tileset.c_str());
     map->s_tileset = SDL_DisplayFormat(temp_surf);
     map->tiles_list = SDL_DisplayFormat(temp_surf);
	 map->tileset_file = tileset;
     SDL_FreeSurface(temp_surf);

     // définie la colorkey du tileset
     SDL_SetColorKey(map->s_tileset, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(map->s_tileset->format, r, v, b));

     map->r = r;
     map->v = v;
     map->b = b;

     map->tile_w = w_tile;
     map->tile_h = h_tile;

     map->n_tile = (map->s_tileset->w/w_tile)*(map->s_tileset->h/h_tile);
     map->tileset = new es_tile[map->n_tile];

     x = y = 0;

     for(i=0; i<map->n_tile; i++)
     {
        map->tileset[i].x = x;
        map->tileset[i].y = y;
        map->tileset[i].col = 1;
        map->tileset[i].next_tile = 0;

        x += w_tile;
        if(x > (map->s_tileset->w-w_tile))
        {
           x = 0;
           y += h_tile;
        }
     }

     map->width = width;
     map->height = height;
     old_w_screen = width*w_tile;
     old_h_screen = height*h_tile;

     map->data = new int[map->width*map->height];
     map->data_layer2 = new int[map->width*map->height];
     map->data_layer3 = new int[map->width*map->height];
     map->data_layer4 = new int[map->width*map->height];

     map->n_layer = 0;

     // On rempli la map avec le tile de départ
     for(i = 0; i < map->width; i++)
     {
        for(j = 0; j < map->height; j++)
        {
           GET_TILE(i ,j ,map) = map->tile_sel;
           GET_TILEl2(i ,j ,map) = map->tile_sel;
           GET_TILEl3(i ,j ,map) = map->tile_sel;
           GET_TILEl4(i ,j ,map) = map->tile_sel;
        }
     }
}


void draw_bg_map(struct es_map *map, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int alpha_value)
{
   // vérifie les dimensions de l'écran (sinon plantage si ça dépasse celle de la map)
   if((w_screen-left) - (map->width*w_tile) < 0)
     old_w_screen = w_screen;
   if(h_screen - (map->height*h_tile) < 0)
     old_h_screen = h_screen;

   // map
   x =- (map->offset_x%w_tile)+left;
   y =- (map->offset_y%h_tile)+height;
   tx = map->offset_x/w_tile;
   ty = map->offset_y/h_tile;

   while(y < old_h_screen)
   {
      x =- (map->offset_x%w_tile)+left;
      tx = map->offset_x/w_tile;
      while(x < old_w_screen)
      {
         src.x = map->tileset[GET_TILE(tx,ty,map)].x;
         src.y = map->tileset[GET_TILE(tx,ty,map)].y;
         dest.x = x;
         dest.y = y;
         src.w = dest.w = w_tile;
         src.h = dest.h = h_tile;

         if(dest.x < left)
         {
            src.x -= dest.x-left;
            src.w += dest.x-left;
            dest.w += dest.x-left;
            dest.x = left;
         }
         if(dest.y < height)
         {
            src.y -= dest.y-height;
            src.h += dest.y-height;
            dest.h += dest.h-height;
            dest.y = height;
         }

         // permet de rendre transparent une couche si l'alpha à changé
         if(alpha_value < 255 && old_alpha_value != alpha_value)
         {
            old_alpha_value = alpha_value;
            SDL_SetAlpha(map->s_tileset, SDL_SRCALPHA | SDL_RLEACCEL, alpha_value);
         }

         SDL_BlitSurface(map->s_tileset, &src, SDL_GetVideoSurface(), &dest);

         x += w_tile;
         tx++;
      }
      y += h_tile;
      ty++;
    }
}

void draw_layer(struct es_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int alpha_value)
{
   // vérifie les dimensions de l'écran (sinon plantage si ça dépasse celle de la map)
   if((w_screen-left) - (map->width*w_tile) < 0)
     old_w_screen = w_screen;
   if(h_screen - (map->height*h_tile) < 0)
     old_h_screen = h_screen;

   x =- (map->offset_x%w_tile)+left;
   y =- (map->offset_y%h_tile)+height;
   tx = map->offset_x/w_tile;
   ty = map->offset_y/h_tile;

   while(y < old_h_screen)
   {
      x =- (map->offset_x%w_tile)+left;
      tx = map->offset_x/w_tile;
      while(x < old_w_screen)
      {
         if(layer == 2)
         {
             src.x = map->tileset[GET_TILEl2(tx,ty,map)].x;
             src.y = map->tileset[GET_TILEl2(tx,ty,map)].y;
         }
         else if(layer == 3)
         {
             src.x = map->tileset[GET_TILEl3(tx,ty,map)].x;
             src.y = map->tileset[GET_TILEl3(tx,ty,map)].y;
         }
         else if(layer == 4)
         {
             src.x = map->tileset[GET_TILEl4(tx,ty,map)].x;
             src.y = map->tileset[GET_TILEl4(tx,ty,map)].y;
         }
         dest.x = x;
         dest.y = y;
         src.w = dest.w = w_tile;
         src.h = dest.h = h_tile;

         if(dest.x < left)
         {
            src.x -= dest.x-left;
            src.w += dest.x-left;
            dest.w += dest.x-left;
            dest.x = left;
         }
         if(dest.y < height)
         {
            src.y -= dest.y-height;
            src.h += dest.y-height;
            dest.h += dest.h-height;
            dest.y = height;
         }

         // permet de rendre transparente une couche si l'alpha à changé
         if(alpha_value < 255 && old_alpha_value != alpha_value)
         {
            old_alpha_value = alpha_value;
            SDL_SetAlpha(map->s_tileset, SDL_SRCALPHA | SDL_RLEACCEL, alpha_value);
         }

         SDL_BlitSurface(map->s_tileset, &src, SDL_GetVideoSurface(), &dest);

         x += w_tile;
         tx++;
      }
      y += h_tile;
      ty++;
    }
}

void draw_tileset(struct es_map *map, int pos_x, int pos_y, int w_tile, int h_tile)
{
    // tileset
    x = pos_x;
    y = pos_y;

    for(i=map->offset_y_tileset; i<map->n_tile; i++)
    {
       src.x = map->tileset[i].x;
       src.y = map->tileset[i].y;
       dest.x = x;
       dest.y = y;
       src.w = dest.w = w_tile;
       src.h = dest.h = h_tile;

       if((unsigned int)i == map->tile_sel)
       {
         dest.x--;
         dest.y--;
         dest.w += 2;
         dest.h += 2;
         SDL_FillRect(SDL_GetVideoSurface(), &dest,
            (map->tileset[i].col == 0)?
               SDL_MapRGB(SDL_GetVideoSurface()->format, 0,255,0)
             : SDL_MapRGB(SDL_GetVideoSurface()->format, 255,0,0));
         dest.x++;
         dest.y++;
         dest.w -= 2;
         dest.h -= 2;
       }

       SDL_BlitSurface(map->tiles_list, &src, SDL_GetVideoSurface(), &dest);

       dest.x += w_tile-2;
       dest.y += h_tile-2;
       dest.w = dest.h = 2;
       SDL_FillRect(SDL_GetVideoSurface(), &dest,
            (map->tileset[i].col == 0)?
               SDL_MapRGB(SDL_GetVideoSurface()->format, 0,255,0)
             : SDL_MapRGB(SDL_GetVideoSurface()->format, 255,0,0));


       x += w_tile+1;
       if(x > map->s_tileset->w)//(width-w_tile))
       {
          x = pos_x;
          y += h_tile+1;
       }
    }

}

void map_event(SDL_Event *event, struct es_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen)
{
     if(!event) return;

     // tiles par colonnes
     ntileparcol=(map->s_tileset->w+w_tile)/(w_tile+1);

     switch(event->type)
     {
      case SDL_KEYDOWN:
	    if(event->key.state==SDL_PRESSED)
	    {
          if(event->key.keysym.sym==SDLK_RIGHT)
          {
             if (map->offset_x<(map->width*w_tile-w_screen)) map->offset_x+=w_tile;
          }
          else if(event->key.keysym.sym==SDLK_LEFT)
          {
             if (map->offset_x!=0) map->offset_x-=w_tile;
          }
          else if(event->key.keysym.sym==SDLK_DOWN)
          {
             if (map->offset_y<(map->height*h_tile-h_screen)) map->offset_y+=h_tile;
          }
          else if(event->key.keysym.sym==SDLK_UP)
          {
             if (map->offset_y!=0) map->offset_y-=h_tile;
          }
          // tileset scroll
          else if(event->key.keysym.sym==SDLK_PAGEUP)
          {
             if (map->offset_y_tileset>=ntileparcol) map->offset_y_tileset-=ntileparcol;
          }
          else if(event->key.keysym.sym==SDLK_PAGEDOWN)
          {
             if (map->offset_y_tileset<map->n_tile-ntileparcol) map->offset_y_tileset+=ntileparcol;
          }
          // fonctions spéciales (remplissages/téléportation)
          else if(event->key.keysym.sym==SDLK_f)
          {
             for(i=0;i<map->width;i++)
             {
                for(j=0;j<map->height;j++)
                {
                    if(layer == 1)
                   GET_TILE(i,j,map)=map->tile_sel;
                   else if(layer == 2)
                   GET_TILEl2(i,j,map)=map->tile_sel;
                   else if(layer == 3)
                   GET_TILEl3(i,j,map)=map->tile_sel;
                   else if(layer == 4)
                   GET_TILEl4(i,j,map)=map->tile_sel;
                }
             }
          }
        }
      break;

      case SDL_MOUSEBUTTONDOWN:
           if(event->button.button==SDL_BUTTON_LEFT)
           {
             if(event->button.x<=(left))
              {
                map->tile_sel=map->offset_y_tileset+(event->button.x/(w_tile+1))+ntileparcol*(event->button.y/(h_tile+1)-1);
                if(map->tile_sel>=(unsigned int)map->n_tile) map->tile_sel=map->n_tile-1;
              } else if(event->button.x>=left+w_tile) {
                 i=map->offset_x+event->button.x-left-w_tile;
                 j=map->offset_y+event->button.y-height+h_tile;
                 i /= w_tile;
                 j /= h_tile;

                 if(i>=map->width) i=map->width-1;
                 if(j>=map->height) j=map->height-1;
                 if(layer == 1)
                    GET_TILE(i,j,map)=map->tile_sel;
                 else if(layer == 2)
                    GET_TILEl2(i,j,map)=map->tile_sel;
                 else if(layer == 3)
                    GET_TILEl3(i,j,map)=map->tile_sel;
                 else if(layer == 4)
                    GET_TILEl4(i,j,map)=map->tile_sel;

                 /* on appuie sur CTRL */
                 if((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_ALT))
                 {
                    if(i<(map->width-1)) GET_TILE(i+1,j,map)=map->tile_sel;
                    if(j>0) GET_TILE(i,j-1,map)=map->tile_sel;
                    if((j>0) && (i<(map->width-1))) GET_TILE(i+1,j-1,map)=map->tile_sel;

                    if(SDL_GetModState() & KMOD_ALT)
                    {
                       if(i>0) GET_TILE(i-1,j,map)=map->tile_sel;
                       if(j<(map->height-1)) GET_TILE(i,j+1,map)=map->tile_sel;

                       if((i>0) && (j<(map->height-1))) GET_TILE(i-1,j+1,map)=map->tile_sel;
                       if((i>0) && (j>0)) GET_TILE(i-1,j-1,map)=map->tile_sel;
                       if((j<(map->height-1)) && (i<(map->width-1)))
                          GET_TILE(i+1,j+1,map)=map->tile_sel;
                    }
                 }
              }
           } else if(event->button.button==SDL_BUTTON_RIGHT)
           {
              if(event->button.x<=(left))
              {
               i=map->offset_y_tileset
                 +(event->button.x/(w_tile+1))
                 +ntileparcol*(event->button.y/(h_tile+1)-1);

               if(i>=map->n_tile) i=map->n_tile-1;
               map->tileset[i].col=(map->tileset[i].col+1)%2;
              }
           }
           else if(event->button.button == SDL_BUTTON_WHEELUP)
           {
               if (map->offset_y_tileset>=ntileparcol) map->offset_y_tileset-=ntileparcol;
           }
           else if(event->button.button == SDL_BUTTON_WHEELDOWN)
           {
               if (map->offset_y_tileset<map->n_tile-ntileparcol) map->offset_y_tileset+=ntileparcol;
           }
      break;
     }
}

// Libération de la map/tileset de la mémoire
void free_map(struct es_map *map)
{
    SDL_FreeSurface(map->s_tileset);
    delete map->data;
    delete map->tileset;
    delete map->data_layer2;
    delete map->data_layer3;
    delete map->data_layer4;
}

// pareil que save_map :>
int load_map(struct es_map *map, const std::string& filename)
{
     FILE *f;
     int i,x,y;
     char buffer[256];

     if(!map) return 1;
     f=fopen(filename.c_str(),"rb");
     if(!f) return 1;

     init_map(map);

     // tileset
     memset(&buffer,0,sizeof(buffer));
     fread(&buffer,255,sizeof(char),f);
     temp_surf=IMG_Load(buffer);
     if(temp_surf == NULL)
        return 3;

	 map->tileset_file = buffer;
     map->s_tileset = SDL_DisplayFormat(temp_surf);
     map->tiles_list = SDL_DisplayFormat(temp_surf);
     SDL_FreeSurface(temp_surf);

     // colorkey
     fread(&map->r,1,sizeof(unsigned int),f);
     fread(&map->v,1,sizeof(unsigned int),f);
     fread(&map->b,1,sizeof(unsigned int),f);

     fread(&map->tile_w,1,sizeof(unsigned int),f);
     fread(&map->tile_h,1,sizeof(unsigned int),f);

     SDL_SetColorKey(map->s_tileset, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(map->s_tileset->format,map->r,map->v,map->b));

     fread(&map->n_tile,1,sizeof(unsigned int),f);
     fread(&map->width,1,sizeof(unsigned int),f);
     fread(&map->height,1,sizeof(unsigned int),f);
     fread(&map->offset_x,1,sizeof(unsigned int),f);
     fread(&map->offset_y,1,sizeof(unsigned int),f);

     old_w_screen = map->width*map->tile_w;
     old_h_screen = map->height*map->tile_h;

     map->tileset=new es_tile[map->n_tile];

     for(i=0;i<map->n_tile;i++)
     {
        fread(&map->tileset[i].x, 1, sizeof(unsigned int),f);
        fread(&map->tileset[i].y, 1, sizeof(unsigned int),f);
        fread(&map->tileset[i].col, 1, sizeof(unsigned char),f);
        fread(&map->tileset[i].next_tile, 1, sizeof(unsigned int),f);
     }

     map->data = new int[map->width*map->height];
     map->data_layer2 = new int[map->width*map->height];
     map->data_layer3 = new int[map->width*map->height];
     map->data_layer4 = new int[map->width*map->height];

     for(x=0;x<map->width;x++)
        for(y=0;y<map->height;y++)
        {
           fread(&GET_TILE(x,y,map), 1, sizeof(unsigned int),f);
           if(GET_TILE(x,y,map)>=map->n_tile)
           {
              return 2;
              GET_TILE(x,y,map)=0;
           }
           fread(&GET_TILEl2(x,y,map), 1, sizeof(unsigned int),f);
           if(GET_TILEl2(x,y,map)>=map->n_tile)
           {
              return 2;
              GET_TILEl2(x,y,map)=0;
           }
           fread(&GET_TILEl3(x,y,map), 1, sizeof(unsigned int),f);
           if(GET_TILEl3(x,y,map)>=map->n_tile)
           {
              return 2;
              GET_TILEl3(x,y,map)=0;
           }
           fread(&GET_TILEl4(x,y,map), 1, sizeof(unsigned int),f);
           if(GET_TILEl4(x,y,map)>=map->n_tile)
           {
              return 2;
              GET_TILEl4(x,y,map)=0;
           }
        }
     fclose(f);

     return 0;
}

// flemme de transformer ça en c++ ça reste en C sinon voir du coté de ofstream et compagnies
void save_esmap(struct es_map *map, const std::string& filename, const std::string& tileset)
{
     FILE *f;
     int i,x,y;
     char buffer[256];

     if(!map) return;
     f=fopen(filename.c_str(),"wb");
     if(!f) return;

     memset(&buffer,0,sizeof(buffer));
     strcpy(buffer,tileset.c_str());
     fwrite(&buffer,255,sizeof(char),f);

     // colorkey
     fwrite(&map->r,1,sizeof(unsigned int),f);
     fwrite(&map->v,1,sizeof(unsigned int),f);
     fwrite(&map->b,1,sizeof(unsigned int),f);

     fwrite(&map->tile_w,1,sizeof(unsigned int),f);
     fwrite(&map->tile_h,1,sizeof(unsigned int),f);

     fwrite(&map->n_tile,1,sizeof(unsigned int),f);
     fwrite(&map->width,1,sizeof(unsigned int),f);
     fwrite(&map->height,1,sizeof(unsigned int),f);
     fwrite(&map->offset_x,1,sizeof(unsigned int),f);
     fwrite(&map->offset_y,1,sizeof(unsigned int),f);

     for(i=0;i<map->n_tile;i++)
     {
        fwrite(&map->tileset[i].x, 1, sizeof(unsigned int),f);
        fwrite(&map->tileset[i].y, 1, sizeof(unsigned int),f);
        fwrite(&map->tileset[i].col, 1, sizeof(unsigned char),f);
        fwrite(&map->tileset[i].next_tile, 1, sizeof(unsigned int),f);
     }

     for(x=0;x<map->width;x++)
     {
        for(y=0;y<map->height;y++)
        {
           fwrite(&GET_TILE(x,y,map), 1, sizeof(unsigned int),f);
           fwrite(&GET_TILEl2(x,y,map), 1, sizeof(unsigned int),f);
           fwrite(&GET_TILEl3(x,y,map), 1, sizeof(unsigned int),f);
           fwrite(&GET_TILEl4(x,y,map), 1, sizeof(unsigned int),f);
        }
     }
     fclose(f);
}
