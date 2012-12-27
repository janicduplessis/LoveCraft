#include "engine.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include "son.h"
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
	GLenum err = glewInit ();
	if( err != GLEW_OK )
	{
		std :: cerr << " Error while initializing glew .. abording (" << glewGetErrorString ( err) << ")" << std :: endl ;
		abort ();
	}

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


	m_player.Init();
	m_projectile.Init();
	m_projectile.SetInitialSpeed(Vector3f(4,0,0));
	m_projectile.SetCollisionRadius(Vector3f(2,2,2));
	m_projectile.SetMaxRot(0.05);
	m_projectile.SetPosition(Vector3f(10,0,0));

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

	//Platoforme de gazon
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

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
	// Texture des blocs 128x128 px
	m_textureArray = new TextureArray(128);

	LoadBlocTexture(BTYPE_BRICK, TEXTURE_PATH "brick_red.jpg");
	LoadBlocTexture(BTYPE_DIRT, TEXTURE_PATH "dirt.bmp");
	LoadBlocTexture(BTYPE_GRASS, TEXTURE_PATH "grass.bmp");
	LoadBlocTexture(BTYPE_SAND, TEXTURE_PATH "sand.jpg");

	m_textureArray->Generate();
	//Texture des spells
	m_textureSpell[0].Load(TEXTURE_PATH "SpellBolt.gif");
	m_textureSpell[1].Load(TEXTURE_PATH "SpellFire.png");
	m_textureSpell[2].Load(TEXTURE_PATH "SpellFreeze.png");
	m_textureSpell[3].Load(TEXTURE_PATH "SpellShock.png");
	m_textureSpell[4].Load(TEXTURE_PATH "SpellPoison.gif");
	m_textureSpell[5].Load(TEXTURE_PATH "SpellStorm.png");
	m_textureSpell[6].Load(TEXTURE_PATH "SpellHeal.gif");
	m_textureSpell[7].Load(TEXTURE_PATH "SpellRain.gif");
	m_textureSpell[8].Load(TEXTURE_PATH "SpellDefend.gif");
	m_textureSpell[9].Load(TEXTURE_PATH "SpellShield.png");


	LoadTexture(m_textureFloor, TEXTURE_PATH "checker.bmp");
	LoadTexture(m_textureInterface, TEXTURE_PATH "rock.jpg");
	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");
	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
	LoadTexture(m_textureCthulhu, TEXTURE_PATH "bewareofcthulhu.png");
	LoadTexture(m_textureGhost, TEXTURE_PATH "boo.png");
	LoadTexture(m_textureNoir, TEXTURE_PATH "noir.jpg");
	LoadTexture(m_textureHealth, TEXTURE_PATH "health.png");
	LoadTexture(m_textureEnergy, TEXTURE_PATH "energy.png");
	LoadTexture(m_textureMana, TEXTURE_PATH "mana.png");

	//Initialisation des éléments de l'interface
	m_healthBar = ProgressBar(Vector2i(400, 20), Vector2i(INTERFACE_SIDE_LEFT_WIDTH + PROGRESS_BAR_OUTLINE, 35));
	m_energyBar = ProgressBar(Vector2i(400, 20), Vector2i(INTERFACE_SIDE_LEFT_WIDTH + PROGRESS_BAR_OUTLINE, 10));
	m_manaBar = ProgressBar(Vector2i(400, 20), Vector2i(Width() - (INTERFACE_SIDE_RIGHT_WIDTH + m_manaBar.Size().x), 35));

	// Load et compile les shaders
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
	static float gameTime = elapsedTime;

	gameTime += elapsedTime;
	float healthValue = m_healthBar.Value();
	float energyValue = m_energyBar.Value();
	// calcul la position du joueur et de la camera
	m_player.Move(m_ghostMode, healthValue, energyValue, elapsedTime);
	m_camera.SetPosition(m_player.Position());
	m_manaBar.SetValue(m_manaBar.Value() + MANA_PASSIVE_REGEN);
	m_healthBar.SetValue(healthValue);
	m_energyBar.SetValue(energyValue);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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

	// Render les cubes
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

	// HUD
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Render2D(elapsedTime);
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_projectile.SetDestination(m_player.Position());
	m_projectile.Move(elapsedTime);
	m_projectile.Render();

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
}

