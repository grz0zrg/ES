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

#include "ES_Resources/resources.h"
#include "ES_Sound/sound.h"
#include "ES_Display/ttf.h"

namespace ES
{
    bool opengl_mode = false;

    #ifdef ES_USE_OPENGL
    std::map<std::string, GLuint> texture; // textures id opengl conteneur
    std::map<std::string, int> text_width; // largeur texture conteneur (à virer plus tard)
    std::map<std::string, int> text_height; // hauteur texture conteneur (à virer plus tard)
    std::map<std::string, int>::iterator it_text_infos; // iterateur associé
    std::map<std::string, GLuint>::iterator it_texture; // iterateur associé
    #endif
    std::map<std::string, SDL_Surface*> img; // images surfaces sdl
    std::map<std::string, SDL_Surface*>::iterator it_img;
    std::map<std::string, SDL_RWops*> img_wop; // surface mem
    std::map<std::string, SDL_RWops*>::iterator it_img_wop;
    std::map<std::string, Mix_Music*> music; // musique
    std::map<std::string, Mix_Music*>::iterator it_music;
    std::map<std::string, SDL_RWops*> music_wop; // musique mem
    std::map<std::string, SDL_RWops*>::iterator it_music_wop;
    std::map<std::string, Mix_Chunk*> sample; // samples
    std::map<std::string, Mix_Chunk*>::iterator it_sample;
    std::map<std::string, SDL_RWops*> sample_wop; // samples mem
    std::map<std::string, SDL_RWops*>::iterator it_sample_wop;
    std::map<std::string, TTF_Font*> sfont; // sdl_ttf font
    std::map<std::string, TTF_Font*>::iterator it_sfont;
    std::vector<std::string> err_queue;

    int tw, th;

