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

#ifndef _ESTELANTE_PACKER_H
#define _ESTELANTE_PACKER_H

 #include <string>
 #include <fstream>
 #include <iostream>
 // boost
 #include <boost/filesystem/path.hpp>
 #include <boost/filesystem/exception.hpp>
 #include <boost/filesystem/convenience.hpp>
 // fichier provenant d'ES
 #include "utils.h"

 /*
  - En tête de l'archive
 */
 struct head
 {
	 char id[4]; // identifiant de l'archive
	 unsigned long int nb_files; // stocke le nombres de fichiers
 };

 /*
  - En tête du fichier
 */
 struct descriptor
 {
	char name[256]; // nom du fichier
	unsigned long size; // taille des données
 };

 /*
 * Rassemble en un seul fichier tous le contenu d'un répertoire (et des sous répertoires)
 * excluant les dossiers.
 */
 class ES_Packer
 {
     public:
         //- Crée une archive
         //  arc_name = nom de l'archive (.esd rajouté automatiquement)
         //  directory = dossier à packer
         bool create_pack(const std::string& arc_name, const std::string& directory);
         //- Désarchive une archive
         //  arc_name nom de l'archive à désarchiver
         //  dst destination du contenu de l'archive
         void depack(const std::string& arc_name, const std::string& dst);

     private:
         void write(const std::string& file, const std::string& directory);

         char *buffer;
         struct head header;
         struct descriptor desc;
         std::ifstream rf;
         std::ofstream f;
         std::string name, dir, tmp_name;
         unsigned long int f_count;
 };

#endif
