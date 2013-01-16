#include "engine.h"
#include "npcinfo.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include "son.h"
#include <SFML/Network.hpp>
#include "interfaceinfos.h"
#include "util/tool.h"



Engine::Engine() : m_wireframe(false), m_angle(0), m_ghostMode(false),
	m_rightClick(false), m_leftClick(false), m_camRadius(10), m_fpstmr(0),
	m_clickTimer(0), m_currentBlockType(0)
{
	m_textureSpell = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureSpellX = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureInterface = new Texture[IMAGE::CUSTIMAGE_LAST];
	m_texturefontColor = new Texture[COLOR::TEXTCOLOR_LAST];
	m_monsters = new Animal*[MONSTER_MAX_NUMBER];

	m_menuUI = new MenuInterface();
	m_gameUI = new GameInterface(this);

	m_camera = new Camera;
	m_player = new Player;
	m_dice = new Dice;
}

Engine::~Engine()
{
	delete m_player;
	delete m_camera;
	delete m_dice;

	// delete les chunks
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2 - 1; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2 - 1; ++j)
		{
			if (m_chunks != 0)
				delete m_chunks->Get(i, j);
		}
	}
	// delete les monstres
#ifdef LOAD_MODELS
	for (unsigned short i = 0; i < MONSTER_MAX_NUMBER; i++)
		delete m_monsters[i];
#else
	delete m_monsters[0];
#endif
	delete [] m_monsters;

	delete m_textureArray;
	delete m_menuUI;
	delete m_gameUI;
}

const Engine& Engine::Get() const
{
	return *this;
}

Engine& Engine::Get()
{
	return *this;
}

void Engine::MenuInit()
{
	//seed random number generator
	srand((unsigned)time(0));
	//Donne une référence vers la camera a info
	Info::Get().SetCamera(m_camera);

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

	CenterMouse();
	HideCursor();
}

void Engine::GameInit()
{
	m_currentBlockType = BTYPE_DIRT;

#pragma region Initialisation des entites

	m_player->Init();

#ifndef MONSTERS_INIALIZED
#define MONSTERS_INIALIZED
#endif
	for (int i = 0; i < MONSTER_MAX_NUMBER; i++)
		m_monsters[i] = new Animal();
	m_monsters[0]->Init(Animal::ANL_GRD_ALIGATR, m_player);

#ifdef LOAD_MODELS
	m_monsters[1]->Init(Animal::ANL_GRD_ARACHNID, m_player);
	m_monsters[2]->Init(Animal::ANL_GRD_ARMDILLO, m_player);
	m_monsters[3]->Init(Animal::ANL_GRD_BISON, m_player);
	m_monsters[4]->Init(Animal::ANL_GRD_BWIDOW, m_player);
	m_monsters[5]->Init(Animal::ANL_GRD_CATBLK, m_player);
	m_monsters[6]->Init(Animal::ANL_GRD_COYOTE, m_player);
	m_monsters[7]->Init(Animal::ANL_GRD_DEER_M, m_player);
	m_monsters[8]->Init(Animal::ANL_GRD_DONKEY, m_player);
	m_monsters[9]->Init(Animal::ANL_GRD_HUSKY, m_player);
	m_monsters[10]->Init(Animal::ANL_GRD_HYENA, m_player);
	m_monsters[11]->Init(Animal::ANL_GRD_PIG, m_player);
	m_monsters[12]->Init(Animal::ANL_GRD_REDFOX, m_player);
	m_monsters[13]->Init(Animal::ANL_GRD_RHINO, m_player);
	m_monsters[14]->Init(Animal::ANL_GRD_SCORPION, m_player);
	m_monsters[15]->Init(Animal::ANL_GRD_SHEEP, m_player);
	m_monsters[16]->Init(Animal::ANL_GRD_TORTOISE, m_player);
	m_monsters[17]->Init(Animal::ANL_GRD_TRANTULA, m_player);
	for (unsigned short i = 18; i < MONSTER_MAX_NUMBER; i++)
		m_monsters[i]->Init(Animal::ANL_AIR_MONARCH, m_player);

	for (unsigned short i = 0; i < MONSTER_MAX_NUMBER; i++)
		m_monsters[i]->SetPosition(Vector3f(m_dice->Next(-(int)(VIEW_DISTANCE*0.5f), (int)(VIEW_DISTANCE*0.5f)), 10 + m_dice->Next(0, 10),
		m_dice->Next(-(int)(VIEW_DISTANCE*0.5f), (int)(VIEW_DISTANCE*0.5f))));
#endif

	m_character = Character();

#pragma endregion

#pragma region Initilisation du chunk principal

#ifndef CHUNK_INITIALIZED
#define CHUNK_INITIALIZED
#endif

	m_chunks = new Array2d<Chunk*>(VIEW_DISTANCE / CHUNK_SIZE_X * 2, VIEW_DISTANCE / CHUNK_SIZE_Z * 2);
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
			Chunk* c = new Chunk();
			*c = chunk;
			m_chunks->Set(i, j, c);
		}
	}

#pragma endregion

	CenterMouse();
}

void Engine::DeInit()
{
}

