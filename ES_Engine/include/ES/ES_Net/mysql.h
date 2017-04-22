#ifndef _ES_MYSQL_H_
#define _ES_MYSQL_H_

 #ifdef WIN32
    #include <windows.h>
 #endif
 #ifndef GP2X
 #include <mysql/mysql.h>
 #include <string>
 #include <iostream>

namespace ES
{
 /**
 * Gestion des bases de donn�es mySQL.
 */
 class ES_Mysql
 {
    public:

        /**
        * Constructeur. Initialise mySQL.
        */

        ES_Mysql();

        /**
        * Destructeur. Ferme mySQL.
        */

        ~ES_Mysql();

        /**
        * Etablie une connection permanente � une base de donn�e mySQL.
        *
        * @param host nom d'h�te (o� est h�berg� la base de donn�e)
        * @param user nom d'utilisateur
        * @param password mot de passe pour acc�der � la base
        * @param db nom de la base de donn�e
        * @param port port de connection
        * @param unix_socket sp�cifie le socket o� le tunnel � utiliser
        * @param client_flag sp�cifie un drapeau pour les connections sp�ciales (voir http://dev.mysql.com/doc/refman/5.0/fr/mysql-real-connect.html pour plus d'informations sur les flags)
        * @return -1 si erreur sinon 0
        */

        int connect(const std::string &host = "localhost", const std::string &user = "root", const std::string &password = "", const std::string &db = "", unsigned int port = 0, const char *unix_socket = NULL, unsigned long client_flag = 0);

        /**
        * Initialise un jeu de r�sultat mySQL apr�s une requ�te.
        * Vous devez utiliser @code fetch_row() @endcode jusqu'a ce que -1 soit retourn� apr�s cette fonction
        *
        * @return -1 si erreur sinon 0
        */

        int useResult();

        /**
        * Lib�re la m�moire d'un jeu de r�sultat.
        */

        void freeResult();

        /**
        * R�cup�re les lignes d'un jeu de r�sultat.
        *
        * @return -1 si erreur o� qu'il y � plus de lignes � r�cup�rer sinon 0
        */

        int fetchRow();

        /**
        * Effectue une requ�te mySQL.
        *
        * @param query requ�te SQL
        * @return -1 si erreur sinon 0
        */

        int query(const std::string &query);

        /**
        * Retourne la derni�re erreur mySQL survenu.
        *
        * @return erreur mySQL
        */

        std::string getError();

    private:
        MYSQL_RES *result;
        MYSQL_ROW row;
        MYSQL mysql;
 };
}
#endif
#endif
