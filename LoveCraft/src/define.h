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
// Musique à l'ouverture du programme
#define OPTION_MUSIC_STARTING_VALUE	false
// Son à l'ouverture du programme
#define OPTION_SOUND_STARTING_VALUE	true
// Montrer les informations à l'ouverture du programme
#define OPTION_INFO_STARTING_VALUE	true

// Nombre de sons que chaque type a
#define SOUND_FOOT_NUMBER			4
// Nombre de temps en secondes qui devront être écoulées 
// avant qu'un autre son se fasse jouer
#define SOUND_FOOT_TIMEOUT			0.4f

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

// Valeurs des controles
#define PNL_SCREEN_CONTROLS_NBR		5	//les 4 cadres externes et la zone de jeu
#define PNL_PLAYSCREEN_CONTROLS_NBR	4	//1 Panel de portrait, 2 images, 1 bar
#define PNL_PORTRAIT_CONTROLS_NBR	7	//3 bars, 3 labels, 

// Panel portrait
#define PNL_PORTRAIT_NAME			"pnl_portrait"
#define PNL_PORTRAIT_SIZE_W			512
#define PNL_PORTRAIT_SIZE_H			128
#define PNL_PORTRAIT_POSITION_X		20
#define PNL_PORTRAIT_POSITION_Y		0
// Picture box dans le panel portrait
#define PB_PORTRAIT_NAME			"pb_portrait"
#define PB_PORTRAIT_SIZE_W			97
#define PB_PORTRAIT_SIZE_H			97
#define PB_PORTRAIT_POSITION_X		14
#define PB_PORTRAIT_POSITION_Y		19
// Bars dans le panel portrait
#define PGB_HEALTH_NAME				"pgb_health"
#define PGB_HEALTH_BACKGROUND		false
#define PGB_HEALTH_POSITION_X		143
#define PGB_HEALTH_POSITION_Y		86
#define PGB_HEALTH_SIZE_W			250
#define PGB_HEALTH_SIZE_H			20
#define PGB_MANA_NAME				"pgb_mana"
#define PGB_MANA_BACKGROUND			false
#define PGB_MANA_POSITION_X			143
#define PGB_MANA_POSITION_Y			56
#define PGB_MANA_SIZE_W				250
#define PGB_MANA_SIZE_H				20
#define PGB_EXP_NAME				"pgb_exp"
#define PGB_EXP_BACKGROUND			false
#define PGB_EXP_POSITION_X			143
#define PGB_EXP_POSITION_Y			26
#define PGB_EXP_SIZE_W				225
#define PGB_EXP_SIZE_H				20
// Labels dans le panel portrait
#define LBL_HEALTH_NAME				"lbl_health"
#define LBL_HEALTH_POSITION_X		400
#define LBL_HEALTH_POSITION_Y		86
#define LBL_HEALTH_SIZE_W			95
#define LBL_HEALTH_SIZE_H			20
#define LBL_HEALTH_AUTOSIZE			false
#define LBL_MANA_NAME				"lbl_mana"
#define LBL_MANA_POSITION_X			400
#define LBL_MANA_POSITION_Y			56
#define LBL_MANA_SIZE_W				95
#define LBL_MANA_SIZE_H				20
#define LBL_MANA_AUTOSIZE			false
#define LBL_EXP_NAME				"lbl_exp"
#define LBL_EXP_POSITION_X			377
#define LBL_EXP_POSITION_Y			26
#define LBL_EXP_SIZE_W				118
#define LBL_EXP_SIZE_H				20
#define LBL_EXP_AUTOSIZE			false
// Taille de la barre d'énergie
#define PGB_ENERGY_NAME				"pgb_energy"
#define PGB_ENERGY_BACKGROUND		true
#define PGB_ENERGY_POSITION_X		10
#define PGB_ENERGY_POSITION_Y		150
#define PGB_ENERGY_SIZE_W			250
#define PGB_ENERGY_SIZE_H			20
// Taille (LxH) des boutons des sorts dans la barre de sorts
#define SPELL_ICON_SIZE				64
// Longueur de l'espace entre chaque images de sorts
#define SPELL_ICON_PADDING			10
// Épaisseur du contour des barres
#define PROGRESS_BAR_OUTLINE		5

// Valeurs en lien avec les particles
// Nombre maximum de particles à la fois
#define PARTICLE_COUNT				500

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
#define MOUVEMENT_SPEED_DECAY		0.9f
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
// Maximum de vie
#define HEALTH_MAX					100
// Régénération passive de la vie
#define HEALTH_PASSIVE_REGEN		0.01f
// Multiplicateur de perte de vie en fonction de la vitesse lors d'un saut trop haut
#define HEALTH_GRAVITY_LOST			0.2f

// Energie (e/s)
// Maximum d'énergie
#define ENERGY_MAX					150
// Vitesse de régénération de l'énergie
#define ENERGY_REGEN				0.3f
// Vitesse que le joueur ne doit pas dépasser s'il veut regagner l'énergie
#define ENERGY_REGEN_THRESHOLD		5.f
// Vitesse de dépense de l'énergie
#define ENERGY_SPENDING				0.5f

// Mana (m/s)
// Maximum de mana
#define MANA_MAX					50
// Régénération passive de la mana
#define MANA_PASSIVE_REGEN			0.05f

// Valeurs des options
#define MOUSE_SENSIBILITY			0.075f
#define CROSSHAIR_SIZE				64
//#define MOUSE_CLICK_TIMEOUT_3RDP	0.05f

typedef uint8 BlockType;
enum BLOCK_TYPE {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_BRICK, BTYPE_SAND, BTYPE_ROCK, BTYPE_SNOW, BTYPE_COUNT};
typedef uint8 Image;
enum IMAGE
{
	IMAGE_BLACK_BACK,
	IMAGE_BOO,
	IMAGE_RUN,
	IMAGE_CROSSHAIR,
	IMAGE_FONT,
	IMAGE_INTERFACE_FRAME,
	IMAGE_PORTRAIT_FRAME,
	IMAGE_PORTRAIT_MALE,
	IMAGE_PGBTEXT_HEALTH,
	IMAGE_PGBTEXT_ENERGY,
	IMAGE_PGBTEXT_MANA,
	IMAGE_PGBTEXT_EXP,
	IMAGE_LAST
};

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
