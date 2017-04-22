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

#ifndef _ES_CONFIGURATION_H_
#define _ES_CONFIGURATION_H_

 #include <iostream>
 #include <fstream>
 #include <list>
 #include <tinyxml.h>
 #include "utils.h"

namespace ES
{
 class ES_Configuration_Structure
 {
        public:
              std::string name, value;
 };

 /**
 * Gestion de fichiers de configurations XML.
 */
 class ES_Configuration
 {
     public:

	     /**
	     * Constructeur.
	     */

		 ES_Configuration();

		 /**
		 * Destructeur. Ferme le fichier si ouvert.
		 */

	     ~ES_Configuration();

	     /**
	     * Vérifie que le fichier existe, si il existe, le lit, sinon le crée avec les paramétres définies via @code set_default_key_value() @endcode
	     *
	     * @param configuration_filename chemin du fichier de configuration à ouvrir où crée
	     */

		 void openXML(std::string configuration_filename);

		 /**
		 * Enregistre les modifications apportés au fichier.
		 *
		 * @param filename fichier où enregistrer
		 */
		 void writeXML(std::string filename);

		 /**
		 * Remplis les champs qui seront écrites si le fichier n'existe pas.
		 *
		 * @param key nom de la clé
		 * @param value valeur par défaut à écrire
		 */

		 void setKeyValue(std::string key, std::string value);

		 /**
		 * Remplis les champs qui seront écrites si le fichier n'existe pas.
		 *
		 * @param key nom de la clé
		 * @param value valeur par défaut à écrire
		 */

		 void setKeyValue(std::string key, int value);

		 /**
		 * Renvoie une la valeur de types string lu dans le fichier de configuration.
		 *
		 * @param key nom de la clé où lire la valeur
		 * @param deflt valeur par défaut à retourner si la clé n'existe pas
		 * @return valeur définis dans le fichier où valeur par défaut
		 */

		 std::string getKeyValue(std::string key, std::string deflt);

		 /**
		 * Renvoie une la valeur lu de type nombres dans le fichier de configuration.
		 *
		 * @param key nom de la clé où lire la valeur
		 * @param deflt valeur par défaut à retourner si la clé n'existe pas
		 * @return valeur définis dans le fichier où valeur par défaut
		 */

		 int getKeyValue(std::string key, int deflt);

	 private:
		 TiXmlDocument xml_doc;
		 TiXmlElement *element;
		 int value_temp;
		 std::list<ES_Configuration_Structure> xml_structure;
		 std::ofstream configuration_file;
		 ES_Configuration_Structure es_struct_config;
 };
}
#endif
