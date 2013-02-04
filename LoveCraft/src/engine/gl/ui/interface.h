#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "interfaceinfos.h"
#include "define.h"
#include "engine/gl/ui/valuesinterface.h"
#include "engine/gl/ui/controls/localizable/textual/singletext/button.h"
#include "engine/gl/ui/controls/localizable/textual/singletext/spellbutton.h"
#include "engine/gl/ui/controls/localizable/container/panel.h"
#include "engine/gl/ui/controls/localizable/textual/label.h"
#include "engine/gl/ui/controls/localizable/textual/listbox.h"
#include "engine/gl/ui/controls/localizable/drawable/picturebox.h"
#include "engine/gl/ui/controls/localizable/drawable/progressbar.h"
#include "engine/gl/ui/controls/localizable/textual/singletext/textbox.h"

class Interface
{
public:
	Interface();
	virtual ~Interface();
	virtual void DeInit();
	virtual void Render() = 0;
protected:
	bool m_loaded;

};

#endif