/*
* Copyright (c) 2006, 2007, 2008 Julien Verneuil
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
// Les parties pour la gestion des fps ont étaît inspirés des travaux du créateur de SDL_gfx -> :
// http://www.ferzkopp.net/Software/SDL_gfx-2.0/

#include "ES_Display/graphics.h"

namespace ES
{
    bool ES_OpenGL_Mode = false;

    ES_Graphics::ES_Graphics(const std::string& title, unsigned short int w_screen , unsigned short int h_screen, unsigned short int bpp, bool fullscreen, bool r_mode, bool hide_cursor, const std::string& cursor_path, unsigned short int r, unsigned short int g, unsigned short int b)
    {
        #ifdef ES_USE_OPENGL
        id_cursor = 0;
        #endif

        ES_OpenGL_Mode = r_mode;

        // Initialisation SDL
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        {
            std::cerr << "_Can't initialize SDL :" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_JoystickOpen(0);

        // mode OpenGL
        if(r_mode == true)
        {
            #ifdef ES_USE_OPENGL
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                opengl_mode = true;
                if(fullscreen == true)
                    screen = SDL_SetVideoMode(w_screen, h_screen, bpp, SDL_OPENGL | SDL_FULLSCREEN);
                else
                    screen = SDL_SetVideoMode(w_screen, h_screen, bpp, SDL_OPENGL);
                resources.set_opengl_mode();
            #endif
        }
        else // mode SDL
        {
            opengl_mode = false;
            #ifndef GP2X
            if(fullscreen == true)
                screen = SDL_SetVideoMode(w_screen, h_screen, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
            else
                screen = SDL_SetVideoMode(w_screen, h_screen, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
            #else
            if(fullscreen == true)
                screen = SDL_SetVideoMode(w_screen, h_screen, bpp, SDL_SWSURFACE | SDL_FULLSCREEN);
            else
                screen = SDL_SetVideoMode(w_screen, h_screen, bpp, SDL_SWSURFACE);
            #endif
        }

        this->fullscreen = fullscreen;

        if(screen == NULL)
        {
            std::cerr << "_Can't set video mode: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        SDL_WM_SetCaption(title.c_str(), NULL); // set un titre à la fenêtre

        // hide cursor
        if(hide_cursor == true)
            SDL_ShowCursor(SDL_DISABLE);

        check_custom_cursor = false;

        // custom cursor
        if(cursor_path == "" && hide_cursor == true)
            check_custom_cursor = false;
        else if(cursor_path != "" && hide_cursor == true)
        {
            #ifdef ES_USE_OPENGL
            if(r_mode == true)
            {
                check_custom_cursor = true;
                id_cursor = resources.load_texture(cursor_path, true, 255, 0, 255);
                if(id_cursor == 0)
                {
                    std::cerr << "_Can't load custom cursor: " << SDL_GetError() << std::endl;
                    check_custom_cursor = false;
                    SDL_ShowCursor(SDL_ENABLE);
                }

            }
            else
            {
            #endif
                check_custom_cursor = true;
                cursor_temp = IMG_Load(cursor_path.c_str());

                // échoué ? on print l'erreur et on set le curseur par défaut
                if(cursor_temp == NULL)
                {
                    std::cerr << "_Can't load custom cursor: " << SDL_GetError() << std::endl;
                    check_custom_cursor = false;
                    SDL_ShowCursor(SDL_ENABLE);
                }
                else
                {
                    cursor = SDL_DisplayFormat(cursor_temp);
                    SDL_FreeSurface(cursor_temp);
                    if(cursor != NULL)
                        SDL_SetColorKey(cursor, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(cursor->format, r, g, b));
                    else
                    {
                        std::cerr << "_Can't convert custom cursor: " << SDL_GetError() << std::endl;
                        check_custom_cursor = false;
                        SDL_ShowCursor(SDL_ENABLE);
                    }
                }
            #ifdef ES_USE_OPENGL
            }
            #endif
        }

        es_timer = SDL_GetTicks(); // récup timer

        // régle les valeurs par défaut du fond et des fondus
        r_screen = 0; v_screen = 0; b_screen = 0;
        r_max = 255; v_max = 255; b_max = 255;
        r_min = 0; v_min = 0; b_min = 0;
        fade = false; fade_in = true; fade_out = false;

        loop = true;
        orto_view = false;
        actual_bpp = bpp;

        framecount = 0;
        rate = ES_FPS_DEFAULT;
        rateticks = (1000.0 / (float) ES_FPS_DEFAULT);
        lastticks = SDL_GetTicks();
        lasttime = SDL_GetTicks();
        frames = 0;
    }

    ES_Graphics::~ES_Graphics()
    {
        if(check_custom_cursor == true && opengl_mode == false)
            SDL_FreeSurface(cursor);

		SDL_Quit();

#ifdef GP2X // return to gp2x menu
        system("/sbin/rmmod mmuhack");
        chdir("/usr/gp2x");
        execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif
    }

    void ES_Graphics::setTitle(const std::string& title)
    {
        SDL_WM_SetCaption(title.c_str(), NULL);
    }

    // méthodes utilisants opengl
    void ES_Graphics::openGL_Init(float r, float g, float b, float a, unsigned int target, unsigned int mode)
    {
        #ifdef ES_USE_OPENGL
        if(opengl_mode == true)
        {
            glClearColor(r,g,b,a);
            glHint(target, mode);
        }
        #else
        r = r; g = g; b = b; a = a; target = 0; mode = 0;
        #endif
    }

    #ifdef ES_USE_OPENGL
    void ES_Graphics::openGL_BeginIMG(const std::string& resource_name, bool blend, GLenum sfactor, GLenum dfactor)
    {
        if(opengl_mode == true)
        {
            glLoadIdentity();

            if(blend == true){
                glEnable(GL_BLEND);
                glBlendFunc(sfactor,dfactor);
            }

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, resources.getOpenGL_IMG(resource_name));
        }
    }

    void ES_Graphics::drawQuad(SDL_Rect dest)
    {
        glBegin(GL_QUADS);
            glTexCoord2i(0,0);
            glVertex3f(dest.x,dest.y,0);
            glTexCoord2i(1,0);
            glVertex3f(dest.x+dest.w,dest.y,0);
            glTexCoord2i(1,1);
            glVertex3f(dest.x+dest.w,dest.y+dest.h,0);
            glTexCoord2i(0,1);
            glVertex3f(dest.x,dest.y+dest.h,0);
        glEnd();
    }

    void ES_Graphics::openGL_EndIMG(bool blend)
    {
        if(opengl_mode == true)
        {
            glDisable(GL_TEXTURE_2D);
            if(blend == true)
                glDisable(GL_BLEND);
        }
    }
    #endif

    void ES_Graphics::setOrthoView(bool orto_default, unsigned short int w, unsigned short int h)
    {
        #ifdef ES_USE_OPENGL
        if(opengl_mode == true)
        {
            if(w == 0 || h == 0)
            {
                w = getWindowWidth();
                h = getWindowHeight();
            }

            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            if(orto_default == true)
                glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
            else
                glOrtho(0, w, h, 0, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            orto_view = true;

            glDisable(GL_DEPTH_TEST);
        }
        #else
        orto_default = orto_default; w = w; h = h;
        #endif
    }

    bool ES_Graphics::getUseOpenGL()
    {
        return opengl_mode;
    }

    int ES_Graphics::setFPS(int rate)
    {
        if((rate >= ES_FPS_LOWER_LIMIT) && (rate <= ES_FPS_UPPER_LIMIT))
        {
            framecount = 0;
            this->rate = rate;
            rateticks = (1000.0 / (float) rate);
            return 0;
        }
        else
            return -1;
    }

    int ES_Graphics::getFrameRate()
    {
        return rate;
    }

    void ES_Graphics::delay(unsigned int ms)
    {
        SDL_Delay(ms);
    }

    void ES_Graphics::fpsDelay()
    {
        framecount++;

        current_ticks = SDL_GetTicks();
        target_ticks = lastticks + (Uint32) ((float) framecount * rateticks);

        if(current_ticks <= target_ticks)
        {
            the_delay = target_ticks - current_ticks;
            SDL_Delay(the_delay);
        }
        else
        {
            framecount = 0;
            lastticks = SDL_GetTicks();
        }
    }

    void ES_Graphics::displayCustomCursor(SDL_Rect dest)
    {
        if(check_custom_cursor == true && opengl_mode == false)
            SDL_BlitSurface(cursor, NULL, screen, mouse.getCursorRect());
        #ifdef ES_USE_OPENGL
            else if(check_custom_cursor == true && opengl_mode == true)
            {
                glLoadIdentity();

                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, id_cursor);

                glBegin(GL_QUADS);
                    glTexCoord2i(0,0);
                    glVertex3f(mouse.getX(),mouse.getY(),0);
                    glTexCoord2i(1,0);
                    glVertex3f(mouse.getX()+dest.w,mouse.getY(),0);
                    glTexCoord2i(1,1);
                    glVertex3f(mouse.getX()+dest.w,mouse.getY()+dest.h,0);
                    glTexCoord2i(0,1);
                    glVertex3f(mouse.getX(),mouse.getY()+dest.h,0);
                glEnd();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
            }
        #else
        dest.x = dest.x;
        #endif
    }

    SDL_Rect ES_Graphics::rect(short int x, short int y, short int w, short int h)
    {
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = w;
        r.h = h;

        return r;
    }

    SDL_Color ES_Graphics::color(short r, short g, short b, short a)
    {
        SDL_Color color;
        color.r = r;
        color.g = g;
        color.b = b;
        color.unused = a;

        return color;
    }

    void ES_Graphics::blitSDLSurface(SDL_Surface *surf, SDL_Rect src, SDL_Rect dest, double zoom)
    {
        if(zoom > 1.0)
        {
            SDL_Surface *rot = rotozoomSurface(surf, 0.0, zoom, 0);
            dest.x = dest.x;
            dest.y = dest.y;
            dest.w = rot->w;
            dest.h = rot->h;
            SDL_BlitSurface(rot, &src, screen, &dest);
            SDL_FreeSurface(rot);
        }
        else
            SDL_BlitSurface(surf, &src, screen, &dest);
    }

    void ES_Graphics::drawIMG(const std::string& resource_name, SDL_Rect src, SDL_Rect dest, short int alpha, bool blend, double angle)
    {
        if(opengl_mode == false) // affiche avec sdl
        {
            if(angle != 0.0f)
            {
                rot = rotozoomSurface(resources.getSDL_IMG(resource_name), -angle, 1.0, 1);
                Uint32 ck = SDL_MapRGB(rot->format, 0, 0, 0);
                SDL_SetColorKey(rot, SDL_RLEACCEL | SDL_SRCCOLORKEY, ck);
                dest.x = dest.x/* - (rot->w / 2)*/;
                dest.y = dest.y/* - (rot->h / 2)*/;
                dest.w = rot->w;
                dest.h = rot->h;
                SDL_BlitSurface(rot, NULL, screen, &dest);
                SDL_FreeSurface(rot);
            }
            else
            {
                SDL_SetAlpha(resources.getSDL_IMG(resource_name), SDL_SRCALPHA | SDL_RLEACCEL, alpha);
                SDL_BlitSurface(resources.getSDL_IMG(resource_name), &src, screen, &dest);
            }
        }
        #ifdef ES_USE_OPENGL
        else // affiche avec opengl
        {
            glLoadIdentity();

            if(blend == true)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
            }

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, resources.getOpenGL_IMG(resource_name));

            if(angle != 0.0f)
            {
                glTranslatef(dest.x+dest.w/2, dest.y+dest.h/2, 0.0f);
                glRotatef(angle, 0.0f, 0.0f, 1.0f);

                glBegin(GL_QUADS);
                    glTexCoord2f((float)src.x/resources.getTextWidth(resource_name),(float)src.y/resources.getTextHeight(resource_name));
                    glVertex2i(-dest.w/2,-dest.h/2);
                    glTexCoord2f((float)(src.x+src.w)/resources.getTextWidth(resource_name),(float)src.y/resources.getTextHeight(resource_name));
                    glVertex2i(-dest.w/2,+dest.h/2);
                    glTexCoord2f((float)(src.x+src.w)/resources.getTextWidth(resource_name),(float)(src.y+src.h)/resources.getTextHeight(resource_name));
                    glVertex2i(+dest.w/2,+dest.h/2);
                    glTexCoord2f((float)src.x/resources.getTextWidth(resource_name),(float)(src.y+src.h)/resources.getTextHeight(resource_name));
                    glVertex2i(+dest.w/2,-dest.h/2);
                glEnd();
            }
            else
            {
                glBegin(GL_QUADS);
                    glTexCoord2f((float)src.x/resources.getTextWidth(resource_name),(float)src.y/resources.getTextHeight(resource_name));
                    glVertex2i(dest.x,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/resources.getTextWidth(resource_name),(float)src.y/resources.getTextHeight(resource_name));
                    glVertex2i(dest.x+dest.w,dest.y);
                    glTexCoord2f((float)(src.x+src.w)/resources.getTextWidth(resource_name),(float)(src.y+src.h)/resources.getTextHeight(resource_name));
                    glVertex2i(dest.x+dest.w,dest.y+dest.h);
                    glTexCoord2f((float)src.x/resources.getTextWidth(resource_name),(float)(src.y+src.h)/resources.getTextHeight(resource_name));
                    glVertex2i(dest.x,dest.y+dest.h);
                glEnd();
            }

            glDisable(GL_TEXTURE_2D);
            if(blend == true)
                glDisable(GL_BLEND);
        }
        #else
        blend = blend;
        #endif
    }

    SDL_Color ES_Graphics::getPixelRGB(SDL_Surface *surf, int x, int y)
    {
        Uint8 r,g,b;
        SDL_Color color;
        int bpp = surf->format->BytesPerPixel;
        Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

        switch(bpp)
        {
            case 1:
                SDL_GetRGB(*p, surf->format,&r,&g,&b);
                color.r = r;
                color.g = g;
                color.b = b;
                color.unused = 0;
                return color;

            case 2:
                SDL_GetRGB(*(Uint16 *)p, surf->format,&r,&g,&b);
                color.r = r;
                color.g = g;
                color.b = b;
                color.unused = 0;
                return color;

            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    SDL_GetRGB(p[0] << 16 | p[1] << 8 | p[2], surf->format,&r,&g,&b);
                    color.r = r;
                    color.g = g;
                    color.b = b;
                    color.unused = 0;
                    return color;
                }
                else
                {
                    SDL_GetRGB(p[0] | p[1] << 8 | p[2] << 16, surf->format,&r,&g,&b);
                    color.r = r;
                    color.g = g;
                    color.b = b;
                    color.unused = 0;
                    return color;
                }

            case 4:
                SDL_GetRGB(*(Uint32 *)p, surf->format,&r,&g,&b);
                color.r = r;
                color.g = g;
                color.b = b;
                color.unused = 0;
                return color;

            default:
                return color;
        }
    }

    void ES_Graphics::setPixel(unsigned short int x, unsigned short int y, unsigned short int r, unsigned short int g, unsigned short int b, bool verif)
    {
        if(opengl_mode == false)
        {
            if(verif == true)
            {
                if(x > 0 && x < screen->w && y > 0 && y < screen->h)
                {
                    SDL_LockSurface(screen);
                    int bpp = screen->format->BytesPerPixel;
                    Uint32 pixel=SDL_MapRGB(screen->format, r,g,b);

                    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

                    switch(bpp)
                    {
                        case 1:
                            *p = pixel;
                            break;

                        case 2:
                            *(Uint16 *)p = pixel;
                            break;

                        case 3:
                            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                                p[0] = (pixel >> 16) & 0xff;
                                p[1] = (pixel >> 8) & 0xff;
                                p[2] = pixel & 0xff;
                            } else {
                                p[0] = pixel & 0xff;
                                p[1] = (pixel >> 8) & 0xff;
                                p[2] = (pixel >> 16) & 0xff;
                            }
                            break;

                        case 4:
                            *(Uint32 *)p = pixel;
                            break;
                    }
                    SDL_UnlockSurface(screen);
                }
            }
            else
            {
                SDL_LockSurface(screen);
                int bpp = screen->format->BytesPerPixel;
                Uint32 pixel=SDL_MapRGB(screen->format, r,g,b);

                Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

                switch(bpp)
                {
                    case 1:
                        *p = pixel;
                        break;

                    case 2:
                        *(Uint16 *)p = pixel;
                        break;

                    case 3:
                        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                            p[0] = (pixel >> 16) & 0xff;
                            p[1] = (pixel >> 8) & 0xff;
                            p[2] = pixel & 0xff;
                        } else {
                            p[0] = pixel & 0xff;
                            p[1] = (pixel >> 8) & 0xff;
                            p[2] = (pixel >> 16) & 0xff;
                        }
                        break;

                    case 4:
                        *(Uint32 *)p = pixel;
                        break;
                }
                SDL_UnlockSurface(screen);
            }
        }
        else
        {
            #ifdef ES_USE_OPENGL
                glLoadIdentity();
                glBegin(GL_POINTS);
                    glColor3ub(r,g,b);
                    glVertex2i(x, y);
                glEnd();
            #endif
        }
    }

    void ES_Graphics::setPixel(SDL_Surface *surf,unsigned short int x, unsigned short int y, unsigned short int r, unsigned short int g, unsigned short int b, bool verif)
    {
        if(opengl_mode == false)
        {
            if(verif == true)
            {
                if(x > 0 && x < surf->w && y > 0 && y < surf->h)
                {
                    SDL_LockSurface(surf);
                    int bpp = surf->format->BytesPerPixel;
                    Uint32 pixel=SDL_MapRGB(surf->format, r,g,b);

                    Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

                    switch(bpp)
                    {
                        case 1:
                            *p = pixel;
                            break;

                        case 2:
                            *(Uint16 *)p = pixel;
                            break;

                        case 3:
                            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                                p[0] = (pixel >> 16) & 0xff;
                                p[1] = (pixel >> 8) & 0xff;
                                p[2] = pixel & 0xff;
                            } else {
                                p[0] = pixel & 0xff;
                                p[1] = (pixel >> 8) & 0xff;
                                p[2] = (pixel >> 16) & 0xff;
                            }
                            break;

                        case 4:
                            *(Uint32 *)p = pixel;
                            break;
                    }
                    SDL_UnlockSurface(surf);
                }
            }
            else
            {
                SDL_LockSurface(surf);
                int bpp = surf->format->BytesPerPixel;
                Uint32 pixel=SDL_MapRGB(surf->format, r,g,b);

                Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

                switch(bpp)
                {
                    case 1:
                        *p = pixel;
                        break;

                    case 2:
                        *(Uint16 *)p = pixel;
                        break;

                    case 3:
                        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                            p[0] = (pixel >> 16) & 0xff;
                            p[1] = (pixel >> 8) & 0xff;
                            p[2] = pixel & 0xff;
                        } else {
                            p[0] = pixel & 0xff;
                            p[1] = (pixel >> 8) & 0xff;
                            p[2] = (pixel >> 16) & 0xff;
                        }
                        break;

                    case 4:
                        *(Uint32 *)p = pixel;
                        break;
                }
                SDL_UnlockSurface(surf);
            }
        }
        else
        {
            #ifdef ES_USE_OPENGL
                glLoadIdentity();
                glBegin(GL_POINTS);
                    glColor3ub(r,g,b);
                    glVertex2i(x, y);
                glEnd();
            #endif
        }
    }

    void ES_Graphics::circle(int x, int y, int ry, unsigned short int r,  unsigned short int g, unsigned short int b, bool fill)
    {
        int d, ytmp, xtmp;

        d = 3 - (2 * ry);
        xtmp = 0;
        ytmp = ry;

        while(ytmp >= xtmp)
        {
            if(fill == false)
            {
                setPixel(x + xtmp, y + ytmp, r, g, b);
                setPixel(x + ytmp, y + xtmp, r, g, b);
                setPixel(x - xtmp, y + ytmp, r, g, b);
                setPixel(x - ytmp, y + xtmp, r, g, b);
                setPixel(x + xtmp, y - ytmp, r, g, b);
                setPixel(x + ytmp, y - xtmp, r, g, b);
                setPixel(x - xtmp, y - ytmp, r, g, b);
                setPixel(x - ytmp, y - xtmp, r, g, b);
            }
            else
            {
                hBar(x - xtmp, y - ytmp, 2 * xtmp + 1, r, g, b);
                hBar(x - xtmp, y + ytmp, 2 * xtmp + 1, r, g, b);
                hBar(x - ytmp, y - xtmp, 2 * ytmp + 1, r, g, b);
                hBar(x - ytmp, y + xtmp, 2 * ytmp + 1, r, g, b);
            }

            if (d < 0)
                d = d + (4 * xtmp) + 6;
            else
            {
                d = d + 4 * (xtmp - ytmp) + 10;
                ytmp--;
            }

            xtmp++;
        }
    }

    void ES_Graphics::circle(SDL_Surface *surf,int x, int y, int ry, unsigned short int r,  unsigned short int g, unsigned short int b, bool fill)
    {
        int d, ytmp, xtmp;

        d = 3 - (2 * ry);
        xtmp = 0;
        ytmp = ry;

        while(ytmp >= xtmp)
        {
            if(fill == false)
            {
                setPixel(surf,x + xtmp, y + ytmp, r, g, b);
                setPixel(surf,x + ytmp, y + xtmp, r, g, b);
                setPixel(surf,x - xtmp, y + ytmp, r, g, b);
                setPixel(surf,x - ytmp, y + xtmp, r, g, b);
                setPixel(surf,x + xtmp, y - ytmp, r, g, b);
                setPixel(surf,x + ytmp, y - xtmp, r, g, b);
                setPixel(surf,x - xtmp, y - ytmp, r, g, b);
                setPixel(surf,x - ytmp, y - xtmp, r, g, b);
            }
            else
            {
                hBar(surf, x - xtmp, y - ytmp, 2 * xtmp + 1, r, g, b);
                hBar(surf,x - xtmp, y + ytmp, 2 * xtmp + 1, r, g, b);
                hBar(surf,x - ytmp, y - xtmp, 2 * ytmp + 1, r, g, b);
                hBar(surf,x - ytmp, y + xtmp, 2 * ytmp + 1, r, g, b);
            }

            if (d < 0)
                d = d + (4 * xtmp) + 6;
            else
            {
                d = d + 4 * (xtmp - ytmp) + 10;
                ytmp--;
            }

            xtmp++;
        }
    }

    void ES_Graphics::hBar(int x, int y, int w, unsigned short int r,  unsigned short int g, unsigned short int b)
    {
        if(opengl_mode == true)
        {
            #ifdef ES_USE_OPENGL
                lineXY(x, y, x+w, y, r, g, b);
            #endif
        }
        else
        {
            SDL_Rect rect;

            rect.x = x;
            rect.y = y;
            rect.w = w;
            rect.h = 1;

            SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
        }
    }

    void ES_Graphics::hBar(SDL_Surface *surf, int x, int y, int w, unsigned short int r,  unsigned short int g, unsigned short int b)
    {
        if(opengl_mode == true)
        {
            #ifdef ES_USE_OPENGL
                lineXY(x, y, x+w, y, r, g, b);
            #endif
        }
        else
        {
            SDL_Rect rect;

            rect.x = x;
            rect.y = y;
            rect.w = w;
            rect.h = 1;

            SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, r, g, b));
        }
    }

    void ES_Graphics::vBar(int x, int y, int h, unsigned short int r,  unsigned short int g, unsigned short int b)
    {
        if(opengl_mode == true)
        {
            #ifdef ES_USE_OPENGL
                lineXY(x, y, x, y+h, r, g, b);
            #endif
        }
        else
        {
            SDL_Rect rect;

            rect.x = x;
            rect.y = y;
            rect.w = 1;
            rect.h = h;

            SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
        }
    }

    void ES_Graphics::vBar(SDL_Surface *surf, int x, int y, int h, unsigned short int r,  unsigned short int g, unsigned short int b)
    {
        if(opengl_mode == true)
        {
            #ifdef ES_USE_OPENGL
                lineXY(x, y, x, y+h, r, g, b);
            #endif
        }
        else
        {
            SDL_Rect rect;

            rect.x = x;
            rect.y = y;
            rect.w = 1;
            rect.h = h;

            SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, r, g, b));
        }
    }

    void ES_Graphics::drawBox(SDL_Rect rect, unsigned short int r,  unsigned short int g, unsigned short int b, bool fill)
    {
        if(opengl_mode == true)
        {
            #ifdef ES_USE_OPENGL
                if(fill == true)
                {
                    glLoadIdentity();
                    glBegin(GL_QUADS);
                        glColor3ub(r,g,b);
                        glVertex2i(rect.x,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y+rect.h);
                        glVertex2i(rect.x,rect.y+rect.h);
                    glEnd();
                }
                else
                {
                    glLoadIdentity();
                    glBegin(GL_LINE_LOOP);
                        glColor3ub(r,g,b);
                        glVertex2i(rect.x,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y+rect.h);
                        glVertex2i(rect.x,rect.y+rect.h);
                    glEnd();
                }
            #endif
        }
        else
        {
            if(fill == true)
                SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
            else
            {
                hBar(rect.x, rect.y, rect.w, r, g, b);
                hBar(rect.x, rect.y + rect.h - 1, rect.w, r, g, b);
                vBar(rect.x, rect.y + 1, rect.h - 2, r, g, b);
                vBar(rect.x + rect.w - 1, rect.y + 1, rect.h - 2, r, g, b);
            }
        }
    }

    void ES_Graphics::drawBox(SDL_Surface *surf,SDL_Rect rect, unsigned short int r,  unsigned short int g, unsigned short int b, bool fill)
    {
        if(opengl_mode == true)
        {
            #ifdef ES_USE_OPENGL
                if(fill == true)
                {
                    glLoadIdentity();
                    glBegin(GL_QUADS);
                        glColor3ub(r,g,b);
                        glVertex2i(rect.x,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y+rect.h);
                        glVertex2i(rect.x,rect.y+rect.h);
                    glEnd();
                }
                else
                {
                    glLoadIdentity();
                    glBegin(GL_LINE_LOOP);
                        glColor3ub(r,g,b);
                        glVertex2i(rect.x,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y);
                        glVertex2i(rect.x+rect.w,rect.y+rect.h);
                        glVertex2i(rect.x,rect.y+rect.h);
                    glEnd();
                }
            #endif
        }
        else
        {
            if(fill == true)
                SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, r, g, b));
            else
            {
                hBar(surf, rect.x, rect.y, rect.w, r, g, b);
                hBar(surf, rect.x, rect.y + rect.h - 1, rect.w, r, g, b);
                vBar(surf, rect.x, rect.y + 1, rect.h - 2, r, g, b);
                vBar(surf, rect.x + rect.w - 1, rect.y + 1, rect.h - 2, r, g, b);
            }
        }
    }

    void ES_Graphics::lineXY(int x, int y, int x2, int y2, unsigned short int r, unsigned short int g, unsigned short int b)
    {
        if(opengl_mode == true)
        {
            #ifdef ES_USE_OPENGL
                glLoadIdentity();
                glBegin(GL_LINES);
                    glColor3ub(r,g,b);
                    glVertex2i(x, y);
                    glVertex2i(x2, y2);
                glEnd();
            #endif
        }
        else // bresenham :<
        {
            int d, dx, dy, aincr, bincr, xincr, yincr, xtmp, ytmp, tmp;

            if(abs(x2 - x) < abs(y2 - y))
            {
                if(y > y2)
                {
                    tmp = x2;
                    x2 = x; x = tmp;
                    tmp = y2;
                    y2 = y; y = tmp;
                }

                xincr = x2 > x ? 1 : -1;
                dy = y2 - y;
                dx = abs(x2 - x);
                d = 2 * dx - dy;
                aincr = 2 * (dx - dy);
                bincr = 2 * dx;
                xtmp = x;
                ytmp = y;

                setPixel(xtmp, ytmp, r, g, b);

                for (ytmp = y+1; ytmp <= y2; ++ytmp)
                {
                    if (d >= 0)
                    {
                        xtmp += xincr;
                        d += aincr;
                    }
                    else
                        d += bincr;
                    setPixel(xtmp, ytmp, r, g, b);
                }
            }
            else
            {
                if (x > x2)
                {
                    tmp = x2;
                    x2 = x; x = tmp;
                    tmp = y2;
                    y2 = y; y = tmp;
                }

                yincr = y2 > y ? 1 : -1;
                dx = x2 - x;
                dy = abs(y2 - y);
                d = 2 * dy - dx;
                aincr = 2 * (dy - dx);
                bincr = 2 * dy;
                xtmp = x;
                ytmp = y;

                setPixel(xtmp, ytmp, r, g, b);

                for (xtmp = x+1; xtmp <= x2; ++xtmp)
                {
                    if (d >= 0)
                    {
                        ytmp += yincr;
                        d += aincr;
                    }
                    else
                        d += bincr;
                    setPixel(xtmp, ytmp, r, g, b);
                }
            }
        }
    }

    void ES_Graphics::setIcon(const std::string& filename)
    {
        SDL_WM_SetIcon(IMG_Load(filename.c_str()), NULL);
    }

    void ES_Graphics::setUnicode(unsigned short int unicode)
    {
        SDL_EnableUNICODE(unicode);
    }

    bool ES_Graphics::run()
    {
        return loop;
    }

    void ES_Graphics::quit()
    {
        loop = false;
    }

    void ES_Graphics::flip(unsigned short int delay)
    {
        es_timer = SDL_GetTicks(); // timer (temp courant)

        #ifdef ES_USE_OPENGL
        if(opengl_mode == true)
            SDL_GL_SwapBuffers();
        else
        #endif
            SDL_Flip(screen);

        if(delay != 0)
            SDL_Delay(delay);
    }

    void ES_Graphics::cls(unsigned int r, unsigned int g, unsigned int b)
    {
        if(fade == true) // si un fade à déjà étaît joué
        {
            r = r_screen; // on passe aux valeurs du fade
            g = v_screen;
            b = b_screen;
        }

        #ifdef ES_USE_OPENGL
        if(opengl_mode == true)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glLoadIdentity();
            glClearColor((GLclampf)r/255, (GLclampf)g/255, (GLclampf)b/255, 0);
        }
        else
        #endif
            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, r, g, b));
    }

    unsigned short int ES_Graphics::screenFade()
    {
        fade = true;

        if(r_screen < r_max && v_screen < r_max && b_screen < r_max && fade_in == true)
        {
            r_screen++*getLastFrame();
            v_screen++*getLastFrame();
            this->b_screen++*getLastFrame();

            if(r_screen == r_max && v_screen == r_max & b_screen == r_max)
            {
                fade_in = false;
                fade_out = true;

                return 1; // fade in finis on retourne 1
            }
        }
        else if(r_screen > r_min && v_screen > r_min && b_screen > r_min && fade_out == true)
        {
            this->r_screen--*getLastFrame();
            this->v_screen--*getLastFrame();
            this->b_screen--*getLastFrame();

            if(r_screen == r_min && v_screen == r_min && b_screen == r_min)
            {
                fade_in = true;
                fade_out = false;

                return 2; // fade out finis on retourne 2
            }
        }

        return 0;
    }

    void ES_Graphics::switchWindowMode(unsigned int w, unsigned int h, unsigned int bpp)
    {
        if(w == 0 && h == 0 && bpp == 0)
        {
            w = getWindowWidth();
            h = getWindowHeight();
            bpp = actual_bpp;
        }

        if(fullscreen == false)
        {
            #ifdef ES_USE_OPENGL
            if(opengl_mode == true)
            {
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                if(orto_view == true)
                    setOrthoView(false, w, h);
                screen = SDL_SetVideoMode(w, h, bpp, SDL_OPENGL | SDL_FULLSCREEN);
            }
            else
            #endif
                screen = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
            fullscreen = true;
            actual_bpp = bpp;
        }
        else
        {
            #ifdef ES_USE_OPENGL
            if(opengl_mode == true)
            {
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                if(orto_view == true)
                    setOrthoView(false, w, h);
                screen = SDL_SetVideoMode(w, h, bpp, SDL_OPENGL);
            }
            else
            #endif
                screen = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
            fullscreen = false;
            actual_bpp = bpp;
        }
    }

    unsigned long ES_Graphics::getTime()
    {
        return SDL_GetTicks();
    }

    unsigned long ES_Graphics::getLastFrame(unsigned long dv)
    {
        return (SDL_GetTicks()-es_timer)/dv;
    }

    unsigned short int ES_Graphics::getWindowWidth()
    {
        return screen->w;
    }

    unsigned short int ES_Graphics::getWindowHeight()
    {
        return screen->h;
    }

    unsigned short int ES_Graphics::getWindowBPP()
    {
        return actual_bpp;
    }

    SDL_Surface *ES_Graphics::getScreen()
    {
        return screen;
    }

    unsigned short int ES_Graphics::getFPS(unsigned int delay)
    {
        frames++;

        if((SDL_GetTicks() - lasttime) >= delay)
        {
            fps = frames;

            frames = 0;
            lasttime = SDL_GetTicks();
        }

        return fps;
    }
}
