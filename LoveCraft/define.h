#ifndef DEFINE_H__
#define DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

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

// Chunk size
#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define VIEW_DISTANCE				64

// Valeurs de l'interface
// Hauteur du cadre inférieur de l'interface
#define INTERFACE_BOTTOM_HEIGHT		64
// Largeur du cadre de gauche de l'interface
#define INTERFACE_SIDE_LEFT_WIDTH	64
// Largeur du cadre de droite de l'interface
#define INTERFACE_SIDE_RIGHT_WIDTH	32
// Hauteur du cadre supérieur de l'interface
#define INTERFACE_TOP_HEIGHT		32
// Nombre de sort disponible dans la barre de sorts (max 10)
#define SPELL_BAR_SPELL_NUMBER		10
// Taille (hxl) des images des sorts dans la barre de sorts
#define SPELL_ICON_SIZE				64
// Longueur de l'espace entre chaque images de sorts
#define SPELL_ICON_PADDING			5
// Épaisseur du contour des barres
#define PROGRESS_BAR_OUTLINE		5

// Valeurs quelconques
#define PII							3.141592654f

// Valeurs du jeu
// Valeurs de vitesse (m/s)
// Vitesse initiale de déplacement (obselete)
#define MOUVEMENT_SPEED_INI			4.f
// Vitesse maximale en mode déplacement normal
#define MOUVEMENT_SPEED_MAX			8.f
// Multiplicateur de la vitesse maximale en mode déplacement rapide
#define MOUVEMENT_SPEED_MAX_RUN_M	1.5f
// Multiplicateur de la vitesse maximale en mode déplacement lent
#define MOUVEMENT_SPEED_SLOW_M		0.6f
// Multiplicateur de la vitesse maximale en mode déplacement arrière
#define MOUVEMENT_SPEED_BACKWARD_M	0.4f
// Perte de vitesse à chaque tour de boucle. Modifier cette valeur changera la vitesse à laquelle le joueur ralenti
#define MOUVEMENT_SPEED_DECAY		0.5f
// Vitesse initiale acquise en santant
#define MOUVEMENT_SPEED_JUMP		6.5f

// Valeurs d'acceleration (m/s^2)
// Accélération de base
#define MOUVEMENT_ACCELERATION			12.f
// Multiplicateur de l'accélération lorsqu'on est dans les airs
#define MOVUEMENT_ACCELERATION_AIR_M	0.4f
// Multiplicateur de l'accélération lorsque le joueur coure
#define MOUVEMENT_ACCELERATION_RUN_M	1.5f
// Accélération en Y représentant la gravité
#define GRAVITY							9.81f

// Vie (v/s)
// Régénération passive de la vie
#define HEALTH_PASSIVE_REGEN		0.01f
// Multiplicateur de perte de vie en fonction de la vitesse lors d'un saut trop haut
#define HEALTH_GRAVITY_LOST			0.2f

// Energie (e/s)
// Minimum d'énergie
#define ENERGY_MINIMUM				0
// Maximum d'énergie
#define ENERGY_MAXIMUM				100
// Vitesse de régénération de l'énergie
#define ENERGY_REGEN				0.3f
// Vitesse que le joueur ne doit pas dépasser s'il veut regagner l'énergie
#define ENERGY_REGEN_THRESHOLD		5.f
// Vitesse de dépense de l'énergie
#define ENERGY_SPENDING				0.5f

// Mana (m/s)
// Régénération passive de la mana
#define MANA_PASSIVE_REGEN			0.05f

// Valeurs des options
#define MOUSE_SENSIBILITY			0.075f
#define CROSSHAIR_SIZE				64

typedef uint8 BlockType;
enum BLOCK_TYPE {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_BRICK, BTYPE_SAND, BTYPE_COUNT};

//Chemins d'accès
#define TEXTURE_PATH        "../lovecraft/media/textures/"
#define SHADER_PATH			"../lovecraft/media/shader/"
#define MODEL_PATH			"../lovecraft/media/models/"
#define SOUND_PATH			"../lovecraft/media/sound/"
#define SOUND_FOOT_PATH		SOUND_PATH "footsteps/"

//Network
#define NW_PORT				10512
#define NW_SERVER_ADDRESS	"127.0.0.1"

#endif // DEFINE_H__
