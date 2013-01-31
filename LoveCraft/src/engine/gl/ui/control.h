#ifndef CONTROL_H_
#define CONTROL_H_

#include "define.h"
#include "../texture.h"
#include "util/vector2.h"
#include <cassert>
#include <string>
#include <iostream>
#include <map>

#pragma region Events

/**
 * Event handler
 */
template <typename ReturnT,typename ParamT>
class EventHandlerBase
{
public:
	virtual ReturnT Notify(ParamT param) = 0;
};

template <typename ListenerT, typename ReturnT, typename ParamT>
class EventHandler : public EventHandlerBase<ReturnT, ParamT>
{
	typedef ReturnT (ListenerT::*PtrMember)(ParamT);
	ListenerT* m_object;
	PtrMember m_member;

public:

	EventHandler(ListenerT* object, PtrMember member) : m_object(object), m_member(member) {}

	virtual ReturnT Notify(ParamT param) {
		return (m_object->*m_member)(param);
	}
};

/**
 * @brief Évenement
 * 
 * Store les évenements (EventHandlers) dans la classe qui les déclenches
 * Pour l'utiliser, déclarer un membre public Event<typeDeRetour, typeDuParam> dans la classe
 * typeDeRetour: Type de retour de la fonction qui est trigger par l'évènement
 * typeDuParam: Paramètre de la fonction qui est trigger
 * 
 * Pour notifier tout les abonnés à l'evenement de la classe, utiliser
 * EventName.Notify(param);
 * 
 * Pour s'abonner, myClass.Event.Attach(objetAAbonner, &ClassAAbonner::methodeATrigger);
 * methodeATrigger doit avoir le même type de retour le parametres que ceux choisi 
 * à la déclaration de l'évenement.
 * 
 * Pour se désabonner il faut garder une copy du event handler lorsque on s'abonne et
 * l'utiliser avec la méthode myClass.Event.Detach(eventHandler);
 */
template <typename ReturnT,typename ParamT>
class Event
{
public:
	typedef int EventHandlerID;

	Event()
		: m_count(0) {}

	template <typename ListenerT>
	EventHandlerID Attach(ListenerT* object,ReturnT (ListenerT::*member)(ParamT))
	{
		typedef ReturnT (ListenerT::*PtrMember)(ParamT);
		m_handlers[m_count] = (new EventHandler<ListenerT,
			ReturnT,ParamT>(object,member));
		m_count++;
		return m_count-1;
	}

	bool Detach(EventHandlerID id)
	{
		HandlersMap::iterator it = m_handlers.find(id);

		if(it == m_handlers.end())
			return false;

		delete it->second;
		m_handlers.erase(it);
		return true;
	}

	bool Notify(ParamT param)
	{
		HandlersMap::iterator it = m_handlers.begin();
		for(; it != m_handlers.end(); it++)
		{
			it->second->Notify(param);
		}

		return true;
	}

private:
	typedef std::map<int,EventHandlerBase<ReturnT,ParamT> *> HandlersMap;
	HandlersMap m_handlers;
	EventHandlerID m_count;
};

#pragma endregion

/**
 * @Brief Controle de l'interface 2d
 */
class Control
{
public:
	enum Type
	{
		CTRLTYPE_NONE,
		CTRLTYPE_PROGRESSBAR,
		CTRLTYPE_LABEL,
		CTRLTYPE_BOUTON,
		CTRLTYPE_PICTUREBOX,
		CTRLTYPE_PANEL,
		CTRLTYPE_LISTBOX,
		CTRLTYPE_TEXTBOX
	};
	enum BlendType
	{
		CBLEND_NONE,
		CBLEND_PNG,
		CBLEND_BLUR
	};

	enum PropBool
	{
		PROPBOL_REPEATTEXTURE,
		PROPBOL_VISIBLE,
		PROPBOL_ENABLED,
		PROPBOL_ITALIC,
		PROPBOL_SHOWBACKGROUND
	};
	enum PropFloat
	{
		PROPFLT_FONTW,
		PROPFLT_FONTH,
		PROPFLT_FONTI,
		PROPFLT_BARMIN,
		PROPFLT_BARMAX
	};
	enum PropUShort
	{
		PROPUSHRT_LINEGAP,
		PROPUSHRT_BORDERSIZE
	};
	enum PropVector2
	{
		PROPVCT2_POSITION,
		PROPVCT2_SIZE,
		PROPVCT2_TEXTOFFSET
	};
	enum PropString
	{
		PROPSTR_NAME,
		PROPSTR_TEXT
	};
	enum PropTexture
	{
		PROPTEXT_BACKGROUND,
		PROPTEXT_FONT_COLOR,
		PROPTEXT_BAR
	};

	/**
	* Constructeur de la classe
	*
	* @param type	Le type du controle - Voir Enum Type
	*/
	Control(Type type);
	/**
	* Destructeur par défaut
	*/
	virtual ~Control();
	void CtrlInit(Control* parent, Vector2i &position, Vector2i &size, Texture* texture, string name);
	/**
	* Dessine l'objet à l'écran
	*/
	virtual void Render();
	/**
	* Dessine l'objet à l'écran en utilisant une texture
	*/
	virtual void Render(Texture* texture);
	/**
	* Obtient la position absolue du controle
	* 
	* @return Vector2i
	*/
	virtual Vector2i AbsolutePosition() const;
	virtual void SP(PropBool boolprop, bool value);
	virtual void SP(PropVector2 vector2prop, Vector2i value);
	virtual void SP(PropString stringprop, string value);
	virtual void SP(PropTexture textureprop, Texture* value);
	virtual void SetBlend(BlendType btype);

	virtual bool MousePressEvents( int x, int y );
	virtual bool MouseReleaseEvents(int x, int y);
	virtual bool KeyPressEvents(int keycode);
	virtual bool KeyReleaseEvents(int keycode);

	virtual bool GP(PropBool boolprop) const;
	virtual Vector2i GP(PropVector2 vector2prop) const;
	virtual string GP(PropString stringprop) const;
	virtual Texture* GP(PropTexture textureprop) const;
	virtual BlendType GetBlend() const;
	virtual bool IsBlend(BlendType btype) const;

protected:
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size);
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture);
	string m_name;
	Type m_type;
	BlendType m_blend;
	bool m_repeatTexture;
	bool m_visible;
	bool m_enabled;
	Texture* m_texture;
	Control* m_parent;
	Vector2i m_position;
	Vector2i m_size;
};

#endif