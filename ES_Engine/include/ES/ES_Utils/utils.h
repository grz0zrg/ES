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

#ifndef _ES_UTILS_H_
#define _ES_UTILS_H_

 #include <sstream>
 #include <cmath>
 #ifdef GP2X
    #include <fcntl.h>
    #include <sys/mman.h>
  #define SYS_CLK_FREQ 7372800
 #endif

 template <typename T>

 // thank's to squidge for the mmu hack and gp2x wiki for others optimization related stuff

 /**
 * Convertit un string en n'importe quel type
 *
 * @param str chaîne à convertir
 * @param dest destination
 */

 bool convertFromStr(const std::string & Str, T & Dest)
 {
    std::istringstream iss(Str);
    return iss >> Dest != 0;
 }

 /**
 * Convertit n'importe quel type connu en string
 *
 * @param arg valeur à transformer
 * @return string
 */

 template <typename T>
 std::string toStr(const T &arg)
 {
    std::ostringstream oss;
    oss << arg;
    return oss.str();
 }

 /**
 * Convertit radian en degrées
 *
 * @param rad valeur radian
 * @return rad transformé en degrée
 */

 double rad2deg(double rad);

 /**
 * Convertit degrées en radian
 *
 * @param deg valeur degrée
 * @return deg transformé en radian
 */

 double deg2rad(double deg);

 /**
 * éleve à la ^ 2
 *
 * @param input valeur à élever
 * @return input à la ^2
 */

 int powerOfTwo(int input);

 /**
 * Interpolation linéaire
 *
 * @param y1 premier point
 * @param y2 deuxiéme point
 * @param mu décalage
 * @return valeur interpolé
 */

 double linearInterpolate(double y1, double y2, double mu);

 /**
 * Interpolation cubique
 *
 * @param y1 premier point
 * @param y2 deuxiéme point
 * @param y3 troisième point
 * @param y4 quatrième point
 * @param mu décalage
 * @return valeur interpolé
 */

 double cubicInterpolate(double y0, double y1, double y2, double y3, double mu);

 /**
 * Redirige la sortie stdout sdl vers la console
 */

 void stdoutRedirect();

 /**
 * Redirige la sortie stderr sdl vers la console
 */

 void stderrRedirect();

 /**
 * Active le squidgehack. Meilleur rapidité de l'application.
 * Pour activer ce hack placez mmu_hack.o à coté de votre application gp2x
 * @return true si l'application du hack à réussi
 */
 bool mmuHack();

 /**
 * Overclock le CPU de la GP2X.
 * @param MHZ nouvelle fréquence (maxi = 266)
 */
 void setCpuClock(unsigned int MHZ);

 /**
 * Active le ram hack sur GP2X. Meilleur rapidité de l'application.
 */
 void ramHack();

 /**
 * Active le ram hack & mmu hack sur GP2X. Meilleur rapidité de l'application.
 * @param MHZ fréquence (maxi = 266)
 */
 void optimizeGP2X(unsigned int MHZ = 266, bool ramhack = true, bool mmuhack = true);

 /**
 * Retourne la version de la GP2X utilisé.
 *
 * @return true = F200, false = F100
 */

 bool getGP2X_Version();

#endif
