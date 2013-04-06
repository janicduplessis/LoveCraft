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
#include "util/vector3.h"

#pragma region Constructeur et reference

Engine::Engine() : m_wireframe(false), m_angle(0), m_ghostMode(false),
	m_rightClick(false), m_leftClick(false), m_camRadius(10), m_fpstmr(0),
	m_clickTimer(0), m_currentBlockType(0), m_chunkLoader(&m_mutex), m_player(0), m_lanternFire(100)
{
	m_particleSpell = new Texture;
	m_textureSpell = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureSpellX = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureInterface = new Texture*[IMAGE::CUSTIMAGE_LAST];
	for (unsigned short i = 0; i < IMAGE::CUSTIMAGE_LAST; i++)
		m_textureInterface[i] = new Texture();
	m_texturefontColor = new Texture*[COLOR::TEXTCOLOR_LAST];
	for (unsigned short i = 0; i < COLOR::TEXTCOLOR_LAST; i++)
		m_texturefontColor[i] = new Texture();
	m_monsters = new Animal*[MONSTER_MAX_NUMBER];

	m_chunks = new Chunks(VIEW_DISTANCE / CHUNK_SIZE_X * 2, VIEW_DISTANCE / CHUNK_SIZE_Z * 2);

	m_skybox = new Skybox();

	m_player = new Player(Vector3f(VIEW_DISTANCE, 0, VIEW_DISTANCE));
	Info::Get().StatusOn(Info::LSTATUS_PLAYER);
	m_camera = new ThirdPersonCamera(m_player, 10);
	Info::Get().StatusOn(Info::LSTATUS_CAMERA);
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
		delete m_chunks;
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

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective( 45.0f, (float)Width() / (float)Height(), 0.1f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	//glEnable (GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_CLAMP);
	//glCullFace(GL_FRONT);
	//glEnable(GL_FOG);

	m_persProjInfo.FOV = 60.f;
	m_persProjInfo.Height = Height();
	m_persProjInfo.Width = Width();
	m_persProjInfo.zNear = 1.f;
	m_persProjInfo.zFar = 100.f;

	/*// Light
	GLfloat light0Pos[4]  = {0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f};
	GLfloat light0Amb[4]  = {0.9f, 0.9f, 0.9f, 1.0f};
	GLfloat light0Diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0Spec[4] = {0.2f, 0.2f, 0.2f, 1.0f};

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

	glFogf(GL_FOG_DENSITY, 1.f / VIEW_DISTANCE);*/
	//glFogf(GL_FOG_DENSITY, 1.f / 3);
	//float fogCol[3] = {0.8f, 0.8f, 0.8f};
	//glFogfv(GL_FOG_COLOR, fogCol);

#pragma endregion

#pragma region Initialisation des timers

	m_timertest = new Timer();
	m_timertest->Init(1000);
	m_timeranimationplus = new Timer();
	m_timeranimationplus->Init(20);
	m_timeranimationplus->InitControl("plus");
	m_timeranimationmoins = new Timer();
	m_timeranimationmoins->Init(20);
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
	Info::Get().SetCubeShader(&m_shaderCube);
	Info::Get().SetChunkArray(m_chunks);

	m_currentBlockType = BTYPE_DIRT;

	m_chunks->Init(&m_lightingShader);
	Info::Get().StatusOn(Info::LSTATUS_CHUNK);

	m_player->Init(&m_modelShader);
	m_skybox->Init(SKYBOX_PATH, "sp3left.jpg", "sp3right.jpg", "sp3top.jpg", "sp3bot.jpg", "sp3back.jpg", "sp3front.jpg");
	m_lanternFire.SetTexture(m_particleSpell);
	m_lanternFire.SetParticlesSize(0.3);
	m_lanternFire.SetColor(Vector3f(1, 153/255.f, 0));
	m_lanternFire.SetAverageVelocity(Vector3f(0,3,0));
	m_lanternFire.SetRange(2);
	m_lanternFire.SetAverageLifespan(150);
	m_lanternFire.Init();

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

#pragma endregion

	m_valuesGameInterface.Init(m_textureInterface, m_texturefontColor, m_textureArray, m_player, m_character);
	m_valuesGameInterface.Update(MousePosition(), Width(), Height(), m_currentBlockType, m_fps);
	m_gameUI.Init(m_valuesGameInterface);

	// Init deferred shading
	InitLights();

	if(!m_gBuffer.Init(Width(), Height())) {
		std::cout << "Error loading gbuffer" << std::endl;
	}
	if(!m_DSDirLightingPassShader.Init()) {
		std::cout << "Error loading deffered directional lighting shader" << std::endl;
	}
	if(!m_nullShader.Init()) {
		std::cout << "Error loading deffered null shader, what a baddie =/" << std::endl;
	}
	if(!m_shadowVolShader.Init()) {
		std::cout << "Error loading shadow volume shader" << std::endl;
	}

	m_DSDirLightingPassShader.Enable();

	m_DSDirLightingPassShader.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_DSDirLightingPassShader.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_DSDirLightingPassShader.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_DSDirLightingPassShader.SetScreenSize(Width(), Height());
	m_DSDirLightingPassShader.SetDirectionalLight(m_dirLight);
	m_DSDirLightingPassShader.SetWVP(Matrix4f::IDENTITY);

	if(!m_DSPointLightingPassShader.Init()) {
		std::cout << "Error loading deffered point lighting shader" << std::endl;
	}

	m_DSPointLightingPassShader.Enable();

	m_DSPointLightingPassShader.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_DSPointLightingPassShader.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_DSPointLightingPassShader.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_DSPointLightingPassShader.SetScreenSize(Width(), Height());
	m_bsphere.LoadDefaultMaterials(false);
	m_quad.LoadDefaultMaterials(false);
	if(!m_bsphere.Init(MODEL_PATH "sphere.obj", false, 0)) {
		std::cout << "Error loading " MODEL_PATH "sphere.obj" << std::endl;
	}
	if(!m_quad.Init(MODEL_PATH "quad.obj", false, 0)) {
		std::cout << "Error loading " MODEL_PATH "quad.obj" << std::endl;
	}

	//Set cube lighting
	m_lightingShader.Enable();
	m_lightingShader.SetColorTextureUnit(0);
	m_lightingShader.SetTextureUnitType(1);
}

void Engine::InitLights()
{
	m_dirLight.AmbientIntensity = 0.1;
	m_dirLight.Color = Vector3f(1,1,1);
	m_dirLight.DiffuseIntensity = 0.1;
	m_dirLight.Direction = Vector3f(0.3, -1, 0.5).Normalize();

	m_pointLights[0].Color = Vector3f(1,1,1);
	m_pointLights[0].DiffuseIntensity = 20;
	m_pointLights[0].Position = Vector3f(64,65,64);
	m_pointLights[0].Attenuation.Exp = 1.f;
	m_pointLights[0].Attenuation.Linear = 0.0f;

	m_pointLights[1].Color = Vector3f(1,1,1);
	m_pointLights[1].DiffuseIntensity = 10;
	m_pointLights[1].Position = Vector3f(32,70,32);
	m_pointLights[1].Attenuation.Exp = 1.f;
	m_pointLights[1].Attenuation.Linear = 0.0f;

	// Lantern light
	m_pointLights[2].Color = Vector3f(1,140/255.f,0);
	m_pointLights[2].AmbientIntensity = 0.2;
	m_pointLights[2].DiffuseIntensity = 2;
	m_pointLights[2].Attenuation.Exp = 1.f;
	m_pointLights[2].Attenuation.Linear = 0.0f;
	m_pointLights[2].Attenuation.Constant = 0;
	m_pointLights[2].Position = Vector3f(50,70,40);
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

	m_normalMap.Load(TEXTURE_PATH "guard1_body_normal.tga", true);
	m_noNormalMap.Load(TEXTURE_PATH "normal_up.jpg", true);
	m_particleSpell->Load(TEXTURE_PATH "particle1.png");

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
	m_textureInterface[CUSTIMAGE_TOOLTIP_BACK]->Load(TEXTURE_PATH "i_tooltip_back.png");
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

	if (!m_lightingShader.Init())
		std::cout << "Error initializing lighting shader" << std::endl;

	if (!m_modelShader.Init())
		std::cout << "Error initializing model shader" << std::endl;

#pragma endregion

}

void Engine::LoadMenuResource()
{

#pragma region Evenements du menu

	m_menuUI.btn_debugStart->OnClick.Attach(this, &Engine::OnClick);
	m_menuUI.btn_normStart->OnClick.Attach(this, &Engine::OnClick);
	m_menuUI.btn_close->OnClick.Attach(this, &Engine::OnClick);


#pragma endregion

}

void Engine::LoadGameResource()
{

#pragma region Evenements du jeu

	m_gameUI.txb_console->GainedFocus.Attach(this, &Engine::GainedFocus);
	m_gameUI.txb_console->LostFocus.Attach(this, &Engine::LostFocus);

#pragma endregion

}

void Engine::UnloadResource()
{

}

#pragma endregion

#pragma region Update

void Engine::UpdateGame(float elapsedTime)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Game Time
	static float gameTime = elapsedTime;
	gameTime += elapsedTime;
	if (m_clickTimerOn)
		m_clickTimer += elapsedTime;

	// Mouse Cursor
	if (!m_rightClick && !m_leftClick)
		m_pb_cursor->Show();
	else
		m_pb_cursor->Hide();

	// Update
	m_character->ReduceGlobalCooldown(elapsedTime);
	m_valuesGameInterface.Update(MousePosition(), Width(), Height(), m_currentBlockType, m_fps);
	m_gameUI.Update(m_valuesGameInterface);
	//m_chunkLoader.CheckPlayerPosition(m_player);
	m_player->Move(m_ghostMode, m_character, elapsedTime);
	m_player->Update(gameTime);
	m_camera->Update(elapsedTime);
	m_chunks->Update();
	//m_lanternFire.Update(elapsedTime);

	//UpdateLights();

	//Vérification de la mort du personnage
	if (m_character->Health() <= 0.999f)
	{
		m_player->ResetPosition();
		m_character->SetHealth(m_character->HealthMax());
		Son::PlaySnd(SON_DEATH, CHANNEL_INTERFACE, true);
		CW("Vous etes mort!");
		m_character->SetExp(-100000);
	}

	// Position des monstres
	//for (uint8 i = 0; i < MONSTER_MAX_NUMBER; i++)
	//{
	//	if (m_monsters[i]->Initialized())
	//		m_monsters[i]->Update(elapsedTime);
	//}

	// Update tous les spells
	//for (SpellList::iterator it = m_spells.begin(); it != m_spells.end(); ++it)
	//	it->Update(elapsedTime);

	// Fps
	m_fpstmr += elapsedTime;
	if (m_fpstmr > 1.5f)
	{
		float fps = 1 / elapsedTime;
		m_fps = fps >= 60 ? 60 : fps;
		m_fpstmr = 0;
	}

	GetBlocAtCursor();
}

