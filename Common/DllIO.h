#pragma once

#ifdef ITEMSERVER
#define	DLL	__declspec(dllexport)
#endif

#ifdef ITEMCLIENT
#define	DLL	__declspec(dllimport)
#endif