/*
* Copyright (c) 2006, 2007, 2008 Julien Verneuil
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

#include "ES_Event/mouse_event.h"

// FIXME: Clean the touchscreen code (the initial touchscreen code is from gp2x spain forum)
namespace ES
{
    #ifdef GP2X
    typedef struct {
        uint16_t pressure;
        uint16_t x;
        uint16_t y;
        uint16_t padding;
        struct timeval stamp;
    } HWTouchScreen;

    struct {
        uint16_t x;
        uint16_t y;
    } OldTouchScreen[SAMPLES_TOUCHSCREEN];
    uint32_t OldIndex;

    HWTouchScreen HWts;
    #endif
    TouchScreen* ts;

    static int ts_cal[7] = {6203, 0, -1501397, 0, -4200, 16132680, 65536};

    bool ES_MouseEvent::initTouchscreen()
    {
        #ifdef GP2X
        fd = open("/dev/touchscreen/wm97xx", O_RDONLY | O_NOCTTY);
        if(!fd)
            return false;
        ts = (TouchScreen*)malloc(sizeof(TouchScreen));

        FILE* pointercal = fopen("/etc/pointercal", "r");
        if(!pointercal)
            return false;

        fscanf(pointercal, "%d %d %d %d %d %d %d", ts_cal+0, ts_cal+1, ts_cal+2, ts_cal+3, ts_cal+4, ts_cal+5, ts_cal+6);
        fclose(pointercal);

        OldIndex = 0;
        #endif
        return true;
    }

	SDL_Rect *ES_MouseEvent::getCursorRect()
	{
		SDL_GetMouseState(&x_mouse,&y_mouse);
		cursor_position.x = x_mouse;
		cursor_position.y = y_mouse;

		return &cursor_position;
	}

	int ES_MouseEvent::getX()
	{
        SDL_GetMouseState(&x_mouse,&y_mouse);

		return x_mouse;
	}

    void ES_MouseEvent::setCursorPosition(int x, int y)
    {
        SDL_WarpMouse(x, y);
    }

	int ES_MouseEvent::getY()
	{
        SDL_GetMouseState(&x_mouse,&y_mouse);

		return y_mouse;
	}

	TouchScreen *ES_MouseEvent::getTouchscreenData()
	{
		#ifdef GP2X
        int i;
        // Si no hemos abierto el device o ha fallado el init devolvemos NULL.
        if (fd < 0) return NULL;

        // Leemos los datos de la pantalla táctil y lo convertimos a coordenadas de la pantalla.
        read(fd, &HWts, sizeof(HWTouchScreen));

        // Calculamos en NewPress y el Pressed.
        ts->NewPress = !ts->Pressed;
        ts->Pressed = (HWts.pressure>0);
        ts->NewPress &= ts->Pressed;

        // Si es un nuevo toque seteamos todos los valores antiguos al nuevo
        if (ts->NewPress) {
            for (i = 0; i < SAMPLES_TOUCHSCREEN; i++) {
                OldTouchScreen[i].x = HWts.x;
                OldTouchScreen[i].y = HWts.y;
            }
        }
        // Sino es nuevo pero esta presionada actualizamos la posición más antigua por la nueva.
        else if (ts->Pressed) {
            OldTouchScreen[OldIndex].x = HWts.x;
            OldTouchScreen[OldIndex].y = HWts.y;
            OldIndex = (OldIndex + 1)%SAMPLES_TOUCHSCREEN;
        }

        // Hacemos la media de los ultimos valores y calculamos.
        int x, y, _x=0, _y=0;
        for (i = 0; i < SAMPLES_TOUCHSCREEN; i++) {
            _x += OldTouchScreen[i].x;
            _y += OldTouchScreen[i].y;
        }
        _x /= SAMPLES_TOUCHSCREEN;
        _y /= SAMPLES_TOUCHSCREEN;
        x = (ts_cal[2] + ts_cal[0]*_x);
        y = (ts_cal[5] + ts_cal[4]*_y);
        if (ts_cal[6] == 65536) { x >>= 16; y >>=16; }
        else { x /= ts_cal[6]; y /= ts_cal[6]; }

        // Comprovamos que no estamos fuera de la pantalla (por un posible error de calibracion, etc.)
        if (x < 0) ts->x = 0;
        else if (x > 319) ts->x = 319;
        else ts->x = x;
        if (y < 0) ts->y = 0;
        else if (y > 239) ts->y = 239;
        else ts->y = y;

        return ts;
        #else
        return NULL;
		#endif
	}

	void ES_MouseEvent::closeTouchscreen()
	{
	    #ifdef GP2X
        close(fd);
        free(ts);
        #endif
	}
}