void Engine::LoadMenuResource()
{

#pragma region Chargement des textures

#pragma region Blocs

#ifndef TEXTUREARRAY_INITIALIZED
#define TEXTUREARRAY_INITIALIZED
#endif

	// Texture des blocs 128x128 px
	m_textureArray = new TextureArray(128);
	LoadBlocTexture(BTYPE_DIRT, TEXTURE_PATH "b_dirt.bmp");
	LoadBlocTexture(BTYPE_GRASS, TEXTURE_PATH "b_grass.jpg");
	LoadBlocTexture(BTYPE_BRICK, TEXTURE_PATH "b_brick_red.jpg");
	LoadBlocTexture(BTYPE_SAND, TEXTURE_PATH "b_sand.jpg");
	LoadBlocTexture(BTYPE_ROCK, TEXTURE_PATH "b_rock.jpg");
	LoadBlocTexture(BTYPE_SNOW, TEXTURE_PATH "b_snow.jpg");
	LoadBlocTexture(BTYPE_SWAMP, TEXTURE_PATH "b_swamp.jpg");

	m_textureArray->Generate();

#pragma endregion

#pragma region Boutons des spells

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

	m_textureInterface[CUSTIMAGE_BLACK_BACK].Load(TEXTURE_PATH "noir.jpg");
	m_textureInterface[CUSTIMAGE_BOO].Load(TEXTURE_PATH "i_boo.png");
	m_textureInterface[CUSTIMAGE_RUN].Load(TEXTURE_PATH "i_bewareofcthulhu.png");
	m_textureInterface[CUSTIMAGE_CROSSHAIR].Load(TEXTURE_PATH "i_cross.bmp");
	m_textureInterface[CUSTIMAGE_INTERFACE_FRAME].Load(TEXTURE_PATH "b_rock.jpg");
	m_textureInterface[CUSTIMAGE_PORTRAIT_FRAME].Load(TEXTURE_PATH "i_portrait-frame.png");
	m_textureInterface[CUSTIMAGE_PORTRAIT_MALE].Load(TEXTURE_PATH "i_portrait-male.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH].Load(TEXTURE_PATH "i_pgb_health.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_ENERGY].Load(TEXTURE_PATH "i_pgb_energy.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_MANA].Load(TEXTURE_PATH "i_pgb_mana.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_EXP].Load(TEXTURE_PATH "i_pgb_exp.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH_BACK].Load(TEXTURE_PATH "i_pgb_health_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_ENERGY_BACK].Load(TEXTURE_PATH "i_pgb_energy_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_MANA_BACK].Load(TEXTURE_PATH "i_pgb_mana_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_EXP_BACK].Load(TEXTURE_PATH "i_pgb_exp_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH_LOW].Load(TEXTURE_PATH "i_pgb_health_low.png");
	m_textureInterface[CUSTIMAGE_CLOCK_BG].Load(TEXTURE_PATH "i_clock_bg.png");
	m_textureInterface[CUSTIMAGE_CONSOLE_BACK].Load(TEXTURE_PATH "i_console_back.png");
	m_textureInterface[CUSTIMAGE_CONSOLE_TEXTBOX_BACK].Load(TEXTURE_PATH "i_console_textbox_back.png");
	m_textureInterface[CUSTIMAGE_PERSONAL_CURSOR].Load(TEXTURE_PATH "i_cursor.png");
	m_textureInterface[CUSTIMAGE_LOADING_SCREEN].Load(TEXTURE_PATH "i_loading.jpg");
	m_textureInterface[CUSTIMAGE_MENU_BACKGROUND].Load(TEXTURE_PATH "i_menu_back.png");
	m_textureInterface[CUSTIMAGE_MENU_MAIN_WINDOW].Load(TEXTURE_PATH "i_menu_main.png");
	m_textureInterface[CUSTIMAGE_MENU_BUTTON_BACK].Load(TEXTURE_PATH "i_menu_button.png");
	m_textureInterface[CUSTIMAGE_MENU_LOGO].Load(TEXTURE_PATH "i_menu_logo.png");

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

#pragma region Controles du menu

#ifndef MENU_INTERFACE_INITIALIZED
#define MENU_INTERFACE_INITIALIZED
#endif

	//Cursor
	m_pb_cursor = new PictureBox(0, Vector2i(), Vector2i(50, 50), &m_textureInterface[CUSTIMAGE_PERSONAL_CURSOR], "pb_cursor");
	//Appel singulier du cursor afin qu'il soit dessiné par dessus tous les éléments de l'interface
	//m_pnl_screen->AddControl(m_pb_cursor);
	m_pb_cursor->SetRepeatTexture(false);

	//Fond d'écran
	m_menuUI->m_menu_screen = new Panel(0, Vector2i(), Vector2i(Width(), Height()), &m_textureInterface[CUSTIMAGE_MENU_BACKGROUND], 1, "menu_main");
	m_menuUI->m_menu_screen->SetRepeatTexture(false);

	//Loading screen
	m_menuUI->m_menu_loading = new PictureBox(0, Vector2i(0, 0), Vector2i(Width(), Height()),&m_textureInterface[CUSTIMAGE_LOADING_SCREEN], "loading");
	m_menuUI->m_menu_loading->SetVisible(false);
	m_menuUI->m_menu_loading->SetRepeatTexture(false);

	//Paneau principal du menu
	m_menuUI->m_menu_panel = new Panel(m_menuUI->m_menu_screen, 
		Vector2i(Width() / 2 - MENU_PANEL_SIZE_X / 2, Height() / 2 - MENU_PANEL_SIZE_Y / 2), 
		Vector2i(MENU_PANEL_SIZE_X, MENU_PANEL_SIZE_Y), &m_textureInterface[CUSTIMAGE_MENU_MAIN_WINDOW], 2, MENU_PANEL_NAME);
	m_menuUI->m_menu_screen->AddControl(m_menuUI->m_menu_panel);
	m_menuUI->m_menu_panel->SetRepeatTexture(false);

	int controlWidth = m_menuUI->m_menu_panel->Size().x * 0.8f;
	int controlPosX = m_menuUI->m_menu_panel->Size().x / 2 - controlWidth / 2;

	//Logo du jeu
	m_menuUI->m_menu_logo = new PictureBox(m_menuUI->m_menu_panel, 
		Vector2i(controlPosX, m_menuUI->m_menu_panel->Size().y - MENU_LOGO_SIZE_Y - controlPosX),
		Vector2i(controlWidth, MENU_LOGO_SIZE_Y), &m_textureInterface[CUSTIMAGE_MENU_LOGO], "logo");
	m_menuUI->m_menu_panel->AddControl(m_menuUI->m_menu_logo);
	m_menuUI->m_menu_logo->SetRepeatTexture(false);

	//Panneau de controle utilisateur
	m_menuUI->m_menu_controls = new Panel(m_menuUI->m_menu_panel,
		Vector2i(controlPosX, controlPosX),
		Vector2i(controlWidth, MENU_CONTROLS_SIZE_Y),
		&m_textureInterface[CUSTIMAGE_MENU_MAIN_WINDOW], 3, "menu_controls");
	m_menuUI->m_menu_panel->AddControl(m_menuUI->m_menu_controls);
	m_menuUI->m_menu_controls->SetRepeatTexture(false);

	int buttonWidth = m_menuUI->m_menu_controls->Size().x * 0.8f;
	int buttonPosX = m_menuUI->m_menu_controls->Size().x / 2 - buttonWidth / 2;

	//Button demarrer fullscreen
	m_menuUI->m_menu_fullscreen = new Button(m_menuUI->m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 3 - MENU_BUTTONS_INTERVAL), Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y),
		&m_textureInterface[CUSTIMAGE_MENU_BUTTON_BACK], &m_texturefontColor[TEXTCOLOR_YELLOW],
		STRING_BUTTON_NORM_START, MENU_BUTTON_START_FULL_NAME);
	m_menuUI->m_menu_controls->AddControl(m_menuUI->m_menu_fullscreen);
	m_menuUI->m_menu_fullscreen->SetRepeatTexture(false);
	m_menuUI->m_menu_fullscreen->OnClick.Attach(this, &Engine::OnClick);

	//Button demarrer debug
	m_menuUI->m_menu_start = new Button(m_menuUI->m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 2 - MENU_BUTTONS_INTERVAL * 2), Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y),
		&m_textureInterface[CUSTIMAGE_MENU_BUTTON_BACK], &m_texturefontColor[TEXTCOLOR_YELLOW],
		STRING_BUTTON_DEBUG_START, MENU_BUTTON_DEBUG);
	m_menuUI->m_menu_controls->AddControl(m_menuUI->m_menu_start);
	m_menuUI->m_menu_start->SetRepeatTexture(false);
	m_menuUI->m_menu_start->OnClick.Attach(this, &Engine::OnClick);

	//Button fermer
	m_menuUI->m_menu_close = new Button(m_menuUI->m_menu_controls,
		Vector2i(buttonPosX, buttonPosX + MENU_BUTTONS_SIZE_Y * 1 - MENU_BUTTONS_INTERVAL * 3), Vector2i(buttonWidth, MENU_BUTTONS_SIZE_Y),
		&m_textureInterface[CUSTIMAGE_MENU_BUTTON_BACK], &m_texturefontColor[TEXTCOLOR_YELLOW],
		STRING_BUTTON_CLOSE, MENU_BUTTON_CLOSE);
	m_menuUI->m_menu_controls->AddControl(m_menuUI->m_menu_close);
	m_menuUI->m_menu_close->SetRepeatTexture(false);
	m_menuUI->m_menu_close->OnClick.Attach(this, &Engine::OnClick);

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