void Engine::UpdateLights()
{
	Vector3f toLantern = m_player->World() * (m_player->LanternBoneTrans() * Vector3f(-39.749374f, -6.182379f, 35.334176f));
	m_pointLights[2].Position = toLantern;
	m_lanternFire.SetPosition(toLantern);
}

void Engine::UpdateMenu(float elapsedTime)
{
	static float gameTime = elapsedTime;
	gameTime += elapsedTime;
	m_gameTime = elapsedTime;

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

	glDepthMask(GL_FALSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_BLEND);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glCullFace(GL_FRONT);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);

#pragma endregion

	m_menuUI.Render();
	m_debugUI.Render();
	m_pb_cursor->Render();

#pragma region OpenGl

	glEnable(GL_DEPTH_TEST);

#pragma endregion
}

void Engine::RenderGame()
{
	m_gBuffer.StartFrame();

	DSGeometryPass();

	glEnable(GL_STENCIL_TEST);

	for (uint32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLights); ++i) {
		RenderShadowVolIntoStencil(i);
		//DSStencilPass(i);
		DSPointLightPass(i);
	}

	glDisable(GL_STENCIL_TEST);

	DSDirectionalLightPass();

	DSFinalPass();

	CHECK_GL_ERROR();

#pragma region Render l interface

	// TODO : Clean this shit too
	/*glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glCullFace(GL_FRONT);

	m_gameUI.Render();
	m_pb_cursor->Render();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	static bool firstRender = true;
	if (firstRender)
	{
		m_player->ResetPosition();
		Son::PlayMusic(MUSIC_PLAY1);
		m_menuUI.pnl_loading->Hide();
		CW("Premier Render de l'engine termine avec succes.");
		firstRender = false;
	}*/
