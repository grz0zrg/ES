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
 * Gestion des bases de données mySQL.
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
        * Etablie une connection permanente à une base de donnée mySQL.
        *
        * @param host nom d'hôte (où est hébergé la base de donnée)
        * @param user nom d'utilisateur
        * @param password mot de passe pour accéder à la base
        * @param db nom de la base de donnée
        * @param port port de connection
        * @param unix_socket spécifie le socket où le tunnel à utiliser
        * @param client_flag spécifie un drapeau pour les connections spéciales (voir http://dev.mysql.com/doc/refman/5.0/fr/mysql-real-connect.html pour plus d'informations sur les flags)
        * @return -1 si erreur sinon 0
        */

        int connect(const std::string &host = "localhost", const std::string &user = "root", const std::string &password = "", const std::string &db = "", unsigned int port = 0, const char *unix_socket = NULL, unsigned long client_flag = 0);

        /**
        * Initialise un jeu de résultat mySQL après une requête.
        * Vous devez utiliser @code fetch_row() @endcode jusqu'a ce que -1 soit retourné après cette fonction
        *
        * @return -1 si erreur sinon 0
        */

        int useResult();

        /**
        * Libére la mémoire d'un jeu de résultat.
        */

        void freeResult();

        /**
        * Récupére les lignes d'un jeu de résultat.
        *
        * @return -1 si erreur où qu'il y à plus de lignes à récupérer sinon 0
        */

        int fetchRow();

        /**
        * Effectue une requête mySQL.
        *
        * @param query requête SQL
        * @return -1 si erreur sinon 0
        */

        int query(const std::string &query);

        /**
        * Retourne la derniére erreur mySQL survenu.
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
