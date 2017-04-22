#ifndef _ES_NET_H_
#define _ES_NET_H_

 #include <SDL/sdl.h>
 #include <SDL/sdl_net.h>
 #include <string>

namespace ES
{
 /**
 * Gestion du r�seau avec SDL_net.
 */
 class ES_Network
 {
    public:

		/**
		* Constructeur.
		*/

        ES_Network();

        /**
        * Destructeur.
        */

        ~ES_Network();

        /**
        * Initialise le r�seau et ouvre un socket.
        *
        * @param mode sp�cifie le type de connection � utiliser (ES_TCP o� ES_UDP)
        * @param host sp�cifie le nom d'h�te � utiliser pour se connecter (NULL en mode serveur et non utilis� en mode UDP)
        * @param port port d'�coute/envoie
        * @return -1 si probl�me lors de l'initialisation/connection sinon 0 (r�cup�rer l'erreur avec @code get_error @endcode
        */

        int init(short int mode = ES_TCP, char *host = "localhost", Uint16 port = "6420");

        /**
        * Met fin aux connections/ferme SDL_Net.
        */

        void quit();

        /**
        * @return socket de type TCP
        */

        TCPsocket getTCP_Socket();

        /**
        * @return socket de type UDP
        */

        UDPsocket getUDP_Socket();

        /**
        * @return derni�re erreur g�n�r� par SDL_net
        */

        std::string getError();

        /**
        * Alloue de la place pour un jeu de sockets (l'alloue avec le jeu de socket par d�faut de la classe)
        *
        * @param max_socks maximum de sockets � allouer
        * @return NULL si erreur sinon le nouveau jeu
        */

        SDLNet_SocketSet allocSocketSet(int max_socks);

        /**
        * Alloue de la place pour un jeu de sockets
        *
        * @param socket_set jeu de socket � utiliser
        * @param max_socks maximum de sockets � allouer
        * @return NULL si erreur sinon le nouveau jeu
        */

        SDLNet_SocketSet allocSocketSet(SDLNet_SocketSet socket_set, int max_socks);

        /**
        * Lib�re le jeu de socket cr�e via la classe.
        */

        void freeSocketSet();

        /**
        * Lib�re un jeu de sockets.
        *
        * @param socket_set jeu de socket � lib�rer
        */

        void freeSocketSet(SDLNet_SocketSet socket_set);

        /**
        * Ajoute un socket UDP au jeu de socket de l'objet.
        *
        * @param sock socket UDP � ajouter
        * @return -1 si erreur sinon le nombres de sockets utilis�s dans le jeu de sockets
        */

        int addUDP_Socket(UDPsocket sock);

        /**
        * Ajoute un socket TCP au jeu de socket de l'objet.
        *
        * @param sock socket TCP � ajouter
        * @return -1 si erreur sinon le nombres de sockets utilis�s dans le jeu de sockets
        */

        int addTCP_Socket(TCPsocket sock);

        /**
        * Ajoute un socket UDP � un jeu de sockets.
        *
        * @param set jeu de sockets � utiliser pour l'ajout
        * @param sock socket UDP � ajouter
        * @return -1 si erreur sinon le nombres de sockets utilis�s dans le jeu de sockets
        */

        int addUDP_Socket(SDLNet_SocketSet set, UDPsocket sock);

        /**
        * Ajoute un socket TCP � un jeu de sockets.
        *
        * @param set jeu de sockets � utiliser pour l'ajout
        * @param sock socket TCP � ajouter
        * @return -1 si erreur sinon le nombres de sockets utilis�s dans le jeu de sockets
        */

        int addTCP_Socket(SDLNet_SocketSet set, TCPsocket sock);

        /**
        * Envoie un paquet UDP.
        *
        * @param src paquet UDP � envoyer
        * @return 0 si erreur sinon le nombres de destinations r�ussis
        */

        int sendUDP(UDPpacket *src);

        /**
        * Re�oie un paquet UDP.
        *
        * @param dest paquet UDP o�u recevoir les donn�es
        * @return 1 si un paquet � �ta�t r�ussi 0 si aucuns paquets � �ta�t re�us, -1 si erreur
        */

        int recvUDP(UDPpacket *dst);

        /**
        * Envoie un paquet TCP.
        *
        * @param data donn�es � envoyers
        * @param len taille des donn�es � envoyers
        * @return nombres de bytes envoy�s, si le nombre retourn� est inf�rieur � la taille d�finie c'est qu'il y � eu une erreur
        */

        int sendTCP(void *data, int len);

        /**
        * Envoie un paquet TCP via un socket donn�.
        *
        * @param socket socket � qui envoyer les donn�es
        * @param data donn�es � envoyers
        * @param len taille des donn�es � envoyers
        * @return nombres de bytes envoy�s, si le nombre retourn� est inf�rieur � la taille d�finie c'est qu'il y � eu une erreur
        */

        int sendTCP(TCPsocket socket, void *data, int len);

        /**
        * Re�ois des donn�es TCP.
        *
        * @param data buffer o� recevoir les donne�s
        * @param maxlen maximum (en bytes) de donn�es lus dans le buffer (g�n�ralement la taille de la structure utilis�)
        * @return le nombres de bytes re�us, si le nombre est inf�rieur o� �gal � 0 le serveur � ferm� la connection o� un probl�me est survenu
        */

        int recvTCP(void *data, int maxlen);

        /**
        * Accepte une connection TCP entrante.
        *
        * @return un socket tcp valide sinon NULL si probl�me
        */

        TCPsocket acceptConnection();

        /**
        * Modes de connections.
        */

        enum NetworkMode{ ES_TCP = 0, ES_UDP = 1};

    private:
        IPaddress ip;
        TCPsocket socketTCP;
        UDPsocket socketUDP;
        SDLNet_SocketSet s_set;
 };
}
#endif