    void ES_Resources::add(const std::string& resource_name, const std::string& resource_filename, unsigned short int resource_type, bool esd, ES_Pack *pack, bool colorkey, unsigned short int r, unsigned short int g, unsigned short int b)
    {
        switch(resource_type)
        {
            case ES_IMG: // image ?
                #ifdef ES_USE_OPENGL
                if(opengl_mode == true) // texture opengl
                {
                    if(esd == true && pack != NULL)
                    {
                        if(pack->getFile(resource_filename) != NULL)
                        {
                            if(pack->getSize(resource_filename) > 0)
                                img_wop[resource_name] = SDL_RWFromMem(pack->getFile(resource_filename), pack->getSize(resource_filename));
                            else
                                std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << "invalid data size." << std::endl;
                            if(img_wop[resource_name] == NULL)
                            {
                                img_wop.erase(resource_name);
                                std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                            }
                            else
                            {
                                image_temp = IMG_Load_RW(img_wop[resource_name], 0);
                                if(image_temp != NULL)
                                {
                                    id_img = load_texture(resource_filename, colorkey, r, g, b, true, pack);
                                    if(id_img != 0) // si aucunes erreurs on ajoute l'id à notre conteneur
                                    {
                                        texture[resource_name] = id_img;
                                        // et les infos de la texture :O (à virer plus tard)
                                        text_width[resource_name] = tw;
                                        text_height[resource_name] = th;
                                    }
                                }
                                else
                                {
                                    std::cerr << "_Can't load texture: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                                    img_wop.erase(resource_name);
                                }

                            }
                        }
                        else
                        {
                            std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack::getFile() problem." << std::endl;
                        }
                    }
                    else if(esd == true && pack == NULL)
                    {
                        std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack class is not initialized." << std::endl;
                    }
                    else if(esd == false && pack == NULL)
                    {
                        id_img = load_texture(resource_filename, colorkey, r, g, b);
                        if(id_img != 0) // si aucunes erreurs on ajoute l'id à notre conteneur
                        {
                            texture[resource_name] = id_img;
                            // et les infos de la texture :O (à virer plus tard)
                            text_width[resource_name] = tw;
                            text_height[resource_name] = th;
                        }
                    }
                }
                else // surface sdl
                {
                #endif
                    if(esd == true && pack != NULL)
                    {
                        if(pack->getFile(resource_filename) != NULL)
                        {
                            if(pack->getSize(resource_filename) > 0)
                                img_wop[resource_name] = SDL_RWFromMem(pack->getFile(resource_filename), pack->getSize(resource_filename));
                            else
                                std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << "invalid data size." << std::endl;
                            if(img_wop[resource_name] == NULL)
                            {
                                img_wop.erase(resource_name);
                                std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                            }
                            else
                            {
                                image_temp = IMG_Load_RW(img_wop[resource_name], 0);
                                if(image_temp == 0)
                                {
                                    std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                                    img_wop.erase(resource_name);
                                    SDL_FreeSurface(image_temp);
                                }
                                else
                                {
                                    img[resource_name] = SDL_DisplayFormat(image_temp);
                                    SDL_FreeSurface(image_temp);
                                    if(img[resource_name] == NULL)
                                    {
                                        std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                                        SDL_FreeSurface(image_temp);
                                        img.erase(resource_name);
                                    }
                                    else
                                    {
                                        if(colorkey == true)
                                            SDL_SetColorKey(img[resource_name], SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(img[resource_name]->format, r, g, b));
                                    }
                                }
                            }
                        }
                        else
                        {
                            std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack::getFile() problem." << std::endl;
                        }
                    }
                    else if(esd == true && pack == NULL)
                    {
                        std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack class is not initialized." << std::endl;
                    }
                    else if(esd == false)
                    {
                        image_temp = IMG_Load(resource_filename.c_str());
                        if(image_temp == 0)
                            std::cerr << "_Can't load img: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                        else
                        {
                            imageo_temp = SDL_DisplayFormat(image_temp); // convertit la surface au format de l'écran
                            SDL_FreeSurface(image_temp);
                            if(imageo_temp!=NULL)
                            {
                                img[resource_name] = SDL_DisplayFormat(imageo_temp);
                                SDL_FreeSurface(imageo_temp);
                                if(colorkey == true)
                                    SDL_SetColorKey(img[resource_name], SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(img[resource_name]->format, r, g, b));
                            }
                            else
                                std::cerr << "_Can't convert img surface: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                        }
                    }
                #ifdef ES_USE_OPENGL
                }
                #endif
            break;

            case ES_MUSIC: // musiques ?
#ifndef GP2X
                if(esd == true && pack != NULL)
                {
                    if(pack->getFile(resource_filename) != NULL)
                    {
                        if(pack->getSize(resource_filename) > 0)
                            music_wop[resource_name] = SDL_RWFromMem(pack->getFile(resource_filename), pack->getSize(resource_filename));
                        else
                            std::cerr << "_Can't load music: \"" << resource_filename.c_str() << "\" -> " << "invalid data size." << std::endl;
                        if(music_wop[resource_name] == NULL)
                        {
                            music_wop.erase(resource_name);
                            std::cerr << "_Can't load music: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                        }
                        else
                        {
                            music[resource_name] = Mix_LoadMUS_RW(music_wop[resource_name]);
                            if(music[resource_name] == 0)
                            {
                                std::cerr << "_Can't load music: \"" << resource_filename.c_str() << "\" -> " << Mix_GetError() << std::endl;
                                music_wop.erase(resource_name);
                                music.erase(resource_name);
                            }
                        }
                    }
                    else
                    {
                        std::cerr << "_Can't load music: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack::getFile() problem." << std::endl;
                    }
                }
                else if(esd == true && pack == NULL)
#else
    if(esd == true && pack == NULL)
#endif
                {
                        std::cerr << "_Can't load music: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack class is not initialized." << std::endl;
                }
                else if(esd == false)
                {
                    music[resource_name] = Mix_LoadMUS(resource_filename.c_str());
                    if(music[resource_name] == 0)
                    {
                        music.erase(resource_name);
                        std::cerr << "_Mix_LoadMUS: " << Mix_GetError() << std::endl;
                    }
                }
            break;

            case ES_SAMPLE: // effets sonores (samples) ?
                if(esd == true && pack != NULL)
                {
                    if(pack->getFile(resource_filename) != NULL)
                    {
                        if(pack->getSize(resource_filename) > 0)
                            sample_wop[resource_name] = SDL_RWFromMem(pack->getFile(resource_filename), pack->getSize(resource_filename));
                        else
                            std::cerr << "_Can't load sample: \"" << resource_filename.c_str() << "\" -> " << "invalid data size." << std::endl;
                        if(sample_wop[resource_name] == NULL)
                        {
                            sample_wop.erase(resource_name);
                            std::cerr << "_Can't load sample: \"" << resource_filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
                        }
                        else
                        {
                            sample[resource_name] = Mix_LoadWAV_RW(sample_wop[resource_name], 0);
                            if(sample[resource_name] == 0)
                            {
                                std::cerr << "_Can't load sample: \"" << resource_filename.c_str() << "\" -> " << Mix_GetError() << std::endl;
                                sample_wop.erase(resource_name);
                                sample.erase(resource_name);
                            }
                        }
                    }
                    else
                    {
                        std::cerr << "_Can't load sample: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack::getFile() problem." << std::endl;
                    }
                }
                else if(esd == true && pack == NULL)
                {
                        std::cerr << "_Can't load sample: \"" << resource_filename.c_str() << "\" -> " << "ES_Pack class is not initialized." << std::endl;
                }
                else if(esd == false)
                {
                    sample[resource_name] = Mix_LoadWAV_RW(SDL_RWFromFile(resource_filename.c_str(), "rb"), 1);
                    if(sample[resource_name] == 0)
                    {
                        sample.erase(resource_name);
                        std::cerr << "_Mix_LoadWAV_RW: " << Mix_GetError() << std::endl;
                    }
                }
            break;

            case ES_TTF_FONT: // font ?
                sfont[resource_name] = TTF_OpenFont(resource_filename.c_str(), sfont_size);
                if(sfont[resource_name] == 0)
                {
                    sfont.erase(resource_name);
                    std::cerr << "_TTF_OpenFont: " << TTF_GetError() << std::endl;
                }
            break;

            default:
            break;
        }
    }

    bool ES_Resources::isLoaded(const std::string& resource_name, unsigned short int resource_type, ES_Pack *pack)
    {
        switch(resource_type)
        {
            case ES_IMG:
                #ifdef ES_USE_OPENGL
                if(opengl_mode == false)
                {
                #endif
                    it_img = img.find(resource_name); // cherche si la ressource existe
                    if(pack != NULL)
                    {
                        it_img_wop = img_wop.find(resource_name);
                        if(it_img_wop != img_wop.end())
                            return true;
                    }
                    if(it_img != img.end())
                        return true;
                    return false;
                #ifdef ES_USE_OPENGL
                }
                else
                {
                    it_texture = texture.find(resource_name);
                    if(pack != NULL)
                    {
                        it_img_wop = img_wop.find(resource_name);
                        if(it_img_wop != img_wop.end())
                            return true;
                    }
                    if(it_texture != texture.end())
                        return true;
                    return false;
                }
                #endif
            break;

            case ES_MUSIC:
                it_music = music.find(resource_name);
                if(pack != NULL)
                {
                    it_music_wop = music_wop.find(resource_name);
                    if(it_music_wop != music_wop.end())
                        return true;
                }
                if(it_music != music.end())
                    return true;
                return false;
            break;

            case ES_SAMPLE:
                it_sample = sample.find(resource_name);
                if(pack != NULL)
                {
                    it_sample_wop = sample_wop.find(resource_name);
                    if(it_sample_wop != sample_wop.end())
                        return true;
                }
                if(it_sample != sample.end())
                    return true;
                return false;
            break;

            case ES_TTF_FONT:
                it_sfont = sfont.find(resource_name);
                if(it_sfont != sfont.end())
                    return true;
                return false;
            break;

            default:
                return false;
            break;
        }
    }

    void ES_Resources::del(const std::string& resource_name, unsigned short int resource_type, ES_Pack *pack, const std::string& resource_filename)
    {
        switch(resource_type)
        {
            case ES_IMG:
                #ifdef ES_USE_OPENGL
                if(opengl_mode == false)
                {
                #endif
                    it_img = img.find(resource_name); // cherche si la ressource existe
                    if(pack != NULL || resource_filename != "")
                    {
                        it_img_wop = img_wop.find(resource_name);
                        if(it_img_wop != img_wop.end())
                        {
                            SDL_FreeRW(img_wop[resource_name]); // libére le rwop manuellement
                            img_wop.erase(resource_name);
                            pack->free(resource_filename); // libére la mémoire alloué pour le fichier
                        }
                    }
                    if(it_img != img.end())
                    {
                        SDL_FreeSurface(img[resource_name]);
                        img.erase(resource_name); // efface l'entrée
                    }
                #ifdef ES_USE_OPENGL
                }
                else
                {
                    it_texture = texture.find(resource_name);
                    if(pack != NULL || resource_filename != "")
                    {
                        it_img_wop = img_wop.find(resource_name);
                        if(it_img_wop != img_wop.end())
                        {
                            SDL_FreeRW(img_wop[resource_name]);
                            img_wop.erase(resource_name);
                            pack->free(resource_filename); // libére la mémoire alloué pour le fichier
                        }
                    }
                    if(it_texture != texture.end())
                    {
                        glDeleteTextures(1, &texture[resource_name]);
                        texture.erase(resource_name);
                    }
                }
                #endif
            break;

            case ES_MUSIC:
                it_music = music.find(resource_name);
                if(pack != NULL || resource_filename != "")
                {
                    it_music_wop = music_wop.find(resource_name);
                    if(it_music_wop != music_wop.end())
                    {
                        SDL_FreeRW(music_wop[resource_name]);
                        music_wop.erase(resource_name);
                        pack->free(resource_filename);
                    }
                }
                if(it_music != music.end())
                {
                    Mix_FreeMusic(music[resource_name]);
                    music.erase(resource_name);
                }
            break;

            case ES_SAMPLE:
                it_sample = sample.find(resource_name);
                if(pack != NULL || resource_filename != "")
                {
                    it_sample_wop = sample_wop.find(resource_name);
                    if(it_sample_wop != sample_wop.end())
                    {
                        SDL_FreeRW(sample_wop[resource_name]);
                        sample_wop.erase(resource_name);
                        pack->free(resource_filename);
                    }
                }
                if(it_sample != sample.end())
                {
                    Mix_FreeChunk(sample[resource_name]);
                    sample.erase(resource_name);
                }
            break;

            case ES_TTF_FONT:
                it_sfont = sfont.find(resource_name);
                if(it_sfont != sfont.end())
                {
                    TTF_CloseFont(sfont[resource_name]);
                    sfont.erase(resource_name);
                }
            break;

            default:
            break;
        }
    }

    void ES_Resources::free()
    {
        // permet de parcourir les conteneur et pour chaque valeur stocker on libére la ressource
        for (std::map<std::string, SDL_Surface*>::const_iterator iter = img.begin(); iter != img.end(); ++iter)
            SDL_FreeSurface(iter->second);

        for (std::map<std::string, SDL_RWops*>::const_iterator iter = img_wop.begin(); iter != img_wop.end(); ++iter)
            SDL_FreeRW(iter->second);

        for (std::map<std::string, Mix_Music*>::const_iterator iter = music.begin(); iter != music.end(); ++iter)
            Mix_FreeMusic(iter->second);

        for (std::map<std::string, SDL_RWops*>::const_iterator iter = music_wop.begin(); iter != music_wop.end(); ++iter)
            SDL_FreeRW(iter->second);

        for (std::map<std::string, Mix_Chunk*>::const_iterator iter = sample.begin(); iter != sample.end(); ++iter)
            Mix_FreeChunk(iter->second);

        for (std::map<std::string, SDL_RWops*>::const_iterator iter = sample_wop.begin(); iter != sample_wop.end(); ++iter)
            SDL_FreeRW(iter->second);

        for (std::map<std::string, TTF_Font*>::const_iterator iter = sfont.begin(); iter != sfont.end(); ++iter)
            TTF_CloseFont(iter->second);

        #ifdef ES_USE_OPENGL
        if(opengl_mode == true)
        {
            for (std::map<std::string, GLuint>::const_iterator iter = texture.begin(); iter != texture.end(); ++iter)
                glDeleteTextures(1, &iter->second);
        }
        #endif

        // enfin on nettoie nos conteneurs
        sfont.clear();
        img.clear();
        img_wop.clear();
        #ifdef ES_USE_OPENGL
        text_width.clear();
        text_height.clear();
        texture.clear();
        #endif
        music.clear();
        music_wop.clear();
        sample.clear();
        sample_wop.clear();
    }

    void ES_Resources::set_opengl_mode()
    {
        opengl_mode = true;
    }

    SDL_Surface *ES_Resources::getSDL_IMG(const std::string& resource_name)
    {
        it_img = img.find(resource_name); // cherche si la ressource existe
        if (it_img == img.end())
        {
            bool ress_exist = false;
            for(unsigned int i = 0; i < err_queue.size(); i++) // vérifie que la ressource n'existe pas dans l'error queue
            {
                if(resource_name == err_queue[i])
                {
                    ress_exist = true;
                    break;
                }
            }
            if(ress_exist == false)
            {
                err_queue.push_back(resource_name);
                std::cerr << "_Unknown resource SDL_IMG: \"" << resource_name << "\"" << std::endl;
            }
            return 0;
        }
        else // si elle existe on retourne la ressource
            return img[resource_name];
    }

    #ifdef ES_USE_OPENGL
    GLuint ES_Resources::getOpenGL_IMG(const std::string& resource_name)
    {
        it_texture = texture.find(resource_name);
        if (it_texture == texture.end())
        {
            bool ress_exist = false;
            for(unsigned int i = 0; i < err_queue.size(); i++) // vérifie que la ressource n'existe pas dans l'error queue
            {
                if(resource_name == err_queue[i])
                {
                    ress_exist = true;
                    break;
                }
            }
            if(ress_exist == false)
            {
                err_queue.push_back(resource_name);
                std::cerr << "_Unknown resource OPENGL_IMG: \"" << resource_name << "\"" << std::endl;
            }
            return 0;
        }
        else
            return texture[resource_name];
    }

    int ES_Resources::getTextWidth(const std::string& resource_name)
    {
        it_text_infos = text_width.find(resource_name);
        if (it_text_infos == text_width.end())
            return 0;
        else
            return text_width[resource_name];
    }

    int ES_Resources::getTextHeight(const std::string& resource_name)
    {
        it_text_infos = text_height.find(resource_name);
        if (it_text_infos == text_height.end())
            return 0;
        else
            return text_height[resource_name];
    }
    #endif

    Mix_Music *ES_Resources::getMusic(const std::string& resource_name)
    {
        it_music = music.find(resource_name);
        if (it_music == music.end())
        {
            bool ress_exist = false;
            for(unsigned int i = 0; i < err_queue.size(); i++) // vérifie que la ressource n'existe pas dans l'error queue
            {
                if(resource_name == err_queue[i])
                {
                    ress_exist = true;
                    break;
                }
            }
            if(ress_exist == false)
            {
                err_queue.push_back(resource_name);
                std::cerr << "_Unknown resource MUSIC: \"" << resource_name << "\"" << std::endl;
            }
            return 0;
        }
        else
            return music[resource_name];
    }

    Mix_Chunk *ES_Resources::getSample(const std::string& resource_name)
    {
        it_sample = sample.find(resource_name);
        if (it_sample == sample.end())
        {
            bool ress_exist = false;
            for(unsigned int i = 0; i < err_queue.size(); i++) // vérifie que la ressource n'existe pas dans l'error queue
            {
                if(resource_name == err_queue[i])
                {
                    ress_exist = true;
                    break;
                }
            }
            if(ress_exist == false)
            {
                err_queue.push_back(resource_name);
                std::cerr << "_Unknown resource SAMPLE: \"" << resource_name << "\"" << std::endl;
            }
            return 0;
        }
        else
            return sample[resource_name];
    }

    TTF_Font *ES_Resources::getTTF(const std::string& resource_name)
    {
        it_sfont = sfont.find(resource_name);
        if (it_sfont == sfont.end())
        {
            bool ress_exist = false;
            for(unsigned int i = 0; i < err_queue.size(); i++) // vérifie que la ressource n'existe pas dans l'error queue
            {
                if(resource_name == err_queue[i])
                {
                    ress_exist = true;
                    break;
                }
            }
            if(ress_exist == false)
            {
                err_queue.push_back(resource_name);
                std::cerr << "_Unknown resource STTF_FONT: \"" << resource_name << "\"" << std::endl;
            }
            return 0;
        }
        else
            return sfont[resource_name];
    }

    #ifdef ES_USE_OPENGL
    GLuint ES_Resources::load_texture(const std::string& filename, bool colorkey, unsigned short int r, unsigned short int g, unsigned short int b, bool esd, ES_Pack *pack)
    {
        if(esd == false && pack == NULL)
            image_temp = IMG_Load(filename.c_str());

        if(image_temp == 0)
        {
            std::cerr << "_Can't load texture: \"" << filename.c_str() << "\" -> " << SDL_GetError() << std::endl;
            if(esd == true && pack != NULL)
                img_wop.erase(filename);
            return 0;
        }
        else
        {
            // éleve à la puissance 2 (accélére le rendu sur les vieilles cartes et permet de régler des problèmes)
            w = powerOfTwo(image_temp->w);
            h = powerOfTwo(image_temp->h);

            tw = w;
            th = h;

            // si le fichier est un bitmap on inverse le format (RGB->BGR)/(RGBA->BGRA)
            if(filename.substr(filename.length() - 4) == ".bmp" && colorkey == true)
                pixels_format = GL_BGRA;
            else if(filename.substr(filename.length() - 4) == ".bmp" && colorkey == false)
                pixels_format = GL_BGR;
            else if(filename.substr(filename.length() - 4) != ".bmp" && colorkey == true)
                pixels_format = GL_RGBA;
            else
                pixels_format = GL_RGB;

            if(colorkey == true)
            {
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

                // temp w/h à donner à opengl si problème plus tard
                surface_temp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
                if(!surface_temp)
                {
                    std::cerr << "_Can't create surface: " << SDL_GetError() << std::endl;
                    return 0;
                }
                else
                {
                    // Equivalent colorkey SDL pour OpenGL
                    pixels = (unsigned char*) surface_temp->pixels;
                    pixels_temp = (unsigned char*) image_temp->pixels;

                    p_cnt = 0;
                    cnt = 0;

                    // Reconstruction de l'image avec la colorkey
                    for(i=0;i<image_temp->w;i++)
                    {
                        for(j=0;j<image_temp->h;j++)
                        {
                            pixels[p_cnt] = pixels_temp[cnt];
                            pixels[p_cnt+1] = pixels_temp[cnt+1];
                            pixels[p_cnt+2] = pixels_temp[cnt+2];

                            if((pixels_temp[cnt]==r) && (pixels_temp[cnt+1]==g) && (pixels_temp[cnt+2]==b))
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
                    glGenTextures(1, &id_img);
                    // 0 = valeur de retour si erreur donc on évite qu'il génére la valeur de retour
                    while(id_img == 0)
                        glGenTextures(1, &id_img);

                    glBindTexture(GL_TEXTURE_2D, id_img);

                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                    glTexImage2D(GL_TEXTURE_2D, 0, surface_temp->format->BytesPerPixel, w, h, 0, pixels_format, GL_UNSIGNED_BYTE, surface_temp->pixels);

                    SDL_FreeSurface(surface_temp);

                //	return id_img;
                }
            }
            else // charge la texture normalement sans colorkey
            {
                glGenTextures (1, &id_img);

                while(id_img == 0)
                    glGenTextures (1, &id_img);

                glBindTexture (GL_TEXTURE_2D, id_img);

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                glTexImage2D(GL_TEXTURE_2D, 0, image_temp->format->BytesPerPixel, w, h, 0, pixels_format, GL_UNSIGNED_BYTE, image_temp->pixels);
            }

            SDL_FreeSurface(image_temp);

            return id_img;
        }
    }
    #endif
}
