# ES
ES Engine is an old (2006 - 2008) open source 2D game engine done in C++. It is cross-platform and can use SDL 1.2 or OpenGL 2 for the rendering.

[alt text](http://garzul.tonsite.biz/ES/images/platformer_sample.png "Platformer sample")

This engine was ported and was successfully used on the ARM [GP2X](https://en.wikipedia.org/wiki/GP2X), a Linux-based handheld video game console and portable media player developed by South Korean company GamePark Holdings, some app. such as [FPaint](http://dl.openhandhelds.org/cgi-bin/gp2x.cgi?0,0,0,0,4,2582) were made with this engine.

The integrated libraries include:
 * SDL
 * SDL Gfx
 * SDL Image
 * SDL Mixer
 * SDL Ttf
 * SDL Net
 * OpenGL
 * Lua
 * tinyXML
 * Box2D

Features:
 * OpenGL or SDL renderer
 * Powerful resources loading system which can load resources directly from a single packed file (oldschool style ;))
 * Build-in functions for sprites rotation, transparencies etc.
 * Built-in set of drawing functions (pixels, circle, lines, rectangles etc.)
 * Built-in set of transitions (screen fading)
 * 2D RPG map renderer (based on the code of this [article](http://www.games-creators.org:80/wiki/R%C3%A9aliser_un_RPG_en_C_avec_SDL_et_FMOD/L%27%C3%A9diteur_de_carte))
 * Small Box2D wrapper for physics stuff
 * Small LUA wrapper
 * Small mySQL wrapper
 * Small SDL_net wrapper for network stuff
 * Handy utilities; writing/reading config. files, math functions, timers, framerate computation, threads, packer, md5
 * Draw TTF fonts text with style and justification
 * Specific functions for OpenGL (drawing 2D quads)
 * GP2X specific stuff (overclocking, applying MMU Hack, touchscreen support etc.)
 * Logs
 * Many examples
 * Documentation with Doxygen

This engine was bundled with a RPG map editor with tile-based collision system, the editor support four tiles layer (this is enhanced code from an article), the maps could be loaded by the engine and rendered very easily, the UI was made with the Guichan library, there was also a self made files packer which use the Boost library.

This is one of my first "big" library project. :)

[alt text](http://www.garzul.tonsite.biz/ES/images/estelante_es_engine.png "Estelante")

[alt text](http://garzul.tonsite.biz/ES/images/editor.png "2D RPG Map editor")
