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


#include "ES_Display/ttf.h"

namespace ES
{
    unsigned short int sfont_size = 16; // taille de la font par défaut

    SDL_Color es_col = {0, 0, 0, 0}; // structure en paramétre par défaut, pas trouver mieux.

    ES_Ttf::ES_Ttf()
    {
            if(TTF_Init() == -1)
                std::cerr << "_TTF_Init: " << TTF_GetError() << std::endl;
    }

    ES_Ttf::~ES_Ttf()
    {
        TTF_Quit();
    }

    void ES_Ttf::drawText(const std::string& resource_name, const std::string& text, short int x, short int y, SDL_Color color, unsigned short int justification, unsigned short int style, SDL_Color color_bg)
    {
        #ifdef ES_USE_OPENGL
        if(ES_OpenGL_Mode == false)
        {
        #endif
            if(resources.getTTF(resource_name) != 0)
            {
                TTF_SizeText(resources.getTTF(resource_name), text.c_str(), &font_width, &font_height);

                switch(justification)
                {
                    case ES_TTF_CENTER:
                        horizontal_justification = font_width/2;
                    break;

                    case ES_TTF_LEFT:
                        horizontal_justification = 0;
                    break;

                    case ES_TTF_RIGHT:
                        horizontal_justification = font_width;
                    break;

                    default:
                    break;
                }

                pos.x = x-horizontal_justification;
                pos.y = y-font_height/3;

                switch(style)
                {
                    case ES_SOLID: // rapide
                        txt = TTF_RenderText_Solid(resources.getTTF(resource_name), text.c_str(), color);
                    break;

                    case ES_SHADED: // moyenement rapide, arriére plan non transparent (rendu lisse)
                        txt = TTF_RenderText_Shaded(resources.getTTF(resource_name), text.c_str(), color, color_bg);
                    break;

                    case ES_BLENDED: // assez lent mais meilleur qualité (rendu lisse + colorkey)
                        txt = TTF_RenderText_Blended(resources.getTTF(resource_name), text.c_str(), color);
                    break;

                    default:
                    break;
                }

                SDL_BlitSurface(txt, NULL, SDL_GetVideoSurface(), &pos);
                SDL_FreeSurface(txt);
            }

        #ifdef ES_USE_OPENGL
        }
        else
        {
            if(resources.getTTF(resource_name) != 0)
            {
                TTF_SizeText(resources.getTTF(resource_name), text.c_str(), &font_width, &font_height);

                switch(justification)
                {
                    case ES_TTF_CENTER:
                        horizontal_justification = font_width/2;
                    break;

                    case ES_TTF_LEFT:
                        horizontal_justification = 0;
                    break;

                    case ES_TTF_RIGHT:
                        horizontal_justification = font_width;
                    break;

                    default:
                    break;
                }

                x = x-horizontal_justification;
                y = y-font_height/3;

                initial = TTF_RenderText_Blended(resources.getTTF(resource_name), text.c_str(), color);

                w = powerOfTwo(initial->w);
                h = powerOfTwo(initial->h);

                intermediary = SDL_CreateRGBSurface(0, w, h, 32,
                        0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

                SDL_BlitSurface(initial, 0, intermediary, 0);

                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA,
                        GL_UNSIGNED_BYTE, intermediary->pixels );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                //glRotatef(180., 1., 0., 0.);
                glLoadIdentity ();
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture);
                glBlendFunc(GL_ONE, GL_ONE);
                glEnable(GL_BLEND);
                //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor3f(1.0f, 1.0f, 1.0f);

                glTranslatef(x,y+h,0);
                glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

                glBegin(GL_QUADS);

                    glTexCoord2f(0.0f, 0.0f);
                    glVertex2f(0, 0 + h);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex2f(0, 0);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex2f(0 + w, 0);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex2f(0 + w, 0 + h);
/*
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex2f(x, y);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex2f(x + w, y);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex2f(x + w, y + h);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex2f(x, y + h);
*/
                glEnd();

                glDisable(GL_BLEND);

                glFinish();

                SDL_FreeSurface(initial);
                SDL_FreeSurface(intermediary);
                glDeleteTextures(1, &texture);

            }
        }
        #endif
    }

    void ES_Ttf::setSize(unsigned short int font_size)
    {
        sfont_size = font_size;
    }
}
