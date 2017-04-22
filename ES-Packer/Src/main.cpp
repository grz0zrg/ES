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

ES_Packer packer;

int main()
{
	bool loop = true;
	std::string dir;
	std::string file;

	std::cout << "ES - Packer \n"
			  << "- Commands: 'q' = quit\n"
			  << "            'p' = packing\n"
			  << "            'd' = depacking\n\n";

	while(loop == true)
	{
	    std::cout << "Mode ? ('p' for packing, 'd' for depacking, 'q' for quit)\n";
	    std::cin >> file;

	    if(file == "p") // mode pack
	    {
            std::cout << "\nOutput filename ?\n";
            std::cin >> file;

            if(file == "q")
            {
                loop = false;
                continue;
            }

            std::cout << "\nDirectory ?\n";
            std::cin >> dir;
            std::cout << "\n";

            if(dir == "q")
            {
                loop = false;
                continue;
            }

            if(dir.substr(dir.length()-1) != "/")
                dir = dir + "/";

            // crée l'archive
            if(packer.create_pack(file, dir) == true)
                std::cout << "\nArchive " << file+".esd" << " created with success.\n\n";
            else
                std::cout << "\n Error while creating the archive...\n\n";
	    }
	    else if(file == "d") // mode depack
	    {
            std::cout << "\nOutput directory ?\n";
            std::cin >> dir;

            if(dir == "q")
            {
                loop = false;
                continue;
            }

            if(dir.substr(dir.length()-1) != "/")
                dir = dir + "/";

            std::cout << "\nArchive name ?\n";
            std::cin >> file;
            std::cout << "\n";

            if(file == "q")
            {
                loop = false;
                continue;
            }

            // depack l'archive spécifié dans le dossier spécifié
            packer.depack(file, dir);
	    }
	    else if(file == "q") // on quitte
	        loop = false;
	}

    return EXIT_SUCCESS;
}
