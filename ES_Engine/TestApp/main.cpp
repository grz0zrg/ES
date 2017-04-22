/* Programme de tests/débug & optimisations futures du moteur */

#include <ES/ES_Display/graphics.h>
#include <ES/ES_Display/ttf.h>
#include <ES/ES_Event/event.h>
#include <ES/ES_Utils/utils.h>

#define R_MODE ES_Graphics::ES_SDL

using namespace ES;

int main(int argc, char *argv[])
{
    int x_img= 0, y_img = 0, vel_x = 1;

    argc = 0;
    argv = NULL;

    ES_Resources res;
    ES_Event event;

#ifdef GP2X
    ES_Graphics *graphics = new ES_Graphics("ES - TestApp", 320, 240, 16, false, R_MODE, true);
#else
    ES_Graphics *graphics = new ES_Graphics("ES - TestApp", 640, 480, 32, false, R_MODE, true);
#endif

    if(R_MODE == ES_Graphics::ES_OPENGL)
    {
        graphics->openGL_Init();
        graphics->setOrthoView();
    }

    ES_Ttf *font = new ES_Ttf();
    font->setSize(18);
    res.add("global font", "arial.ttf", ES_Resources::ES_TTF_FONT);

    res.add("test img", "img.png", ES_Resources::ES_IMG, false, NULL, true, 255, 0, 255);

    event.setKeyRepeat(100, 2);

    while(graphics->run())
    {

        graphics->cls();

        while(event.pollEvent())
        {
            switch(event.getEventType())
            {
                case ES_JOYBUTTONDOWN:
                    switch(event.getEvent().jbutton.button)
                    {
                        case GP2X_BUTTON_START:
                            graphics->quit();
                        break;
                    }
                break;

                case ES_QUIT:
                    graphics->quit();
                break;
            }

        }

        for(int i = 0; i < graphics->getWindowHeight(); i+=16)
            graphics->drawIMG("test img", graphics->rect(0, 0, 64, 16), graphics->rect(x_img, y_img+i, 64, 16), 255, true);

        if(x_img > graphics->getWindowWidth()-64)
            vel_x = -vel_x;
        else if(x_img < 0)
            vel_x = -vel_x;

        x_img += vel_x;

        graphics->flip(5);
    }

    res.free();
    delete font;
    delete graphics;

    return EXIT_SUCCESS;
}
