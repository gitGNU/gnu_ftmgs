/*----------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain           */
/*                                                                            */
/* This file is part of the <FTMGS> Library                                   */
/*                                                                            */
/* This library is free software: you can redistribute it and/or              */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation, either               */
/* version 2.1 of the License, or (at your option) any later                  */
/* version.                                                                   */
/*                                                                            */
/* This library is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               */
/* GNU Lesser General Public License for more details.                        */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General                  */
/* Public License along with this library. If not, see                        */
/* <http://www.gnu.org/licenses/>.                                            */
/*----------------------------------------------------------------------------*/
/* 
 * System Dependencies
 */
/*----------------------------------------------------------------------------*/
#include "system_rnd.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#if defined __linux__ || defined __unix__ || defined __APPLE__
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined _WIN32 || defined _MSC_VER || defined __WIN32__
#include <windows.h>
#include <process.h>
#include <Wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#endif

/*----------------------------------------------------------------------------*/
extern unsigned memcat(void* dst, unsigned* len, unsigned maxlen,
					   const void* org, unsigned n);
/*----------------------------------------------------------------------------*/
unsigned entropy(void* dst, unsigned maxlen, unsigned entropy_src)
{
	unsigned len = 0;
#if defined __linux__ || defined __unix__ || defined __APPLE__
	int fd;
	const char* random_device;
	if (entropy_src == 0) {
		random_device = "/dev/random";
	} else {
		random_device = "/dev/urandom";
	}
	if ((fd = open(random_device,O_RDONLY)) > 0) {
		ssize_t nb = 0;
		do {
			if ((nb = read(fd, (char*)(dst)+len, maxlen)) > 0) {
				len += (unsigned)nb;
				maxlen -= (unsigned)nb;
			}
		} while (nb > 0 && maxlen > 0);
	}
#elif defined _WIN32 || defined _MSC_VER || defined __WIN32__
	HCRYPTPROV hProvider = 0;
	if (CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL,
							CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
		if (CryptGenRandom(hProvider, maxlen, (char*)dst)) {
			len = maxlen;
		}
	}
	CryptReleaseContext(hProvider, 0);
	entropy_src = 0;
#else
#error "Undefined entropy method for this system"
#endif
	return len;
}
/*-----------------------------------------------------------------*/
unsigned nonce(void* dst, unsigned maxlen)
{
	unsigned len = 0;
#if defined __linux__ || defined __unix__ || defined __APPLE__
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		if (len < maxlen && tv.tv_sec > 0) {
			memcat(dst, &len, maxlen, &tv.tv_sec, sizeof(tv.tv_sec));
		}
		if (len < maxlen && tv.tv_usec > 0) {
			memcat(dst, &len, maxlen, &tv.tv_usec, sizeof(tv.tv_usec));
		}
	}
	{
		pid_t pid = getpid();
		if (len < maxlen && pid > 0) {
			memcat(dst, &len, maxlen, &pid, sizeof(pid));
		}
	}
#elif defined _WIN32 || defined _MSC_VER || defined __WIN32__
	{
		FILETIME tv;
		GetSystemTimeAsFileTime(&tv);
		tv.dwLowDateTime;
		tv.dwHighDateTime;
		if (len < maxlen && tv.dwLowDateTime > 0) {
			memcat(dst, &len, maxlen,
				   &tv.dwLowDateTime,sizeof(tv.dwLowDateTime));
		}
		if (len < maxlen && tv.dwHighDateTime > 0) {
			memcat(dst, &len, maxlen,
				   &tv.dwHighDateTime,sizeof(tv.dwHighDateTime));
		}
	}
	{
		int pid = __getpid();
		if (len < maxlen && pid > 0) {
			memcat(dst, &len, maxlen, &pid, sizeof(pid));
		}
	}
#else
	{
		time_t t = time(NULL);
		if (len < maxlen && t > 0) {
			memcat(dst, &len, maxlen, &t, sizeof(t));
		}
	}
#endif	
	{
		clock_t c = clock();
		if (len < maxlen && c > 0) {
			memcat(dst, &len, maxlen, &c, sizeof(c));
		}
	}
	return len;
}
/*----------------------------------------------------------------------------*/
