/*
    Une partie du code provient de l'article suivant :
    http://www.games-creators.org/wiki/Réaliser_un_RPG_en_C_avec_SDL_et_FMOD/L'éditeur_de_carte
*/
#include "ES_Map/esmap.h"

int x, y, tx, ty, i, j, ntileparcol;
int l_iter = 1;
unsigned int last_r, last_g, last_b, last_a = 255;
SDL_Rect src, dest;
#ifdef ES_USE_OPENGL
GLuint tileset_ogl, tileset_ogl_alpha, pixels_format;
GLuint rmask, gmask, bmask, amask;
#endif
ES::ES_Resources res_map;
SDL_Surface *surface_temp, *surface_tmp, *surface_tmp2, *map_col_layer;
SDL_RWops *tileset_wop;
char *buffer_map;
unsigned char *pixels, *pixels_temp;
int cnt, p_cnt;
short int alpha_value = -1;

void init_map(struct ess_map *map)
{
     map->n_tile = map->width = map->height = map->offset_x = map->offset_y = map->offset_y_tileset = 0;
     map->tile_sel = 0;
     map->tileset = NULL;
     map->s_tileset = NULL;
}

#ifdef ES_USE_OPENGL
void draw_bg_map(struct ess_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
#else
void draw_bg_map(struct ess_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen)
#endif
{
   // map
  // x =- (map->offset_x%w_tile)+left;
   y =- (map->offset_y%h_tile)+height;
   tx = map->offset_x/w_tile;
   ty = map->offset_y/h_tile;

   while(y < h_screen)
   {
      x =- (map->offset_x%w_tile)+left;
      tx = map->offset_x/w_tile;
      while(x < w_screen)
      {
        #ifdef ES_USE_OPENGL
            if(ES::ES_OpenGL_Mode == true)
            {
                if(layer == 1 || layer == 0)
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
                    dest.h += dest.y-height;
                    dest.y = height;
                }

                glLoadIdentity();
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, tileset_ogl);

                glBegin(GL_QUADS);
                    glColor4ub(r, g, b, a); // définie la couleur des vertex
                    glTexCoord2f((float)src.x/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y + dest.h);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x, dest.y + dest.h);
                glEnd();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
            }

            // Layer 2
            if(layer == 2 || layer == 0)
            {
                src.x = map->tileset[GET_TILEl2(tx,ty,map)].x;
                src.y = map->tileset[GET_TILEl2(tx,ty,map)].y;

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
                    dest.h += dest.y-height;
                    dest.y = height;
                }

                glLoadIdentity();
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, tileset_ogl);

                glBegin(GL_QUADS);
                    glColor4ub(r, g, b, a); // définie la couleur des vertex
                    glTexCoord2f((float)src.x/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y + dest.h);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x, dest.y + dest.h);
                glEnd();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
            }
/* SHADOW - FIXME
            // affiche le tileset avec la couleur qui doit être transparente
            if(alpha_value <= 0)
            {
                glLoadIdentity();
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, tileset_ogl_alpha);

                glBegin(GL_QUADS);
                    glColor4ub(r, g, b, alpha_value);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y + dest.h);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x, dest.y + dest.h);
                glEnd();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
            }*/
        }
        else
        {
        #endif
         if(layer == 1 || layer == 0)
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
                dest.h += dest.y-height;
                dest.y = height;
            }

            SDL_BlitSurface(map->s_tileset, &src, SDL_GetVideoSurface(), &dest);
         }

         if(layer == 2 || layer == 0)
         {
             src.x = map->tileset[GET_TILEl2(tx,ty,map)].x;
             src.y = map->tileset[GET_TILEl2(tx,ty,map)].y;

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
                dest.h += dest.y-height;
                dest.y = height;
             }

             SDL_BlitSurface(map->s_tileset, &src, SDL_GetVideoSurface(), &dest);
         }
        #ifdef ES_USE_OPENGL
        }
        #endif

         x += w_tile;
         tx++;
      }
      y += h_tile;
      ty++;
    }
}