#pragma endregion
}

void Engine::DSGeometryPass() 
{
	m_gBuffer.BindForGeometryPass();

	glCullFace(GL_BACK);

	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	RenderSceneIntoDepth();

	RenderShadowedScene();

	glDepthMask(GL_FALSE);
}

void Engine::RenderSceneIntoDepth()
{
	glDrawBuffer(GL_NONE);
	glDepthMask(GL_TRUE);

	m_nullShader.Enable();

	Pipeline p;
	p.SetCamera(m_camera->GetPosition(), m_camera->GetTarget(), m_camera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_player->RenderDepth(p, &m_nullShader);
	m_chunks->Render(p, &m_nullShader);
}

void Engine::RenderShadowVolIntoStencil(uint32 index)
{
	
	glDrawBuffer(GL_NONE);
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glStencilFunc(GL_ALWAYS, 0, 0xff);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	m_shadowVolShader.Enable();
	m_shadowVolShader.SetLightPos(m_pointLights[index].Position);

	Pipeline p;
	p.SetCamera(m_camera->GetPosition(), m_camera->GetTarget(), m_camera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	
	m_player->RenderShadowVolume(p, &m_shadowVolShader);

	glEnable(GL_CULL_FACE);
}

void Engine::RenderShadowedScene()
{
	m_gBuffer.BindForGeometryPass();

	glDepthMask(GL_FALSE);

	// prevent update to the stencil buffer
	//glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
	//glStencilFunc(GL_EQUAL, 0x0, 0xff);
	Pipeline p;
	p.SetCamera(m_camera->GetPosition(), m_camera->GetTarget(), m_camera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	m_player->Render(p);

	//Set cube lighting
	m_lightingShader.Enable();
	m_lightingShader.SetColorTextureUnit(0);
	m_lightingShader.SetTextureUnitType(1);
	m_textureArray->Use(GL_TEXTURE1);
	m_noNormalMap.Bind(GL_TEXTURE2);
	m_chunks->Render(p);

	//m_skybox->Render(p);

	//m_shaderModel.Use();

	/*for (uint8 i = 0; i < MONSTER_MAX_NUMBER; i++)
	{
	if (m_monsters[i]->Initialized())
	m_monsters[i]->Render();
	}
	Shader::Disable();*/

	/*glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);

	//m_lanternFire.Render(p.GetWVPTrans());

	// Render tous les spells
	for (SpellList::iterator it = m_spells.begin(); it != m_spells.end(); ++it) {
	it->SetDestination(m_monsters[0]->Position());
	it->Render(p.GetWVPTrans());
	if (it->HasHit())
	{
	//m_spells.erase(it);
	break;
	}
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);*/
}

void Engine::DSStencilPass(uint32 index)
{
	m_nullShader.Enable();

	m_gBuffer.BindForStencilPass();

	//glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	//glStencilFunc(GL_ALWAYS, 0x0, 0xff);
	glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);

	glStencilOpSeparate(GL_BACK, GL_ZERO, GL_INCR, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_ZERO, GL_DECR, GL_KEEP);

	Pipeline p;
	p.SetCamera(m_camera->GetPosition(), m_camera->GetTarget(), m_camera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.WorldPos(m_pointLights[index].Position);
	float BSphereScale = CalcPointLightBSphere(m_pointLights[index].Color, 
		m_pointLights[index].TotalIntensity());
	p.Scale(BSphereScale, BSphereScale, BSphereScale);

	m_nullShader.SetWVP(p.GetWVPTrans());
	m_bsphere.Render();
}

void Engine::DSPointLightPass(uint32 index)
{
	m_gBuffer.BindForLightPass();

	m_DSPointLightingPassShader.Enable();
	m_DSPointLightingPassShader.SetEyeWorldPos(m_camera->GetPosition());

	glStencilFunc(GL_EQUAL, 0x0, 0xff);
	//glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	Pipeline p;
	p.SetCamera(m_camera->GetPosition(), m_camera->GetTarget(), m_camera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.WorldPos(m_pointLights[index].Position);
	float BSphereScale = CalcPointLightBSphere(m_pointLights[index].Color, 
		m_pointLights[index].TotalIntensity());
	p.Scale(BSphereScale, BSphereScale, BSphereScale);	
	m_DSPointLightingPassShader.SetWVP(p.GetWVPTrans());
	m_DSPointLightingPassShader.SetPointLight(m_pointLights[index]);
	m_bsphere.Render();

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void Engine::DSDirectionalLightPass()
{
	m_gBuffer.BindForLightPass();

	m_DSDirLightingPassShader.Enable();
	m_DSDirLightingPassShader.SetEyeWorldPos(m_camera->GetPosition());

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	m_quad.Render(); 

	glDisable(GL_BLEND);
}

void Engine::DSFinalPass()
{
	m_gBuffer.BindForFinalPass();
	glBlitFramebuffer(0, 0, Width(), Height(),
					  0, 0, Width(), Height(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
				Son::PlaySnd(SON_BOLT, CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Test  de particule!";
			}

			if (c.n2())
			{
				for (unsigned short i = 0; i < MONSTER_MAX_NUMBER; i++)
					m_monsters[i]->SetPosition(Vector3f(m_monsters[i]->Position().x, 10, m_monsters[i]->Position().z));
				Son::PlaySnd(SON_FIRE, CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: teleportation de NPC!";
			}
			if (c.n3())
			{
				Son::PlaySnd(SON_FREEZE, CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Glace";
			}
			if (c.n4())
			{
				Son::PlaySnd(SON_SHOCK, CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Shock";
			}
			if (c.n5())
			{
				Son::PlaySnd(SON_POISON, CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Poison";
			}
			if (c.n7())
			{
				if (m_character->Mana() - 15 >= 0)
				{
					Son::PlaySnd(SON_HEAL1, CHANNEL_SPELL);
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
					Son::PlaySnd(SON_HEAL2, CHANNEL_SPELL);
					m_character->ResetGlobalCooldown();
					m_character->SetEnergy(10);
					m_character->SetMana(-10);
					ss << "Lancement de sort: Rafraichissement";
				}
			}
			if (c.n9())
			{
				Son::PlaySnd(SON_DEFEND, CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Defense";
			}
			if (c.n0())
			{
				Son::PlaySnd(SON_SHIELD, CHANNEL_SPELL);
				m_character->ResetGlobalCooldown();
				ss << "Lancement de sort: Bouclier magique";
			}
		}
		//Sorts hors global cooldown
		if (c.n6())
		{
			if (m_character->Mana() - 5 >= 0)
			{
				Son::PlaySnd(SON_STORM, CHANNEL_SPELL);
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

		}
		if (c.M())
			Son::PlayNextTrack();
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
			m_timertest->AddIntervalTime(-50);
		if (c.n2())
			m_timertest->AddIntervalTime(50);
		c.Set(key, false);
#pragma endregion
	}
}

void Engine::MouseMoveEvent(const MouseEventArgs& e)
{
	if (!IsMenuOpen())
	{
		m_gameUI.MouseMoveEvents(e);

		if (m_leftClick)
		{
			m_camera->MouseMoveEvent(e);

		}

		if (m_rightClick) 
		{
			m_player->MouseMoveEvent(e);
			m_camera->MouseMoveEvent(e);
		}
	}
	else
	{
		m_menuUI.MouseMoveEvents(e);
	}

	m_pb_cursor->SetPosition(Point(e.GetPosition().x, e.GetPosition().y));
}

void Engine::MousePressEvent(const MouseEventArgs& e)
{
	if (!IsMenuOpen())
	{
#pragma region Jeu
		if (m_gameUI.MousePressEvent(e.GetMouseButtons(), e.GetPosition().x, e.GetPosition().y))
			return;

		switch (e.GetMouseButtons())
		{
		case MOUSE_BUTTON_RIGHT:
			m_clickTimerOn = true;
			m_clickTimer = 0;
			m_rightClick = true;
			m_lastTarget = m_camera->GetTarget();
			SetMousePos(e.GetLastPosition().x, e.GetLastPosition().y);
			break;
		case MOUSE_BUTTON_LEFT:
			m_clickTimerOn = true;
			m_clickTimer = 0;
			SetMousePos(e.GetLastPosition().x, e.GetLastPosition().y);
			m_leftClick = true;
			m_lastTarget = m_camera->GetTarget();
			break;
		case MOUSE_BUTTON_WHEEL_UP:
			// Zoom in camera
			if (m_camRadius > 0)
			{
				m_camRadius--;
			}
			m_camera->SetCamRadius(m_camRadius);
			break;
		case MOUSE_BUTTON_WHEEL_DOWN:
			// Zoom out camera
			if (m_camRadius < 20)
			{
				m_camRadius++;
			}
			m_camera->SetCamRadius(m_camRadius);
			break;

		}
#pragma endregion
	}
	else
	{
#pragma region Menu

		m_menuUI.MousePressEvents(e.GetMouseButtons(), e.GetPosition().x, e.GetPosition().y);

#pragma endregion
	}
}

void Engine::MouseReleaseEvent(const MouseEventArgs& e)
{
	if (!IsMenuOpen())
	{
		m_gameUI.MouseRleaseEvent(e.GetMouseButtons(), e.GetPosition().x, e.GetPosition().y);

		switch (e.GetMouseButtons())
		{
		case MOUSE_BUTTON_RIGHT:
			m_rightClick = false;
			m_clickTimerOn = false;
			if (!m_gameUI.pnl_playscreen->GetTopControl(e.GetPosition().x, e.GetPosition().y))
			{
				if (m_clickTimer < 0.5f && abs(m_lastTarget.x - m_camera->GetTarget().x) <= 1 
					&& abs(m_lastTarget.y - m_camera->GetTarget().y) <= 1 && m_currentBlock.y != 0)
					RemoveBlock();
			}
			break;
		case MOUSE_BUTTON_LEFT:
			m_leftClick = false;
			m_clickTimerOn = false;
			if (!m_gameUI.pnl_playscreen->GetTopControl(e.GetPosition().x, e.GetPosition().y))
			{
				std::cout << (float)m_currentBlockType << std::endl;
				if (m_clickTimer < 0.5f && m_lastTarget == m_camera->GetTarget())
					AddBlock(m_currentBlockType);
			}
			break;
		}
	}
	else
	{
		m_menuUI.MouseReleaseEvents(e.GetMouseButtons(), e.GetPosition().x, e.GetPosition().y);
	}
}

#pragma endregion

#pragma region Evenements Controles

void Engine::OnClick(Control* sender)
{
	string n = sender->GetName();
	Son::PlaySnd(SON_CLICK, CHANNEL_INTERFACE, true);

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

float Engine::CalcPointLightBSphere(const Vector3f& color, float intensity)
{
	float maxChannel = Tool::Max(Tool::Max(color.x, color.y), color.z);
	float c = maxChannel * intensity;
	return (8.0f * sqrtf(c) + 1.0f);
}

#pragma endregion
