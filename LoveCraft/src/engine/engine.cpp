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

#pragma region Constructeur et reference

Engine::Engine() : m_wireframe(false), m_angle(0), m_ghostMode(false),
	m_rightClick(false), m_leftClick(false), m_camRadius(10), m_fpstmr(0),
	m_clickTimer(0), m_currentBlockType(0), m_chunkLoader(&m_mutex)
{
	m_textureSpell = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureSpellX = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureInterface = new Texture*[IMAGE::CUSTIMAGE_LAST];
	for (unsigned short i = 0; i < IMAGE::CUSTIMAGE_LAST; i++)
		m_textureInterface[i] = new Texture();
	m_texturefontColor = new Texture*[COLOR::TEXTCOLOR_LAST];
	for (unsigned short i = 0; i < COLOR::TEXTCOLOR_LAST; i++)
		m_texturefontColor[i] = new Texture();
	m_monsters = new Animal*[MONSTER_MAX_NUMBER];

	//m_skybox = new Skybox();

	m_camera = new Camera;
	Info::Get().StatusOn(Info::LSTATUS_CAMERA);
	m_player = new Player(Vector3f(VIEW_DISTANCE, 0, VIEW_DISTANCE));
	Info::Get().StatusOn(Info::LSTATUS_PLAYER);
	m_character = new Character;
	Info::Get().StatusOn(Info::LSTATUS_CHARACTER);
}

Engine::~Engine()
{
	if (Info::Get().GetStatus(Info::LSTATUS_PLAYER))
		delete m_player;
	if (Info::Get().GetStatus(Info::LSTATUS_CAMERA))
		delete m_camera;
	if (Info::Get().GetStatus(Info::LSTATUS_CHARACTER))
		delete m_character;
	if (Info::Get().GetStatus(Info::LSTATUS_CHUNK))
	{
		// delete les chunks
		for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2 - 1; i++)
		{
			for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2 - 1; ++j)
			{
				if (m_chunks != 0)
					delete m_chunks->Get(i, j);
			}
		}
	}
	if (Info::Get().GetStatus(Info::LSTATUS_TEXTURE_IMAGE))
	{
		for (int i = 0; i < CUSTIMAGE_LAST; i++)
		{
			delete m_textureInterface[i];
		}
		delete [] m_textureInterface;
	}
	if (Info::Get().GetStatus(Info::LSTATUS_TEXTURE_FONTS))
	{
		for (int i = 0; i < TEXTCOLOR_LAST; i++)
		{
			delete m_texturefontColor[i];
		}
		delete [] m_texturefontColor;
	}
	if (Info::Get().GetStatus(Info::LSTATUS_MONSTERS))
	{
		// delete les monstres
#ifdef LOAD_MODELS
		for (unsigned short i = 0; i < MONSTER_MAX_NUMBER; i++)
			delete m_monsters[i];
#else
		delete m_monsters[0];
#endif
	}
	if (Info::Get().GetStatus(Info::LSTATUS_SKYBOX))
		//delete m_skybox;
			delete [] m_monsters;
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

#pragma endregion

#pragma region Initialisation

void Engine::GlobalInit()
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

#pragma region Initialisation des timers

	m_timertest = new Timer();
	m_timertest->Init(2.0f);
	m_timeranimationplus = new Timer();
	m_timeranimationplus->Init(0.01f);
	m_timeranimationplus->InitControl("plus");
	m_timeranimationmoins = new Timer();
	m_timeranimationmoins->Init(0.01f);
	m_timeranimationmoins->InitControl("moins");

	m_timertest->OnTick.Attach(this, &Engine::Timertest_OnTick);
	m_timeranimationplus->OnTick.Attach(this, &Engine::TimerAnimation_OnTick);
	m_timeranimationmoins->OnTick.Attach(this, &Engine::TimerAnimation_OnTick);

#pragma endregion

	CenterMouse();
	HideCursor();
}

void Engine::MenuInit()
{
	m_valuesMenuInterface.Init(m_textureInterface, m_texturefontColor);
	m_valuesMenuInterface.Update(MousePosition(), Width(), Height());
	m_menuUI.Init(m_valuesMenuInterface);
	m_debugUI.Init(m_valuesMenuInterface);
}

