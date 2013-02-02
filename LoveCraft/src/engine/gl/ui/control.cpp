#include "control.h"

Control::Control(CONTROLTYPE type) : Object(), m_type(type), m_parent(0), m_enabled(true)
{
}

Control::~Control()
{

}

void Control::Init(string name, Control* parent)
{
	m_parent = parent;
	m_name = name;
}

#pragma region Enabled

bool Control::IsEnabled() const
{
	return m_enabled;
}
void Control::Enable()
{
	m_enabled = true;
}
void Control::Disable()
{
	m_enabled = false;
}

#pragma endregion

Control* Control::GetParent() const
{
	return m_parent;
}

CONTROLTYPE Control::GetType() const
{
	return m_type;
}

//void Control::CtrlInit(Control* parent, Vector2i &position, Vector2i &size, Texture* texture, string name)
//{
//	m_parent = parent;
//	m_position = position;
//	m_size = size;
//	m_texture = texture;
//	m_name = name;
//}
//
//void Control::Render()
//{
//	if (m_visible)
//	{
//		if (m_texture)
//			RenderSquare(AbsolutePosition(), m_size, m_texture);
//	}
//}



// =============================================================================
//
//void Control::SP(PropBool boolprop, bool value)
//{
//	switch (boolprop)
//	{
//	case PROPBOL_REPEATTEXTURE:
//		m_repeatTexture = value;
//		break;
//	case PROPBOL_VISIBLE:
//		m_visible = value;
//		break;
//	case PROPBOL_ENABLED:
//		m_enabled = value;
//		break;
//	default:
//		assert(false);
//		break;
//	}
//}
//void Control::SP(PropVector2 vector2prop, Vector2i value)
//{
//	switch (vector2prop)
//	{
//	case PROPVCT2_POSITION:
//		m_position = value;
//		break;
//	case PROPVCT2_SIZE:
//		m_size = value;
//		break;
//	default:
//		assert(false);
//		break;
//	}
//}
//void Control::SP(PropString stringprop, string value)
//{
//	switch (stringprop)
//	{
//	case PROPSTR_NAME:
//		m_name = value;
//		break;
//	default:
//		assert(false);
//		break;
//	}
//}
//void Control::SP(PropTexture textureprop, Texture* value)
//{
//	switch (textureprop)
//	{
//	case PROPTEXT_BACKGROUND:
//		m_texture = value;
//		break;
//	default:
//		assert(false);
//		break;
//	}
//}




// =============================================================================