void draw_layer(struct ess_map *map, int layer, int left, int height, int w_tile, int h_tile, int w_screen, int h_screen, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
   x =- (map->offset_x%w_tile)+left;
   y =- (map->offset_y%h_tile)+height;
   tx = map->offset_x/w_tile;
   ty = map->offset_y/h_tile;

   while(y < h_screen)
   {
      x =- (map->offset_x%w_tile)+left;
      tx = map->offset_x/w_tile;
      while(x < w_screen)
      {

        #ifdef ES_USE_OPENGL
        if(ES::ES_OpenGL_Mode == true)
        {
            // layer 3
            if(layer == 3 || layer == 0)
            {
                src.x = map->tileset[GET_TILEl3(tx,ty,map)].x;
                src.y = map->tileset[GET_TILEl3(tx,ty,map)].y;

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
                    dest.h += dest.y-height;
                    dest.y = height;
                }

                glLoadIdentity();
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, tileset_ogl);

                glBegin(GL_QUADS);
                    glColor4ub(r, g, b, a);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y + dest.h);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x, dest.y + dest.h);
                glEnd();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
            }

            // Layer 4
            if(layer == 4 || layer == 0)
            {
                src.x = map->tileset[GET_TILEl4(tx,ty,map)].x;
                src.y = map->tileset[GET_TILEl4(tx,ty,map)].y;

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
                    dest.h += dest.y-height;
                    dest.y = height;
                }

                glLoadIdentity();
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, tileset_ogl);

                glBegin(GL_QUADS);
                    glColor4ub(r, g, b, a);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y + dest.h);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x, dest.y + dest.h);
                glEnd();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
            }

/* SHADOW - FIXME
            // affiche le tileset avec la couleur qui doit être transparente
            if(alpha_value >= 0)
            {
                glLoadIdentity();
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
                //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, tileset_ogl_alpha);

                glBegin(GL_QUADS);
                    glColor4ub(r, g, b, alpha_value);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)src.y/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y);
                    glTexCoord2f((float)(src.x+src.w)/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x + dest.w, dest.y + dest.h);
                    glTexCoord2f((float)src.x/map->tileset_w, (float)(src.y+src.h)/map->tileset_h);
                    glVertex2i(dest.x, dest.y + dest.h);
                glEnd();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
#endif
            }*/
        }
        else
        {
        #endif
         // layer 3
         if(layer == 3 || layer == 0)
         {
             src.x = map->tileset[GET_TILEl3(tx,ty,map)].x;
             src.y = map->tileset[GET_TILEl3(tx,ty,map)].y;

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
                dest.h += dest.y-height;
                dest.y = height;
             }

             SDL_BlitSurface(map->s_tileset, &src, SDL_GetVideoSurface(), &dest);
         }

         // layer 4
         if(layer == 4 || layer == 0)
         {
             src.x = map->tileset[GET_TILEl4(tx,ty,map)].x;
             src.y = map->tileset[GET_TILEl4(tx,ty,map)].y;

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
                dest.h += dest.y-height;
                dest.y = height;
             }

             SDL_BlitSurface(map->s_tileset, &src, SDL_GetVideoSurface(), &dest);
         }
        #ifdef ES_USE_OPENGL
        }
        #endif

         x += w_tile;
         tx++;
      }
      y += h_tile;
      ty++;
    }

    #ifdef ES_USE_OPENGL
    if(ES::ES_OpenGL_Mode == false && layer == 4)
    {
    #endif
        if(last_r != r || last_g != g || last_b != b || last_a != a)
        {
            SDL_FillRect(map_col_layer, NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, r, g, b));
            SDL_SetAlpha(map_col_layer, SDL_SRCALPHA | SDL_RLEACCEL, a);
        }

        SDL_BlitSurface(map_col_layer, NULL, SDL_GetVideoSurface(), NULL);

        last_r = r; last_g = g; last_b = b; last_a = a;
    #ifdef ES_USE_OPENGL
    }
    #endif
}