void Engine::LoadGameResource()
{

#pragma region Controles du jeu

#ifndef GAME_INTERFACE_INITIALIZED
#define GAME_INTERFACE_INITIALIZED
#endif
	// Écran
	m_gameUI->m_pnl_screen = new Panel(0, Vector2i(), Vector2i(Width(), Height()), 0, 1, "main");

#pragma region Enfants de Main

	// Zone de jeu
	m_gameUI->m_pnl_playscreen = new Panel(m_gameUI->m_pnl_screen, 
		Vector2i(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(m_gameUI->m_pnl_screen->Size().x - INTERFACE_SIDE_LEFT_WIDTH - INTERFACE_SIDE_RIGHT_WIDTH, 
		m_gameUI->m_pnl_screen->Size().y - INTERFACE_BOTTOM_HEIGHT - INTERFACE_TOP_HEIGHT),
		0, PNL_PLAYSCREEN_CONTROLS_NBR, "playscreen");
	m_gameUI->m_pnl_screen->AddControl(m_gameUI->m_pnl_playscreen);

#pragma region Enfants de Playscreen

	// Informations
	m_gameUI->m_lb_infos = new ListBox(m_gameUI->m_pnl_playscreen, Vector2i(5, m_gameUI->m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*6 - 26*12), 200, &m_texturefontColor[TEXTCOLOR_RED], 
		0, 26, 2, 12.f, 12.f, 0.5f, false, "lb_infos");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lb_infos);
	m_gameUI->m_lb_infos->AddLine("Controles Mouvements");
	m_gameUI->m_lb_infos->AddLine("Avancer:           W");
	m_gameUI->m_lb_infos->AddLine("Reculer:           S");
	m_gameUI->m_lb_infos->AddLine("Droite:            D");
	m_gameUI->m_lb_infos->AddLine("Gauche:            A");
	m_gameUI->m_lb_infos->AddLine("Sauter:            Espace");
	m_gameUI->m_lb_infos->AddLine("Marcher:           Ctrl");
	//m_gameUI->m_lb_infos->AddLine("Se pencher:        C");
	m_gameUI->m_lb_infos->AddLine("Courir:            Shift");
	m_gameUI->m_lb_infos->AddLine("");
	m_gameUI->m_lb_infos->AddLine("Controles Debug");
	m_gameUI->m_lb_infos->AddLine("Tirer:             1");
	m_gameUI->m_lb_infos->AddLine("Cochon:            2");
	m_gameUI->m_lb_infos->AddLine("Teleporter:        6");
	m_gameUI->m_lb_infos->AddLine("Remplir Vie:       7");
	m_gameUI->m_lb_infos->AddLine("Remplir Energie:   8");
	m_gameUI->m_lb_infos->AddLine("Augmenter Exp:     P");
	m_gameUI->m_lb_infos->AddLine("");
	m_gameUI->m_lb_infos->AddLine("Options");
	m_gameUI->m_lb_infos->AddLine("Wireframe:         Y");
	m_gameUI->m_lb_infos->AddLine("Music On/off       O");
	m_gameUI->m_lb_infos->AddLine("Music Next         M");
	m_gameUI->m_lb_infos->AddLine("Aff/Cach Infos:    F9");
	//m_gameUI->m_lb_infos->AddLine("Fullscreen         F10");
	m_gameUI->m_lb_infos->AddLine("Quitter            Esc");

	//Fenetre de console
	m_gameUI->m_lb_console = new ListBox(m_gameUI->m_pnl_playscreen, 
		Vector2i(m_gameUI->m_pnl_playscreen->Size().x - 64 - (int)LB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H + 5), 
		LB_CONSOLE_SIZE_W, 
		&m_texturefontColor[TEXTCOLOR_YELLOW], 
		&m_textureInterface[CUSTIMAGE_CONSOLE_BACK], 
		LB_CONSOLE_LINE_NUMBER, 
		LB_CONSOLE_LINE_GAP, 
		LB_CONSOLE_CHAR_W, 
		LB_CONSOLE_CHAR_H, 
		LB_CONSOLE_CHAR_I, 
		LB_CONSOLE_SCROLLABLE, 
		LB_CONSOLE_NAME,
		Vector2i(LB_CONSOLE_BODER_OFFSET_S, LB_CONSOLE_BODER_OFFSET_B));
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lb_console);
	m_gameUI->m_lb_console->SetRepeatTexture(false);
	Info::Get().SetConsole(m_gameUI->m_lb_console);
	//Texbox de la console
	m_gameUI->m_txb_console = new Textbox(m_gameUI->m_pnl_playscreen,
		Vector2i(m_gameUI->m_pnl_playscreen->Size().x - 64 - (int)LB_CONSOLE_SIZE_W, 0),
		Vector2i(TXB_CONSOLE_SIZE_W, TXB_CONSOLE_SIZE_H),
		&m_texturefontColor[TEXTCOLOR_WHITE],
		&m_textureInterface[CUSTIMAGE_CONSOLE_TEXTBOX_BACK],
		Label::TEXTDOCK_MIDDLELEFT,
		LBL_GENERIC_ITALIC,
		TXB_CONSOLE_SIZE_H * 0.75f,
		TXB_CONSOLE_CHAR_I,
		Vector2f(TXB_CONSOLE_OFFSET_X, TXB_CONSOLE_OFFSET_Y),
		TXB_CONSOLE_NAME);
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_txb_console);
	m_gameUI->m_txb_console->SetRepeatTexture(false);
	m_gameUI->m_txb_console->SetVisible(false);
	m_gameUI->m_txb_console->SetFocus(false);
	// Frame portrait
	m_gameUI->m_pnl_portrait = new Panel(m_gameUI->m_pnl_playscreen,
		Vector2i(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Vector2i(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H),
		&m_textureInterface[CUSTIMAGE_PORTRAIT_FRAME], PNL_PORTRAIT_CONTROLS_NBR, PNL_PORTRAIT_NAME);
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_pnl_portrait);

	m_gameUI->m_lbl_currentBlockType = new Label(m_gameUI->m_pnl_playscreen, Vector2i(m_gameUI->m_pnl_portrait->Position().x,
		m_gameUI->m_pnl_portrait->Position().y + m_gameUI->m_pnl_portrait->Size().y + 10),
		&m_texturefontColor[TEXTCOLOR_BLUE], "Bloc : ", Label::TEXTDOCK_NONE, false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "lblcurblock");
	m_gameUI->m_pb_currentBlockType = new PictureBox(m_gameUI->m_pnl_playscreen, Vector2i(m_gameUI->m_lbl_currentBlockType->Position().x + 84, m_gameUI->m_lbl_currentBlockType->Position().y), Vector2i(20,20), 
		m_textureArray->GetTexture(m_currentBlockType - 1), "pbcurbloc");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lbl_currentBlockType);
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_pb_currentBlockType);

