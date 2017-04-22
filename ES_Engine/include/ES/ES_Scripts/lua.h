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

#ifndef _ES_SCRIPTS_H_
#define _ES_SCRIPTS_H_

 #include <string>
 #include <iostream>
 extern "C"
 {
  #include <LUA/lua.h>
  #include <LUA/lauxlib.h>
  #include <LUA/lualib.h>
 }
 #include "../ES_Pack/pack.h"

namespace ES
{
 /**
 * Gestion des scripts Lua.
 */
 class ES_Scripts
 {
	 public:
		/**
		* Constructeur. Initialise Lua.
		*/

		ES_Scripts();

		/**
		* Destructeur. Ferme Lua.
		*/

		~ES_Scripts();

		/**
		* Charge un script Lua.
		*
		* @param file chemin du script Lua
		* @return -1 si erreur sinon 0
		*/

		int load(const std::string& file);

		/**
		* Charge un script Lua depuis un esd.
		*
		* @param file nom du script Lua dans le pack
		* @param pack objet ES_Pack avec la liste initialisé
		* @return -1 si erreur sinon 0
		*/

		int load(const std::string& file, ES_Pack *pack);

		/**
		* Ajoute une fonction C à appeler depuis Lua.
		*
		* @param func nom de la fonction dans le script
		* @param fn fonction C à appeler
		*/

		void addFunction(const std::string& func, lua_CFunction fn);

		/**
		* Ajoute une fonction Lua à appeler depuis le C/C++
		*
		* @param func nom de la fonction dans le script
		*/

		void addLuaFonction(const std::string& func);

		/**
		* Ajoute un nombre en passage de paramètres à une fonction Lua
		*
		* @param num nombre (float / int)
		*/

		void addArgNumber(float nun);

		/**
		* Ajoute une chaîne de caratéres en passage de paramètres à une fonction Lua
		*
		* @param str chaîne à passer
		*/

		void addArgStr(const std::string& str);

		/**
		* Appel une fonction Lua
		*
		* @param argc nombres d'arguments de la fonctions
		* @param retc nombres de retours de la fonctions
		*/

		void callLuaFunction(int argc = 0, int retc = 0);

		/**
		* Lit le script derniérement chargé.
		*
		* @return -1 erreur de syntaxe dans le script, -2 erreur d'allocations mémoire durant le chargement, -3 erreur de chargement du script, 0 aucunes erreurs
		*/

		int read();

		/**
		* @return -1 si la fonction précédente n'existe pas, sinon 0
		*/

		int isFunction();

		/**
		* @return nombres d'arguments de la fonction depuis Lua
		*/

		int getArg();

		/**
		* Lit la valeur d'une variable et la pousse dans la pile. (à lire après avec @code get_value_isint() @endcode où @code get_value_isstr() @endcode
		*/

		void getVar(const std::string& var);

		/**
		* Pousse une valeur sur la pile.
		*/

		void push();

		/**
		* Traverse une table Lua.
		*
		* @param p position dans la pile
		* @return 0 si aucuns éléments
		*/

		int nextValue(int p = -2);

		/**
		* @param p position dans la pile
		* @param pop true pour revenir à la position initiale de la pile automatiquement sinon false pour choisir
		* @return la valeur de la variable précédement lu en entier (si la valeur est un nombre sinon renvoie la derniére valeur lu)
		*/

		int getNumber(int p = 1, bool pop = true);

		/**
		* @param indice du tableau (position de lecture)
		* @return 0 si la variable n'est pas un tableau sinon 1
		*/

		int isTable(int ind, int add);

		/**
		* @param p position dans la pile
		* @param pop true pour revenir à la position initiale de la pile automatiquement sinon false pour choisir
		* @return la valeur de la variable précédement lu en flottants (si la valeur est un nombre sinon renvoie la derniére valeur lu)
		*/

		float getFloat(int p = 1, bool pop = true);

		/**
		* @param p position dans la pile
		* @param pop true pour revenir à la position initiale de la pile automatiquement sinon false pour choisir
		* @return la valeur de la variable précédement lu en chaînes de caractéres (si la valeur est une chaîne sinon renvoie la derniére valeur lu)
		*/

		std::string getStr(int p = 1, bool pop = true);

		/**
		* @return la raison d'une erreur survenu si il n'y à pas d'erreurs, renvoi la derniére erreur
		*/

		std::string getError();

		/**
		* La classe gére les possibles erreurs elle même.
		*/

		void setDefaultError();

		/**
		* Dépile la pile (ouais logique ;P) utilisé par Lua.
		*
		* @param p combien faut-il dépiler
		*/

		void pop(int p = 1);

     private:
		lua_State *ls;
		std::string lua_err;
		int s, result, vali;
		float valf;
		std::string valstr;
		bool d_error;
 };
}
#endif
