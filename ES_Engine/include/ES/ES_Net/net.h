#ifndef _ES_NET_H_
#define _ES_NET_H_

 #include <SDL/sdl.h>
 #include <SDL/sdl_net.h>
 #include <string>

namespace ES
{
 /**
 * Gestion du réseau avec SDL_net.
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
        * Initialise le réseau et ouvre un socket.
        *
        * @param mode spécifie le type de connection à utiliser (ES_TCP où ES_UDP)
        * @param host spécifie le nom d'hôte à utiliser pour se connecter (NULL en mode serveur et non utilisé en mode UDP)
        * @param port port d'écoute/envoie
        * @return -1 si problème lors de l'initialisation/connection sinon 0 (récupérer l'erreur avec @code get_error @endcode
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
        * @return derniére erreur généré par SDL_net
        */

        std::string getError();

        /**
        * Alloue de la place pour un jeu de sockets (l'alloue avec le jeu de socket par défaut de la classe)
        *
        * @param max_socks maximum de sockets à allouer
        * @return NULL si erreur sinon le nouveau jeu
        */

        SDLNet_SocketSet allocSocketSet(int max_socks);

        /**
        * Alloue de la place pour un jeu de sockets
        *
        * @param socket_set jeu de socket à utiliser
        * @param max_socks maximum de sockets à allouer
        * @return NULL si erreur sinon le nouveau jeu
        */

        SDLNet_SocketSet allocSocketSet(SDLNet_SocketSet socket_set, int max_socks);

        /**
        * Libére le jeu de socket crée via la classe.
        */

        void freeSocketSet();

        /**
        * Libére un jeu de sockets.
        *
        * @param socket_set jeu de socket à libérer
        */

        void freeSocketSet(SDLNet_SocketSet socket_set);

        /**
        * Ajoute un socket UDP au jeu de socket de l'objet.
        *
        * @param sock socket UDP à ajouter
        * @return -1 si erreur sinon le nombres de sockets utilisés dans le jeu de sockets
        */

        int addUDP_Socket(UDPsocket sock);

        /**
        * Ajoute un socket TCP au jeu de socket de l'objet.
        *
        * @param sock socket TCP à ajouter
        * @return -1 si erreur sinon le nombres de sockets utilisés dans le jeu de sockets
        */

        int addTCP_Socket(TCPsocket sock);

        /**
        * Ajoute un socket UDP à un jeu de sockets.
        *
        * @param set jeu de sockets à utiliser pour l'ajout
        * @param sock socket UDP à ajouter
        * @return -1 si erreur sinon le nombres de sockets utilisés dans le jeu de sockets
        */

        int addUDP_Socket(SDLNet_SocketSet set, UDPsocket sock);

        /**
        * Ajoute un socket TCP à un jeu de sockets.
        *
        * @param set jeu de sockets à utiliser pour l'ajout
        * @param sock socket TCP à ajouter
        * @return -1 si erreur sinon le nombres de sockets utilisés dans le jeu de sockets
        */

        int addTCP_Socket(SDLNet_SocketSet set, TCPsocket sock);

        /**
        * Envoie un paquet UDP.
        *
        * @param src paquet UDP à envoyer
        * @return 0 si erreur sinon le nombres de destinations réussis
        */

        int sendUDP(UDPpacket *src);

        /**
        * Reçoie un paquet UDP.
        *
        * @param dest paquet UDP oèu recevoir les données
        * @return 1 si un paquet à étaît réussi 0 si aucuns paquets à étaît reçus, -1 si erreur
        */

        int recvUDP(UDPpacket *dst);

        /**
        * Envoie un paquet TCP.
        *
        * @param data données à envoyers
        * @param len taille des données à envoyers
        * @return nombres de bytes envoyés, si le nombre retourné est inférieur à la taille définie c'est qu'il y à eu une erreur
        */

        int sendTCP(void *data, int len);

        /**
        * Envoie un paquet TCP via un socket donné.
        *
        * @param socket socket à qui envoyer les données
        * @param data données à envoyers
        * @param len taille des données à envoyers
        * @return nombres de bytes envoyés, si le nombre retourné est inférieur à la taille définie c'est qu'il y à eu une erreur
        */

        int sendTCP(TCPsocket socket, void *data, int len);

        /**
        * Reçois des données TCP.
        *
        * @param data buffer où recevoir les donneés
        * @param maxlen maximum (en bytes) de données lus dans le buffer (généralement la taille de la structure utilisé)
        * @return le nombres de bytes reçus, si le nombre est inférieur où égal à 0 le serveur à fermé la connection où un problème est survenu
        */

        int recvTCP(void *data, int maxlen);

        /**
        * Accepte une connection TCP entrante.
        *
        * @return un socket tcp valide sinon NULL si problème
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