void Engine::GameInit()
{
	m_currentBlockType = BTYPE_DIRT;

#pragma region Initialisation des chunks

	Info::Get().SetCubeShader(&m_shaderCube);

	m_chunks = new Array2d<Chunk*>(VIEW_DISTANCE / CHUNK_SIZE_X * 2, VIEW_DISTANCE / CHUNK_SIZE_Z * 2);
	Info::Get().SetChunkArray(m_chunks);

	int test = VIEW_DISTANCE / CHUNK_SIZE_X * 2;

	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; ++i)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk* c = new Chunk(Vector2i(i,j), Vector2f(i,j));
			m_chunks->Set(i, j, c);
		}
	}

	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; ++i)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk* c = m_chunks->Get(i,j);
			c->SetSurroundings(Vector2i(i,j));
			c->GenerateTrees();
			c->SetIsReady(true);
		}
	}

	Info::Get().StatusOn(Info::LSTATUS_CHUNK);

#pragma endregion

	m_player->Init();

#pragma region Initialisation des entites

#ifndef MONSTERS_INIALIZED
#define MONSTERS_INIALIZED
#endif
	for (int i = 0; i < MONSTER_MAX_NUMBER; i++)
		m_monsters[i] = new Animal(Vector3f(Info::Get().Dic().Next((int)(VIEW_DISTANCE*0.5f), (int)(VIEW_DISTANCE*1.5f)), 0,
		Info::Get().Dic().Next((int)(VIEW_DISTANCE*0.5f), (int)(VIEW_DISTANCE*1.5f))));
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

#endif

	Info::Get().StatusOn(Info::LSTATUS_MONSTERS);

#pragma region Skybox

	//m_shaderCube.Use();
	//m_skybox->Init();
	//Shader::Disable();

#pragma endregion

#pragma endregion

	m_valuesGameInterface.Init(m_textureInterface, m_texturefontColor, m_textureArray, m_player, m_character);
	m_valuesGameInterface.Update(MousePosition(), Width(), Height(), m_currentBlockType, m_fps);
	m_gameUI.Init(m_valuesGameInterface);
}

void Engine::DeInit()
{
	m_menuUI.DeInit();
	m_gameUI.DeInit();
	m_debugUI.DeInit();
}

#pragma endregion

#pragma region Resources