void scroll(struct ess_map *map, int x, int y, int screen_w, int screen_h)
{
    if(map->offset_x!=0 || map->offset_x<(map->width*map->tile_w-screen_w)) map->offset_x=x;
    if(map->offset_y!=0 || map->offset_y<(map->height*map->tile_h-screen_h)) map->offset_y=y;
}

// Libération de la map/tileset de la mémoire
void free_map(struct ess_map *map)
{
    SDL_FreeSurface(map_col_layer);

    #ifdef ES_USE_OPENGL
    if(ES::ES_OpenGL_Mode == true)
        glDeleteTextures(1, &tileset_ogl);
    if(ES::ES_OpenGL_Mode == true && alpha_value >= 0)
        glDeleteTextures(1, &tileset_ogl_alpha);
    #endif
    SDL_FreeSurface(map->s_tileset);
    delete map->data;
    delete map->tileset;
    delete map->data_layer2;
    delete map->data_layer3;
    delete map->data_layer4;
}

// pareil que save_map :>
#ifdef ES_USE_OPENGL
int load_esmap(struct ess_map *map, const std::string& filename, short int alpha, unsigned short int r, unsigned short int g, unsigned short int b/*, bool espack, ES_Pack* pack*/)
#else
int load_esmap(struct ess_map *map, const std::string& filename, short int alpha/*, bool espack, ES_Pack* pack*/)
#endif
{
/*    if(espack == true && pack == NULL)
    {*/
        FILE *f;
        int i,x,y;
        char buffer[256];

        if(!map) free_map(map);
        f=fopen(filename.c_str(),"rb");
        if(!f)
            return 1;

        init_map(map);

        // tileset
        memset(&buffer,0,sizeof(buffer));
        fread(&buffer,255,sizeof(char),f);

        surface_tmp=IMG_Load(buffer);
        if(surface_tmp == NULL)
            return 3;
        map->s_tileset = SDL_DisplayFormat(surface_tmp);

        // colorkey
        fread(&map->r,1,sizeof(unsigned int),f);
        fread(&map->v,1,sizeof(unsigned int),f);
        fread(&map->b,1,sizeof(unsigned int),f);

        alpha_value = alpha;

        #ifdef ES_USE_OPENGL
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
        #else
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;
        #endif

        if(ES::ES_OpenGL_Mode == true && alpha >= 0)
        {
            // conserve la taille du tileset élevé à la puissance 2
            map->tileset_w = powerOfTwo(map->s_tileset->w);
            map->tileset_h = powerOfTwo(map->s_tileset->h);

            // si le fichier est un bitmap on inverse le format (RGB->BGR)/(RGBA->BGRA)
            if(toStr(buffer).substr(toStr(buffer).length() - 4) == ".bmp")
                pixels_format = GL_BGRA;
            else if(toStr(buffer).substr(toStr(buffer).length() - 4) != ".bmp")
                pixels_format = GL_RGBA;

            // temp w/h à donner à opengl si problème plus tard
            surface_temp = SDL_CreateRGBSurface(SDL_SWSURFACE, map->tileset_w, map->tileset_h, 32, rmask, gmask, bmask, amask);
            if(!surface_temp)
                return 3;
            else
            {
                // Equivalent colorkey SDL pour OpenGL
                pixels = (unsigned char*) surface_temp->pixels;
                pixels_temp = (unsigned char*) map->s_tileset->pixels;

                p_cnt = 0;
                cnt = 0;

                // Reconstruction de l'image avec la colorkey
                for(int i=0;i<surface_tmp->w;i++)
                {
                    for(int j=0;j<map->s_tileset->h;j++)
                    {
                        pixels[p_cnt] = pixels_temp[cnt];
                        pixels[p_cnt+1] = pixels_temp[cnt+1];
                        pixels[p_cnt+2] = pixels_temp[cnt+2];

                        if((pixels_temp[cnt]==(unsigned int)map->r) && (pixels_temp[cnt+1]==(unsigned int)map->v) && (pixels_temp[cnt+2]==(unsigned int)map->b) || (pixels_temp[cnt]==r) && (pixels_temp[cnt+1]==g) && (pixels_temp[cnt+2]==b))
                        {
                            pixels[p_cnt+3] = 255;
                        }
                        else
                        {
                            pixels[p_cnt+3] = 0;
                        }

                        cnt += 3;
                        p_cnt += 4;
                    }
                }

                // Génére la texture OpenGL
                glGenTextures(1, &tileset_ogl);

                glBindTexture(GL_TEXTURE_2D, tileset_ogl);

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                glTexImage2D(GL_TEXTURE_2D, 0, surface_temp->format->BytesPerPixel, map->tileset_w, map->tileset_h, 0, pixels_format, GL_UNSIGNED_BYTE, surface_temp->pixels);

                SDL_FreeSurface(surface_temp);
            }

            surface_temp = SDL_CreateRGBSurface(SDL_SWSURFACE, map->tileset_w, map->tileset_h, 32, rmask, gmask, bmask, amask);
            if(!surface_temp)
                return 3;
            else
            {
                // recupére les pixels du tileset
                pixels = (unsigned char*) surface_temp->pixels;
                pixels_temp = (unsigned char*) map->s_tileset->pixels;

                p_cnt = 0;
                cnt = 0;

                // Reconstruction du tileset avec seulement la couleur qui doit être semi-transparente
                for(int i=0;i<surface_tmp->w;i++)
                {
                    for(int j=0;j<map->s_tileset->h;j++)
                    {
                        pixels[p_cnt] = pixels_temp[cnt];
                        pixels[p_cnt+1] = pixels_temp[cnt+1];
                        pixels[p_cnt+2] = pixels_temp[cnt+2];

                        // si la couleur du pixel actuel n'est pas de la couleur qui doit être semi-transparente
                        if((pixels_temp[cnt]!=r) && (pixels_temp[cnt+1]!=g) && (pixels_temp[cnt+2]!=b))
                        {
                            // alors on met ses pixels de la couleur de la colorkey
                            //pixels[p_cnt+3] = 255;
                            pixels[p_cnt] = 0;
                            pixels[p_cnt+1] = 0;
                            pixels[p_cnt+2] = 0;
                        }
                        else
                        {
                            //pixels[p_cnt+3] = 0;
                        }
                        cnt += 3;
                        p_cnt += 4;
                    }
                }

                // Génére la texture OpenGL
                glGenTextures(1, &tileset_ogl_alpha);

                glBindTexture(GL_TEXTURE_2D, tileset_ogl_alpha);

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                glTexImage2D(GL_TEXTURE_2D, 0, surface_temp->format->BytesPerPixel, map->tileset_w, map->tileset_h, 0, pixels_format, GL_UNSIGNED_BYTE, surface_temp->pixels);

                SDL_FreeSurface(surface_temp);
            }
            SDL_FreeSurface(surface_tmp);

        }
        else if(ES::ES_OpenGL_Mode == true)
        {
            tileset_ogl = res_map.load_texture(buffer, true, (unsigned int)map->r, (unsigned int)map->v, (unsigned int)map->b);
            if(tileset_ogl == 0)
                return 3;

            // conserve la taille du tileset élevé à la puissance 2
            map->tileset_w = powerOfTwo(map->s_tileset->w);
            map->tileset_h = powerOfTwo(map->s_tileset->h);
            SDL_FreeSurface(surface_tmp);
        }
        else
        {
        #endif
            map_col_layer = SDL_CreateRGBSurface(SDL_HWSURFACE, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h, 32, 0, 0, 0, 0);
            SDL_FillRect(map_col_layer, NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0));
            SDL_SetAlpha(map_col_layer, SDL_SRCALPHA | SDL_RLEACCEL, 0);

            SDL_SetColorKey(map->s_tileset, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(map->s_tileset->format,map->r,map->v,map->b));
            SDL_FreeSurface(surface_tmp);
        #ifdef ES_USE_OPENGL
        }
        #endif

        fread(&map->tile_w,1,sizeof(unsigned int),f);
        fread(&map->tile_h,1,sizeof(unsigned int),f);

        fread(&map->n_tile,1,sizeof(unsigned int),f);
        fread(&map->width,1,sizeof(unsigned int),f);
        fread(&map->height,1,sizeof(unsigned int),f);
        fread(&map->offset_x,1,sizeof(unsigned int),f);
        fread(&map->offset_y,1,sizeof(unsigned int),f);

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
 /*   }
    else if(pack == NULL && espack == true)
        return 1;
    else // choppe la map dans l'archive
    {
        if(pack->getSize(filename) > 0 && pack->getFile(filename) != NULL)
        {
            // chargement dynamique de la map pour pécho les données
            buffer_map = new char[pack->getSize(filename)];
            buffer_map = pack->getFile(filename);
            if(buffer_map != NULL)
            {
                if(!map) free_map(map);

                init_map(map);

                // tileset
                buffer_tmp = new char[255];
                memcpy(&buffer_tmp, &buffer_map, sizeof(buffer_tmp));

                if(pack->getSize(buffer_tmp) > 0 && pack->getFile(buffer_tmp) != NULL)
                    tileset_wop = SDL_RWFromMem(buffer_tmp, pack->getSize(buffer_tmp));
                else
                    return 3;

                if(tileset_wop == NULL)
                    return 3;

                surface_temp = IMG_Load_RW(tileset_wop, 0);
                if(surface_temp == NULL)
                    return 3;
                map->s_tileset = SDL_DisplayFormat(surface_temp); // tileset copié
                SDL_FreeSurface(surface_temp); // on libére tout ce qui n'est plus utiles
                SDL_FreeRW(tileset_wop);
                pack->free(buffer_tmp);

                // colorkey
                fread(&map->r,1,sizeof(unsigned int),f);
                fread(&map->v,1,sizeof(unsigned int),f);
                fread(&map->b,1,sizeof(unsigned int),f);

                #ifdef ES_USE_OPENGL
                if(ES::ES_OpenGL_Mode == true)
                {
                    tileset_ogl = res_map.load_texture(buffer, true, (unsigned int)map->r, (unsigned int)map->v, (unsigned int)map->b);
                    if(tileset_ogl == 0)
                        return 3;

                    // conserve la taille du tileset élevé à la puissance 2
                    map->tileset_w = power_of_two(map->s_tileset->w);
                    map->tileset_h = power_of_two(map->s_tileset->h);
                }
                else
                {
                #endif
                    map_col_layer = SDL_CreateRGBSurface(SDL_HWSURFACE, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h, 32, 0, 0, 0, 0);
                    SDL_FillRect(map_col_layer, NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0));
                    SDL_SetAlpha(map_col_layer, SDL_SRCALPHA | SDL_RLEACCEL, 0);

                    SDL_SetColorKey(map->s_tileset, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(map->s_tileset->format,map->r,map->v,map->b));
                #ifdef ES_USE_OPENGL
                }
                #endif

                fread(&map->tile_w,1,sizeof(unsigned int),f);
                fread(&map->tile_h,1,sizeof(unsigned int),f);

                fread(&map->n_tile,1,sizeof(unsigned int),f);
                fread(&map->width,1,sizeof(unsigned int),f);
                fread(&map->height,1,sizeof(unsigned int),f);
                fread(&map->offset_x,1,sizeof(unsigned int),f);
                fread(&map->offset_y,1,sizeof(unsigned int),f);

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
            }
            else
                return 1;
        }
        else
            return 1;
    }*/

    return 0;
}
