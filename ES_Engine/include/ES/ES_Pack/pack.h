#ifndef _ES_PACK_H_
#define _ES_PACK_H_

 #include <string>
 #include <map>
 #include <fstream>
 #include <iostream>
 #include "../ES_Utils/utils.h"

namespace ES
{
 /*
  - En t�te de l'archive
 */
 struct head
 {
	 char id[4]; // identifiant de l'archive
	 unsigned long int nb_files; // stocke le nombres de fichiers
 };

 /*
  - En t�te du fichier
 */
 struct descriptor
 {
	char name[256]; // nom du fichier
	unsigned long size; // taille des donn�es
 };

 /**
 * Gestion des archives ES.
 */
 class ES_Pack
 {
     public:
         /**
         * Constructeur.
         */

         ES_Pack(){ };

         /**
         * Destructeur, lib�re toutes les donn�es charg�s depuis les archives.
         */

         ~ES_Pack();

         /**
         * Initialise la liste des fichiers d'une archive pour la recherche rapide.
         *
         * @param arch_name chemin de l'archive
         * @return false si il y � eu un probl�me lors de l'initialisation de la liste sinon true
         */

         bool initList(const std::string& arch_name);

         /**
         * Active la recherche dans l'archive s�l�ctionner.
         *
         * @param arch_name chemin de l'archive
         */

         void setArchive(const std::string& arch_name);

         /**
         * R�cup�re un fichier en m�moire depuis l'archive actuel.
         *
         * @param file nom du fichier � r�cup�rer
         * @return les donn�es lus
         */

         char* getFile(const std::string& file);

         /**
         * R�cup�re la taille des donn�es d'un fichier de l'archive actuel.
         *
         * @param file nom du fichier
         * @return taille des donn�es
         */

         unsigned long getSize(const std::string& file);

         /**
         * Lib�re manuellement la m�moire pour un fichier d�j� charg�.
         *
         * @param file nom du fichier � lib�rer
         */

         void free(const std::string& file);

     private:
         struct head header;
         struct descriptor desc;
         std::string archive;
         std::ifstream rf;
         unsigned long int f_count;
         std::map<std::string, std::streampos> pack_list;
         std::map<std::string, unsigned long> pack_list2;
         std::map<std::string, char*> pack_file;
         std::map<std::string, std::streampos>::iterator it_pack_list;
         std::map<std::string, unsigned long>::iterator it_pack_list2;
         std::map<std::string, char*>::iterator it_pack_file;
 };
}
#endif
