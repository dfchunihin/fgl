#ifndef _COMMON_H_
#define _COMMON_H_

#define NDEBUG

#define _USE_MATH_DEFINES

#ifndef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING 0 //STL Debug makes FGL slowpoke
#endif

#ifndef _SECURE_SCL
#define  _SECURE_SCL 0 //STL Debug makes FGL slowpoke
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>

#define GLEW_STATIC
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment (lib,"opengl32.lib")

#include <math.h>
#include <time.h>

#include <memory>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/container/vector.hpp>
#include <boost/container/stable_vector.hpp>
#include <boost/container/map.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/container/deque.hpp>

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#pragma comment (lib,"winapims.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"DevIL.lib")
#pragma comment (lib,"ILU.lib")
#pragma comment (lib,"ILUT.lib")


#endif
