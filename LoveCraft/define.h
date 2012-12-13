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

//Chunk size
#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

//Valeurs de l'interface
#define INTERFACE_BOTTOM_HEIGHT		64.f
#define INTERFACE_SIDE_LEFT_WIDTH	64.f
#define INTERFACE_SIDE_RIGHT_WIDTH	32.f
#define INTERFACE_TOP_HEIGHT		32.f


//Valeurs quelconques
#define PII							3.141592654f

//Valeurs du jeu
#define MOUVEMENT_SPEED				0.15f
#define MOUVEMENT_SPEED_RUN			0.3f
#define MOUVEMENT_SPEED_BACKWARD	0.075f

//Valeurs des options
#define MOUSE_SENSIBILITY			0.075f
#define CROSSHAIR_SIZE				64

typedef uint8 BlockType;
enum BLOCK_TYPE {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_BRICK, BTYPE_SAND, BTYPE_EMPTY};


#define TEXTURE_PATH        "../lovecraft/media/textures/"
#define SHADER_PATH			"../lovecraft/media/shader/"
#define MODEL_PATH			"../lovecraft/media/models/"
#define VIEW_DISTANCE       128

#endif // DEFINE_H__
