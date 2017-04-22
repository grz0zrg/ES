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

#include "ES_Utils/threads.h"

namespace ES
{
	ES_Threads::ES_Threads(int (*fn)(void *), void *data)
	{
		thread = SDL_CreateThread(fn, data);
	}

	ES_Threads::~ES_Threads()
	{
		SDL_WaitThread(thread, &c_err);
	}

	int ES_Threads::wait()
	{
		SDL_WaitThread(thread, &c_err);

		return c_err;
	}

	void ES_Threads::kill()
	{
		SDL_KillThread(thread);
	}

	SDL_Thread *ES_Threads::getSDL_Thread()
	{
		return thread;
	}

	std::string ES_Threads::getError()
	{
	    #ifndef GP2X
		return SDL_GetError();
		#else
		const std::string err = "No error message for SDL_Threads";
		return err;
		#endif
	}
}
