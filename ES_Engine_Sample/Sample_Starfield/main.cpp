// ES engine - Sample Starfield
//- Crée un effet Starfield pseudo 3D
#include <ES/ES_Display/graphics.h>
#include <ES/ES_Event/event.h>

using namespace ES;

int main(int argc, char **argv)
{
    ES_Graphics *graphics;
	ES_Event event;

    // première valeur = nombres d'étoiles
    int stars[1001][6];

	graphics = new ES_Graphics("ES engine - Starfield", 320, 240, 16, false, false, true);

    // permet de générer aléatoirement des nombres
    srand(graphics->get_time());

    // crée et place les premières étoiles aléatoirement
    for(int i=0;i<1001;i++)
    {
		stars[i][0]=(rand()%4000)-2000;
		stars[i][1]=(rand()%4000)-2000;
		stars[i][2]=(rand()%4000)+1000;
		stars[i][5]=(25+(rand()%100));
    }

    // on indique qu'on souhaite seulement un taux d'affichage de 30fps
    graphics->set_framerate(30);

    // récupére le temp
    int time = graphics->get_time();

	while (graphics->run())
	{
		graphics->clear_screen();

		while(event.poll_event())
		{
            switch(event.get_event_type())
            {
                case SDL_JOYBUTTONDOWN:
                    switch(event.get_sdlevent().jbutton.button)
                    {
                        case GP2X_BUTTON_START:
                            graphics->quit();
                        break;
                    }
                break;

                case SDL_QUIT:
                    graphics->quit();
                    break;

                case SDL_KEYDOWN:
                    switch (event.get_event_key_type())
                    {
                        case SDLK_ESCAPE:
                            graphics->quit();
                        break;
                    }
                break;
            }
		}

        // bouge les étoiles, les replaces si elles sont plus visibles
        for(int i=0;i<1001;i++)
        {
			stars[i][2]=stars[i][2]-stars[i][5];
			if(stars[i][2]<=0)
			{
				stars[i][0]=(rand()%4000)-2000;
				stars[i][1]=(rand()%4000)-2000;
				stars[i][2]=(rand()%4000)+1000;
				stars[i][5]=(25+(rand()%50))*time/25;
			}
			stars[i][3]=(stars[i][0]*100)/stars[i][2];
			stars[i][4]=(stars[i][1]*100)/stars[i][2];
			// affiche les étoiles
			graphics->set_pixel(320/2+stars[i][3], 240/2+stars[i][4]);
        }

		graphics->flip();
		// on reste à 30fps
		graphics->framerate_delay();
	}

	delete graphics;

	return EXIT_SUCCESS;
}
