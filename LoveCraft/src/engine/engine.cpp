﻿#include "engine.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include "son.h"
#include <SFML/Network.hpp>
#include "interface.h"



Engine::Engine() : m_wireframe(false), m_angle(0), m_ghostMode(false),
	m_rightClick(false), m_leftClick(false), m_camRadius(10), m_fpstmr(0),
	m_playScreenBotLeft(Vector2i(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT)),
	m_playScreenTopLeft(Vector2i(INTERFACE_SIDE_LEFT_WIDTH, Height() - INTERFACE_TOP_HEIGHT * 3)),
	m_playScreenTopRight(Vector2i(Width() - INTERFACE_SIDE_RIGHT_WIDTH, Height() - INTERFACE_TOP_HEIGHT * 3)),
	m_playScreenBotRight(Vector2i(Width() - INTERFACE_SIDE_RIGHT_WIDTH, INTERFACE_BOTTOM_HEIGHT)),
	m_playScreenSize(Vector2i(m_playScreenTopRight.x - m_playScreenTopLeft.x, m_playScreenTopLeft.y - m_playScreenBotLeft.y))
{
	m_textureSpell = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureSpellX = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureInterface = new Texture[IMAGE::IMAGE_LAST];
	m_texturefontColor = new Texture[COLOR::TEXTCOLOR_LAST];
}

Engine::~Engine()
{
	delete m_textureArray;
}

const Engine& Engine::Get() const
{
	return *this;
}

Engine& Engine::Get()
{
	return *this;
}

void Engine::Init()
{
	//seed random number generator
	srand((unsigned)time(0));
	//Donne une référence vers la camera a info
	Info::Get().SetCamera(&m_camera);

#pragma region Initilisation de Glew

	GLenum err = glewInit ();
	if( err != GLEW_OK )
	{
		std :: cerr << " Error while initializing glew .. abording (" << glewGetErrorString ( err) << ")" << std :: endl ;
		abort ();
	}

#pragma endregion

#pragma region Initilisation des elements OpenGl

	//glClearColor( 0.f, 0.75f, 1.f, 1.0f );
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable( GL_TEXTURE_2D );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, (float)Width() / (float)Height(), 0.1f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable (GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_FOG);

	// Light
	GLfloat light0Pos[4]  = {0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f};
	GLfloat light0Amb[4]  = {0.9f, 0.9f, 0.9f, 1.0f};
	GLfloat light0Diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0Spec[4] = {0.2f, 0.2f, 0.2f, 1.0f};

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

	glFogf(GL_FOG_DENSITY, 1.f / VIEW_DISTANCE);
	//glFogf(GL_FOG_DENSITY, 1.f / 3);
	//float fogCol[3] = {0.8f, 0.8f, 0.8f};
	//glFogfv(GL_FOG_COLOR, fogCol);

#pragma endregion

#pragma region Initialisation des entites

	m_player.Init();

	m_testpig.Init(&m_player);
	m_testpig.SetPosition(Vector3f(10,5,10));
	m_character = Character();

#pragma endregion

#pragma region Initilisation du chunk principal

	m_chunks = new Array2d<Chunk>(VIEW_DISTANCE / CHUNK_SIZE_X * 2, VIEW_DISTANCE / CHUNK_SIZE_Z * 2);
	Info::Get().SetChunkArray(m_chunks);

	//Genere un chunk plancher
	Chunk chunk(&m_shaderCube);
	for (int i = 0; i < CHUNK_SIZE_X; ++i)
	{
		for (int j = 0; j < CHUNK_SIZE_Z; ++j)
		{
			chunk.SetBloc(i, 0, j, BTYPE_DIRT);
		}
	}

	//Blocs en escalier
	for (int k = 1; k <= 3; k++)
	{
		for (int j = 1; j < CHUNK_SIZE_Z-2; j++)
		{
			for (int i = 1; i < CHUNK_SIZE_X-2; i++)
			{
				if (i <= j)
					chunk.SetBloc(k,i,j, BTYPE_BRICK);
			}
		}
	}
	//Blocs dans le trou de sable
	for (int i = 1; i <= 8; i++)
	{
		for (int j = 1; j <= 6; j++)
		{
			chunk.SetBloc(i,0,j, BTYPE_SAND);

		}
	}

	//Blocs de l'igloo
	for (int k = 0; k <= 4; k++)
	{
		for (int i = 10; i < CHUNK_SIZE_X - 1; i++)
		{
			for (int j = 8; j < CHUNK_SIZE_Z - 1; j++)
			{
				if (k == 0 || k == 4)
					chunk.SetBloc(i,k,j, BTYPE_SNOW);
				else
				{
					if (i == 10 || j == 8 || i == CHUNK_SIZE_X - 2 || j == CHUNK_SIZE_Z - 2)
						chunk.SetBloc(i,k,j, BTYPE_SNOW);
				}
			}
		}
	}
	chunk.SetBloc(12, 1, 14, BTYPE_AIR);
	chunk.SetBloc(12, 2, 14, BTYPE_AIR);
	chunk.SetBloc(12, 1, 8, BTYPE_AIR);
	chunk.SetBloc(12, 2, 8, BTYPE_AIR);

	//Blocs de roche
	for (int i = 1; i <= 7; i++)
	{
		for (int j = 7; j < CHUNK_SIZE_Z - 1; j++)
		{
			chunk.SetBloc(i,0,j, BTYPE_ROCK);
		}
	}

	//Platforme de gazon + lac
	for (int k = 14; k < 16; k++)
	{
		for (int i = CHUNK_SIZE_X-1; i >= 4; i--)
		{
			for (int j = CHUNK_SIZE_Z-1; j >= 2; j--)
			{
				chunk.SetBloc(i,k,j, BTYPE_GRASS);
				if (k == 15 && (i < CHUNK_SIZE_X - 3 && i > 6 && j < CHUNK_SIZE_Z - 3 && j > 4))
					chunk.SetBloc(i,k,j, BTYPE_SWAMP);
				if (k == 15 && (i == CHUNK_SIZE_X - 3 || i == 6 || j == CHUNK_SIZE_Z - 3 || j == 4))
					chunk.SetBloc(i,k,j, BTYPE_ROCK);
			}
		}
	}

	//Place les chunks
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			chunk.SetPosition(Vector2i(i, j));
			m_chunks->Set(i, j, chunk);
		}
	}

