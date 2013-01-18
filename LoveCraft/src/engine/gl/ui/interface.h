#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "interfaceinfos.h"
#include "define.h"
#include "engine/engine.h"
#include "engine/gl/ui/button.h"
#include "engine/gl/ui/panel.h"
#include "engine/gl/ui/label.h"
#include "engine/gl/ui/listbox.h"
#include "engine/gl/ui/picturebox.h"
#include "engine/gl/ui/progressbar.h"
#include "engine/gl/ui/textbox.h"

class Engine;
class Interface
{
public:
	Interface();
	virtual ~Interface();
	virtual void Init(Engine* engine);
	virtual void InitControls() = 0;
protected:
	bool m_loaded;
	Engine* m_engine;
};

#endif