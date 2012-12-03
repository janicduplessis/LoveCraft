#include "engine.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "arraybool.h"



Engine::Engine() : m_wireframe(false), m_angle(0), m_dirBack(false), m_dirFront(false), m_dirLeft(false), m_dirRight(false), 
	m_space(false), m_ctrl(false), m_run(false), m_ghostMode(false), m_rightClick(false), m_leftClick(false), m_camRadius(10)
{
	Array<bool> a01(128);
	a01.Reset(false);
	a01.Set(0, true);
	a01.Set(3, true);

	std::cout << a01.Get(0) << std::endl;
	std::cout << a01.Get(1) << std::endl;
	std::cout << a01.Get(3) << std::endl;

}

Engine::~Engine()
{
}

void Engine::Init()
{
	GLenum err = glewInit ();
	if( err != GLEW_OK )
	{
		std :: cerr << " Error while initializing glew .. abording (" << glewGetErrorString ( err) << ")" << std :: endl ;
		abort ();
	}

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
	if (!m_wireframe)
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

	m_player.Init();

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
	LoadTexture(m_textureFloor, TEXTURE_PATH "checker.bmp");
	std::cout << " Loading and compiling shaders ..." << std::endl;
	if (!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
	{
		std::cout << " Failed to load shader " << std::endl;
		exit(1) ;
	}
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
	static float gameTime = elapsedTime;

	gameTime += elapsedTime;

	// calcul la position du joueur et de la camera
	m_player.Move(m_dirFront, m_dirBack, m_dirLeft, m_dirRight, m_space, m_ctrl, m_run, m_ghostMode, elapsedTime);
	m_camera.SetPosition(m_player.GetPosition());

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
		m_shader01.Use();
		m_player.Render(m_wireframe);
		Shader::Disable();
	} 
	// first person
	else
	{
		m_camera.ApplyRotation();
		m_camera.ApplyTranslation();
	}


	// Plancher
	m_textureFloor.Bind();
	float nbRep = 200.f;

	glBegin(GL_QUADS);
	glNormal3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-100.f, -2.f, 100.f);
	glTexCoord2f(nbRep, 0);
	glVertex3f(100.f, -2.f, 100.f);	glTexCoord2f(nbRep, nbRep);
	glVertex3f(100.f, -2.f, -100.f);
	glTexCoord2f(0, nbRep);
	glVertex3f(-100.f, -2.f, -100.f);
	glEnd();

	// Test du chunk
	if(m_testChunk.IsDirty())
		m_testChunk.Update();
	m_shader01.Use();
	m_testChunk.Render();
	Shader::Disable();

}

void Engine::Render2D( float elapsedTime )
{

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
	case 57:   // Space
		m_space = true;
		break;
	case 37:   // CTRL
		m_ctrl = true;
		break;
	case 38:   // Shift
		m_run = true;
		break;
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
	case 57:   // Space
		m_space = false;
		break;
	case 37:   // CTRL
		m_ctrl = false;
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

		m_camera.SetRotation(m_player.GetRotation());

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

