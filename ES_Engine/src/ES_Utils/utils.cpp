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

#include "ES_Utils/utils.h"

double rad2deg(double rad)
{
    return 180.*rad/M_PI;
}

double deg2rad(double deg)
{
    return M_PI*deg/180.;
}

int powerOfTwo(int input)
{
    int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}

double linearInterpolate(double y1, double y2, double mu)
{
    if(mu >= 1)
        mu = 1;
    return(y1*(1-mu)+y2*mu);
}

// Wikipedia
double cubicInterpolate(double y0,double y1,double y2,double y3,double mu)
{
    double a0,a1,a2,a3,mu2;

    mu2 = mu*mu;
    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;

    return (a0*mu*mu2+a1*mu2+a2*mu+a3);
}

void stdoutRedirect()
{
    freopen("CON", "w", stdout );
}

void stderrRedirect()
{
    freopen("CON", "w", stderr);
}

bool mmuHack()
{
    #ifdef GP2X
    // charge le hack mmu
    int mmufd;
    system("/sbin/rmmod mmuhack");
    system("/sbin/insmod mmuhack.o");
    mmufd = open("/dev/mmuhack", O_RDWR);
    if(mmufd < 0)
        return false;

    close(mmufd);
    #endif
    return true;
}

void setCpuClock(unsigned int mhz)
{
    #ifdef GP2X
    if(mhz <= 266 && mhz >= 50)
    {
        // variables
        unsigned long gp2x_mem;
        unsigned short *gp2x_memregs;
        volatile unsigned short *MEM_REG;

        // open MEM_REG
        gp2x_mem = open("/dev/mem", O_RDWR);
        gp2x_memregs=(unsigned short *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, gp2x_mem, 0xc0000000);
        MEM_REG=&gp2x_memregs[0];

        //mhz = constrain(mhz,50,maxClock);
        unsigned v;
        unsigned mdiv,pdiv=3,scale=0;

        mhz*=1000000;
        mdiv=(mhz*pdiv)/SYS_CLK_FREQ;
        mdiv=((mdiv-8)<<8) & 0xff00;
        pdiv=((pdiv-2)<<2) & 0xfc;

        scale&=3;
        v=mdiv | pdiv | scale;
        MEM_REG[0x910>>1]=v;
    }
    #else
        mhz = 0;
    #endif
}

void ramHack()
{
    #ifdef GP2X
    unsigned long gp2x_mem;
    unsigned short *gp2x_memregs;
    volatile unsigned short *MEM_REG;

    // open MEM_REG
    gp2x_mem = open("/dev/mem", O_RDWR);
    gp2x_memregs=(unsigned short *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, gp2x_mem, 0xc0000000);
    MEM_REG=&gp2x_memregs[0];

    unsigned long interrupt_flags = MEM_REG[0x808>>2],

    CAS=0,tRC=6-1,tRAS=4-1,tWR=1-1,tMRD=1-1,tRFC=1-1,tRP=2-1,tRCD=2-1; //craigix's timings

      MEM_REG[0x0808>>2] = 0xFF8FFFE7;        // Mask interrupts

      //gp2x_memregs[0x0910>>1] = s;                 // Set clock and wait

      //while(gp2x_memregs[0x0902>>1] & 1);

      gp2x_memregs[0x3802>>1] = ((tMRD<<12)|(tRFC<<8)|(tRP<<4)|(tRCD)); //set RAM tweaks
      gp2x_memregs[0x3804>>1] = ((CAS<<12)|(tRC<<8)|(tRAS<<4)|(tWR));

      gp2x_memregs[0x0924>>1] = 0x8900 + ((1)<<8); // Set upll timing prescaler to 1 (0x5A00 for fw 1.0.x)

      MEM_REG[0x0808>>2] = interrupt_flags;   // Turn on interrupts

    close(gp2x_mem);
    #endif
}

bool getGP2X_Version()
{
    #ifdef GP2X
    int fd;
    fd = open("/dev/touchscreen/wm97xx", O_RDONLY | O_NOCTTY);
    if(!fd)
        return false;
    close(fd);
    #endif
    return true;
}

void optimizeGP2X(unsigned int MHZ, bool ramhack, bool mmuhack)
{
    #ifdef GP2X
    if(ramhack == true)
        ramHack();
    if(mmuhack == true)
        mmuHack();
    setCpuClock(MHZ);
    #else
    MHZ = 0;
    ramhack = false;
    mmuhack = false;
    #endif
}
