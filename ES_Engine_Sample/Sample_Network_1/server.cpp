// ES engine - Exemple Esnet 1
//- Démonstration client / serveur avec es net et les threads / mutex (protections threads)
#include <ES/es.h>
#include <ES/ES_Net/net.h> // pour l'utilisation des fonctions réseaux
#include <ES/ES_Utils/threads.h> // pour l'utilisation des threads

using namespace ES;

// les identifiants client / serveur pour que chacun puisse savoir ce qu'est le paquet reçus
enum id_info{ id_bonjour };

// notre paquet qui va nous servir pour les échanges client / serveur
struct Paquet
{
	// /!\ le réseau n'accepte que les types basiques donc pas de std::string etc
	int id; // sert pour la reconnaissance du paquet par le serveur
	char message[255]; // pour stocker les messages envoyés par le serveur
}paquet;

ES_Network *net;
ES_Graphics *graphics;

TCPsocket temp_sock; // socket client temporaire

// fonction qui s'occupe de recevoir les données des clients et de les traiter
int reception(void*)
{
	while (graphics->run())
	{
		// le serveur est en route ! on attend qu'un client envoie quelque chose (fonction bloquante)
		net->tcp_receive(&paquet, sizeof(paquet));
		// si il à reçus quelque chose et que l'id du paquet est bien un id_bonjour
		if(paquet.id == id_bonjour)
		{
			// prépare le paquet avec le message
			paquet.id = id_bonjour;
			strcpy(paquet.message, "Bienvenue sur le serveur !");
			if(temp_sock != NULL) // vérifie qu'un client est bien connecté
				net->tcp_send(temp_sock, &paquet, sizeof(paquet)); // envoie le paquet au client connecté
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	ES_Resources ressources;
	ES_Event event;

	graphics = new ES_Graphics("ES engine - Sample Network 1 - Server", 400, 300, 32, false);

    // initialisation des fonts
	ES_Ttf *fonts = new ES_Ttf();
	fonts->set_size(24);

    // charge notre font
    ressources.add_resource("police", "data/arial.ttf", ES_Resources::ES_TTF_FONT);

    // Initialisation serveur
	net = new ES_Network();
	// écoute sur le port 4400 TCP (pas d'indication sur l'adresse car on est en mode serveur !)
    if(net->init(ES_Network::ES_TCP, NULL, 4400) == -1) // NULL = MODE SERVEUR
    {
    	// si erreur on affiche le problème et on quitte
        std::cerr << net->get_error() << std::endl;
        ressources.clear_resources();
        delete fonts;
        delete graphics;
        return EXIT_FAILURE;
    }

	// on lance la fonction de réception dans un thread (processus séparé car elle est bloquante)
    ES_Threads *thread = new ES_Threads(reception, NULL);

	while (graphics->run())
	{
	    graphics->clear_screen(0, 125);

		while(event.poll_event())
		{
            switch(event.get_event_type())
            {
                case SDL_QUIT:
                    graphics->quit();
                    break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        case SDLK_ESCAPE:
                            graphics->quit(); // quitte si echap
                        break;
                    }
                break;
            }
		}

		// si il y'a une connection on accepte et on l'attribue à un socket temporaire (qui identifiera le client pour l'envoi)
		temp_sock = net->accept_tcp_connection();

		fonts->draw_text("police", "Le serveur est en route!", 0, 6, graphics->color(0,0,0), ES_Ttf::ES_TTF_LEFT, ES_Ttf::ES_BLENDED);

		graphics->flip(2);
	}

	// on libére la mémoire
	ressources.clear_resources(); // décharge toutes les ressources chargés
	delete thread;
	delete net;
	delete fonts;
	delete graphics;

	return EXIT_SUCCESS;
}
