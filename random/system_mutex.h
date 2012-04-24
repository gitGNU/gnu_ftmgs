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
#ifndef umalccvbg_system_mutex_h__
#define umalccvbg_system_mutex_h__	1
#ifdef __cplusplus
extern "C" {
#endif
#if defined NTHREADS__
#define MUTEX_VAR(mutex_var__)
#define CREATE_MUTEX(mutex_var__)
#define BEGIN_MUTEX(mutex_var__)
#define END_MUTEX(mutex_var__)
#define DESTROY_MUTEX(mutex_var__)
#elif defined __linux__ || defined __unix__ || defined __APPLE__
#include <pthread.h>
#define MUTEX_VAR(mutex_var__)		pthread_mutex_t mutex_var__;
#define CREATE_MUTEX(mutex_var__)	pthread_mutex_init(mutex_var__, NULL)
#define BEGIN_MUTEX(mutex_var__)	do{{pthread_mutex_lock(mutex_var__)
#define END_MUTEX(mutex_var__)		pthread_mutex_unlock(mutex_var__);}}while(0)
#define DESTROY_MUTEX(mutex_var__)	pthread_mutex_destroy(mutex_var__)
#elif defined _MSC_VER || defined _WIN32 || defined __WIN32__
#include <windows.h>
#define MUTEX_VAR(mutex_var__)		CRITICAL_SECTION mutex_var__;
#define CREATE_MUTEX(mutex_var__)	InitializeCriticalSection(mutex_var__)
#define BEGIN_MUTEX(mutex_var__)	do{{EnterCriticalSection(mutex_var__)
#define END_MUTEX(mutex_var__)		LeaveCriticalSection(mutex_var__);}}while(0)
#define DESTROY_MUTEX(mutex_var__)	DeleteCriticalSection(mutex_var__)
/* 
 * #define MUTEX_VAR(mutex_var__)		HANDLE mutex_var__;
 * #define CREATE_MUTEX(mutex_var__)	mutex_var__ = CreateMutex(NULL, 0, NULL)
 * #define BEGIN_MUTEX(mutex_var__)	do{{WaitForSingleObject(mutex_var__,INFINITE)
 * #define END_MUTEX(mutex_var__)		ReleaseMutex(mutex_var__);}}while(0)
 * #define DESTROY_MUTEX(mutex_var__)	CloseHandle(mutex_var__)
 */
#else
#error "No threads defined for this system"
#endif
#ifdef __cplusplus
}
#endif
#endif
