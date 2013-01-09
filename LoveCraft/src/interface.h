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
	IMAGE_CONSOLE_BACK,
	IMAGE_CONSOLE_TEXTBOX_BACK,
	IMAGE_CLOCK_BG,
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
#define INTERFACE_SIDE_RIGHT_WIDTH	64
// Hauteur du cadre supérieur de l'interface
#define INTERFACE_TOP_HEIGHT		64
// Nombre de sort disponible dans la barre de sorts (max 10)
#define SPELL_BAR_SPELL_NUMBER		10

// Valeurs des controles
#define PNL_SCREEN_CONTROLS_NBR		5
#define PNL_PLAYSCREEN_CONTROLS_NBR	7
#define PNL_PORTRAIT_CONTROLS_NBR	9 

#define LBL_GENERIC_ITALIC			false
#define LBL_GENERIC_CHAR_W			12
#define LBL_GENERIC_CHAR_H			25
#define LBL_GENERIC_CHAR_I			0.6f

// ListBox Console
#define LB_CONSOLE_NAME				"lb_console"
#define LB_CONSOLE_SCROLLABLE		true
#define LB_CONSOLE_SIZE_W			500.0f
#define LB_CONSOLE_LINE_NUMBER		10
#define LB_CONSOLE_LINE_GAP			3
#define LB_CONSOLE_CHAR_H			18.0f
#define LB_CONSOLE_CHAR_W			12.0f
#define LB_CONSOLE_CHAR_I			0.5f
#define LB_CONSOLE_BODER_OFFSET_S	25
#define LB_CONSOLE_BODER_OFFSET_B	15
// Texbox console
#define TXB_CONSOLE_NAME			"txb_console"
#define TXB_CONSOLE_SIZE_H			30
#define TXB_CONSOLE_SIZE_W			500
#define TXB_CONSOLE_CHAR_I			0.5f
#define TXB_CONSOLE_OFFSET_X		5.0f
#define TXB_CONSOLE_OFFSET_Y		3.0f

// Panel portrait
#define PNL_PORTRAIT_NAME			"pnl_portrait"
#define PNL_PORTRAIT_SIZE_W			512
#define PNL_PORTRAIT_SIZE_H			128
#define PNL_PORTRAIT_POSITION_X		20
#define PNL_PORTRAIT_POSITION_Y		0
// Picture box dans le panel portrait
#define PB_PORTRAIT_NAME			"pnl_portrait_image"
#define PB_PORTRAIT_SIZE_W			87
#define PB_PORTRAIT_SIZE_H			87
#define PB_PORTRAIT_POSITION_X		16
#define PB_PORTRAIT_POSITION_Y		23
// Label du niveau
#define LBL_PLAYER_LEVEL_NAME		"lbl_player_level"
#define LBL_PLAYER_LEVEL_W			30
#define LBL_PLAYER_LEVEL_H			20
#define LBL_PLAYER_LEVEL_I			0.4f
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