void Engine::LoadGlobalResource()
{

#pragma region Chargement des textures

#pragma region Blocs
	// Texture des blocs 128x128 px
	m_textureArray = new TextureArray(128);
	LoadBlocTexture(BTYPE_DIRT, BFACE_ALL, TEXTURE_PATH "b_dirt.png");
	LoadBlocTexture(BTYPE_GRASS, BFACE_TOP, TEXTURE_PATH "b_grass.png");
	LoadBlocTexture(BTYPE_GRASS, BFACE_SIDES, TEXTURE_PATH "b_half_dirt.png");
	LoadBlocTexture(BTYPE_GRASS, BFACE_BOTTOM, TEXTURE_PATH "b_dirt.png");
	LoadBlocTexture(BTYPE_BRICK, BFACE_ALL, TEXTURE_PATH "b_brick_red.jpg");
	LoadBlocTexture(BTYPE_SAND, BFACE_ALL, TEXTURE_PATH "b_sand.jpg");
	LoadBlocTexture(BTYPE_ROCK, BFACE_ALL, TEXTURE_PATH "b_rock.jpg");
	LoadBlocTexture(BTYPE_SNOW, BFACE_ALL, TEXTURE_PATH "b_snow.jpg");
	LoadBlocTexture(BTYPE_SWAMP, BFACE_ALL, TEXTURE_PATH "b_swamp.jpg");
	LoadBlocTexture(BTYPE_TREELEAF, BFACE_ALL, TEXTURE_PATH "b_tree_leaf.png");
	LoadBlocTexture(BTYPE_TREETRUNK, BFACE_SIDES, TEXTURE_PATH "b_tree_trunk.png");
	LoadBlocTexture(BTYPE_TREETRUNK, BFACE_TOP_AND_BOT, TEXTURE_PATH "b_trunk_topbot.png");

	m_textureArray->Generate();

#pragma endregion

	m_noNormalMap.Load(TEXTURE_PATH "normal_map.jpg", true);

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

	m_textureInterface[CUSTIMAGE_BLACK_BACK]->Load(TEXTURE_PATH "noir.jpg");
	m_textureInterface[CUSTIMAGE_BOO]->Load(TEXTURE_PATH "i_boo.png");
	m_textureInterface[CUSTIMAGE_RUN]->Load(TEXTURE_PATH "i_bewareofcthulhu.png");
	m_textureInterface[CUSTIMAGE_CROSSHAIR]->Load(TEXTURE_PATH "i_cross.bmp");
	m_textureInterface[CUSTIMAGE_INTERFACE_FRAME]->Load(TEXTURE_PATH "b_rock.jpg");
	m_textureInterface[CUSTIMAGE_PORTRAIT_FRAME]->Load(TEXTURE_PATH "i_portrait-frame.png");
	m_textureInterface[CUSTIMAGE_PORTRAIT_MALE]->Load(TEXTURE_PATH "i_portrait-male.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH]->Load(TEXTURE_PATH "i_pgb_health.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_ENERGY]->Load(TEXTURE_PATH "i_pgb_energy.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_MANA]->Load(TEXTURE_PATH "i_pgb_mana.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_EXP]->Load(TEXTURE_PATH "i_pgb_exp.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH_BACK]->Load(TEXTURE_PATH "i_pgb_health_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_ENERGY_BACK]->Load(TEXTURE_PATH "i_pgb_energy_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_MANA_BACK]->Load(TEXTURE_PATH "i_pgb_mana_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_EXP_BACK]->Load(TEXTURE_PATH "i_pgb_exp_back.png");
	m_textureInterface[CUSTIMAGE_PGBTEXT_HEALTH_LOW]->Load(TEXTURE_PATH "i_pgb_health_low.png");
	m_textureInterface[CUSTIMAGE_CLOCK_BG]->Load(TEXTURE_PATH "i_clock_bg.png");
	m_textureInterface[CUSTIMAGE_CONSOLE_BACK]->Load(TEXTURE_PATH "i_console_back.png");
	m_textureInterface[CUSTIMAGE_CONSOLE_TEXTBOX_BACK]->Load(TEXTURE_PATH "i_console_textbox_back.png");
	m_textureInterface[CUSTIMAGE_PERSONAL_CURSOR]->Load(TEXTURE_PATH "i_cursor.png");
	m_textureInterface[CUSTIMAGE_LOADING_SCREEN]->Load(TEXTURE_PATH "i_loading.jpg");
	m_textureInterface[CUSTIMAGE_MENU_BACKGROUND]->Load(TEXTURE_PATH "i_menu_back.png");
	m_textureInterface[CUSTIMAGE_MENU_MAIN_WINDOW]->Load(TEXTURE_PATH "i_menu_main.png");
	m_textureInterface[CUSTIMAGE_MENU_BUTTON_BACK]->Load(TEXTURE_PATH "i_menu_button.png");
	m_textureInterface[CUSTIMAGE_MENU_LOGO]->Load(TEXTURE_PATH "i_menu_logo.png");
	m_textureInterface[CUSTIMAGE_WELCOME_FACE]->Load(TEXTURE_PATH "i_welcomeface.png");
	m_textureInterface[CUSTIMAGE_ARROWBUTTON_UP]->Load(TEXTURE_PATH "i_arrowbutton_up.jpg");
	m_textureInterface[CUSTIMAGE_ARROWBUTTON_DOWN]->Load(TEXTURE_PATH "i_arrowbutton_down.jpg");
	Info::Get().SetTexturesInterface(m_textureInterface);

	Info::Get().StatusOn(Info::LSTATUS_TEXTURE_IMAGE);
#pragma endregion

#pragma region Couleurs label

	m_texturefontColor[TEXTCOLOR_WHITE]->Load(TEXTURE_PATH "font.png");
	m_texturefontColor[TEXTCOLOR_RED]->Load(TEXTURE_PATH "font_red.png");
	m_texturefontColor[TEXTCOLOR_GREEN]->Load(TEXTURE_PATH "font_green.png");
	m_texturefontColor[TEXTCOLOR_BLUE]->Load(TEXTURE_PATH "font_blue.png");
	m_texturefontColor[TEXTCOLOR_YELLOW]->Load(TEXTURE_PATH "font_yellow.png");
	Info::Get().SetFonts(m_texturefontColor);

	Info::Get().StatusOn(Info::LSTATUS_TEXTURE_FONTS);
#pragma endregion

#pragma endregion

	//Cursor
	m_pb_cursor = new PictureBox();
	m_pb_cursor->InitControl("pb_cursor");
	m_pb_cursor->InitLocalizable(Point(), Size(50, 50), m_textureInterface[CUSTIMAGE_PERSONAL_CURSOR], 0);

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
	DirectionalLight dirLight;
	dirLight.AmbientIntensity = 0.5;
	dirLight.Color = Vector3f(1,1,1);
	dirLight.DiffuseIntensity = 0.3;
	dirLight.Direction = Vector3f(0.3, -1, 0.5).Normalise();

	PointLight* pointLights = new PointLight[2];
	PointLight& p1 = pointLights[0];
	p1.Color = Vector3f(1,1,1);
	p1.DiffuseIntensity = 0.5;
	p1.Position = Vector3f(64,65,64);
	p1.Attenuation.Linear = 0.1f;
	PointLight& p2 = pointLights[1];
	p2.Color = Vector3f(1,1,1);
	p2.DiffuseIntensity = 0.5;
	p2.Position = Vector3f(32,70,32);
	p2.Attenuation.Linear = 0.1f;

	if (!m_lightingShader.Init())
		std::cout << "Error initializing lighting shader" << std::endl;

	m_lightingShader.Use();
	m_lightingShader.SetDirectionalLight(dirLight);
	m_lightingShader.SetMatSpecualarIntensity(0.5);
	m_lightingShader.SetMatSpecularPower(16);
	m_lightingShader.SetColorTextureUnit(0);
	m_lightingShader.SetNormalTextureUnit(2);
	m_lightingShader.SetPointLights(2, pointLights);

	Shader::Disable();

	CHECK_GL_ERROR();

#pragma endregion

}

