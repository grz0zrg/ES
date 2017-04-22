/*
* Copyright (c) 2006, 2007 Julien Verneuil
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

#include "configuration.h"

ES_Configuration::ES_Configuration()
{
	element = new TiXmlElement("option");
}

ES_Configuration::~ES_Configuration()
{
	if(configuration_file.is_open())
		configuration_file.close();
	if(element)
		delete element;
}

void ES_Configuration::check_configuration_file(std::string configuration_filename)
{
    configuration_file.open(configuration_filename.c_str(), std::ios_base::in);

	// impossible d'ouvrir ? on le crée avec les valeurs par défauts!
    if (!configuration_file.is_open())
    {
    	configuration_file.open(configuration_filename.c_str(), std::ios_base::trunc);
		if(!configuration_file.is_open())
		{
			std::cout << "Error while creating : \"" << configuration_filename << "\"" << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			xml_doc.SaveFile(configuration_filename.c_str());
		}
    }
    else // lecture fichier
    {
    	// merci http://khayyam.developpez.com/articles/cpp/tinyxml/ pour l'aide sur tinyxml
		TiXmlDocument xml_doc(configuration_filename.c_str());
		if(!xml_doc.LoadFile())
		{
			std::cout << "_Error while opening :\"" << configuration_filename << "\"" << std::endl;
			exit(EXIT_SUCCESS);
		}

		TiXmlHandle handle(&xml_doc);
		element = handle.FirstChildElement().FirstChildElement().Element();

		while (element) // à changer plus tard pour supporter les std::map, plus simple à modifier ensuite
		{
			es_struct_config.name = element->Attribute("name");
			es_struct_config.value = element->Attribute("value");
			xml_structure.push_back(es_struct_config);

			element = element->NextSiblingElement(); // on passe à l'élement suivant

			/* Début d'utilisation des std::map, à peaufiner
			element->QueryIntAttribute("value", &value_temp);
			option[(const char*)element->Attribute("name")] = (const char*)value_temp;
			element = element->NextSiblingElement(); // on passe au noeud suivant*/
		}
    }
}

void ES_Configuration::write_xml(std::string filename)
{
	xml_doc.SaveFile(filename.c_str());
}

void ES_Configuration::set_default_key_value(std::string key, std::string value)
{
    element->SetAttribute(key.c_str(), value.c_str());
}

void ES_Configuration::set_default_key_value(std::string key, int value)
{
    element->SetAttribute(key.c_str(), value);
}

std::string ES_Configuration::get_key_value(std::string key, std::string deflt)
{
	std::list<ES_Configuration_Structure>::iterator i = xml_structure.begin();
	while(i != xml_structure.end()) // on parcoure la liste pour récup la valeur de la clé donné
	{
		if(i->name == key) // clé trouvé on retourne la valeur
		{
			return i->value;
		}
		i++;
	}

	return deflt; // retourne la valeur par défaut si la clé n'as pas étaît trouvé
}

int ES_Configuration::get_key_value(std::string key, int deflt)
{
	std::list<ES_Configuration_Structure>::iterator i = xml_structure.begin();
	while(i != xml_structure.end())
	{
		if(i->name == key)
		{
			convert_from_string(i->value, value_temp);
			return value_temp;
		}
		i++;
	}

	return deflt;
}
