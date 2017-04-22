// ES engine - Sample Touschscreen 1°
//- Montre l'utilisation de l'écran tactile avec la GP2X
#include <ES/es.h>
#include <ES/ES_Utils/utils.h>

using namespace ES;

int main(int argc, char **argv)
{
    ES_Graphics *graphics;
	ES_Event event;
	ES_MouseEvent mouse;
	bool gp2x_f200 = false;

    graphics = new ES_Graphics("ES engine - Touchscreen", 320, 240, 16, false, false, true);

    // test la version de la GP2X (la F100 ne dispose pas d'écrans tactiles)
    if(get_gp2x_version() == true)
        gp2x_f200 = true;

    // initialise l'écran tactile de la GP2X
    if(mouse.init_touchscreen() == false && gp2x_f200 == false)
    {
        delete graphics;
        return 0;
    }

	while (graphics->run())
	{
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

        // Si l'écran reçoit une pression
        if(mouse.get_touchscreen_data()->Pressed)
            graphics->set_pixel(mouse.get_touchscreen_data()->x, mouse.get_touchscreen_data()->y); // affiche un pixel à la position de la pression

		graphics->flip();
	}

    // libére l'écran tactile
    mouse.close_touchscreen();

	delete graphics;

	return EXIT_SUCCESS;
}
