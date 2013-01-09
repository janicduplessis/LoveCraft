#ifndef CONTROL_H_
#define CONTROL_H_

#include "define.h"
#include "../texture.h"
#include "util/vector2.h"
#include <cassert>
#include <string>
#include <iostream>
#include <map>

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
 * @brief �venement
 * 
 * Store les �venements (EventHandlers) dans la classe qui les d�clenches
 * Pour l'utiliser, d�clarer un membre public Event<typeDeRetour, typeDuParam> dans la classe
 * typeDeRetour: Type de retour de la fonction qui est trigger par l'�v�nement
 * typeDuParam: Param�tre de la fonction qui est trigger
 * 
 * Pour notifier tout les abonn�s � l'evenement de la classe, utiliser
 * EventName.Notify(param);
 * 
 * Pour s'abonner, myClass.Event.Attach(objetAAbonner, &ClassAAbonner::methodeATrigger);
 * methodeATrigger doit avoir le m�me type de retour le parametres que ceux choisi 
 * � la d�claration de l'�venement.
 * 
 * Pour se d�sabonner il faut garder une copy du event handler lorsque on s'abonne et
 * l'utiliser avec la m�thode myClass.Event.Detach(eventHandler);
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
		CTRLTYPE_LISTBOX
	};
	/**
	* Constructeur par d�faut
	*/
	Control();
	/**
	* Constructeur de la classe
	*
	* @param type	Le type du controle - Voir Enum Type
	*/
	Control(Type type);
	/**
	* Constructeur de la classe
	*
	* @param type		Le type du controle - Voir Enum Type
	* @param parent		Le controle qui joue le r�le de parent
	* @param position	La poisition initiale du controle par rapport a son parent
	* @param size		La taille du controle
	* @param name		Le nom du controle
	*/
	Control(Type type, Control* parent, Vector2i position, Vector2i size, Texture* texture, const std::string& name);
	/**
	* Destructeur par d�faut
	*/
	virtual ~Control();
	/**
	* Dessine l'objet � l'�cran
	*/
	virtual void Render();
	/**
	* Dessine l'objet � l'�cran en utilisant une texture
	*/
	virtual void Render(Texture* texture);
	/**
	* Obtient le nom du cotnrole
	* 
	* @return std::string
	*/
	virtual std::string Name() const;
	/**
	* Obtient la valeur indiquant si le controle est visible
	* 
	* @return bool
	*/
	virtual bool Visible() const;
	/**
	* D�finit la valeur indiquant si le controle est visible
	*/
	virtual void SetVisible(const bool value);
	/**
	* Obtient la taille du controle
	* 
	* @return Vector2i
	*/
	virtual Vector2i Size() const;
	/**
	* D�finit la taille du controle
	*/
	virtual void SetSize(Vector2i value);
	/**
	* Obtient la position relative au conteneur du controle
	* 
	* @return Vector2i
	*/
	virtual Vector2i Position() const;
	/**
	* D�finit la position relative au conteneur du controle
	*/
	virtual void SetPosition(Vector2i value);
	/**
	* Obtient la position absolue du controle
	* 
	* @return Vector2i
	*/
	virtual Vector2i AbsolutePosition() const;
	/**
	* D�finit la texture qui sera utilis�e lors du Render
	*/
	virtual void SetTexture(Texture* text);
	/**
	* Obtient la texture qui sera utilis�e lors du Render
	* 
	* @return Texture*
	*/
	virtual Texture* GetTexture() const;
	/**
	* Obtient la valeur indiquant si le blend doit �tre activ� lors du Render
	* 
	* @return bool
	*/
	virtual bool GetPngBlend() const;
	/**
	* D�finit la valeur indiquant si le blend doit �tre activ� lors du Render
	*/
	virtual void SetPngBlend(const bool value);
	virtual bool GetRepeatTexture() const;
	virtual void SetRepeatTexture(const bool value);

	Control& operator=(const Control& c);

protected:
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size);
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture);

	std::string m_name;
	Type m_type;
	bool m_pngBlend;
	bool m_repeatTexture;
	bool m_visible;
	Texture* m_texture;
	Control* m_parent;
	Vector2i m_position;
	Vector2i m_size;
};

#endif