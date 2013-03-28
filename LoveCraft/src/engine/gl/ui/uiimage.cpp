#include "uiimage.h"
#include "engine/gl/texture.h"
#include <cassert>

Texture** UIImage::m_textures = 0;

Texture* UIImage::Get(IMAGE image)
{
	if (!m_textures)
	{
		m_textures = new Texture*[CUSTIMAGE_NONE];
		for (uint8 i = 0; i < CUSTIMAGE_NONE; i++)
			m_textures[i] = new Texture();
		InitImages();
	}
	return m_textures[image];
}

void UIImage::InitImages()
{
	m_textures[CUSTIMAGE_BLACK_BACK]->Load(TEXTURE_PATH "noir.jpg");
	m_textures[CUSTIMAGE_BOO]->Load(TEXTURE_PATH "i_boo.png");
	m_textures[CUSTIMAGE_RUN]->Load(TEXTURE_PATH "i_bewareofcthulhu.png");
	m_textures[CUSTIMAGE_CROSSHAIR]->Load(TEXTURE_PATH "i_cross.bmp");
	m_textures[CUSTIMAGE_INTERFACE_FRAME]->Load(TEXTURE_PATH "b_rock.jpg");
	m_textures[CUSTIMAGE_PORTRAIT_FRAME]->Load(TEXTURE_PATH "i_portrait-frame.png");
	m_textures[CUSTIMAGE_PORTRAIT_MALE]->Load(TEXTURE_PATH "i_portrait-male.png");
	m_textures[CUSTIMAGE_PGBTEXT_HEALTH]->Load(TEXTURE_PATH "i_pgb_health.png");
	m_textures[CUSTIMAGE_PGBTEXT_ENERGY]->Load(TEXTURE_PATH "i_pgb_energy.png");
	m_textures[CUSTIMAGE_PGBTEXT_MANA]->Load(TEXTURE_PATH "i_pgb_mana.png");
	m_textures[CUSTIMAGE_PGBTEXT_EXP]->Load(TEXTURE_PATH "i_pgb_exp.png");
	m_textures[CUSTIMAGE_PGBTEXT_HEALTH_BACK]->Load(TEXTURE_PATH "i_pgb_health_back.png");
	m_textures[CUSTIMAGE_PGBTEXT_ENERGY_BACK]->Load(TEXTURE_PATH "i_pgb_energy_back.png");
	m_textures[CUSTIMAGE_PGBTEXT_MANA_BACK]->Load(TEXTURE_PATH "i_pgb_mana_back.png");
	m_textures[CUSTIMAGE_PGBTEXT_EXP_BACK]->Load(TEXTURE_PATH "i_pgb_exp_back.png");
	m_textures[CUSTIMAGE_PGBTEXT_HEALTH_LOW]->Load(TEXTURE_PATH "i_pgb_health_low.png");
	m_textures[CUSTIMAGE_CLOCK_BG]->Load(TEXTURE_PATH "i_clock_bg.png");
	m_textures[CUSTIMAGE_CONSOLE_BACK]->Load(TEXTURE_PATH "i_console_back.png");
	m_textures[CUSTIMAGE_CONSOLE_TEXTBOX_BACK]->Load(TEXTURE_PATH "i_console_textbox_back.png");
	m_textures[CUSTIMAGE_PERSONAL_CURSOR]->Load(TEXTURE_PATH "i_cursor.png");
	m_textures[CUSTIMAGE_LOADING_SCREEN]->Load(TEXTURE_PATH "i_loading.jpg");
	m_textures[CUSTIMAGE_MENU_BACKGROUND]->Load(TEXTURE_PATH "i_menu_back.png");
	m_textures[CUSTIMAGE_MENU_MAIN_WINDOW]->Load(TEXTURE_PATH "i_menu_main.png");
	m_textures[CUSTIMAGE_MENU_BUTTON_BACK]->Load(TEXTURE_PATH "i_menu_button.png");
	m_textures[CUSTIMAGE_MENU_LOGO]->Load(TEXTURE_PATH "i_menu_logo.png");
	m_textures[CUSTIMAGE_WELCOME_FACE]->Load(TEXTURE_PATH "i_welcomeface.png");
	m_textures[CUSTIMAGE_ARROWBUTTON_UP]->Load(TEXTURE_PATH "i_arrowbutton_up.jpg");
	m_textures[CUSTIMAGE_ARROWBUTTON_DOWN]->Load(TEXTURE_PATH "i_arrowbutton_down.jpg");
	m_textures[CUSTIMAGE_TOOLTIP_BACK]->Load(TEXTURE_PATH "i_tooltip_back.png");

	m_textures[CUSTIMAGE_SPELL_BOLT]->Load(TEXTURE_PATH "s_spellbolt.gif");
	m_textures[CUSTIMAGE_SPELL_FIRE]->Load(TEXTURE_PATH "s_spellfire.png");
	m_textures[CUSTIMAGE_SPELL_FREEZE]->Load(TEXTURE_PATH "s_spellfreeze.png");
	m_textures[CUSTIMAGE_SPELL_SHOCK]->Load(TEXTURE_PATH "s_spellshock.png");
	m_textures[CUSTIMAGE_SPELL_POISON]->Load(TEXTURE_PATH "s_spellpoison.gif");
	m_textures[CUSTIMAGE_SPELL_STORM]->Load(TEXTURE_PATH "s_spellstorm.png");
	m_textures[CUSTIMAGE_SPELL_HEAL]->Load(TEXTURE_PATH "s_spellheal.gif");
	m_textures[CUSTIMAGE_SPELL_RAIN]->Load(TEXTURE_PATH "s_spellrain.gif");
	m_textures[CUSTIMAGE_SPELL_DEFEND]->Load(TEXTURE_PATH "s_spelldefend.gif");
	m_textures[CUSTIMAGE_SPELL_SHIELD]->Load(TEXTURE_PATH "s_spellshield.png");
	m_textures[CUSTIMAGE_SPELL_OFFLINE]->Load(TEXTURE_PATH "s_offlinespell.png");

	for (int i = 0; i < CUSTIMAGE_NONE; i++)
		assert(m_textures[i]->IsValid());
}