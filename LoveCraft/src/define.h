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

// Options
// Musique � l'ouverture du programme
#define OPTION_MUSIC_STARTING_VALUE	true
// Son � l'ouverture du programme
#define OPTION_SOUND_STARTING_VALUE	true
// Montrer les informations � l'ouverture du programme
#define OPTION_INFO_STARTING_VALUE	true

// Nombre de sons que chaque type a
#define SOUND_FOOT_NUMBER			4
// Nombre de temps en secondes qui devront �tre �coul�es 
// avant qu'un autre son se fasse jouer
#define SOUND_FOOT_TIMEOUT			0.4f
// Nombre de temps en secondes qui devront �tre �coul�es 
// avant qu'un autre son se fasse jouer lorsque le joueur coure
#define SOUND_FOOT_RUN_TIMEOUT		0.25f

// Valeurs de l'interface
// Hauteur du cadre inf�rieur de l'interface
#define INTERFACE_BOTTOM_HEIGHT		64
// Largeur du cadre de gauche de l'interface
#define INTERFACE_SIDE_LEFT_WIDTH	64
// Largeur du cadre de droite de l'interface
#define INTERFACE_SIDE_RIGHT_WIDTH	32
// Hauteur du cadre sup�rieur de l'interface
#define INTERFACE_TOP_HEIGHT		32
// Nombre de sort disponible dans la barre de sorts (max 10)
#define SPELL_BAR_SPELL_NUMBER		10
// Taille (LxH) des images des sorts dans la barre de sorts
#define SPELL_ICON_SIZE				64
// Longueur de l'espace entre chaque images de sorts
#define SPELL_ICON_PADDING			10
// �paisseur du contour des barres
#define PROGRESS_BAR_OUTLINE		5

// Valeurs en lien avec les particles
// Nombre maximum de particles � la fois
#define PARTICLE_COUNT				500

// Valeurs quelconques
#define PII							3.141592654f

// Valeurs du jeu
// Valeurs de vitesse (m/s)
// Vitesse initiale de d�placement (obselete)
#define MOUVEMENT_SPEED_INI			4.f
// Vitesse maximale en mode d�placement normal
#define MOUVEMENT_SPEED_MAX			8.f
// Multiplicateur de la vitesse maximale en mode d�placement rapide
#define MOUVEMENT_SPEED_MAX_RUN_M	1.5f
// Multiplicateur de la vitesse maximale en mode d�placement lent
#define MOUVEMENT_SPEED_SLOW_M		0.6f
// Multiplicateur de la vitesse maximale en mode d�placement arri�re
#define MOUVEMENT_SPEED_BACKWARD_M	0.4f
// Perte de vitesse � chaque tour de boucle. Modifier cette valeur changera la vitesse � laquelle le joueur ralenti
#define MOUVEMENT_SPEED_DECAY		0.9f
// Vitesse initiale acquise en santant
#define MOUVEMENT_SPEED_JUMP		6.5f

// Valeurs d'acceleration (m/s^2)
// Acc�l�ration de base
#define MOUVEMENT_ACCELERATION			12.f
// Multiplicateur de l'acc�l�ration lorsqu'on est dans les airs
#define MOVUEMENT_ACCELERATION_AIR_M	0.4f
// Multiplicateur de l'acc�l�ration lorsque le joueur coure
#define MOUVEMENT_ACCELERATION_RUN_M	1.5f
// Acc�l�ration en Y repr�sentant la gravit�
#define GRAVITY							9.81f

// Vie (v/s)
// Maximum de vie
#define HEALTH_MAX					100
// R�g�n�ration passive de la vie
#define HEALTH_PASSIVE_REGEN		0.01f
// Multiplicateur de perte de vie en fonction de la vitesse lors d'un saut trop haut
#define HEALTH_GRAVITY_LOST			0.2f

// Energie (e/s)
// Maximum d'�nergie
#define ENERGY_MAX					150
// Vitesse de r�g�n�ration de l'�nergie
#define ENERGY_REGEN				0.3f
// Vitesse que le joueur ne doit pas d�passer s'il veut regagner l'�nergie
#define ENERGY_REGEN_THRESHOLD		5.f
// Vitesse de d�pense de l'�nergie
#define ENERGY_SPENDING				0.5f

// Mana (m/s)
// Maximum de mana
#define MANA_MAX					50
// R�g�n�ration passive de la mana
#define MANA_PASSIVE_REGEN			0.05f

// Valeurs des options
#define MOUSE_SENSIBILITY			0.075f
#define CROSSHAIR_SIZE				64
//#define MOUSE_CLICK_TIMEOUT_3RDP	0.05f

typedef uint8 BlockType;
enum BLOCK_TYPE {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_BRICK, BTYPE_SAND, BTYPE_COUNT};

//Chemins d'acc�s
#define TEXTURE_PATH        "../lovecraft/media/textures/"
#define SHADER_PATH			"../lovecraft/media/shader/"
#define MODEL_PATH			"../lovecraft/media/models/"
#define SOUND_PATH			"../lovecraft/media/sound/"
#define SOUND_FOOT_PATH		SOUND_PATH "footsteps/"

//Network
#define NW_PORT				10512
#define NW_SERVER_ADDRESS	"127.0.0.1"

#endif // DEFINE_H__
