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

#include "ES_Pack/pack.h"

namespace ES
{
    ES_Pack::~ES_Pack()
    {
        // vérifie les conteneurs et libére la mémoire alloué
        for (std::map<std::string, char*>::const_iterator iter = pack_file.begin(); iter != pack_file.end(); ++iter)
            delete iter->second;
    }

    bool ES_Pack::initList(const std::string& arch_name)
    {
        f_count = 0;

        rf.open(arch_name.c_str(), std::ios::binary);
        if(!rf)
            return false;

        rf.read((char*)&header, sizeof(header));

        if(strcmp(header.id, "ESP") == 0)
        {
            if(header.nb_files > 0)
            {
                while(f_count < header.nb_files)
                {
                    rf.read((char*)&desc, sizeof(desc));
                    pack_list[toStr(desc.name)] = rf.tellg(); // place à quel pos est le fichier dans l'archive
                    pack_list2[toStr(desc.name)] = desc.size; // place la taille des données du fichier

                    rf.seekg((int)rf.tellg()+desc.size); // jump àu prochain descripteur

                    f_count++;
                }
            }
            else
            {
                rf.close();
                return false;
            }
        }
        else
        {
            rf.close();
            return false;
        }

        rf.close();
        archive = arch_name;
        return true;
    }

    void ES_Pack::setArchive(const std::string& arch_name)
    {
        archive = arch_name;
    }

    char* ES_Pack::getFile(const std::string& file)
    {
        // vérifie que le fichier existe dans le conteneur
        it_pack_list = pack_list.find(file);
        it_pack_list2 = pack_list2.find(file);
        if (it_pack_list == pack_list.end() || it_pack_list2 == pack_list2.end())
            return NULL;
        else
        {
            // ouvre l'archive
            rf.open(archive.c_str(), std::ios::binary);
            if(!rf)
                return NULL;

            rf.seekg(pack_list[file]); // met le pointeur à la position correct du fichier

            if(pack_file[file] != NULL) // si il existe déjà on libére la mémoire et tant pis pour lui :O
                delete pack_file[file];

            pack_file[file] = new char[pack_list2[file]];

            rf.read(pack_file[file], pack_list2[file]); // lit les données et les place dans un buffer

            rf.close();

            return pack_file[file];
        }
    }

    unsigned long ES_Pack::getSize(const std::string& file)
    {
        // vérifie que le fichier existe dans le conteneur
        it_pack_list = pack_list.find(file);
        it_pack_list2 = pack_list2.find(file);
        if (it_pack_list == pack_list.end() || it_pack_list2 == pack_list2.end())
            return 0;
        else
        {
            return pack_list2[file]; // retourne la taille des données d'un fichier
        }
    }

    void ES_Pack::free(const std::string& file)
    {
        // vérifie que le fichier existe dans le conteneur
        it_pack_file = pack_file.find(file);
        if (it_pack_file == pack_file.end())
            return;
        else
        {
            delete pack_file[file];
            pack_file.erase(file);
        }
    }
}
