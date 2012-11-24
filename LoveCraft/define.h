#ifndef DEFINE_H__
#define DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include <gl/GL.h>
#include <gl/GLU.h>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;


#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define MOUSE_SENSIBILITY			0.1f
#define MOUVEMENT_SPEED				0.2f
#define MOUVEMENT_SPEED_RUN			0.35f
#define MOUVEMENT_SPEED_BACKWARD	0.1f

typedef uint8 T;
enum BLOCK_TYPE {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS};


#define TEXTURE_PATH        "../lovecraft/media/textures/"
#define SHADER_PATH			"../lovecraft/media/shader/"
#define MODEL_PATH			"../lovecraft/media/models/"
#define VIEW_DISTANCE       128

#endif // DEFINE_H__
