#ifndef __ILUT_CONFIG_H__
#define __ILUT_CONFIG_H__

#define IL_USE_PRAGMA_LIBS

// Supported APIs (ILUT)

//
// sorry just
// cant get this one to work under windows
// have disabled for the now
//
// will look at it some more later
//
// Kriss
//
#undef ILUT_USE_ALLEGRO

#ifdef fglOGL
#undef ILUT_USE_DIRECTX8
#undef ILUT_USE_DIRECTX9
#undef ILUT_USE_DIRECTX10
#define ILUT_USE_OPENGL
#undef ILUT_USE_SDL
#endif

#ifdef fglD3D9
#undef ILUT_USE_DIRECTX8
#define ILUT_USE_DIRECTX9
#undef ILUT_USE_DIRECTX10
#undef ILUT_USE_OPENGL
#undef ILUT_USE_SDL
#endif

#ifdef fglD3D10
#undef ILUT_USE_DIRECTX8
#undef ILUT_USE_DIRECTX9
#define ILUT_USE_DIRECTX10
#undef ILUT_USE_OPENGL
#undef ILUT_USE_SDL
#endif

#define ILUT_USE_WIN32

#endif//__ILUT_CONFIG_H__
