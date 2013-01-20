#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "interfaceinfos.h"
#include "define.h"
#include "engine/gl/ui/interfacevalues.h"
#include "engine/gl/ui/control.h"
#include "engine/gl/ui/button.h"
#include "engine/gl/ui/panel.h"
#include "engine/gl/ui/label.h"
#include "engine/gl/ui/listbox.h"
#include "engine/gl/ui/picturebox.h"
#include "engine/gl/ui/progressbar.h"
#include "engine/gl/ui/textbox.h"

class Interface
{
public:
	Interface();
	virtual ~Interface();
	virtual void DeInit();
	virtual void Init(InterfaceValues* values);
	virtual void InitControls() = 0;
	virtual void Render() = 0;
protected:
	bool m_loaded;
	InterfaceValues* m_val;
};

#endif