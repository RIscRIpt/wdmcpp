#pragma once

#define __PLACEMENT_NEW_INLINE
#define _NO_CRT_STDIO_INLINE

void *__cdecl operator new(size_t count);
void *__cdecl operator new(size_t, void *address);
void __cdecl operator delete(void *Object);
void __cdecl operator delete(void *, void *);
void __cdecl operator delete(void *Object, size_t);

void InvokeCtors();
void InvokeDtors();