#pragma region Enfants pnl portrait

	// Barre de vie
	m_gameUI->m_pgb_health = new ProgressBar(m_gameUI->m_pnl_portrait,
		Vector2i(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Vector2i(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H),
		&m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH], &m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_HEALTH_BACKGROUND, PGB_HEALTH_BORDER_SIZE, PGB_HEALTH_NAME);
	m_gameUI->m_pnl_portrait->AddControl(m_gameUI->m_pgb_health);
	// Barre de mana
	m_gameUI->m_pgb_mana = new ProgressBar(m_gameUI->m_pnl_portrait,
		Vector2i(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Vector2i(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H),
		&m_textureInterface[CUSTIMAGE_PGBTEXT_MANA], &m_textureInterface[CUSTIMAGE_PGBTEXT_MANA_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_MANA_BACKGROUND, PGB_MANA_BORDER_SIZE, PGB_MANA_NAME);
	m_gameUI->m_pnl_portrait->AddControl(m_gameUI->m_pgb_mana);
	// Barre d'expérience
	m_gameUI->m_pgb_exp = new ProgressBar(m_gameUI->m_pnl_portrait,
		Vector2i(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Vector2i(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H),
		&m_textureInterface[CUSTIMAGE_PGBTEXT_EXP], &m_textureInterface[CUSTIMAGE_PGBTEXT_EXP_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_EXP_BACKGROUND, PGB_EXP_BORDER_SIZE, PGB_EXP_NAME);
	m_gameUI->m_pnl_portrait->AddControl(m_gameUI->m_pgb_exp);
	m_gameUI->m_lbl_health = new Label(m_gameUI->m_pnl_portrait, Vector2i(LBL_HEALTH_POSITION_X, LBL_HEALTH_POSITION_Y), &m_texturefontColor[TEXTCOLOR_RED], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_HEALTH_NAME);
	m_gameUI->m_pnl_portrait->AddControl(m_gameUI->m_lbl_health);
	// Label de mana
	m_gameUI->m_lbl_mana = new Label(m_gameUI->m_pnl_portrait, Vector2i(LBL_MANA_POSITION_X, LBL_MANA_POSITION_Y), &m_texturefontColor[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, PNL_PORTRAIT_CHAR_H, PNL_PORTRAIT_CHAR_W, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_MANA_NAME);
	m_gameUI->m_pnl_portrait->AddControl(m_gameUI->m_lbl_mana);
	// Label d'exp
	m_gameUI->m_lbl_exp = new Label(m_gameUI->m_pnl_portrait, Vector2i(LBL_EXP_POSITION_X, LBL_EXP_POSITION_Y), &m_texturefontColor[TEXTCOLOR_YELLOW], "", 
		Label::TEXTDOCK_NONE, PNL_PORTRAIT_ITALIC, LBL_EXP_CHAR_W, LBL_EXP_CHAR_H, PNL_PORTRAIT_CHAR_I, Vector2f(), LBL_EXP_NAME);
	m_gameUI->m_pnl_portrait->AddControl(m_gameUI->m_lbl_exp);

	// Image du joueur
	m_gameUI->m_pnl_playerImage = new Panel(m_gameUI->m_pnl_portrait, 
		Vector2i(PB_PORTRAIT_POSITION_X, PB_PORTRAIT_POSITION_Y),
		Vector2i(PB_PORTRAIT_SIZE_W, PB_PORTRAIT_SIZE_H),
		&m_textureInterface[CUSTIMAGE_PORTRAIT_MALE], 1, PB_PORTRAIT_NAME);
	m_gameUI->m_pnl_portrait->AddControl(m_gameUI->m_pnl_playerImage);

#pragma region Enfants de m_pnl_playerImage

	m_gameUI->m_lbl_playerLevel = new Label(m_gameUI->m_pnl_playerImage, Vector2i(), &m_texturefontColor[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_TOPCENTER, LBL_GENERIC_ITALIC, LBL_PLAYER_LEVEL_W, LBL_PLAYER_LEVEL_H, LBL_PLAYER_LEVEL_I, Vector2f(), LBL_PLAYER_LEVEL_NAME);
	m_gameUI->m_pnl_playerImage->AddControl(m_gameUI->m_lbl_playerLevel);

#pragma endregion

#pragma endregion

	//Barre d'énergie verticale
	m_gameUI->m_pgb_energy = new ProgressBar(m_gameUI->m_pnl_playscreen,
		Vector2i(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Vector2i(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H),
		&m_textureInterface[CUSTIMAGE_PGBTEXT_ENERGY], &m_textureInterface[CUSTIMAGE_PGBTEXT_ENERGY_BACK],
		ProgressBar::BARMODE_VERTICAL_DTU, PGB_ENERGY_BACKGROUND, PGB_ENERGY_BORDER_SIZE, PGB_ENERGY_NAME);
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_pgb_energy);
	//Label d'énergie
	m_gameUI->m_lbl_energy = new Label(m_gameUI->m_pnl_playscreen, Vector2i(LBL_ENERGY_POSITION_X, LBL_ENERGY_POSITION_Y), &m_texturefontColor[TEXTCOLOR_GREEN], "", 
		Label::TEXTDOCK_NONE, LBL_ENERGY_ITALIC, LBL_ENERGY_CHAR_H, LBL_ENERGY_CHAR_W, LBL_ENERGY_CHAR_I, Vector2f(), LBL_ENERGY_NAME);
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lbl_energy);

#pragma region Controles de debug

	//Label Position
	m_gameUI->m_lbl_plrPos = new Label(m_gameUI->m_pnl_playscreen, Vector2i(5, m_gameUI->m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H), &m_texturefontColor[TEXTCOLOR_GREEN], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "pos");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lbl_plrPos);
	//Label Vitesse
	m_gameUI->m_lbl_plrSpd = new Label(m_gameUI->m_pnl_playscreen, Vector2i(5, m_gameUI->m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*2), &m_texturefontColor[TEXTCOLOR_BLUE], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "spd");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lbl_plrSpd);
	//Label Acceleration
	m_gameUI->m_lbl_plrAcc = new Label(m_gameUI->m_pnl_playscreen, Vector2i(5, m_gameUI->m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*3), &m_texturefontColor[TEXTCOLOR_RED], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "acc");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lbl_plrAcc);
	//Label mouse position
	m_gameUI->m_lbl_mousePos = new Label(m_gameUI->m_pnl_playscreen, Vector2i(5, m_gameUI->m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*4), &m_texturefontColor[TEXTCOLOR_WHITE], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "Mpos");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lbl_mousePos);
	//Label FPS
	m_gameUI->m_lbl_FPS = new Label(m_gameUI->m_pnl_playscreen, Vector2i(5, m_gameUI->m_pnl_playscreen->Size().y - LBL_GENERIC_CHAR_H*5), &m_texturefontColor[TEXTCOLOR_YELLOW], "", 
		Label::TEXTDOCK_NONE, LBL_GENERIC_ITALIC, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "fps");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_lbl_FPS);

#pragma endregion

	//Heure
	m_gameUI->m_pnl_time = new Panel(m_gameUI->m_pnl_playscreen, Vector2i(m_gameUI->m_pnl_playscreen->Size().x - 128, m_gameUI->m_pnl_playscreen->Size().y - 64), 
		Vector2i(128, 64), &m_textureInterface[CUSTIMAGE_CLOCK_BG], 1, "clock");
	m_gameUI->m_pnl_playscreen->AddControl(m_gameUI->m_pnl_time);

#pragma region Enfants de m_pnl_time

	m_gameUI->m_lbl_time = new Label(m_gameUI->m_pnl_time, Vector2i(0,0), &m_texturefontColor[TEXTCOLOR_WHITE], "", Label::TEXTDOCK_MIDDLECENTER, false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "time");
	m_gameUI->m_pnl_time->AddControl(m_gameUI->m_lbl_time);

#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion

}

void Engine::LoadBlocTexture(BLOCK_TYPE type, std::string path)
{
	Info::Get().GetBlocInfo(type)->SetTextureIndex(m_textureArray->AddTexture(path));
}

void Engine::UnloadResource()
{
}

