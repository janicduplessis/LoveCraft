#include "engine.h"
#include <iostream>
#include <algorithm>
#include <cmath>



Engine::Engine() : m_wireframe(false), m_angle(0), m_dirBack(false), m_dirFront(false), m_dirLeft(false), m_dirRight(false),
	m_run(false), m_ghostMode(false)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable( GL_TEXTURE_2D );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, (float)Width() / (float)Height(), 0.1f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable (GL_LINE_SMOOTH);
	glFrontFace( GL_CCW );
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);


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

	m_player = Player();

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
	LoadTexture(m_textureFloor, TEXTURE_PATH "checker.bmp");
	LoadTexture(m_textureCube, TEXTURE_PATH "cu_default.png");
	LoadTexture(m_textureWall, TEXTURE_PATH "wall-red.bmp");
	LoadTexture(m_textureCeiling, TEXTURE_PATH "checker-gold.bmp");
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
	static float gameTime = elapsedTime;

	gameTime += elapsedTime;

	m_player.Move(m_dirFront, m_dirBack, m_dirLeft, m_dirRight, m_run, m_ghostMode, elapsedTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_player.ApplyRotation();
	m_player.ApplyTranslation();

	// Plancher
	m_textureFloor.Bind();
	float nbRep = 200.f;

	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-100.f, -2.f, 100.f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(100.f, -2.f, 100.f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(100.f, -2.f, -100.f);
	glTexCoord2f(0, nbRep);
	glVertex3f(-100.f, -2.f, -100.f);
	glEnd();

	//Plafond
	m_textureCeiling.Bind();
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-100.f, 2.f, 100.f);
	glTexCoord2f(0, nbRep);
	glVertex3f(-100.f, 2.f, -100.f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(100.f, 2.f, -100.f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(100.f, 2.f, 100.f);
	glEnd();

	// Murs
	m_textureWall.Bind();
	//Gauche
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-8.f, -2.f, -100.f);
	glTexCoord2f(1, 0);
	glVertex3f(-8.f, -2.f, 100.f);
	glTexCoord2f(1, 1);
	glVertex3f(-8.f, 2.f, 100.f);
	glTexCoord2f(0, 1);
	glVertex3f(-8.f, 2.f, -100.f);
	glEnd();
	//Droite
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(8.f, -2.f, -100.f);
	glTexCoord2f(0, 1);
	glVertex3f(8.f, 2.f, -100.f);
	glTexCoord2f(1, 1);
	glVertex3f(8.f, 2.f, 100.f);
	glTexCoord2f(1, 0);
	glVertex3f(8.f, -2.f, 100.f);
	glEnd();
	//Front
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-8.f, -2.f, -16.f);
	glTexCoord2f(1, 0);
	glVertex3f(8.f, -2.f, -16.f);
	glTexCoord2f(1, 1);
	glVertex3f(8.f, 2.f, -16.f);
	glTexCoord2f(0, 1);
	glVertex3f(-8.f, 2.f, -16.f);
	glEnd();
	//Back
	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-8.f, -2.f, 8.f);
	glTexCoord2f(0, 1);
	glVertex3f(-8.f, 2.f, 8.f);
	glTexCoord2f(1, 1);
	glVertex3f(8.f, 2.f, 8.f);
	glTexCoord2f(1, 0);
	glVertex3f(8.f, -2.f, 8.f);
	glEnd();

	//Cube
	m_textureCube.Bind();

	glTranslatef(0, 0, -8.f);
	glRotatef(m_angle, -0.5f, 0.7f, 1.f);
	m_angle+= 3.f;

	// Face dessus
	nbRep = 1.f;
	m_textureCube.Bind();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, nbRep);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();

	// Face dessous
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, nbRep);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();

	// Face devant
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(0, nbRep);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();

	// Face arriere
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, nbRep);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glEnd();

	// Face gauche
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(0, nbRep);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();

	// Face droite
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, nbRep);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(nbRep, nbRep);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
}

void Engine::KeyPressEvent(unsigned char key)
{
	switch(key)
	{
	case 0:    // A
		m_dirLeft = true;
		break;
	case 3:    // D
		m_dirRight = true;
		break;
	case 22:   // W
		m_dirFront = true;
		break;
	case 18:   // S
		m_dirBack = true;
		break;
	case 38:   // Shift
		m_run = true;
		break;
	case 71:   // Left arrow
		m_player.TurnLeftRight(-3.f);
		break;
	case 72:   // Right arrow
		m_player.TurnLeftRight(3.f);
		break;
	case 73:   // Up arrow
		m_player.TurnTopBottom(3.f);
		break;
	case 74:   // Down arrow
		m_player.TurnTopBottom(-3.f);
		break;
	case 36:	// ESC
		Stop();
		break;
	case 94:	// F10
		SetFullscreen(!IsFullscreen());
		break;
	default:
		std::cout << "Unhandled key: " << (int)key << std::endl;
	}

}

void Engine::KeyReleaseEvent(unsigned char key)
{
	switch(key)
	{
	case 0:    // A
		m_dirLeft = false;
		break;
	case 3:    // D
		m_dirRight = false;
		break;
	case 22:   // W
		m_dirFront = false;
		break;
	case 18:   // S
		m_dirBack = false;
		break;
	case 38:   // Shift
		m_run = false;
		break;
	case 24:       // Y
		m_wireframe = !m_wireframe;
		if(m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 6:		   // G
		m_ghostMode = !m_ghostMode;
		std::cout << "Ghost Mode set to: " << m_ghostMode << std::endl;
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
	if(x == (Width() / 2) && y == (Height() / 2))
		return;
	MakeRelativeToCenter(x, y);
	m_player.TurnLeftRight((float)x);
	m_player.TurnTopBottom((float)y);
	CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
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

