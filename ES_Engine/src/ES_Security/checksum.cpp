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

#include "ES_Security/checksum.h"

namespace ES
{
	int ES_Checksum::md5(const char *fp_exe)
	{
		FILE *fp;
		md5_state_t state;
		md5_byte_t digest[16];
		char *hex_output = (char *)malloc(33);
		char data[64];
		int di;
		size_t ln;

		fp = fopen (fp_exe, "rb");
		if (fp==NULL) {return -1; exit (EXIT_FAILURE);}

		md5_init(&state);
		while(!feof(fp))
		{
			ln = fread(data, 1, sizeof(data), fp);
			md5_append(&state, (const md5_byte_t *)data, (int)ln);
		}
		md5_finish(&state, digest);

		for (di = 0; di < 16; di++){
			sprintf(hex_output + di * 2, "%02x", digest[di]);
		}

		fclose(fp);

		md5_hash=toStr(hex_output);

		return 0;
	}

	std::string ES_Checksum::getMD5()
	{
		return md5_hash;
	}
}