void Engine::LoadMenuResource()
{

#pragma region Controles du menu

	m_menuUI.btn_debugStart->OnClick.Attach(this, &Engine::OnClick);
	m_menuUI.btn_normStart->OnClick.Attach(this, &Engine::OnClick);
	m_menuUI.btn_close->OnClick.Attach(this, &Engine::OnClick);


#pragma endregion

}

void Engine::LoadGameResource()
{
	m_gameUI.txb_console->GainedFocus.Attach(this, &Engine::GainedFocus);
	m_gameUI.txb_console->LostFocus.Attach(this, &Engine::LostFocus);
}

void Engine::UnloadResource()
{

}

#pragma endregion

#pragma region Update

void Engine::UpdateGame(float elapsedTime)
{
	m_mutex.lock();

#pragma region GameTime

	static float gameTime = elapsedTime;
	gameTime += elapsedTime;
	m_character->ReduceGlobalCooldown(elapsedTime);
	if (m_clickTimerOn)
		m_clickTimer += elapsedTime;

#pragma endregion

	m_valuesGameInterface.Update(MousePosition(), Width(), Height(), m_currentBlockType, m_fps);
	m_gameUI.Update(m_valuesGameInterface);
	//m_skybox->Update(m_player->Position());
	m_chunkLoader.CheckPlayerPosition(m_player);

#pragma region Calcul la position du joueur et de la camera

	m_player->Move(m_ghostMode, m_character, elapsedTime);
	m_camera->SetPosition(m_player->Position());

	//Vérification de la mort du personnage
	if (m_character->Health() <= 0.999f)
	{
		m_player->ResetPosition();
		m_character->SetHealth(m_character->HealthMax());
		Info::Get().Sound().PlaySnd(Son::SON_DEATH, Son::CHANNEL_INTERFACE, true);
		CW("Vous etes mort!");
		m_character->SetExp(-100000);
	}

	m_mutex.unlock();

#pragma endregion

#pragma region Calcul de la position des monstres

	for (uint8 i = 0; i < MONSTER_MAX_NUMBER; i++)
	{
		if (m_monsters[i]->Initialized())
			m_monsters[i]->Update(elapsedTime);
	}

#pragma endregion

	// Update tous les spells
	for (SpellList::iterator it = m_spells.begin(); it != m_spells.end(); ++it)
		it->Update(elapsedTime);

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

void Engine::UpdateMenu(float elapsedTime)
{
	static float gameTime = elapsedTime;
	gameTime += elapsedTime;

#pragma region Premier tour

	static bool first = true;
	if (first)
	{
		m_timeranimationplus->Start();
		first = false;
	}

	if (!IsFirstRun())
	{
		if (!m_menuUI.btn_debugStart->IsMsg(STRING_BUTTON_NORM_CONT))
			m_menuUI.btn_debugStart->SetMsg(STRING_BUTTON_NORM_CONT);
		if (!m_menuUI.btn_normStart->IsMsg(STRING_BUTTON_DEBUG_ON) && !Info::Get().Options().GetOptDebug())
			m_menuUI.btn_normStart->SetMsg(STRING_BUTTON_DEBUG_ON);
		else if (!m_menuUI.btn_normStart->IsMsg(STRING_BUTTON_DEBUG_OFF) && Info::Get().Options().GetOptDebug())
			m_menuUI.btn_normStart->SetMsg(STRING_BUTTON_DEBUG_OFF);
	}

#pragma endregion

	// Met les controles à jour
	m_valuesMenuInterface.Update(MousePosition(), Width(), Height());
	m_timertest->Update(elapsedTime);
	m_timeranimationmoins->Update(elapsedTime);
	m_timeranimationplus->Update(elapsedTime);
	m_debugUI.m_timertesttime->SetVariableMsg(m_timertest->GetIntervalTime());
}

#pragma endregion

#pragma region Render

void Engine::RenderMenu()
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

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_menuUI.Render();
	m_debugUI.Render();
	m_pb_cursor->Render();

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma region OpenGl

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

#pragma endregion
}

