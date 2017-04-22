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

#include "packer.h"

bool ES_Packer::create_pack(const std::string& arc_name, const std::string& directory)
{
	name = arc_name; // nom de l'archive
	name = name + ".esd"; // on lui rajoute l'extension du packer automatiquement "esd" = "es data"
	dir = directory; // dossier à parcourir
	strcpy(header.id, "ESP"); // identifiant "ESP" pour "es packer"
	header.nb_files = 0; // 0 fichiers pour le moment

	f.open(name.c_str(), std::ios::out | std::ios::binary); // crée l'archive en mode binaire
	f.write((char*)&header, sizeof(header)); // écrit l'entête de l'archive

	if(!f) // erreur de création/ouverture
		return false;

	/* pack les données du répertoire spécifié! */
    boost::filesystem::path path(dir);
	try
	{
		if(!boost::filesystem::exists(path)) // si le chemin existe pas ça sert à rien de continuer
			return false;

		// parcoure les fichiers et dossiers du chemin spécifié
		for(boost::filesystem::recursive_directory_iterator i(path); i != boost::filesystem::recursive_directory_iterator(); ++i)
		{
			if(boost::filesystem::is_directory((*i).string())) // trouve un dossier
				continue; // on évite de l'ajouter à l'archive sinon plantage

			// ont inclus seulement les fichiers qui ne sont pas vides
			if(!boost::filesystem::is_empty((*i).string()))
				write((*i).leaf(), (*i).string()); // on ajoute le fichier dans l'archive
		}
	}
	catch(boost::filesystem::filesystem_error& x)
	{
	    // exception boost levé
		std::cout << "\n" << x.what() << "\n";
	}

	// plus de fichiers à ajouter ?
	// revient au début de l'archive
	f.seekp(0, std::ios::beg);
	f.write((char*)&header, sizeof(header)); // réecrit l'en tête de l'archive avec le nombres correct de fichiers

    std::cout << "\n" << header.nb_files << " files added.";

	f.close(); // ferme le fichier

	return true; // tout est ok !
}

void ES_Packer::depack(const std::string& arc_name, const std::string& dst)
{
    f_count = 0;

    // lui rajoute l'extension automatiquement
    tmp_name = arc_name + ".esd";

    // ouvre l'archive en mode binaire
    rf.open(tmp_name.c_str(), std::ios::binary);
	if(!rf) // impossible d'ouvrir l'archive
	{
	    std::cout << "Archive error." << std::endl;
		return;
	}

    // lit le header
    rf.read((char*)&header, sizeof(header));
    // vérifie que l'id est bien le même que celui du packer
    if(strcmp(header.id, "ESP") == 0)
    {
        // vérifie le nombres de fichiers
        if(header.nb_files > 0)
        {
            // pour chaques fichiers de l'archive
            while(f_count < header.nb_files)
            {
                // lit le descripteur
                rf.read((char*)&desc, sizeof(desc));
                std::cout << "File: " << desc.name << " found, depacking..." << std::endl;
                // compléte avec le chemin
                tmp_name = dst + to_string(desc.name);
                // crée le fichier dans le répertoire voulu
                f.open(tmp_name.c_str(), std::ios::out | std::ios::binary);
                if(!f)
                {
                    std::cout << "Archive error." << std::endl;
                    rf.close();
                    return;
                }
                buffer = new char[desc.size];

                rf.read(buffer, desc.size);
                f.write(buffer, desc.size);

                delete buffer;

                f.close();

                f_count++;
            }

            std::cout << "\n" << f_count << " files depacked.\n" << std::endl;
        }
        else
        {
            std::cout << "Nothing files found." << std::endl;
            rf.close();
            return;
        }
    }
    else // sinon mauvais format
    {
	    std::cout << "Invalid format." << std::endl;
	    rf.close();
		return;
    }

    rf.close(); // ferme l'archive
}

void ES_Packer::write(const std::string& file, const std::string& directory)
{
	// copie le nom du fichier dans le descripteur
	strncpy(desc.name, file.c_str(), 254);
	desc.name[254] = '\0';

	// ouvre le fichier en mode binaire
	rf.open(directory.c_str(), std::ios::binary);

	if(!rf) // impossible d'ouvrir le fichier
	{
	    std::cout << "File: " << file << " -> Error." << std::endl;
		return;
	}

	// copie la taille des données du fichier dans le descripteur
	rf.seekg(0, std::ios::end);
	desc.size = rf.tellg();
	rf.seekg(0, std::ios::beg);
	// écrit le descripteur à la suite dans l'archive
	f.write((char*)&desc, sizeof(desc));

	// lit le fichier en mémoire
	buffer = new char[desc.size];
	rf.read(buffer, desc.size);
	// copie les données du fichier dans l'archive à la suite du descripteur
	f.write(buffer, desc.size);
	rf.close(); // ferme le fichier

	delete[] buffer; // libére la mémoire alloué pour la lecture du fichier
	header.nb_files++; // et 1 fichier de plus dans l'archive!

    // fichier packé correctement
	std::cout << "File: " << file << " -> Ok." << std::endl;
}
