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

#include "ES_Scripts/lua.h"

namespace ES
{
	ES_Scripts::ES_Scripts()
	{
		ls = lua_open();
		#ifdef GP2X
		luaL_openlibs(ls);
		#else
        luaopen_io(ls);
        luaopen_base(ls);
        luaopen_table(ls);
        luaopen_string(ls);
        luaopen_math(ls);
        luaopen_loadlib(ls);
        #endif
	}

	ES_Scripts::~ES_Scripts()
	{
		lua_close(ls);
	}

	int ES_Scripts::load(const std::string& file)
	{
		if(ls == NULL)
		{
			if(d_error == true)
				std::cerr << "_Lua initialization error." << std::endl;
			else
				return -1;
		}
		else
		{
			s = luaL_loadfile(ls, file.c_str());

			if(s != 0)
			{
				if(d_error == true)
					std::cerr << "_Cant load script file : " << file << std::endl;
				else
					return -1;
			}
		}

		return 0;
	}

	int ES_Scripts::load(const std::string& file, ES_Pack *pack)
	{
		if(ls == NULL)
		{
			if(d_error == true)
				std::cerr << "_Lua initialization error." << std::endl;
			else
				return -1;
		}
		else
		{
			s = luaL_loadbuffer(ls, pack->getFile(file), pack->getSize(file), file.c_str());

			if(s != 0)
			{
				if(d_error == true)
					std::cerr << "_Cant load script file from es pack: " << file << std::endl;
				else
					return -1;
			}
		}

		return 0;
	}

	void ES_Scripts::addFunction(const std::string& func, lua_CFunction fn)
	{
		lua_register(ls, func.c_str(), fn);
	}

	void ES_Scripts::addLuaFonction(const std::string& func)
	{
		lua_getglobal(ls, func.c_str());
	}

	int ES_Scripts::isFunction()
	{
		if (!lua_isfunction(ls, -1))
		{
			if(d_error == true)
				std::cerr << "_Cant find lua function. -> " << getError()<< std::endl;
			else
				return -1;

			lua_pop(ls,1);
		}
		else
			return 0;

		return 0;
	}

	void ES_Scripts::addArgNumber(float num)
	{
		lua_pushnumber(ls, num);
	}

	void ES_Scripts::addArgStr(const std::string& str)
	{
		lua_pushstring(ls, str.c_str());
	}

	void ES_Scripts::callLuaFunction(int argc, int retc)
	{
		lua_call(ls, argc, retc);
	}

	int ES_Scripts::read()
	{
		if(s == 0)
		{
			result = lua_pcall(ls, 0, LUA_MULTRET, 0);

			switch (result)
			{
				case 0: // aucunes erreurs
						return 0;
				break;

				case LUA_ERRSYNTAX:	// erreur de syntaxe
					if(d_error == true)
						std::cerr << getError() << std::endl;
					else
						return -1;
				break;

				case LUA_ERRMEM: // allocation mémoire
					if(d_error == true)
						std::cerr << getError() << std::endl;
					else
						return -2;
				break;

				case LUA_ERRFILE:	// erreur de lecture du fichier
					if(d_error == true)
						std::cerr << getError() << std::endl;
					else
						return -3;
				break;
			}
		}

		return 0;
	}

	std::string ES_Scripts::getError()
	{
		if(s != 0)
		{
			lua_err = lua_tostring(ls, -1);
			lua_pop(ls, 1);
			return lua_err;
		}

		return lua_err;
	}

	void ES_Scripts::setDefaultError()
	{
		d_error = true;
	}

	int ES_Scripts::getArg()
	{
		return lua_gettop(ls);
	}

	void ES_Scripts::getVar(const std::string& var)
	{
		lua_settop(ls,0);
		lua_getglobal(ls, var.c_str());
	}

	void ES_Scripts::push()
	{
		lua_pushnil(ls);
	}

	int ES_Scripts::nextValue(int p)
	{
		return lua_next(ls, p);
	}

	int ES_Scripts::getNumber(int p, bool pop)
	{
		if (lua_isnumber(ls, p))
		{
			vali = (int)lua_tonumber(ls, p);
			if(pop == true)
				lua_pop(ls, 1);
			return vali;
		}

		return vali;
	}

	int ES_Scripts::isTable(int ind, int add)
	{
		if (!lua_istable(ls, 1))
		{
			lua_pop(ls, 1);
			return 0;
		}
		else
		{
			lua_pushnumber(ls, ind);
			lua_gettable(ls, -add);
			return 1;
		}
	}

	float ES_Scripts::getFloat(int p, bool pop)
	{
		if (lua_isnumber(ls, p))
		{
			valf = (float)lua_tonumber(ls, p);
			if(pop == true)
				lua_pop(ls, 1);
			return valf;
		}

		return valf;
	}

	std::string ES_Scripts::getStr(int p, bool pop)
	{
		if(lua_isstring(ls, p))
		{
			valstr = lua_tostring(ls, p);
			if(pop == true)
				lua_pop(ls, 1);
			return valstr;
		}

		return valstr;
	}

	void ES_Scripts::pop(int p)
	{
		lua_pop(ls, p);
	}
}
