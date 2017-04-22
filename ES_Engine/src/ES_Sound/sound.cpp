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

#include "ES_Sound/sound.h"

namespace ES
{
    bool f_finished = false; // je kiff les trucs static...

    ES_Sound::ES_Sound(int frequency, Uint16 format, int channels, int chunksize)
    {
        if(Mix_OpenAudio(frequency, format, channels, chunksize) == -1)
        {
            std::cerr << "_Mix_OpenAudio: " << Mix_GetError() << std::endl;
            SDL_Quit(); // quitte la sdl proprement
            exit(EXIT_FAILURE);
        }
    }

    ES_Sound::~ES_Sound()
    {
        Mix_CloseAudio();
    }

    void ES_Sound::ess_bla()
    {
        ES_Sound::setFadeFinished();
    }

    void ES_Sound::playMusic(const std::string& resource_name, short int loop)
    {
        if(loop == -1 || loop == 0)
        {
            if(Mix_PlayMusic(resources.getMusic(resource_name), loop) == -1)
                std::cerr << "_Mix_Play_Music: " << Mix_GetError() << std::endl;
        }
    }

    void ES_Sound::switchMusicMode()
    {
        if(isPlayed() == true && isPaused() == true) // si une musique est en train d'être joué et est en pause
            Mix_ResumeMusic();
        else if(isPlayed() == true && isPaused() == false)
            Mix_PauseMusic();
    }

    void ES_Sound::playSample(const std::string& resource_name, short int loop, short int channel)
    {
        if(loop == -1 || loop == 0)
        {
            if(Mix_PlayChannel(channel, resources.getSample(resource_name), loop) == -1)
                std::cerr << "_Mix_PlayChannel: " << Mix_GetError() << std::endl;
        }
    }

    unsigned short int ES_Sound::sampleVol(const std::string& resource_name, short int volume)
    {
        return Mix_VolumeChunk(resources.getSample(resource_name), volume);
    }

    unsigned short int ES_Sound::channelVol(short int channel, short int volume)
    {
        return Mix_Volume(channel, volume);
    }

    unsigned short int ES_Sound::musicVol(short int volume)
    {
        return Mix_VolumeMusic(volume);
    }

    void ES_Sound::musicFadeIn(const std::string& resource_name, unsigned short int ms, short int loop)
    {
        if(loop == -1 || loop == 0)
        {
            if(Mix_FadeInMusic(resources.getMusic(resource_name), loop, ms) == -1)
                std::cerr << "_Mix_FadeInMusic: " << Mix_GetError() << std::endl;
        }
    }

    void ES_Sound::musicFadeOut(unsigned short int duration)
    {
        if(isPlayed() == true)
        {
            Mix_HookMusicFinished(ess_bla);
            f_finished = false;
            Mix_FadeOutMusic(duration);
        }
    }

    void ES_Sound::musicStop()
    {
        if(isPlayed() == true)
            Mix_HaltMusic();
    }

    bool ES_Sound::isPlayed()
    {
        if(Mix_PlayingMusic() == 1)
            return true;
        else
            return false;
    }

    bool ES_Sound::isPaused()
    {
        if(Mix_PausedMusic() == 1)
            return true;
        else
            return false;
    }

    void ES_Sound::setFadeFinished()
    {
        f_finished = true;
    }

    bool ES_Sound::getFadeState()
    {
        return f_finished;
        f_finished = false;
    }
}