void Engine::RenderMenu(float elapsedTime)
{

#pragma region OpenGl

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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

	m_menuUI->m_menu_screen->Render();
	m_pb_cursor->Render();
	m_menuUI->m_menu_loading->Render();

#pragma region Premier render

	if (!IsFirstRun())
	{
		if (m_menuUI->m_menu_fullscreen->GetText() != STRING_BUTTON_NORM_CONT)
			m_menuUI->m_menu_fullscreen->SetTextTo(STRING_BUTTON_NORM_CONT);
		if (m_menuUI->m_menu_start->GetText() != STRING_BUTTON_DEBUG_ON && !Info::Get().Options().GetOptDebug())
			m_menuUI->m_menu_start->SetTextTo(STRING_BUTTON_DEBUG_ON);
		else if (m_menuUI->m_menu_start->GetText() != STRING_BUTTON_DEBUG_OFF && Info::Get().Options().GetOptDebug())
			m_menuUI->m_menu_start->SetTextTo(STRING_BUTTON_DEBUG_OFF);
	}

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

void Engine::Update(float elapsedTime)
{

#pragma region GameTime

	static float gameTime = elapsedTime;
	gameTime += elapsedTime;
	m_character.ReduceGlobalCooldown(elapsedTime);
	if (m_clickTimerOn)
		m_clickTimer += elapsedTime;

#pragma endregion

#pragma region Calcul la position du joueur et de la camera

	m_player->Move(m_ghostMode, m_character, elapsedTime);
	m_camera->SetPosition(m_player->Position());

	//Vérification de la mort du personnage
	if (m_character.Health() <= 0.999f)
	{
		m_player->ResetPosition();
		m_character.SetHealth(m_character.HealthMax());
		Info::Get().Sound().PlaySnd(Son::SON_DEATH, Son::CHANNEL_INTERFACE, true);
		CW("Vous etes mort!");

	}

#pragma endregion

#pragma region premier tour de boucle de l application

	//Solution temporaire pour changer la musique lors du premier render de l'engine
	static bool ttt = true;
	if (ttt)
	{
		Info::Get().Sound().PlayNextTrack();
		ttt = false;
		m_menuUI->m_menu_loading->SetVisible(false);
		CW("Premier Render de l'engine termine avec succes.");
	}

#pragma endregion

#pragma region Proprietes des controles

	//Affiche ou cache les infos s'il y a un changement
	if (m_gameUI->m_lb_infos->Visible() && !Info::Get().Options().GetOptInfos())
		m_gameUI->m_lb_infos->SetVisible(false);
	else if (!m_gameUI->m_lb_infos->Visible() && Info::Get().Options().GetOptInfos())
		m_gameUI->m_lb_infos->SetVisible(true);
	//Change la texture de la barre de vie en fonction du %. Ne réassigne la texture que si on en a besoin
	if (m_character.HealthPerc() <= PGB_HEALTH_LOW_TRESHOLD && m_gameUI->m_pgb_health->GetTexture() == &m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH])
		m_gameUI->m_pgb_health->SetTexture(&m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH_LOW]);
	else if (m_character.HealthPerc() > PGB_HEALTH_LOW_TRESHOLD && m_gameUI->m_pgb_health->GetTexture() == &m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH_LOW])
		m_gameUI->m_pgb_health->SetTexture(&m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH]);
	//Affiche ou cache la barre d'énergie selon la situation
	if (m_character.Energy() != m_character.EnergyMax() || Info::Get().Ctrls().Shift())
	{
		m_gameUI->m_pgb_energy->SetVisible(true);
		m_gameUI->m_lbl_energy->SetVisible(true);
	}
	else if (m_character.Energy() == m_character.EnergyMax())
	{
		m_gameUI->m_pgb_energy->SetVisible(false);
		m_gameUI->m_lbl_energy->SetVisible(false);
	}
	// Controles de debug
	if (Info::Get().Options().GetOptDebug() && !m_gameUI->m_lbl_FPS->Visible())
	{
		m_gameUI->m_lbl_plrPos->SetVisible(true);
		m_gameUI->m_lbl_plrSpd->SetVisible(true);
		m_gameUI->m_lbl_plrAcc->SetVisible(true);
		m_gameUI->m_lbl_mousePos->SetVisible(true);
		m_gameUI->m_lbl_FPS->SetVisible(true);
	}
	else if (!Info::Get().Options().GetOptDebug() && m_gameUI->m_lbl_FPS->Visible())
	{
		m_gameUI->m_lbl_plrPos->SetVisible(false);
		m_gameUI->m_lbl_plrSpd->SetVisible(false);
		m_gameUI->m_lbl_plrAcc->SetVisible(false);
		m_gameUI->m_lbl_mousePos->SetVisible(false);
		m_gameUI->m_lbl_FPS->SetVisible(false);
	}

#pragma endregion

#pragma region Actualisation des valeurs

	//Actualisation des valeurs des progressbars
	m_gameUI->m_pgb_health->SetValue(m_character.HealthPerc());
	m_gameUI->m_pgb_energy->SetValue(m_character.EnergyPerc());
	m_gameUI->m_pgb_mana->SetValue(m_character.ManaPerc());
	m_gameUI->m_pgb_exp->SetValue(m_character.ExpPerc());
	//Actualisation du texte dans les différents Label
	m_gameUI->TextUpdate();

#pragma endregion

#pragma region Ecriture console a partir de l exterieur

	if (Info::Get().LineToPrint() != "")
	{
		m_gameUI->m_lb_console->AddLine(Info::Get().LineToPrint());
		Info::Get().NextPrint("");
	}

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

#pragma region FPS

	m_fpstmr += elapsedTime;
	if (m_fpstmr > 1.5f)
	{
		float fps = 1 / elapsedTime;
		m_fps = fps >= 60 ? 60 : fps;
		m_fpstmr = 0;
	}

#pragma endregion

}

void Engine::Render(float elapsedTime)
{

#pragma region Game time

	static float gameTime = elapsedTime;
	gameTime += elapsedTime;

#pragma endregion

#pragma region OpenGl

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#pragma endregion

#pragma region Elements de la camera

	// 3rd person
	if (m_camera->GetMode() == Camera::CAM_THIRD_PERSON ) {
		// hide/show cursor
		if (!m_rightClick && !m_leftClick)
			m_pb_cursor->SetVisible(true);
		else
			m_pb_cursor->SetVisible(false);

		// recule la camera
		glTranslatef(0,0,-m_camRadius);
		// applique les transformations normales de la camera
		m_camera->ApplyRotation();
		m_camera->ApplyTranslation();

		// render le modele du player
		m_shaderModel.Use();
		m_player->Render(m_wireframe);
		Shader::Disable();
	} 
	// first person
	else
	{
		m_camera->ApplyRotation();
		m_camera->ApplyTranslation();
	}

	// Différentes matrices de opengl
	// pour utiliser avec render (fix batard)
	GLfloat mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	Matrix4f modelView(mv[0], mv[1], mv[2], mv[3],
		mv[4], mv[5], mv[6], mv[7],
		mv[8], mv[9], mv[10], mv[11],
		mv[12], mv[13], mv[14], mv[15]);

	GLfloat p[16];
	glGetFloatv(GL_PROJECTION_MATRIX, p);
	Matrix4f projection(p[0], p[1], p[2], p[3],
		p[4], p[5], p[6], p[7],
		p[8], p[9], p[10], p[11],
		p[12], p[13], p[14], p[15]);

	Matrix4f vp = modelView * projection;

#pragma endregion

#pragma region Render les cubes

	Shader::Disable();
	m_shaderCube.Use();
	m_textureArray->Use();
	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk* c = m_chunks->Get(i,j);
			if (c->IsDirty())
				c->Update();
			c->Render();
		}
	}

	Shader::Disable();