void Engine::Render2D(float elapsedTime)
{
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// Bind de la texture pour le font
	//m_textureFont.Bind();
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
	//Affichage du crosshair
	if (m_camera.GetMode() == Camera::CAM_FIRST_PERSON)
	{
		RenderSquare(
			Vector2i(Width() / 2 - CROSSHAIR_SIZE / 2, Height() / 2 - CROSSHAIR_SIZE / 2),
			Vector2i(CROSSHAIR_SIZE, CROSSHAIR_SIZE),
			m_textureCrosshair, false);
	}
	//Optimisation possible par la surcharge d'opérateurs
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))		//Mode course
		RenderSquare(
		m_playScreenBotLeft,
		Vector2i(m_textureCthulhu.GetWidth(), m_textureCthulhu.GetHeight()),
		m_textureCthulhu);
	if (m_ghostMode)			//Mode ghost
		RenderSquare(
		Vector2i(Width() / 2 - m_textureGhost.GetWidth() /2, m_playScreenBotLeft.y),
		Vector2i(m_textureGhost.GetWidth(), m_textureGhost.GetHeight()),
		m_textureGhost);

	glDisable(GL_BLEND);
	//Affichage de l'interface
	//Bottom
	RenderSquare(
		Vector2i(0, 0), 
		Vector2i(Width(), INTERFACE_BOTTOM_HEIGHT), 
		m_textureInterface);
	//Left
	RenderSquare(
		Vector2i(0, INTERFACE_BOTTOM_HEIGHT), 
		Vector2i(INTERFACE_SIDE_LEFT_WIDTH, Height() - INTERFACE_TOP_HEIGHT * 3), 
		m_textureInterface);
	//Top
	RenderSquare(
		Vector2i(0, Height() - INTERFACE_TOP_HEIGHT),
		Vector2i(Width(), INTERFACE_TOP_HEIGHT),
		m_textureInterface);
	//Right
	RenderSquare(
		Vector2i(Width() - INTERFACE_SIDE_RIGHT_WIDTH, INTERFACE_BOTTOM_HEIGHT),
		Vector2i(Width(), Height() - INTERFACE_TOP_HEIGHT * 3),
		m_textureInterface);

	RenderSpells();
	//m_healthBar.Render(Vector2i(20, Height() / 2));
	//m_energyBar.Render(Vector2i(20, Height() / 2 - 30));

	//============================================
	RenderSquare(Vector2i(m_healthBar.Position().x - PROGRESS_BAR_OUTLINE, m_healthBar.Position().y - PROGRESS_BAR_OUTLINE),
		Vector2i(m_healthBar.Size().x + PROGRESS_BAR_OUTLINE * 2, m_healthBar.Size().y + PROGRESS_BAR_OUTLINE * 2), 
		m_textureNoir);
	RenderSquare(m_healthBar.Position(), 
		Vector2i(m_healthBar.ValueWidth(), m_healthBar.Size().y), 
		m_textureHealth);
	RenderSquare(Vector2i(m_energyBar.Position().x - PROGRESS_BAR_OUTLINE, m_energyBar.Position().y - PROGRESS_BAR_OUTLINE),
		Vector2i(m_energyBar.Size().x + PROGRESS_BAR_OUTLINE * 2, m_energyBar.Size().y + PROGRESS_BAR_OUTLINE * 2), 
		m_textureNoir);
	RenderSquare(m_energyBar.Position(), 
		Vector2i(m_energyBar.ValueWidth(), m_energyBar.Size().y), 
		m_textureEnergy);
	//============================================

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

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

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t)
{
	m_textureFont.Bind();
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
		if (m_manaBar.Value() - 15 >= 0)
		{
			sound.PlaySnd(Son::SON_HEAL1, Son::CHANNEL_SPELL);
			m_healthBar.SetValue(m_healthBar.Value() + 5);
			m_manaBar.SetValue(m_manaBar.Value() - 15);
		}
		break;
	case 34:
		sound.PlaySnd(Son::SON_HEAL2, Son::CHANNEL_SPELL);
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
	case MOUSEEVENTF_MIDDLEDOWN:
		Info::Get().Sound().PlayMusic();
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