void Engine::RenderGame()
{

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
			m_pb_cursor->Show();
		else
			m_pb_cursor->Hide();

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

	m_mxProjection = projection;
	m_mxWVP = modelView * projection;
	m_mxWorld = Matrix4f::IDENTITY;

#pragma endregion

#pragma region Player

	////////////////////////////////////////////////////////////////////////////////
	// BE ASHAMED OF THIS CODE
	//////////////////////////////////////////////////////////////////////////////// 


	Matrix4f test = m_player->GetWorldMatrix();
	//std::cout << test.ToString() << std::endl;

	// render le modele du player
	m_noNormalMap.Bind(GL_TEXTURE2);
	m_lightingShader.Use();
	m_lightingShader.SetTextureUnitType(0);
	m_lightingShader.SetEyeWorldPos(m_camera->GetRealPosition());
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(m_player->Position().x, m_player->Position().y - 1.7f, m_player->Position().z);
	glRotatef(-m_player->Rotation().y + 180, 0,1,0);
	glRotatef(-90, 1,0,0);
	glScalef(0.05, 0.05, 0.05);

	GLfloat mv2[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv2);
	Matrix4f modelView2(mv2[0], mv2[1], mv2[2], mv2[3],
		mv2[4], mv2[5], mv2[6], mv2[7],
		mv2[8], mv2[9], mv2[10], mv2[11],
		mv2[12], mv2[13], mv2[14], mv2[15]);

	m_player->Update();
	m_lightingShader.SetWVP(modelView2 * modelView * m_mxProjection);
	m_lightingShader.SetWorld(modelView2);
	m_player->Render();
	glPopMatrix();

	CHECK_GL_ERROR();

	Shader::Disable();

#pragma endregion

#pragma region Render les cubes

	Shader::Disable();
	m_textureArray->Use(GL_TEXTURE1);
	m_lightingShader.Use();
	m_lightingShader.SetTextureUnitType(1);
	m_lightingShader.SetWVP(m_mxWVP);
	m_lightingShader.SetWorld(m_mxWorld);
	m_mutex.lock();
	int updated = 0;

	for (int i = 0; i < VIEW_DISTANCE / CHUNK_SIZE_X * 2; i++)
	{
		for (int j = 0; j < VIEW_DISTANCE / CHUNK_SIZE_Z * 2; ++j)
		{
			Chunk* c = m_chunks->Get(i,j);
			if (c->IsReady()) {
				if (c->IsDirty() && updated < 2) {
					c->Update();
					updated++;
				}
				c->Render();
			}
		}
	}
	m_mutex.unlock();

	CHECK_GL_ERROR();

	Shader::Disable();

#pragma endregion

#pragma region Render Skybox

	//m_shaderCube.Use();
	//m_skybox->Render();
	//Shader::Disable();

#pragma endregion

#pragma region Render models

	m_shaderModel.Use();

	m_mutex.lock();
	for (uint8 i = 0; i < MONSTER_MAX_NUMBER; i++)
	{
		if (m_monsters[i]->Initialized())
			m_monsters[i]->Render();
	}
	Shader::Disable();
	m_mutex.unlock();

#pragma endregion

#pragma region Render spells

	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Render tous les spells
	for (SpellList::iterator it = m_spells.begin(); it != m_spells.end(); ++it) {
		it->SetDestination(m_monsters[0]->Position());
		it->Render(m_mxWVP);
		if (it->HasHit())
		{
			//m_spells.erase(it);
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

#pragma region OpenGL
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

	m_gameUI.Render();
	m_pb_cursor->Render();

#pragma region OpenGL
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
#pragma endregion

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma endregion

#pragma region Fin du premier Render

	static bool ttt = true;
	if (ttt)
	{
		m_player->ResetPosition();
		Info::Get().Sound().PlayMusic(Son::MUSIC_PLAY1);
		m_menuUI.pnl_loading->Hide();
		CW("Premier Render de l'engine termine avec succes.");
		ttt = false;
	}

#pragma endregion

}

#pragma endregion

#pragma region Evenements SFML

void Engine::TextenteredEvent(unsigned int val)
{
	if (!IsMenuOpen())
	{
		if(m_gameUI.TextenteredEvent(val))
			return;
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

		if (m_gameUI.KeyPressEvent(key))
			return;

		c.Set(key, true);
		//Sorts qui subissent le global cooldown
		if(m_character->GlobalCooldown() == 0)
		{
			if (c.n1())
			{
				Spell* newSpell = new Spell;
				newSpell->SetPosition(m_player->Position());
				newSpell->Init(4.f, m_player->RotationQ());
				newSpell->Shoot();
				m_spells.push_back(*newSpell);
				sound.PlaySnd(Son::SON_BOLT, Son::CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Test  de particule!";
			}

			if (c.n2())
			{
				for (unsigned short i = 0; i < MONSTER_MAX_NUMBER; i++)
					m_monsters[i]->SetPosition(Vector3f(m_monsters[i]->Position().x, 10, m_monsters[i]->Position().z));
				sound.PlaySnd(Son::SON_FIRE, Son::CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: teleportation de NPC!";
			}
			if (c.n3())
			{
				sound.PlaySnd(Son::SON_FREEZE, Son::CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Glace";
			}
			if (c.n4())
			{
				sound.PlaySnd(Son::SON_SHOCK, Son::CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Shock";
			}
			if (c.n5())
			{
				sound.PlaySnd(Son::SON_POISON, Son::CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Poison";
			}
			if (c.n7())
			{
				if (m_character->Mana() - 15 >= 0)
				{
					sound.PlaySnd(Son::SON_HEAL1, Son::CHANNEL_SPELL);
					m_character->ResetGlobalCooldown();
					m_character->SetHealth(15);
					m_character->SetMana(-15);
					ss << "Lancement de sort: Soin";
				}
			}
			if (c.n8())
			{
				if (m_character->Mana() - 10 >= 0)
				{
					sound.PlaySnd(Son::SON_HEAL2, Son::CHANNEL_SPELL);
					m_character->ResetGlobalCooldown();
					m_character->SetEnergy(10);
					m_character->SetMana(-10);
					ss << "Lancement de sort: Rafraichissement";
				}
			}
			if (c.n9())
			{
				sound.PlaySnd(Son::SON_DEFEND, Son::CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Defense";
			}
			if (c.n0())
			{
				sound.PlaySnd(Son::SON_SHIELD, Son::CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Bouclier magique";
			}
		}
		//Sorts hors global cooldown
		if (c.n6())
		{
			if (m_character->Mana() - 5 >= 0)
			{
				sound.PlaySnd(Son::SON_STORM, Son::CHANNEL_SPELL);
				m_character->SetMana(-5);
				m_player->Teleport();
				ss << "Lancement de sort: Teleportation";
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
	std::ostringstream ss;
	Controls& c = Info::Get().Ctrls();

	if (!IsMenuOpen())
	{
#pragma region Touches dans le jeu

		if (m_gameUI.KeyReleaseEvent(key))
			return;

		if(c.E())
		{
			m_currentBlockType++;
			if (m_currentBlockType % BTYPE_COUNT == BTYPE_AIR)
				m_currentBlockType++;
			m_currentBlockType = m_currentBlockType % BTYPE_COUNT;
		}
		if(c.Q())
		{
			m_currentBlockType--;
			if (m_currentBlockType == BTYPE_AIR)
				m_currentBlockType--;
			m_currentBlockType = (m_currentBlockType + BTYPE_COUNT) % BTYPE_COUNT;
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
				m_pb_cursor->Show();
				ss << "Affichage de la camera a la troisieme personne";
			}
			else 
			{
				m_pb_cursor->Hide();
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
			m_character->SetExp(75);
			ss << "Ajout de 75 points d'exp";
		}
		if (ss.str() != "")
		{
			CW(ss.str());
			ss.str("");
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
		if (c.Space())
		{
			if (m_timertest->IsEnabled())
				m_timertest->Stop();
			else m_timertest->Start();
		}
		if (c.n1())
			m_timertest->AddIntervalTime(-0.05f);
		if (c.n2())
			m_timertest->AddIntervalTime(0.05f);
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

	m_pb_cursor->SetPosition(Point(MousePosition().x, MousePosition().y - m_pb_cursor->GetSize().h));
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
	if (!IsMenuOpen())
	{
		if (m_gameUI.MousePressEvent(button, x, y))
			return;

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
			if (m_camera->GetMode() == Camera::CAM_THIRD_PERSON)
			{
				// Zoom in camera
				if (m_camRadius > 0)
				{
					m_camRadius--;
				}
				m_camera->SetCamRadius(m_camRadius);
				break;
		case MOUSE_BUTTON_WHEEL_DOWN:
			if (m_camera->GetMode() == Camera::CAM_THIRD_PERSON)
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
			m_menuUI.btn_close->MousePressEvents(x, m_menuUI.pnl_screen->GetSize().h - y);
			m_menuUI.btn_normStart->MousePressEvents(x, m_menuUI.pnl_screen->GetSize().h - y);
			m_menuUI.btn_debugStart->MousePressEvents(x, m_menuUI.pnl_screen->GetSize().h - y);
			break;
		}
	}
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
	if (!IsMenuOpen())
	{
		m_gameUI.MouseRleaseEvent(button, x, y);

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
			m_menuUI.btn_close->MouseReleaseEvents(x,y);
			m_menuUI.btn_normStart->MouseReleaseEvents(x,y);
			m_menuUI.btn_debugStart->MouseReleaseEvents(x,y);
			break;
		}
	}
}

#pragma endregion

#pragma region Evenements Controles

void Engine::OnClick(Control* sender)
{
	string n = sender->GetName();
	Info::Get().Sound().PlaySnd(Son::SON_CLICK, Son::CHANNEL_INTERFACE, true);

	if (n == MENU_BUTTON_START_FULL_NAME)
	{
		if (!IsFirstRun())
			SetMenuStatus(false);
		else
		{
			Info::Get().Options().SetOptDebug(false);
			ActivateFirstRun();
			m_menuUI.pnl_loading->Show();
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
			m_menuUI.pnl_loading->Show();
			SetMenuStatus(false);
		}
	}
	if (n == MENU_BUTTON_CLOSE)
		Stop();
}

void Engine::GainedFocus(Textbox* sender)
{

}

void Engine::LostFocus(Textbox* sender)
{
	if (!sender->IsMsg(""))
		CW(sender->GetMsg());
}

void Engine::Timertest_OnTick(Timer* sender)
{
	if (sender->GetLaps() <= 1)
		m_debugUI.m_timertesttext->SetMsg("1 - plus vite. 2 - moins vite");
	m_debugUI.m_timertesttext->UseNextDocking();
}

void Engine::TimerAnimation_OnTick(Timer* sender)
{
	if (sender->GetName() == "plus")
	{
		if (m_menuUI.pb_logo->GetSize().h <= MENU_LOGO_SIZE_Y + 30)
			m_menuUI.pb_logo->AddSize(Size(0, 3));
		else
		{
			m_timeranimationmoins->Start();
			sender->Stop();
		}
	}
	if (sender->GetName() == "moins")
	{
		if (m_menuUI.pb_logo->GetSize().h > MENU_LOGO_SIZE_Y)
			m_menuUI.pb_logo->AddSize(Size(0, -2));
		else
		{
			m_timeranimationplus->Start();
			sender->Stop();
		}
	}
}

#pragma endregion

#pragma region Fonctions de bloc

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

	posX += 0.5f;
	posY += 2.5f;
	posZ += 0.5f;

	// Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
	// de valeurs negatives
	int px = (int)(posX);
	int py = (int)(posY);
	int pz = (int)(posZ);

	bool found = false;

	if((m_player->Position() - Vector3f(posX, posY, posZ)).Lenght() < MAX_SELECTION_DISTANCE)
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
						BlockType bt = Info::Get().GetBlocFromWorld(Vector3f(x - 0.5f, y, z - 0.5f));
						if(bt == BTYPE_AIR)
							continue;

						// Skip water blocs
						//if(bloc->Type == BT_WATER)
						//    continue;

						m_currentBlock.x = x - Info::Get().GetOffsetMap().x * CHUNK_SIZE_X;
						m_currentBlock.y = y;
						m_currentBlock.z = z - Info::Get().GetOffsetMap().y * CHUNK_SIZE_Z;

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

#pragma endregion

#pragma region Fonctions privees

void Engine::LoadBlocTexture(BLOCK_TYPE type, BLOCK_FACE faces, string colorMapPath, string normalMapPath)
{
	int colorIndex;
	int normalIndex;

	BlockInfo* blocInfo = Info::Get().GetBlocInfo(type);

	colorIndex = m_textureArray->AddTexture(colorMapPath);
	if (normalMapPath == "")
		normalIndex = m_textureArray->AddTexture(TEXTURE_PATH "normal_up.jpg");
	else
		normalIndex = m_textureArray->AddTexture(normalMapPath);

	switch (faces)
	{
	case BFACE_ALL:
		blocInfo->SetColorTextureIndex(0, colorIndex);
		blocInfo->SetColorTextureIndex(1, colorIndex);
		blocInfo->SetColorTextureIndex(2, colorIndex);
		blocInfo->SetNormalTextureIndex(0, normalIndex);
		blocInfo->SetNormalTextureIndex(1, normalIndex);
		blocInfo->SetNormalTextureIndex(2, normalIndex);
		break;
	case BFACE_TOP_AND_BOT:
		blocInfo->SetColorTextureIndex(0, colorIndex);
		blocInfo->SetColorTextureIndex(2, colorIndex);
		blocInfo->SetNormalTextureIndex(0, normalIndex);
		blocInfo->SetNormalTextureIndex(2, normalIndex);
		break;
	case BFACE_TOP:
		blocInfo->SetColorTextureIndex(0, colorIndex);
		blocInfo->SetNormalTextureIndex(0, normalIndex);
		break;
	case BFACE_SIDES:
		blocInfo->SetColorTextureIndex(1, colorIndex);
		blocInfo->SetNormalTextureIndex(1, normalIndex);
		break;
	case BFACE_BOTTOM:
		blocInfo->SetColorTextureIndex(2, colorIndex);
		blocInfo->SetNormalTextureIndex(2, normalIndex);
		break;
	}
}

bool Engine::LoadTexture(Texture& texture, const string& filename, bool stopOnError)
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

void Engine::CW(const std::string& line)
{
	m_gameUI.ConsoleWriteLine(line);
}

void Engine::CWL(const std::string& line)
{
	//lbx_console->SetLine(0, lbx_console->GetLine(0) + line);
}

#pragma endregion