#pragma endregion

#pragma region Render models

	m_shaderModel.Use();

	for (unsigned short i = 0; i < MONSTER_MAX_NUMBER; i++)
	{
		if (m_monsters[i]->Initialized())
		{
			m_monsters[i]->Update(elapsedTime);
			m_monsters[i]->Render();
		}
	}
	Shader::Disable();


#pragma endregion

#pragma region Render spells

	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Update et render tous les spells
	for (SpellList::iterator it = m_spells.begin(); it != m_spells.end(); ++it) {
		it->SetDestination(m_monsters[0]->Position());
		it->Update(elapsedTime);
		it->Render(vp);
		if (it->HasHit())
		{
			m_spells.erase(it);
			break;
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	GetBlocAtCursor();

#pragma endregion

#pragma region Render l interface
	// HUD
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Render2D(elapsedTime);
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma endregion

}

void Engine::Render2D(float elapsedTime)
{

#pragma region OpenGl

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

#pragma region Affichage des controles

	//Render de l'écran au complet avec tous ses contrôles.
	m_gameUI->m_pnl_screen->Render();

#pragma endregion

#pragma region Images qui subissent le blend par le contraste

	//Activation du blend normal
	StartBlendPNG(false);


#pragma region Crosshair

	if (m_camera->GetMode() == Camera::CAM_FIRST_PERSON)
	{
		RenderSquare(
			Vector2i(Width() / 2 - CROSSHAIR_SIZE / 2, Height() / 2 - CROSSHAIR_SIZE / 2),
			Vector2i(CROSSHAIR_SIZE, CROSSHAIR_SIZE),
			m_textureInterface[CUSTIMAGE_CROSSHAIR], false);
	}

#pragma endregion

	glDisable(GL_BLEND);

#pragma endregion

#pragma region Affichage de l interface sans transparence

	//============================================

	//Bottom
	RenderSquare(
		Vector2i(0, 0), 
		Vector2i(Width(), INTERFACE_BOTTOM_HEIGHT), 
		m_textureInterface[CUSTIMAGE_INTERFACE_FRAME]);
	//============================================
	RenderSpells();
	//============================================


#pragma endregion

	m_pb_cursor->Render();

#pragma region OpenGl

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

#pragma endregion

}

void Engine::TextenteredEvent(unsigned int val)
{
	if (!IsMenuOpen())
	{
		static std::ostringstream ss;

		//Appuie sur Enter pour acquérir le focus
		if (val == 13 && !m_gameUI->m_txb_console->HasFocus())
		{
			m_gameUI->m_txb_console->SetFocus(true);
			m_gameUI->m_txb_console->SetVisible(true);
			return;
		}

		//Capture du texte en mode Textbox
		if (m_gameUI->m_txb_console->HasFocus())
		{
			//Vérification qu'il s'agit d'un caractère valide
			if (val >= 32 && val <= 126)
			{
				ss << m_gameUI->m_txb_console->GetMsg() << static_cast<char>(val);
				m_gameUI->m_txb_console->SetMessage(ss.str());
				ss.str("");
				return;
			}
			//Si c'est un backspace
			if (val == 8)
			{
				std::string mes = m_gameUI->m_txb_console->GetMsg();
				if (mes.length() > 0)
				{
					for (int i = 0; i < mes.length()-1; i++)
					{
						ss << mes[i];
					}
					m_gameUI->m_txb_console->SetMessage(ss.str());
					ss.str("");
				}
			}
			//Si c'est un return
			if (val == 13)
			{
				if (m_gameUI->m_txb_console->GetMsg() != "")
					CW(m_gameUI->m_txb_console->GetMsg());
				m_gameUI->m_txb_console->SetVisible(false);
				m_gameUI->m_txb_console->SetFocus(false);
				if (m_gameUI->m_txb_console->GetMsg() == "/dance")
					Info::Get().Sound().PlaySnd(Son::SON_DEATH, Son::CHANNEL_INTERFACE);
				m_gameUI->m_txb_console->SetMessage("");
			}
		}
	}
}

void Engine::KeyPressEvent(unsigned char key)
{
	Son& sound = Info::Get().Sound();
	static std::ostringstream ss;
	Controls& c = Info::Get().Ctrls();

	if (!IsMenuOpen())
	{
#pragma region Touches dans le jeu
		//Affiche les numéros de touche dans la console de jeu
		//ss << (int)key;
		//CW(ss.str());
		//ss.str("");

		//Si on est pas en train d'écrire, envoie les données de touches
		if (!m_gameUI->m_txb_console->HasFocus())
		{
			c.Set(key, true);
			//Sorts qui subissent le global cooldown
			if(m_character.GlobalCooldown() == 0)
			{
				if (c.n1())
				{
					Spell* newSpell = new Spell;
					newSpell->SetPosition(m_player->Position());
					newSpell->Init(4.f, m_player->RotationQ());
					newSpell->Shoot();
					m_spells.push_back(*newSpell);
					sound.PlaySnd(Son::SON_BOLT, Son::CHANNEL_SPELL);
					m_character.ResetGlobalCooldown();
					ss << "Lancement de sort: Test  de particule!";
				}

				if (c.n2())
				{
					for (unsigned short i = 0; i < MONSTER_MAX_NUMBER; i++)
						m_monsters[i]->SetPosition(Vector3f(m_monsters[i]->Position().x, 10, m_monsters[i]->Position().z));
					sound.PlaySnd(Son::SON_FIRE, Son::CHANNEL_SPELL);
					m_character.ResetGlobalCooldown();
					ss << "Lancement de sort: teleportation de NPC!";
				}
				if (c.n3())
				{
					sound.PlaySnd(Son::SON_FREEZE, Son::CHANNEL_SPELL);
					m_character.ResetGlobalCooldown();
					ss << "Lancement de sort: Glace";
				}
				if (c.n4())
				{
					sound.PlaySnd(Son::SON_SHOCK, Son::CHANNEL_SPELL);
					m_character.ResetGlobalCooldown();
					ss << "Lancement de sort: Shock";
				}
				if (c.n5())
				{
					sound.PlaySnd(Son::SON_POISON, Son::CHANNEL_SPELL);
					m_character.ResetGlobalCooldown();
					ss << "Lancement de sort: Poison";
				}
				if (c.n7())
				{
					if (m_character.Mana() - 15 >= 0)
					{
						sound.PlaySnd(Son::SON_HEAL1, Son::CHANNEL_SPELL);
						m_character.ResetGlobalCooldown();
						m_character.SetHealth(15);
						m_character.SetMana(-15);
						ss << "Lancement de sort: Soin";
					}
				}
				if (c.n8())
				{
					if (m_character.Mana() - 10 >= 0)
					{
						sound.PlaySnd(Son::SON_HEAL2, Son::CHANNEL_SPELL);
						m_character.ResetGlobalCooldown();
						m_character.SetEnergy(10);
						m_character.SetMana(-10);
						ss << "Lancement de sort: Rafraichissement";
					}
				}
				if (c.n9())
				{
					sound.PlaySnd(Son::SON_DEFEND, Son::CHANNEL_SPELL);
					m_character.ResetGlobalCooldown();
					ss << "Lancement de sort: Defense";
				}
				if (c.n0())
				{
					sound.PlaySnd(Son::SON_SHIELD, Son::CHANNEL_SPELL);
					m_character.ResetGlobalCooldown();
					ss << "Lancement de sort: Bouclier magique";
				}
			}
			//Sorts hors global cooldown
			if (c.n6())
			{
				if (m_character.Mana() - 5 >= 0)
				{
					sound.PlaySnd(Son::SON_STORM, Son::CHANNEL_SPELL);
					m_character.SetMana(-5);
					m_player->Teleport();
					ss << "Lancement de sort: Teleportation";
				}
			}
		}
		if (ss.str() != "")
		{
			CW(ss.str());
			ss.str("");
		}
#pragma endregion
	}
	else
	{
#pragma region Touches dans le menu
		c.Set(key, true);
		//Actions à prendre lors de l'appui de touches dans le menu
#pragma endregion
	}

}

void Engine::KeyReleaseEvent(unsigned char key)
{
	static std::ostringstream ss;
	Controls& c = Info::Get().Ctrls();

	if (!IsMenuOpen())
	{
#pragma region Touches dans le jeu

		if (!m_gameUI->m_txb_console->HasFocus())
		{
			if(c.E())
			{
				m_currentBlockType++;
				if (m_currentBlockType % BTYPE_COUNT == BTYPE_AIR)
					m_currentBlockType++;
				m_currentBlockType = m_currentBlockType % BTYPE_COUNT;
				m_gameUI->m_pb_currentBlockType->SetTexture(m_textureArray->GetTexture(
					Info::Get().GetBlocInfo(m_currentBlockType)->GetTextureIndex()));
			}
			if(c.Q())
			{
				m_currentBlockType--;
				if (m_currentBlockType == BTYPE_AIR)
					m_currentBlockType--;
				m_currentBlockType = (m_currentBlockType + BTYPE_COUNT) % BTYPE_COUNT;
				m_gameUI->m_pb_currentBlockType->SetTexture(m_textureArray->GetTexture(
					Info::Get().GetBlocInfo(m_currentBlockType)->GetTextureIndex()));
			}
			if (c.Esc())
			{
				SetMenuStatus(!IsMenuOpen());
			}
			if (c.f9())
			{
				Info::Get().Options().SetOptInfos(!Info::Get().Options().GetOptInfos());
				ss << "Affichage des infos a: " << (Info::Get().Options().GetOptInfos() ? "on" : "off");
			}
			//if (c.f10())
			//{
			//	SetFullscreen(!IsFullscreen());
			//	ss << "Mode plein ecran mis a: " << (IsFullscreen() ? "on" : "off");
			//}
			if (c.G())
			{
				m_ghostMode = !m_ghostMode;
				ss << "Mode fantome mis a: " << (m_ghostMode ? "on" : "off");
			}
			if (c.Y())
			{
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
				ss << "Affichage Wireframe mis a: " << (m_wireframe ? "on" : "off");
			}
			if (c.V())
			{
				if (m_camera->GetMode() == Camera::CAM_FIRST_PERSON) 
				{
					m_camera->SetMode(Camera::CAM_THIRD_PERSON);
					ss << "Affichage de la camera a la troisieme personne";
				}
				else 
				{
					m_pb_cursor->SetVisible(false);
					m_camera->SetMode(Camera::CAM_FIRST_PERSON);
					ss << "Affichage de la camera a la premiere personne";
				}
			}
			if (c.M())
				Info::Get().Sound().PlayNextTrack();
			if (c.O())
			{
				Info::Get().Options().SetOptMusic(!Info::Get().Options().GetOptMusic());
				ss << "Musique mis a: " << (Info::Get().Options().GetOptMusic() ? "on" : "off");

			}
			if (c.P())
			{
				m_character.SetExp(75);
				ss << "Ajout de 75 points d'exp";
			}
			if (ss.str() != "")
			{
				CW(ss.str());
				ss.str("");
			}
		}
		c.Set(key, false);
#pragma endregion
	}
	else
	{
#pragma region Touches dans le menu
		//Actions à prendre lors du relachement des touches dans le menu
		if (c.Esc())
		{
			if (IsFirstRun())
				Stop();
			else SetMenuStatus(!IsMenuOpen());
		}
		c.Set(key, false);
#pragma endregion
	}
}

void Engine::MouseMoveEvent(int x, int y)
{
	// Centrer la souris seulement si elle n'est pas déjà centrée
	// Il est nécessaire de faire la vérification pour éviter de tomber
	// dans une boucle infinie où l'appel à CenterMouse génère un
	// MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre 
	// MouseMoveEvent, etc

	if (!IsMenuOpen())
	{
		// Camera 3rd person
		if (m_camera->GetMode() == Camera::CAM_THIRD_PERSON && m_rightClick || m_leftClick)
		{
			if (!MousePosChanged(x, y))
				return;

			MakeRelativeToMouse(x, y);
			m_camera->TurnLeftRight((float)x);
			m_camera->TurnTopBottom((float)y);

			if (m_rightClick) 
			{
				m_player->SetRotation(m_camera->GetRotation());
			}

			ResetMouse();
		}
		// Camera first person
		else if (m_camera->GetMode() == Camera::CAM_FIRST_PERSON)
		{
			if(x == (Width() / 2) && y == (Height() / 2))
				return;
			MakeRelativeToCenter(x, y);
			m_player->TurnLeftRight((float)x);
			m_player->TurnTopBottom((float)y);

			m_camera->SetRotation(m_player->Rotation());

			CenterMouse();
		}
	}

	m_pb_cursor->SetPosition(Vector2i(MousePosition().x, MousePosition().y - m_pb_cursor->Size().y));
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
	if (!IsMenuOpen())
	{
		Vector2i& pos = m_gameUI->m_lb_console->AbsolutePosition();
		Vector2i& size = m_gameUI->m_lb_console->Size();
		Vector2i& play = m_gameUI->m_pnl_screen->Size();
		switch (button)
		{
		case MOUSE_BUTTON_RIGHT:
			m_clickTimerOn = true;
			m_clickTimer = 0;
			m_rightClick = true;
			m_player->SetRotation(m_camera->GetRotation());
			SetMousePos(x, y);
			m_lastRot = m_camera->GetRotation();
			break;
		case MOUSE_BUTTON_LEFT:
			m_clickTimerOn = true;
			m_clickTimer = 0;
			m_gameUI->m_lb_console->MouseClick(x, m_gameUI->m_pnl_playscreen->Size().y - y);
			if (m_camera->GetMode() == Camera::CAM_THIRD_PERSON)
			{
				m_leftClick = true;
				SetMousePos(x, y);
			}
			if (m_camera->GetMode() == Camera::CAM_FIRST_PERSON)
				Info::Get().Sound().PlaySnd(Son::SON_CLICK, Son::CHANNEL_INTERFACE);
			m_lastRot = m_camera->GetRotation();
			break;
		case MOUSE_BUTTON_WHEEL_UP:
			if (x >= pos.x && x <= pos.x + size.x && play.y - y <= pos.y + size.y && play.y - y >= pos.y) {
				m_gameUI->m_lb_console->Scroll(1);
			} 
			else if (m_camera->GetMode() == Camera::CAM_THIRD_PERSON)
			{
				// Zoom in camera
				if (m_camRadius > 0)
				{
					m_camRadius--;
				}
				m_camera->SetCamRadius(m_camRadius);
				break;
		case MOUSE_BUTTON_WHEEL_DOWN:
			if (x >= pos.x && x <= pos.x + size.x && play.y - y <= pos.y + size.y && play.y - y >= pos.y) {
				m_gameUI->m_lb_console->Scroll(-1);
			} 
			else if (m_camera->GetMode() == Camera::CAM_THIRD_PERSON)
			{
				// Zoom out camera
				if (m_camRadius < 20)
				{
					m_camRadius++;
				}
				m_camera->SetCamRadius(m_camRadius);
			}
			break;
			}
		}
	}
	else
	{
		switch (button)
		{
		case MOUSE_BUTTON_LEFT:
			m_menuUI->m_menu_close->isClicked(x, m_menuUI->m_menu_screen->Size().y - y);
			m_menuUI->m_menu_start->isClicked(x, m_menuUI->m_menu_screen->Size().y - y);
			m_menuUI->m_menu_fullscreen->isClicked(x, m_menuUI->m_menu_screen->Size().y - y);
			break;
		}
	}
}

void Engine::OnClick(Control* sender)
{
	std::string n = sender->Name();
	Info::Get().Sound().PlaySnd(Son::SON_CLICK, Son::CHANNEL_INTERFACE, true);

	if (n == MENU_BUTTON_START_FULL_NAME)
	{
		if (!IsFirstRun())
			SetMenuStatus(false);
		else
		{
			Info::Get().Options().SetOptDebug(false);
			ActivateFirstRun();
			m_menuUI->m_menu_loading->SetVisible(true);
			SetMenuStatus(false);
		}
	}
	if (n == MENU_BUTTON_DEBUG)
	{
		if (!IsFirstRun())
			Info::Get().Options().SetOptDebug(!Info::Get().Options().GetOptDebug());
		else
		{
			Info::Get().Options().SetOptDebug(true);
			ActivateFirstRun();
			m_menuUI->m_menu_loading->SetVisible(true);
			SetMenuStatus(false);
		}
	}
	if (n == MENU_BUTTON_CLOSE)
		Stop();
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
	if (!IsMenuOpen())
	{
		switch (button)
		{
		case MOUSE_BUTTON_RIGHT:
			m_rightClick = false;
			m_clickTimerOn = false;
			if (m_clickTimer < 0.5f && abs(m_lastRot.x - m_camera->GetRotation().x) <= 1 
				&& abs(m_lastRot.y - m_camera->GetRotation().y) <= 1 && m_currentBlock.y != 0)
				RemoveBlock();
			break;
		case MOUSE_BUTTON_LEFT:
			m_leftClick = false;
			m_clickTimerOn = false;
			std::cout << (float)m_currentBlockType << std::endl;
			m_gameUI->m_lb_console->MouseRelease();
			if (m_clickTimer < 0.5f && m_lastRot == m_camera->GetRotation())
				AddBlock(m_currentBlockType);
			break;
		}
	}
	else
	{
		switch (button)
		{
		case MOUSE_BUTTON_LEFT:
			m_menuUI->m_menu_close->Release();
			m_menuUI->m_menu_start->Release();
			m_menuUI->m_menu_fullscreen->Release();
			break;
		}
	}
}

void Engine::AddBlock(BlockType type)
{
	if(m_currentBlock.x == -1)
		return;
	Vector3f pos = m_currentBlock;
	// Enleve la partie decimale
	Vector3i iPos(pos.x, pos.y, pos.z);
	// Calcul dans quel chunk la position est
	int chunkX, chunkZ;
	chunkX = iPos.x / CHUNK_SIZE_X;
	chunkZ = iPos.z / CHUNK_SIZE_Z;
	Chunk* c = m_chunks->Get(chunkX, chunkZ);
	// Calcul dans quel bloc la position est
	Vector3f bloc(
		iPos.x - chunkX * CHUNK_SIZE_X,
		iPos.y,
		iPos.z - chunkZ * CHUNK_SIZE_Z);

	bloc += m_currentFaceNormal;
	c->SetBloc(bloc.x, bloc.y, bloc.z, type);
}

void Engine::RemoveBlock()
{
	if(m_currentBlock.x == -1)
		return;
	Vector3f pos = m_currentBlock;
	// Enleve la partie decimale
	Vector3i iPos(pos.x, pos.y, pos.z);
	// Calcul dans quel chunk la position est
	int chunkX, chunkZ;
	chunkX = iPos.x / CHUNK_SIZE_X;
	chunkZ = iPos.z / CHUNK_SIZE_Z;
	Chunk* c = m_chunks->Get(chunkX, chunkZ);
	// Calcul dans quel bloc la position est
	Vector3f bloc(
		iPos.x - chunkX * CHUNK_SIZE_X,
		iPos.y,
		iPos.z - chunkZ * CHUNK_SIZE_Z);

	c->SetBloc(bloc.x, bloc.y, bloc.z, BTYPE_AIR);
}

//Private

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

void Engine::GetBlocAtCursor()
{
	int x = MousePosition().x;
	int y = Height() - MousePosition().y;

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	posX += .5f + VIEW_DISTANCE;
	posY += 2.5f;
	posZ += .5f + VIEW_DISTANCE;

	// Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
	// de valeurs negatives
	int px = (int)(posX);
	int py = (int)(posY);
	int pz = (int)(posZ);

	bool found = false;

	if((m_player->Position() - Vector3f(posX - VIEW_DISTANCE, posY, posZ - VIEW_DISTANCE)).Lenght() < MAX_SELECTION_DISTANCE)
	{
		// Apres avoir determine la position du bloc en utilisant la partie entiere du hit
		// point retourne par opengl, on doit verifier de chaque cote du bloc trouve pour trouver
		// le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
		// nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
		// 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
		// A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
		// position 15 par exemple nous retourne plutot la position 15.0001
		for(int x = px - 1; !found && x <= px + 1; ++x)
		{
			for(int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
			{
				for(int z = pz - 1; !found && y >= 0  && z <= pz + 1; ++z)
				{
					if(z >= 0)
					{
						BlockType bt = Info::Get().GetBlocFromWorld(Vector3f(x - VIEW_DISTANCE - 0.5f, y, z - VIEW_DISTANCE - 0.5f));
						if(bt == BTYPE_AIR)
							continue;

						// Skip water blocs
						//if(bloc->Type == BT_WATER)
						//    continue;

						m_currentBlock.x = x;
						m_currentBlock.y = y;
						m_currentBlock.z = z;

						if(Tool::InRangeWithEpsilon<float>(posX, x, x+1, 0.05) && Tool::InRangeWithEpsilon<float>(posY, y, y+1, 0.05) && Tool::InRangeWithEpsilon<float>(posZ, z, z+1, 0.05))
						{
							found = true;
						}
					}
				}
			}
		}
	}

	if(!found)
	{
		m_currentBlock.x = -1;
	}
	else
	{
		// Find on which face of the bloc we got an hit
		m_currentFaceNormal.Zero();

		// Front et back:
		if(Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z, 0.005f))
			m_currentFaceNormal.z = -1;
		else if(Tool::EqualWithEpsilon<float>(posZ, m_currentBlock.z + 1, 0.005f))
			m_currentFaceNormal.z = 1;
		else if(Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x, 0.005f))
			m_currentFaceNormal.x = -1;
		else if(Tool::EqualWithEpsilon<float>(posX, m_currentBlock.x + 1, 0.005f))
			m_currentFaceNormal.x = 1;
		else if(Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y, 0.005f))
			m_currentFaceNormal.y = -1;
		else if(Tool::EqualWithEpsilon<float>(posY, m_currentBlock.y + 1, 0.005f))
			m_currentFaceNormal.y = 1;
	}
}


void Engine::CW(const std::string& line)
{
	m_gameUI->m_lb_console->AddLine(line);
}

void Engine::CWL(const std::string& line)
{
	//m_lb_console->SetLine(0, m_lb_console->GetLine(0) + line);
}