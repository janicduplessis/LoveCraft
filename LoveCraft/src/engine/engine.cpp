#include "engine.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include "son.h"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Network.hpp>


Engine::Engine() : m_wireframe(false), m_angle(0), m_ghostMode(false),
	m_rightClick(false), m_leftClick(false), m_camRadius(10),
	m_playScreenBotLeft(Vector2i(INTERFACE_SIDE_LEFT_WIDTH, INTERFACE_BOTTOM_HEIGHT)),
	m_playScreenTopLeft(Vector2i(INTERFACE_SIDE_LEFT_WIDTH, Height() - INTERFACE_TOP_HEIGHT * 3)),
	m_playScreenTopRight(Vector2i(Width() - INTERFACE_SIDE_RIGHT_WIDTH, Height() - INTERFACE_TOP_HEIGHT * 3)),
	m_playScreenBotRight(Vector2i(Width() - INTERFACE_SIDE_RIGHT_WIDTH, INTERFACE_BOTTOM_HEIGHT)),
	m_playScreenSize(Vector2i(m_playScreenTopRight.x - m_playScreenTopLeft.x, m_playScreenTopLeft.y - m_playScreenBotLeft.y))
{
	m_textureSpell = new Texture[SPELL_BAR_SPELL_NUMBER];
	m_textureInterface = new Texture[IMAGE::IMAGE_LAST];
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
	m_projectile.Load();
	m_projectile.Init(4.f, Quaternion());
	m_projectile.SetCollisionRadius(Vector3f(2,2,2));
	m_projectile.SetMaxRot(0.05);
	m_projectile.SetPosition(Vector3f(10,0,0));

	m_testpig.Init(&m_player);
	m_testpig.SetPosition(Vector3f(0,0,0));
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

	chunk.SetBloc(5,3,5, BTYPE_DIRT);
	chunk.SetBloc(1,1,0, BTYPE_BRICK);
	chunk.SetBloc(1,2,0, BTYPE_DIRT);
	chunk.SetBloc(1,3,0, BTYPE_DIRT);
	chunk.SetBloc(2,1,0, BTYPE_DIRT);
	chunk.SetBloc(3,1,0, BTYPE_GRASS);
	chunk.SetBloc(5,3,6, BTYPE_DIRT);
	chunk.SetBloc(6,3,5, BTYPE_DIRT);
	chunk.SetBloc(5,4,6, BTYPE_DIRT);
	chunk.SetBloc(6,4,5, BTYPE_DIRT);
	chunk.SetBloc(6,3,6, BTYPE_DIRT);
	chunk.SetBloc(6,4,6, BTYPE_DIRT);
	chunk.SetBloc(5,4,5, BTYPE_DIRT);
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
			chunk.SetBloc(i,1,j, BTYPE_AIR);

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
			chunk.SetBloc(i,1,j, BTYPE_AIR);
		}
	}

	//Platforme de gazon
	for (int i = CHUNK_SIZE_X-1; i >= 2; i--)
	{
		for (int j = CHUNK_SIZE_Z-1; j >= 2; j--)
		{
			chunk.SetBloc(i,14,j, BTYPE_GRASS);
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

	// Texture des blocs 128x128 px
	m_textureArray = new TextureArray(128);

	LoadBlocTexture(BTYPE_BRICK, TEXTURE_PATH "b_brick_red.jpg");
	LoadBlocTexture(BTYPE_DIRT, TEXTURE_PATH "b_dirt.bmp");
	LoadBlocTexture(BTYPE_GRASS, TEXTURE_PATH "b_grass.bmp");
	LoadBlocTexture(BTYPE_ROCK, TEXTURE_PATH "b_rock.jpg");
	LoadBlocTexture(BTYPE_SAND, TEXTURE_PATH "b_sand.jpg");
	LoadBlocTexture(BTYPE_SNOW, TEXTURE_PATH "b_snow.jpg");


	m_textureArray->Generate();
	//Texture des spells
	m_textureSpell[0].Load(TEXTURE_PATH "spellbolt.gif");
	m_textureSpell[1].Load(TEXTURE_PATH "spellfire.png");
	m_textureSpell[2].Load(TEXTURE_PATH "spellfreeze.png");
	m_textureSpell[3].Load(TEXTURE_PATH "spellshock.png");
	m_textureSpell[4].Load(TEXTURE_PATH "spellpoison.gif");
	m_textureSpell[5].Load(TEXTURE_PATH "spellstorm.png");
	m_textureSpell[6].Load(TEXTURE_PATH "spellheal.gif");
	m_textureSpell[7].Load(TEXTURE_PATH "spellrain.gif");
	m_textureSpell[8].Load(TEXTURE_PATH "spelldefend.gif");
	m_textureSpell[9].Load(TEXTURE_PATH "spellshield.png");

	m_textureInterface[IMAGE_BLACK_BACK].Load(TEXTURE_PATH "noir.jpg");
	m_textureInterface[IMAGE_BOO].Load(TEXTURE_PATH "i_boo.png");
	m_textureInterface[IMAGE_RUN].Load(TEXTURE_PATH "i_bewareofcthulhu.png");
	m_textureInterface[IMAGE_CROSSHAIR].Load(TEXTURE_PATH "i_cross.bmp");
	m_textureInterface[IMAGE_FONT].Load(TEXTURE_PATH "font.bmp");
	m_textureInterface[IMAGE_INTERFACE_FRAME].Load(TEXTURE_PATH "b_rock.jpg");
	m_textureInterface[IMAGE_PORTRAIT_FRAME].Load(TEXTURE_PATH "i_portrait-frame.png");
	m_textureInterface[IMAGE_PORTRAIT_MALE].Load(TEXTURE_PATH "i_portrait-male");
	m_textureInterface[IMAGE_PGBTEXT_HEALTH].Load(TEXTURE_PATH "i_pgb_health.png");
	m_textureInterface[IMAGE_PGBTEXT_ENERGY].Load(TEXTURE_PATH "i_pgb_energy.png");
	m_textureInterface[IMAGE_PGBTEXT_MANA].Load(TEXTURE_PATH "i_pgb_mana.png");
	m_textureInterface[IMAGE_PGBTEXT_EXP].Load(TEXTURE_PATH "i_pgb_exp.png");


#pragma endregion

#pragma region Chargement des elements de l interface
	// Écran
	m_pnl_screen = Panel(Vector2i(), Vector2i(), Vector2i(Width(), Height()), 0, 1, "main");
	// Zone de jeu
	m_pnl_playscreen = Panel(m_pnl_screen.AbsolutePosition(), 
		Vector2i(INTERFACE_SIDE_RIGHT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(m_pnl_screen.Size().x - INTERFACE_SIDE_LEFT_WIDTH - INTERFACE_SIDE_RIGHT_WIDTH, 
		m_pnl_screen.Size().y - INTERFACE_TOP_HEIGHT * 3 - INTERFACE_BOTTOM_HEIGHT),
		0, PNL_PLAYSCREEN_CONTROLS_NBR, "playscreen");
	//m_pnl_screen.AddControl(m_pnl_playscreen);
	// Portrait
	m_pnl_portrait = Panel(m_pnl_playscreen.AbsolutePosition(),
		Vector2i(PNL_PORTRAIT_POSITION_X, PNL_PORTRAIT_POSITION_Y),
		Vector2i(PNL_PORTRAIT_SIZE_W, PNL_PORTRAIT_SIZE_H),
		&m_textureInterface[IMAGE_PORTRAIT_FRAME], PNL_PORTRAIT_CONTROLS_NBR, PNL_PORTRAIT_NAME);
	//m_pnl_playscreen.AddControl(m_pnl_portrait);
	// Controles du portrait
	m_pgb_health = ProgressBar(m_pnl_portrait.AbsolutePosition(),
		Vector2i(PGB_HEALTH_POSITION_X, PGB_HEALTH_POSITION_Y),
		Vector2i(PGB_HEALTH_SIZE_W, PGB_HEALTH_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_HEALTH], &m_textureInterface[IMAGE_BLACK_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_HEALTH_BACKGROUND, PGB_HEALTH_NAME);
	//m_pnl_portrait.AddControl(m_pgb_health);
	m_pgb_mana = ProgressBar(m_pnl_portrait.AbsolutePosition(),
		Vector2i(PGB_MANA_POSITION_X, PGB_MANA_POSITION_Y),
		Vector2i(PGB_MANA_SIZE_W, PGB_MANA_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_MANA], &m_textureInterface[IMAGE_BLACK_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_MANA_BACKGROUND, PGB_MANA_NAME);
	//m_pnl_portrait.AddControl(m_pgb_mana);
	m_pgb_exp = ProgressBar(m_pnl_portrait.AbsolutePosition(),
		Vector2i(PGB_EXP_POSITION_X, PGB_EXP_POSITION_Y),
		Vector2i(PGB_EXP_SIZE_W, PGB_EXP_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_EXP], &m_textureInterface[IMAGE_BLACK_BACK],
		ProgressBar::BARMODE_HORIZONTAL_LTR, PGB_EXP_BACKGROUND, PGB_EXP_NAME);
	//m_pnl_portrait.AddControl(m_pgb_exp);
	m_pgb_energy = ProgressBar(m_pnl_playscreen.AbsolutePosition(),
		Vector2i(PGB_ENERGY_POSITION_X, PGB_ENERGY_POSITION_Y),
		Vector2i(PGB_ENERGY_SIZE_W, PGB_ENERGY_SIZE_H),
		&m_textureInterface[IMAGE_PGBTEXT_ENERGY], &m_textureInterface[IMAGE_BLACK_BACK],
		ProgressBar::BARMODE_VERTICAL_DTU, PGB_ENERGY_BACKGROUND, PGB_ENERGY_NAME);
	//m_pnl_portrait.AddControl(m_pgb_energy);

	//m_healthBar = ProgressBar(Vector2i(300, 20), 
	//	Vector2i(INTERFACE_SIDE_LEFT_WIDTH + PROGRESS_BAR_OUTLINE, 35), ProgressBar::BARMODE_HORIZONTAL_LTR);
	//m_energyBar = ProgressBar(Vector2i(20, 250), 
	//	Vector2i(m_playScreenBotLeft.x + PROGRESS_BAR_OUTLINE, m_playScreenBotLeft.y + PROGRESS_BAR_OUTLINE), 
	//	ProgressBar::BARMODE_VERTICAL_DTU);
	//m_manaBar = ProgressBar(Vector2i(300, 20), 
	//	Vector2i(INTERFACE_SIDE_LEFT_WIDTH + PROGRESS_BAR_OUTLINE, 10), ProgressBar::BARMODE_HORIZONTAL_LTR);
	//m_testbar = ProgressBar(Vector2i(20, 400),
	//	Vector2i(Width() / 2, Height() / 2), ProgressBar::BARMODE_HORIZONTAL_RTL);
	//m_testbar.SetVisible(false);
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

#pragma Elements de la camera

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
	m_projectile.SetDestination(m_player.Position());
	m_projectile.Move(elapsedTime);
	m_projectile.Render();

	m_testpig.Update(elapsedTime);
	m_testpig.Render();

	Shader::Disable();

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

#pragma region Affichage du texte

	std::ostringstream ss;
	//Print de la position
	ss << "Position : " << m_player.Position();
	PrintText(INTERFACE_SIDE_LEFT_WIDTH + 10, Height() - INTERFACE_TOP_HEIGHT - 20, ss.str());
	ss.str("");
	//Print de la vitesse
	ss << "Vitesse : " << m_player.Speed();
	PrintText(INTERFACE_SIDE_LEFT_WIDTH + 10, Height() - INTERFACE_TOP_HEIGHT - 35, ss.str());
	ss.str("");
	//Print de l'Accélération
	ss << "Acceleration : " << m_player.Acceleration();
	PrintText(INTERFACE_SIDE_LEFT_WIDTH + 10, Height() - INTERFACE_TOP_HEIGHT - 50, ss.str());
	ss.str("");
	//Print du nombre de FPS
	ss << "Fps : " << std::setprecision(5) << 1 / elapsedTime;
	PrintText(Width() - INTERFACE_SIDE_RIGHT_WIDTH - 120, Height() - INTERFACE_TOP_HEIGHT - 20, ss.str());
	ss.str("");

#pragma endregion

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

	m_pnl_portrait.Render();
	m_pgb_energy.Render();
	m_pgb_health.Render();
	m_pgb_mana.Render();
	m_pgb_exp.Render();


	//Activation du blend par PNG
	StartBlendPNG();

	//// Portrait
	//RenderSquare(
	//	Vector2i(m_playScreenBotLeft.x + m_pgb_energy.Size().y, m_playScreenBotLeft.y),
	//	Vector2i((int)m_textureInterface[IMAGE_PORTRAIT_FRAME].GetWidth(), (int)m_textureInterface[IMAGE_PORTRAIT_FRAME].GetHeight()),
	//	m_textureInterface[IMAGE_PORTRAIT_FRAME]);
	//Optimisation possible par la surcharge d'opérateurs
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))		//Mode course
		RenderSquare(
		m_playScreenBotLeft,
		Vector2i(m_textureInterface[IMAGE_RUN].GetWidth(), m_textureInterface[IMAGE_RUN].GetHeight()),
		m_textureInterface[IMAGE_RUN]);
	if (m_ghostMode)			//Mode ghost
		RenderSquare(
		Vector2i(Width() / 2 - m_textureInterface[IMAGE_BOO].GetWidth() /2, m_playScreenBotLeft.y),
		Vector2i(m_textureInterface[IMAGE_BOO].GetWidth(), m_textureInterface[IMAGE_BOO].GetHeight()),
		m_textureInterface[IMAGE_BOO]);

	glDisable(GL_BLEND);

#pragma endregion

#pragma region Affichage de l interface sans transparence

	//============================================

	//Bottom
	RenderSquare(
		Vector2i(0, 0), 
		Vector2i(Width(), INTERFACE_BOTTOM_HEIGHT), 
		m_textureInterface[IMAGE_INTERFACE_FRAME]);
	//Left
	RenderSquare(
		Vector2i(0, INTERFACE_BOTTOM_HEIGHT), 
		Vector2i(INTERFACE_SIDE_LEFT_WIDTH, Height() - INTERFACE_TOP_HEIGHT * 3), 
		m_textureInterface[IMAGE_INTERFACE_FRAME]);
	//Top
	RenderSquare(
		Vector2i(0, Height() - INTERFACE_TOP_HEIGHT),
		Vector2i(Width(), INTERFACE_TOP_HEIGHT),
		m_textureInterface[IMAGE_INTERFACE_FRAME]);
	//Right
	RenderSquare(
		Vector2i(Width() - INTERFACE_SIDE_RIGHT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(Width(), Height() - INTERFACE_TOP_HEIGHT * 3),
		m_textureInterface[IMAGE_INTERFACE_FRAME]);
	//============================================
	RenderSpells();
	//============================================
	//Mise à jour des données
	m_pgb_health.SetValue(m_character.HealthPerc());
	m_pgb_energy.SetValue(m_character.EnergyPerc());
	m_pgb_mana.SetValue(m_character.ManaPerc());
	m_pgb_exp.SetValue(m_character.EnergyPerc());
	//============================================
	RenderProgressBars();
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

void Engine::RenderProgressBars()
{
	std::ostringstream ss;
	m_pnl_portrait.Render();

	//Textes des bars
	StartBlendPNG(false);
	ss << "Vie             " << (int)m_character.Health() << " / " << (int)m_character.HealthMax();
	PrintText(m_pgb_health.AbsolutePosition().x + PROGRESS_BAR_OUTLINE, 
		m_pgb_health.AbsolutePosition().y + PROGRESS_BAR_OUTLINE, ss.str());
	ss.str("");

	ss << "Energie";
	PrintText(m_pgb_energy.AbsolutePosition().x, 
		m_pgb_energy.AbsolutePosition().y + m_pgb_energy.Size().x + PROGRESS_BAR_OUTLINE * 2 + 12, ss.str());
	ss.str("");
	ss << (int)m_character.Energy() << " / " << (int)m_character.EnergyMax();
	PrintText(m_pgb_energy.AbsolutePosition().x, 
		m_pgb_energy.AbsolutePosition().y + m_pgb_energy.Size().x + PROGRESS_BAR_OUTLINE * 2, ss.str());
	ss.str("");

	ss << "Mana            " << (int)m_character.Mana() << " / " << (int)m_character.ManaMax();
	glEnable(GL_BLEND);
	PrintText(m_pgb_mana.AbsolutePosition().x + PROGRESS_BAR_OUTLINE, 
		m_pgb_mana.AbsolutePosition().y + PROGRESS_BAR_OUTLINE, ss.str());
	ss.str("");
	glDisable(GL_BLEND);
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t)
{
	m_textureInterface[IMAGE_FONT].Bind();
	glLoadIdentity();
	glTranslated(x, y, 0);
	for (unsigned int i = 0; i < t.length(); ++i)
	{
		float left = (float)((t[i] - 32) % 16) / 16.0f;
		float top = (float)((t[i] - 32) / 16) / 16.0f;
		top += 0.5f;
		glBegin(GL_QUADS);
		glTexCoord2f(left, 1.0f - top - 0.0625f);
		glVertex2i(0, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
		glVertex2i(12 , 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top);
		glVertex2i(12, 12);
		glTexCoord2f(left , 1.0f - top );
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
		m_projectile.SetPosition(Vector3f(10,0,0));
		m_projectile.Shoot();
		sound.PlaySnd(Son::SON_BOLT, Son::CHANNEL_SPELL);
		break;
	case 28:
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
		sound.PlaySnd(Son::SON_STORM, Son::CHANNEL_SPELL);
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
		m_projectile.Shoot();
		break;
	default:
		std::cout << "Unhandled key: " << (int)key << std::endl;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			Info::Get().Sound().PlayNextTrack();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
			Info::Get().Sound().TestSon();
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

