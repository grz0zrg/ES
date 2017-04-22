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

#include "ES_Map/map.h"

namespace ES
{
	ES_Map::ES_Map(){}

	ES_Map::~ES_Map()
	{
		if(&map!=NULL)
			free_map(&map);

		r = g = b = 0; a = 255;
	}

	int ES_Map::loadMap(const std::string& esm_file, short int alpha, unsigned short int r, unsigned short int v, unsigned short int b/*, bool espack, ES_Pack* pack*/)
	{
	    #ifdef ES_USE_OPENGL
		return load_esmap(&map, esm_file.c_str(), alpha, r, v, b);
		#else
		r = r; v = v; b = b;
		return load_esmap(&map, esm_file.c_str(), alpha);
		#endif
	}

	void ES_Map::drawBG(short int n_layer, int x, int y, int w, int h, int tile_w, int tile_h)
	{
		if(tile_w == 0 && tile_h == 0)
		{
			z_tile_w = map.tile_w;
			z_tile_h = map.tile_h;
		}
		else if(tile_w >= map.tile_w || tile_h >= map.tile_h)
		{
			z_tile_w = map.tile_w;
			z_tile_h = map.tile_h;
		}
		else
		{
			z_tile_w = tile_w;
			z_tile_h = tile_h;
		}

        if(n_layer == 0 || n_layer == 1 || n_layer == 2)
	    #ifdef ES_USE_OPENGL
		draw_bg_map(&map, n_layer, x, y, z_tile_w, z_tile_h, w, h, this->r ,this->g ,this->b, this->a);
		#else
		draw_bg_map(&map, n_layer, x, y, z_tile_w, z_tile_h, w, h);
		#endif
	}

	void ES_Map::drawLayer(short int n_layer, int x, int y, int w, int h, int tile_w, int tile_h)
	{
		if(tile_w == 0 && tile_h == 0)
		{
			lz_tile_w = map.tile_w;
			lz_tile_h = map.tile_h;
		}
		else if(tile_w >= map.tile_w || tile_h >= map.tile_h)
		{
			lz_tile_w = map.tile_w;
			lz_tile_h = map.tile_h;
		}
		else
		{
			lz_tile_w = tile_w;
			lz_tile_h = tile_h;
		}

		if(n_layer == 0 || n_layer == 3 || n_layer == 4)
			draw_layer(&map, n_layer, x, y, lz_tile_w, lz_tile_h, w, h, this->r ,this->g ,this->b, this->a);

	}

	void ES_Map::centerScroll(int x, int y)
	{
		if(x < (SDL_GetVideoSurface()->w/2))
			map.offset_x = 0;
		else
		{
			map.offset_x = x-(SDL_GetVideoSurface()->w/2);
			if(map.offset_x > map.width*map.tile_w-SDL_GetVideoSurface()->w) map.offset_x = map.width*map.tile_w-SDL_GetVideoSurface()->w;
		}

		if(y < (SDL_GetVideoSurface()->h/2))
			map.offset_y = 0;
		else
		{
			map.offset_y = y-(SDL_GetVideoSurface()->h/2);
			if(map.offset_y > map.height*map.tile_h-SDL_GetVideoSurface()->h) map.offset_y = map.height*map.tile_h-SDL_GetVideoSurface()->h;
		}
	}

	void ES_Map::move(int x, int y)
	{
	    if(x >= 0 && y >= 0 && x <= map.width*map.tile_w && y <= map.height*map.tile_h)
            scroll(&map, x, y, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);
	}

	bool ES_Map::tilesCollide(int x, int y, int offsetl, int offsetr, int offsetu, int offsetd)
	{
		hgx = (x+offsetl)/map.tile_w;
		hgy = (y+offsetu)/map.tile_h;

		hdx = (x+offsetl+map.tile_w-offsetr)/map.tile_w;
		hdy = (y+offsetu)/map.tile_h;

		bgx = (x+offsetl)/map.tile_w;
		bgy = (y+offsetu+map.tile_h-offsetd)/map.tile_h;

		bdx = (x+offsetl+map.tile_w-offsetr)/map.tile_w;
		bdy = (y+offsetu+map.tile_h-offsetd)/map.tile_h;

		if (  (map.tileset[GET_TILEp(hgx,hgy,map)].col==0) || (map.tileset[GET_TILEpl2(hgx,hgy,map)].col==0) || (map.tileset[GET_TILEpl3(hgx,hgy,map)].col==0) || (map.tileset[GET_TILEpl4(hgx,hgy,map)].col==0)
			&& (map.tileset[GET_TILEp(hdx,hdy,map)].col==0) || (map.tileset[GET_TILEpl2(hdx,hdy,map)].col==0) || (map.tileset[GET_TILEpl3(hdx,hdy,map)].col==0) || (map.tileset[GET_TILEpl4(hdx,hdy,map)].col==0)
			&& (map.tileset[GET_TILEp(bgx,bgy,map)].col==0) || (map.tileset[GET_TILEpl2(bgx,bgy,map)].col==0) || (map.tileset[GET_TILEpl3(bgx,bgy,map)].col==0) || (map.tileset[GET_TILEpl4(bgx,bgy,map)].col==0)
			&& (map.tileset[GET_TILEp(bdx,bdy,map)].col==0) || (map.tileset[GET_TILEpl2(bdx,bdy,map)].col==0) || (map.tileset[GET_TILEpl3(bdx,bdy,map)].col==0) || (map.tileset[GET_TILEpl4(bdx,bdy,map)].col==0))
		{
			return true;
		}

		return false;
	}

	void ES_Map::setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
	{
		this->r = r; this->g = g; this->b = b; this->a = a;
	}

	int ES_Map::getOffX()
	{
		return map.offset_x;
	}

	int ES_Map::getOffY()
	{
		return map.offset_y;
	}

	unsigned int ES_Map::getWidth()
	{
		return map.width;
	}

	unsigned int ES_Map::getHeight()
	{
		return map.height;
	}
}
