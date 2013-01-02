#ifndef INTERFACE_H_
#define INTERFACE_H_

enum IMAGE
{
	IMAGE_BLACK_BACK,
	IMAGE_BOO,
	IMAGE_RUN,
	IMAGE_CROSSHAIR,
	IMAGE_INTERFACE_FRAME,
	IMAGE_PORTRAIT_FRAME,
	IMAGE_PORTRAIT_MALE,
	IMAGE_PGBTEXT_HEALTH,
	IMAGE_PGBTEXT_HEALTH_BACK,
	IMAGE_PGBTEXT_HEALTH_LOW,
	IMAGE_PGBTEXT_ENERGY,
	IMAGE_PGBTEXT_ENERGY_BACK,
	IMAGE_PGBTEXT_MANA,
	IMAGE_PGBTEXT_MANA_BACK,
	IMAGE_PGBTEXT_EXP,
	IMAGE_PGBTEXT_EXP_BACK,
	IMAGE_LAST
};

enum SPELL_IMAGE
{
	SPELL_IMAGE_BOLT,
	SPELL_IMAGE_FIRE,
	SPELL_IMAGE_FREEZE,
	SPELL_IMAGE_SHOCK,
	SPELL_IMAGE_POISON,
	SPELL_IMAGE_STORM,
	SPELL_IMAGE_HEAL,
	SPELL_IMAGE_RAIN,
	SPELL_IMAGE_DEFEND,
	SPELL_IMAGE_SHIELD

};

/**
* Palette de couleurs possibles pour le texte une fois ajoutées via AddFontColor()
*/
enum COLOR
{
	TEXTCOLOR_WHITE,
	TEXTCOLOR_RED,
	TEXTCOLOR_GREEN,
	TEXTCOLOR_BLUE,
	TEXTCOLOR_YELLOW,
	TEXTCOLOR_LAST
};

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
#define PGB_HEALTH_BACKGROUND		true
#define PGB_HEALTH_BORDER_SIZE		2
#define PGB_HEALTH_POSITION_X		160
#define PGB_HEALTH_POSITION_Y		86
#define PGB_HEALTH_SIZE_W			250
#define PGB_HEALTH_SIZE_H			20
#define PGB_HEALTH_LOW_TRESHOLD		35
#define PGB_MANA_NAME				"pgb_mana"
#define PGB_MANA_BACKGROUND			true
#define PGB_MANA_BORDER_SIZE		2
#define PGB_MANA_POSITION_X			160
#define PGB_MANA_POSITION_Y			56
#define PGB_MANA_SIZE_W				250
#define PGB_MANA_SIZE_H				20
#define PGB_EXP_NAME				"pgb_exp"
#define PGB_EXP_BACKGROUND			true
#define PGB_EXP_BORDER_SIZE			2
#define PGB_EXP_POSITION_X			160
#define PGB_EXP_POSITION_Y			26
#define PGB_EXP_SIZE_W				225
#define PGB_EXP_SIZE_H				20
// Labels dans le panel portrait
#define PNL_PORTRAIT_ITALIC			false
#define PNL_PORTRAIT_CHAR_H			20
#define PNL_PORTRAIT_CHAR_W			15
#define PNL_PORTRAIT_CHAR_I			0.5f
#define LBL_HEALTH_NAME				"lbl_health"
#define LBL_HEALTH_POSITION_X		410
#define LBL_HEALTH_POSITION_Y		86
#define LBL_MANA_NAME				"lbl_mana"
#define LBL_MANA_POSITION_X			410
#define LBL_MANA_POSITION_Y			56
#define LBL_EXP_NAME				"lbl_exp"
#define LBL_EXP_POSITION_X			390
#define LBL_EXP_POSITION_Y			26
#define LBL_EXP_CHAR_W				18
#define LBL_EXP_CHAR_H				12
// Taille de la barre d'énergie
#define PGB_ENERGY_NAME				"pgb_energy"
#define PGB_ENERGY_BACKGROUND		true
#define PGB_ENERGY_BORDER_SIZE		2
#define PGB_ENERGY_POSITION_X		10
#define PGB_ENERGY_POSITION_Y		150
#define PGB_ENERGY_SIZE_W			250
#define PGB_ENERGY_SIZE_H			20
#define LBL_ENERGY_NAME				"lbl_energy"
#define LBL_ENERGY_POSITION_X		5
#define LBL_ENERGY_POSITION_Y		420
#define LBL_ENERGY_ITALIC			false
#define LBL_ENERGY_CHAR_H			25
#define LBL_ENERGY_CHAR_W			35
#define LBL_ENERGY_CHAR_I			0.5f

// Taille (LxH) des boutons des sorts dans la barre de sorts
#define SPELL_ICON_SIZE				64
// Longueur de l'espace entre chaque images de sorts
#define SPELL_ICON_PADDING			10

#endif