#pragma endregion

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{

#pragma region Chargement des textures

#pragma region Blocs
	// Texture des blocs 128x128 px
	m_textureArray = new TextureArray(128);

	LoadBlocTexture(BTYPE_BRICK, TEXTURE_PATH "b_brick_red.jpg");
	LoadBlocTexture(BTYPE_DIRT, TEXTURE_PATH "b_dirt.bmp");
	LoadBlocTexture(BTYPE_GRASS, TEXTURE_PATH "b_grass.jpg");
	LoadBlocTexture(BTYPE_ROCK, TEXTURE_PATH "b_rock.jpg");
	LoadBlocTexture(BTYPE_SAND, TEXTURE_PATH "b_sand.jpg");
	LoadBlocTexture(BTYPE_SNOW, TEXTURE_PATH "b_snow.jpg");
	LoadBlocTexture(BTYPE_SWAMP, TEXTURE_PATH "b_swamp.jpg");

#pragma endregion

#pragma region Boutons des spells

	m_textureArray->Generate();
	//Texture des spells
	m_textureSpell[SPELL_IMAGE_BOLT].Load(TEXTURE_PATH "s_spellbolt.gif");
	m_textureSpell[SPELL_IMAGE_FIRE].Load(TEXTURE_PATH "s_spellfire.png");
	m_textureSpell[SPELL_IMAGE_FREEZE].Load(TEXTURE_PATH "s_spellfreeze.png");
	m_textureSpell[SPELL_IMAGE_SHOCK].Load(TEXTURE_PATH "s_spellshock.png");
	m_textureSpell[SPELL_IMAGE_POISON].Load(TEXTURE_PATH "s_spellpoison.gif");
	m_textureSpell[SPELL_IMAGE_STORM].Load(TEXTURE_PATH "s_spellstorm.png");
	m_textureSpell[SPELL_IMAGE_HEAL].Load(TEXTURE_PATH "s_spellheal.gif");
	m_textureSpell[SPELL_IMAGE_RAIN].Load(TEXTURE_PATH "s_spellrain.gif");
	m_textureSpell[SPELL_IMAGE_DEFEND].Load(TEXTURE_PATH "s_spelldefend.gif");
	m_textureSpell[SPELL_IMAGE_SHIELD].Load(TEXTURE_PATH "s_spellshield.png");

	m_textureSpellX[SPELL_IMAGE_BOLT].Load(TEXTURE_PATH "s_spellboltx.gif");
	m_textureSpellX[SPELL_IMAGE_FIRE].Load(TEXTURE_PATH "s_spellfirex.png");
	m_textureSpellX[SPELL_IMAGE_FREEZE].Load(TEXTURE_PATH "s_spellfreezex.png");
	m_textureSpellX[SPELL_IMAGE_SHOCK].Load(TEXTURE_PATH "s_spellshockx.png");
	m_textureSpellX[SPELL_IMAGE_POISON].Load(TEXTURE_PATH "s_spellpoisonx.gif");
	m_textureSpellX[SPELL_IMAGE_STORM].Load(TEXTURE_PATH "s_spellstormx.png");
	m_textureSpellX[SPELL_IMAGE_HEAL].Load(TEXTURE_PATH "s_spellhealx.gif");
	m_textureSpellX[SPELL_IMAGE_RAIN].Load(TEXTURE_PATH "s_spellrainx.gif");
	m_textureSpellX[SPELL_IMAGE_DEFEND].Load(TEXTURE_PATH "s_spelldefendx.gif");
	m_textureSpellX[SPELL_IMAGE_SHIELD].Load(TEXTURE_PATH "s_spellshieldx.png");

#pragma endregion

#pragma region Images et textures

	m_textureInterface[IMAGE_BLACK_BACK].Load(TEXTURE_PATH "noir.jpg");
	m_textureInterface[IMAGE_BOO].Load(TEXTURE_PATH "i_boo.png");
	m_textureInterface[IMAGE_RUN].Load(TEXTURE_PATH "i_bewareofcthulhu.png");
	m_textureInterface[IMAGE_CROSSHAIR].Load(TEXTURE_PATH "i_cross.bmp");
	m_textureInterface[IMAGE_INTERFACE_FRAME].Load(TEXTURE_PATH "b_rock.jpg");
	m_textureInterface[IMAGE_PORTRAIT_FRAME].Load(TEXTURE_PATH "i_portrait-frame.png");
	m_textureInterface[IMAGE_PORTRAIT_MALE].Load(TEXTURE_PATH "i_portrait-male.png");
	m_textureInterface[IMAGE_PGBTEXT_HEALTH].Load(TEXTURE_PATH "i_pgb_health.png");
	m_textureInterface[IMAGE_PGBTEXT_ENERGY].Load(TEXTURE_PATH "i_pgb_energy.png");
	m_textureInterface[IMAGE_PGBTEXT_MANA].Load(TEXTURE_PATH "i_pgb_mana.png");
	m_textureInterface[IMAGE_PGBTEXT_EXP].Load(TEXTURE_PATH "i_pgb_exp.png");
	m_textureInterface[IMAGE_PGBTEXT_HEALTH_BACK].Load(TEXTURE_PATH "i_pgb_health_back.png");
	m_textureInterface[IMAGE_PGBTEXT_ENERGY_BACK].Load(TEXTURE_PATH "i_pgb_energy_back.png");
	m_textureInterface[IMAGE_PGBTEXT_MANA_BACK].Load(TEXTURE_PATH "i_pgb_mana_back.png");
	m_textureInterface[IMAGE_PGBTEXT_EXP_BACK].Load(TEXTURE_PATH "i_pgb_exp_back.png");
	m_textureInterface[IMAGE_PGBTEXT_HEALTH_LOW].Load(TEXTURE_PATH "i_pgb_health_low.png");
	m_textureInterface[IMAGE_CLOCK_BG].Load(TEXTURE_PATH "i_clock_bg.png");

#pragma endregion

#pragma region Couleurs label

	m_texturefontColor[TEXTCOLOR_WHITE].Load(TEXTURE_PATH "font.png");
	m_texturefontColor[TEXTCOLOR_RED].Load(TEXTURE_PATH "font_red.png");
	m_texturefontColor[TEXTCOLOR_GREEN].Load(TEXTURE_PATH "font_green.png");
	m_texturefontColor[TEXTCOLOR_BLUE].Load(TEXTURE_PATH "font_blue.png");
	m_texturefontColor[TEXTCOLOR_YELLOW].Load(TEXTURE_PATH "font_yellow.png");


#pragma endregion

#pragma endregion

#pragma region Chargement des elements de l interface

	// Écran
	m_pnl_screen = Panel(0, Vector2i(), Vector2i(Width(), Height()), 0, 1, "main");

#pragma region Enfants de Main

	// Zone de jeu
	m_pnl_playscreen = Panel(&m_pnl_screen, 
		Vector2i(0, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(m_pnl_screen.Size().x, 
		m_pnl_screen.Size().y - INTERFACE_BOTTOM_HEIGHT),
		0, PNL_PLAYSCREEN_CONTROLS_NBR, "playscreen");
	m_pnl_screen.AddControl(&m_pnl_playscreen);

#pragma region Enfants de Playscreen

	// Frame portrait
	m_pnl_portrait = Panel(&m_pnl_playscreen,
		Vector2i(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Vector2i(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H),
		&m_textureInterface[IMAGE_PORTRAIT_FRAME], PNL_PORTRAIT_CONTROLS_NBR, PNL_PORTRAIT_NAME);
	m_pnl_playscreen.AddControl(&m_pnl_portrait);

	//m_ptest = Panel(&m_pnl_playscreen, Vector2i(Width()/2, Height()/2), Vector2i(400, 200), &m_textureInterface[IMAGE_BLACK_BACK], 1, "ptest");
	//m_pnl_screen.AddControl(&m_ptest);

	//m_testlabel = Label(&m_ptest, Vector2i(), Vector2i(), &m_texturefontColor[TEXTCOLOR_YELLOW], "123", 
	//	Label::TEXTDOCK_MIDDLECENTER, false, 20.0f, 42.0f, 1.50f, Vector2f(), "lbl_test");
	//m_ptest.AddControl(&m_testlabel);

	//m_lb_infos = ListBox(&m_pnl_playscreen, Vector2i(0, m_pnl_playscreen.Size().y / 2 - 200), Vector2i(200, 400), &m_texturefontColor[TEXTCOLOR_RED], 
	//	&m_textureInterface[IMAGE_BLACK_BACK], 15, 2, 12.f, 12.f, 0.5f, "lb_infos");
	//m_pnl_playscreen.AddControl(&m_lb_infos);
	//m_lb_infos.SetLine(0, "Avancer:     W");
	//m_lb_infos.SetLine(1, "Reculer:     S");
	//m_lb_infos.SetLine(2, "Droite:      D");
	//m_lb_infos.SetLine(3, "Gauche:      A");
	//m_lb_infos.SetLine(4, "Sauter:      Espace");
	//m_lb_infos.SetLine(5, "Marcher:     Ctrl");
	//m_lb_infos.SetLine(6, "Se pencher:  C");
	//m_lb_infos.SetLine(7, "Courir:      Shift");
	//m_lb_infos.SetLine(8, "Tirer:       1");
	//m_lb_infos.SetLine(9, "Cochon:      2");
	//m_lb_infos.SetLine(10, "Wireframe:   Y");
	//m_lb_infos.SetLine(11, "Music On/off O");
	//m_lb_infos.SetLine(12, "Music Next   M");
	//m_lb_infos.SetLine(13, "Fullscreen   F10");
	//m_lb_infos.SetLine(14, "Quitter      Esc");


#pragma region Enfants pnl portrait

	// Barre de vie
	m_pgb_health = ProgressBar(&m_pnl_portrait,
		Vector2i(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Vector2i(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_HEALTH], &m_textureInterface[IMAGE_PGBTEXT_HEALTH_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_HEALTH_BACKGROUND, PGB_HEALTH_BORDER_SIZE, PGB_HEALTH_NAME);
	m_pnl_portrait.AddControl(&m_pgb_health);
	// Barre de mana
	m_pgb_mana = ProgressBar(&m_pnl_portrait,
		Vector2i(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Vector2i(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_MANA], &m_textureInterface[IMAGE_PGBTEXT_MANA_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_MANA_BACKGROUND, PGB_MANA_BORDER_SIZE, PGB_MANA_NAME);
	m_pnl_portrait.AddControl(&m_pgb_mana);
	// Barre d'expérience
	m_pgb_exp = ProgressBar(&m_pnl_portrait,
		Vector2i(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Vector2i(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_EXP], &m_textureInterface[IMAGE_PGBTEXT_EXP_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_EXP_BACKGROUND, PGB_EXP_BORDER_SIZE, PGB_EXP_NAME);
	m_pnl_portrait.AddControl(&m_pgb_exp);
	// Label de vie
	m_lbl_health = Label(&m_pnl_portrait, Vector2i(LBL_HEALTH_POSITION_X, LBL_HEALTH_POSITION_Y), &m_texturefontColor[TEXTCOLOR_RED], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_HEALTH_NAME);
	m_pnl_portrait.AddControl(&m_lbl_health);
	// Label de mana
	m_lbl_mana = Label(&m_pnl_portrait, Vector2i(LBL_MANA_POSITION_X, LBL_MANA_POSITION_Y), &m_texturefontColor[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_MANA_NAME);
	m_pnl_portrait.AddControl(&m_lbl_mana);
	// Label d'exp
	m_lbl_exp = Label(&m_pnl_portrait, Vector2i(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), &m_texturefontColor[TEXTCOLOR_YELLOW], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, LBL_EXP_CHAR_W, LBL_EXP_CHAR_H, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_EXP_NAME);
	m_pnl_portrait.AddControl(&m_lbl_exp);

	// Image du joueur
	m_pnl_playerImage = Panel(&m_pnl_portrait, 
		Vector2i(PB_PORTRAIT_POSITION_X, PB_PORTRAIT_POSITION_Y),
		Vector2i(PB_PORTRAIT_SIZE_W, PB_PORTRAIT_SIZE_H),
		&m_textureInterface[IMAGE_PORTRAIT_MALE], 1, PB_PORTRAIT_NAME);
	m_pnl_portrait.AddControl(&m_pnl_playerImage);

#pragma region Enfants de m_pnl_playerImage

	m_lbl_playerLevel = Label(&m_pnl_playerImage, Vector2i(), &m_texturefontColor[TEXTCOLOR_WHITE], "10", 
		Label::TEXTDOCK_TOPCENTER, LBL_GENERIC_ITALIC, LBL_PLAYER_LEVEL_W, LBL_PLAYER_LEVEL_H, LBL_PLAYER_LEVEL_I, Vector2f(), LBL_PLAYER_LEVEL_NAME);
	m_pnl_playerImage.AddControl(&m_lbl_playerLevel);

#pragma endregion

#pragma endregion

	//Barre d'énergie verticale
	m_pgb_energy = ProgressBar(&m_pnl_playscreen,
		Vector2i(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Vector2i(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_ENERGY], &m_textureInterface[IMAGE_PGBTEXT_ENERGY_BACK],
		ProgressBar::BARMODE_VERTICAL_DTU, PGB_ENERGY_BACKGROUND, PGB_ENERGY_BORDER_SIZE, PGB_ENERGY_NAME);
	m_pnl_playscreen.AddControl(&m_pgb_energy);
	//Label d'énergie
	m_lbl_energy = Label(&m_pnl_playscreen, Vector2i(LBL_ENERGY_POSITION_X, LBL_ENERGY_POSITION_Y), &m_texturefontColor[TEXTCOLOR_GREEN], "", 
		Label::TEXTDOCK_NONE, LBL_ENERGY_ITALIC, LBL_ENERGY_CHAR_H, LBL_ENERGY_CHAR_W, LBL_ENERGY_CHAR_I, Vector2f(), LBL_ENERGY_NAME);
	m_pnl_playscreen.AddControl(&m_lbl_energy);
	//Label Position
	m_lbl_plrPos = Label(&m_pnl_playscreen, Vector2i(5, m_pnl_playscreen.Size().y - LBL_GENERIC_CHAR_H), &m_texturefontColor[TEXTCOLOR_GREEN], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "pos");
	m_pnl_playscreen.AddControl(&m_lbl_plrPos);
	//Label Vitesse
	m_lbl_plrSpd = Label(&m_pnl_playscreen, Vector2i(5, m_pnl_playscreen.Size().y - LBL_GENERIC_CHAR_H*2), &m_texturefontColor[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "spd");
	m_pnl_playscreen.AddControl(&m_lbl_plrSpd);
	//Label Acceleration
	m_lbl_plrAcc = Label(&m_pnl_playscreen, Vector2i(5, m_pnl_playscreen.Size().y - LBL_GENERIC_CHAR_H*3), &m_texturefontColor[TEXTCOLOR_RED], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "acc");
	m_pnl_playscreen.AddControl(&m_lbl_plrAcc);
	//Label FPS
	m_lbl_FPS = Label(&m_pnl_playscreen, Vector2i(5, m_pnl_playscreen.Size().y - LBL_GENERIC_CHAR_H*4), &m_texturefontColor[TEXTCOLOR_YELLOW], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "fps");
	m_pnl_playscreen.AddControl(&m_lbl_FPS);
	//Heure
	m_pnl_time = Panel(&m_pnl_playscreen, Vector2i(m_pnl_playscreen.Size().x - 128, m_pnl_playscreen.Size().y - 64), 
		Vector2i(128, 64), &m_textureInterface[IMAGE_CLOCK_BG], 1, "clock");
	m_pnl_playscreen.AddControl(&m_pnl_time);
	m_lbl_time = Label(&m_pnl_time, Vector2i(0,0), &m_texturefontColor[TEXTCOLOR_WHITE], "", Label::TEXTDOCK_MIDDLECENTER, false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "time");
	m_pnl_time.AddControl(&m_lbl_time);

#pragma endregion

#pragma endregion

#pragma endregion

#pragma region Load et compile les shaders
	std::cout << " Loading and compiling shaders ..." << std::endl;
	if (!m_shaderModel.Load(SHADER_PATH "modelshader.vert", SHADER_PATH "modelshader.frag", true))
	{
		std::cout << " Failed to load model shader" << std::endl;
		exit(1) ;
	}
	if (!m_shaderCube.Load(SHADER_PATH "cubeshader.vert", SHADER_PATH "cubeshader.frag", true))
	{
		std::cout << " Failed to load cube shader" << std::endl;
		exit(1) ;
	}
	if (!m_shaderSpells.Load(SHADER_PATH "particleshader.vert", SHADER_PATH "particleshader.frag", true))
	{
		std::cout << " Failed to load cube shader" << std::endl;
		exit(1) ;
	}
#pragma endregion

}

void Engine::LoadBlocTexture(BLOCK_TYPE type, std::string path)
{
	Info::Get().GetBlocInfo(type)->SetTextureIndex(m_textureArray->AddTexture(path));
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{

#pragma region Game time

	m_character.ReduceGlobalCooldown(elapsedTime);
	static float gameTime = elapsedTime;
	gameTime += elapsedTime;

#pragma endregion

#pragma region Calcul la position du joueur et de la camera

	m_player.Move(m_ghostMode, m_character, elapsedTime);
	m_camera.SetPosition(m_player.Position());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#pragma endregion

#pragma region Elements de la camera

	// 3rd person
	if (m_camera.GetMode() == Camera::CAM_THIRD_PERSON ) {
		// hide/show cursor
		if (!m_rightClick && !m_leftClick)
			ShowCursor();
		else
			HideCursor();

		// recule la camera
		glTranslatef(0,0,-m_camRadius);
		// applique les transformations normales de la camera
		m_camera.ApplyRotation();
		m_camera.ApplyTranslation();

		// render le modele du player
		m_shaderModel.Use();
		m_player.Render(m_wireframe);
		Shader::Disable();
	} 
	// first person
	else
	{
		m_camera.ApplyRotation();
		m_camera.ApplyTranslation();
	}

#pragma endregion

#pragma region Render les cubes

	m_shaderCube.Use();
	m_textureArray->Use();
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk &c = m_chunks->Get(i,j);
			if (c.IsDirty())
				c.Update();
			c.Render();
		}
	}

	Shader::Disable();

#pragma endregion

#pragma region Render models

	m_shaderModel.Use();

	m_testpig.Update(elapsedTime);
	m_testpig.Render();

	Shader::Disable();

#pragma endregion

#pragma region Render spells

	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_shaderSpells.Use();

	// Update et render tous les spells
	for (SpellList::iterator it = m_spells.begin(); it != m_spells.end(); ++it) {
		it->SetDestination(m_testpig.Position());
		it->Update(elapsedTime);
		it->Render();
	}

	Shader::Disable();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

#pragma endregion

#pragma region Render l interface

	// HUD
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Render2D(elapsedTime);
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma endregion

#pragma region Reseau

	//Test réseau - Dessigne un carré en haut de la position du joueur
	//sf::Packet p;
	//if (Info::Get().Network().Receive(p))
	//{
	//	float x;
	//	float y;
	//	float z;
	//	p >> x >> y >> z;
	//	glLoadIdentity();
	//	glTranslated(x, y+5, z);
	//	m_textureFloor.Bind();
	//	glBegin(GL_QUADS);

	//	glTexCoord2f(0, 0);
	//	glVertex3f(x - 0.5f, y, z - 0.5f);

	//	glTexCoord2f(0, 1);
	//	glVertex3f(x + 0.5f, y, z - 0.5f);

	//	glTexCoord2f(1, 1);
	//	glVertex3f(x + 0.5f, y, z + 0.5f);

	//	glTexCoord2f(1, 0);
	//	glVertex3f(x - 0.5f, y, z + 0.5f);

	//	glEnd();
	//}

#pragma endregion

#pragma region Musique

	//Solution temporaire pour changer la musique lors du premier render de l'engine
	static bool ttt = true;
	if (ttt)
	{
		Info::Get().Sound().PlayNextTrack();
		ttt = false;
	}

#pragma endregion

#pragma region FPS

	m_fpstmr += gameTime;
	if (m_fpstmr > 1.5f)
	{
		float fps = 1 / elapsedTime;
		m_fps = fps >= 60 ? 60 : fps;
		m_fpstmr = 0;
	}

#pragma endregion

}

void Engine::Render2D(float elapsedTime)
{

#pragma region OpenGl

	//Setter le blend function, tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

#pragma endregion

#pragma region Images qui subissent le blend par le contraste

	//Activation du blend normal
	StartBlendPNG(false);


#pragma region Crosshair

	if (m_camera.GetMode() == Camera::CAM_FIRST_PERSON)
	{
		RenderSquare(
			Vector2i(Width() / 2 - CROSSHAIR_SIZE / 2, Height() / 2 - CROSSHAIR_SIZE / 2),
			Vector2i(CROSSHAIR_SIZE, CROSSHAIR_SIZE),
			m_textureInterface[IMAGE_CROSSHAIR], false);
	}

#pragma endregion

	glDisable(GL_BLEND);

#pragma endregion

#pragma region Images qui subissent le blend pour les PNG

	//Change la texture de la barre de vie en fonction du %. Ne réassigne la texture que si on en a besoin
	if (m_character.HealthPerc() <= PGB_HEALTH_LOW_TRESHOLD && m_pgb_health.GetTexture() == &m_textureInterface[IMAGE_PGBTEXT_HEALTH])
		m_pgb_health.SetTexture(&m_textureInterface[IMAGE_PGBTEXT_HEALTH_LOW]);
	else if (m_character.HealthPerc() > PGB_HEALTH_LOW_TRESHOLD && m_pgb_health.GetTexture() == &m_textureInterface[IMAGE_PGBTEXT_HEALTH_LOW])
		m_pgb_health.SetTexture(&m_textureInterface[IMAGE_PGBTEXT_HEALTH]);

	//Affiche ou cache la barre d'énergie selon la situation
	if (m_character.Energy() == m_character.EnergyMax())
	{
		m_pgb_energy.SetVisible(false);
		m_lbl_energy.SetVisible(false);
	}
	else if (m_character.Energy() != m_character.EnergyMax() || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		m_pgb_energy.SetVisible(true);
		m_lbl_energy.SetVisible(true);
	}
	
	//Render de l'écran au complet avec tous ses contrôles.
	m_pnl_screen.Render();

	//Activation du blend par PNG
	StartBlendPNG();

	//// Portrait
	//RenderSquare(
	//	Vector2i(m_playScreenBotLeft.x + m_pgb_energy.Size().y, m_playScreenBotLeft.y),
	//	Vector2i((int)m_textureInterface[IMAGE_PORTRAIT_FRAME].GetWidth(), (int)m_textureInterface[IMAGE_PORTRAIT_FRAME].GetHeight()),
	//	m_textureInterface[IMAGE_PORTRAIT_FRAME]);


	//Optimisation possible par la surcharge d'opérateurs
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))		//Mode course
	//	RenderSquare(
	//	m_playScreenBotLeft,
	//	Vector2i(m_textureInterface[IMAGE_RUN].GetWidth(), m_textureInterface[IMAGE_RUN].GetHeight()),
	//	m_textureInterface[IMAGE_RUN]);
	//if (m_ghostMode)			//Mode ghost
	//	RenderSquare(
	//	Vector2i(Width() / 2 - m_textureInterface[IMAGE_BOO].GetWidth() /2, m_playScreenBotLeft.y),
	//	Vector2i(m_textureInterface[IMAGE_BOO].GetWidth(), m_textureInterface[IMAGE_BOO].GetHeight()),
	//	m_textureInterface[IMAGE_BOO]);

	glDisable(GL_BLEND);

#pragma endregion

#pragma region Affichage de l interface sans transparence

	//============================================

	//Bottom
	RenderSquare(
		Vector2i(0, 0), 
		Vector2i(Width(), INTERFACE_BOTTOM_HEIGHT), 
		m_textureInterface[IMAGE_INTERFACE_FRAME]);
	////Left
	//RenderSquare(
	//	Vector2i(0, INTERFACE_BOTTOM_HEIGHT), 
	//	Vector2i(INTERFACE_SIDE_LEFT_WIDTH, Height() - INTERFACE_TOP_HEIGHT * 3), 
	//	m_textureInterface[IMAGE_INTERFACE_FRAME]);
	////Top
	//RenderSquare(
	//	Vector2i(0, Height() - INTERFACE_TOP_HEIGHT),
	//	Vector2i(Width(), INTERFACE_TOP_HEIGHT),
	//	m_textureInterface[IMAGE_INTERFACE_FRAME]);
	////Right
	//RenderSquare(
	//	Vector2i(Width() - INTERFACE_SIDE_RIGHT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
	//	Vector2i(Width(), Height() - INTERFACE_TOP_HEIGHT * 3),
	//	m_textureInterface[IMAGE_INTERFACE_FRAME]);
	//============================================
	RenderSpells();
	//============================================
	//Mise à jour des données
	m_pgb_health.SetValue(m_character.HealthPerc());
	m_pgb_energy.SetValue(m_character.EnergyPerc());
	m_pgb_mana.SetValue(m_character.ManaPerc());
	m_pgb_exp.SetValue(m_character.ExpPerc());
	//============================================
	TextUpdate();
	//============================================

#pragma endregion

#pragma region OpenGl

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

#pragma endregion

}

void Engine::RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture, bool repeat)
{
	texture.Bind();
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f((repeat ? size.x / texture.GetWidth() : 1), 0);
	glVertex2i(size.x, 0);

	glTexCoord2f((repeat ? size.x / texture.GetWidth() : 1), (repeat ? size.y / texture.GetHeight() : 1));
	glVertex2i(size.x, size.y);

	glTexCoord2f(0, (repeat ? size.y / texture.GetHeight() : 1));
	glVertex2i(0, size.y);

	glEnd();
}

void Engine::RenderSpells()
{
	static short spellSize = SPELL_ICON_SIZE;
	static short spellPadding = SPELL_ICON_PADDING;
	static short spellBarWidth = SPELL_BAR_SPELL_NUMBER * (spellSize + spellPadding);
	static short spellBarPosX = Width() / 2 - spellBarWidth / 2;
	static short spellBarPosY = INTERFACE_BOTTOM_HEIGHT;
	std::string nombre;

	for (int i = 0; i < SPELL_BAR_SPELL_NUMBER; i++)
	{
		if (m_textureSpell[i].IsValid())
		{
			RenderSquare(Vector2i(i == 0 ? spellBarPosX : (spellBarPosX + i * (spellSize + spellPadding)), 0), Vector2i(spellSize, spellSize), m_textureSpell[i], false);

			std::ostringstream convertisseur;
			convertisseur << (i + 1) % 10;
			nombre = convertisseur.str();
			PrintText(spellBarPosX + i * (spellSize + spellPadding), spellSize - 12, nombre);
			nombre = "";
		}
	}


}

void Engine::TextUpdate()
{
	std::ostringstream ss;
	//Vie
	ss << (int)m_character.Health() << " / " << (int)m_character.HealthMax();
	m_lbl_health.SetMessage(ss.str());
	ss.str("");
	//Energie
	ss << (int)m_character.Energy() << " / " << (int)m_character.EnergyMax();
	m_lbl_energy.SetMessage(ss.str());
	ss.str("");
	//Mana
	ss << (int)m_character.Mana() << " / " << (int)m_character.ManaMax();
	m_lbl_mana.SetMessage(ss.str());
	ss.str("");
	//Experience
	ss << (int)m_character.Exp() << " / " << (int)m_character.ExpNext();
	m_lbl_exp.SetMessage(ss.str());
	ss.str("");
	//Position
	ss << "Position :     ( " << std::setprecision(4) << m_player.Position().x << ", " << std::setprecision(4) <<
		m_player.Position().y << ", " << std::setprecision(4) << m_player.Position().z << " )";
	m_lbl_plrPos.SetMessage(ss.str());
	ss.str("");
	//Vitesse
	ss << "Vitesse :      " << m_player.Speed();
	m_lbl_plrSpd.SetMessage(ss.str());
	ss.str("");
	//Accélération
	ss << "Acceleration : " << m_player.Acceleration();
	m_lbl_plrAcc.SetMessage(ss.str());
	ss.str("");
	//FPS
	ss << "Fps : " << std::setprecision(2) << m_fps;
	m_lbl_FPS.SetMessage(ss.str());
	ss.str("");
	//Heure
	time_t currentTime;
	time (&currentTime);
	struct tm ptm;
	localtime_s(&ptm, &currentTime);
	ss << std::setfill('0');
	ss << std::setw(2) << ptm.tm_hour << ":" << std::setw(2) << ptm.tm_min;
	m_lbl_time.SetMessage(ss.str());
	ss.str("");
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t)
{
	m_texturefontColor[TEXTCOLOR_WHITE].Bind();
	glLoadIdentity();
	glTranslated(x, y, 0);
	for (unsigned int i = 0; i < t.length(); ++i)
	{
		float left = (float)((t[i] - 32) % 16) / 16.0f;
		float top = (float)((t[i] - 32) / 16) / 16.0f;
		top += 1.0f;
		glBegin(GL_QUADS);
		glTexCoord2f(left, 1.0f - top - 0.0625f);
		glVertex2i(0, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
		glVertex2i(12 , 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top);
		glVertex2i(12, 12);
		glTexCoord2f(left , 1.0f - top);
		glVertex2i(0, 12);
		glEnd();
		glTranslated(8, 0, 0);
	}
}

void Engine::StartBlendPNG(bool value) const
{
	glEnable(GL_BLEND);
	if (value)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Blend PNG
	else glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Blend original
}

void Engine::KeyPressEvent(unsigned char key)
{
	Son& sound = Info::Get().Sound();
	switch(key)
	{
	case 36:	// ESC
		Stop();
		break;
	case 94:	// F10
		SetFullscreen(!IsFullscreen());
		break;
	case 21:	// V
		if (m_camera.GetMode() == Camera::CAM_FIRST_PERSON) 
		{
			m_camera.SetMode(Camera::CAM_THIRD_PERSON);
		}
		else 
		{
			HideCursor();
			m_camera.SetMode(Camera::CAM_FIRST_PERSON);
		}
		break;
	case 27:
		if(m_character.GlobalCooldown() == 0)
		{
			Spell newSpell;
			newSpell.SetPosition(m_player.Position());
			newSpell.Init(4.f, m_player.RotationQ(), &m_texSpell);
			newSpell.Shoot();
			m_spells.push_back(newSpell);
			sound.PlaySnd(Son::SON_BOLT, Son::CHANNEL_SPELL);
			m_character.ResetGlobalCooldown();
		}
		break;
	case 28:
		m_testpig.SetPosition(Vector3f(m_testpig.Position().x, 10, m_testpig.Position().z));
		sound.PlaySnd(Son::SON_FIRE, Son::CHANNEL_SPELL);
		break;
	case 29:
		sound.PlaySnd(Son::SON_FREEZE, Son::CHANNEL_SPELL);
		break;
	case 30:
		sound.PlaySnd(Son::SON_SHOCK, Son::CHANNEL_SPELL);
		break;
	case 31:
		sound.PlaySnd(Son::SON_POISON, Son::CHANNEL_SPELL);
		break;
	case 32:
		if (m_character.Mana() - 5 >= 0)
		{
			sound.PlaySnd(Son::SON_STORM, Son::CHANNEL_SPELL);
			m_character.SetMana(-5);
			m_player.Teleport();
		}
		break;
	case 33:
		if (m_character.Mana() - 15 >= 0)
		{
			sound.PlaySnd(Son::SON_HEAL1, Son::CHANNEL_SPELL);
			m_character.SetHealth(15);
			m_character.SetMana(-15);
		}
		break;
	case 34:
		if (m_character.Mana() - 10 >= 0)
		{
			sound.PlaySnd(Son::SON_HEAL2, Son::CHANNEL_SPELL);
			m_character.SetEnergy(10);
			m_character.SetMana(-10);
		}
		break;
	case 35:
		sound.PlaySnd(Son::SON_DEFEND, Son::CHANNEL_SPELL);
		break;
	case 26:
		sound.PlaySnd(Son::SON_SHIELD, Son::CHANNEL_SPELL);
		break;
	case 37:
		break;
	default:
		std::cout << "Unhandled key: " << (int)key << std::endl;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			Info::Get().Sound().PlayNextTrack();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			Info::Get().Options().SetOptMusic(!Info::Get().Options().GetOptMusic());
	}

}

void Engine::KeyReleaseEvent(unsigned char key)
{
	switch(key)
	{
	case 24:       // Y
		m_wireframe = !m_wireframe;
		if(m_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
		break;
	case 6:		   // G
		m_ghostMode = !m_ghostMode;
		std::cout << "Ghost Mode set to: " << (m_ghostMode ? "True" : "False") << std::endl;
		break;
	}
}

void Engine::MouseMoveEvent(int x, int y)
{
	// Centrer la souris seulement si elle n'est pas déjà centrée
	// Il est nécessaire de faire la vérification pour éviter de tomber
	// dans une boucle infinie où l'appel à CenterMouse génère un
	// MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre 
	// MouseMoveEvent, etc

	// Camera 3rd person
	if (m_camera.GetMode() == Camera::CAM_THIRD_PERSON && m_rightClick || m_leftClick)
	{
		if (!MousePosChanged(x, y))
			return;
		MakeRelativeToMouse(x, y);
		m_camera.TurnLeftRight((float)x);
		m_camera.TurnTopBottom((float)y);

		if (m_rightClick) 
		{
			m_player.SetRotation(m_camera.GetRotation());
		}

		ResetMouse();
	}
	// Camera first person
	else if (m_camera.GetMode() == Camera::CAM_FIRST_PERSON)
	{
		if(x == (Width() / 2) && y == (Height() / 2))
			return;
		MakeRelativeToCenter(x, y);
		m_player.TurnLeftRight((float)x);
		m_player.TurnTopBottom((float)y);

		m_camera.SetRotation(m_player.Rotation());

		CenterMouse();
	}

}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		if (m_camera.GetMode() == Camera::CAM_THIRD_PERSON)
		{
			m_rightClick = true;
			m_player.SetRotation(m_camera.GetRotation());
			SetMousePos(x, y);
		}
		break;
	case MOUSE_BUTTON_LEFT:
		if (m_camera.GetMode() == Camera::CAM_THIRD_PERSON)
		{
			m_leftClick = true;
			SetMousePos(x, y);
		}
		if (m_camera.GetMode() == Camera::CAM_FIRST_PERSON)
			Info::Get().Sound().PlaySnd(Son::SON_CLICK, Son::CHANNEL_INTERFACE);
		break;
	case MOUSE_BUTTON_WHEEL_UP:
		if (m_camera.GetMode() == Camera::CAM_THIRD_PERSON)
		{
			// Zoom in camera
			if (m_camRadius > 0)
			{
				m_camRadius -= 1;
			}
		}
		break;
	case MOUSE_BUTTON_WHEEL_DOWN:
		if (m_camera.GetMode() == Camera::CAM_THIRD_PERSON)
		{
			// Zoom out camera
			if (m_camRadius < 20)
			{
				m_camRadius += 1;
			}
		}
		break;
	}
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
	switch (button)
	{
	case MOUSE_BUTTON_RIGHT:
		m_rightClick = false;
		break;
	case MOUSE_BUTTON_LEFT:
		m_leftClick = false;
		break;

	}
}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
	texture.Load(filename);
	if(!texture.IsValid())
	{
		std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
		if(stopOnError)
			Stop();

		return false;
	}

	return true;
}

