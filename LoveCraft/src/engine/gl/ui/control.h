#ifndef CONTROL_H_
#define CONTROL_H_

#include "define.h"
#include "enumerators.h"
#include "engine/info.h"
#include "util/array.h"
#include "../texture.h"
#include "object.h"
#include <cassert>
#include <cmath>
#include <string>
#include <sstream>
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
class Control : public Object
{
public:
	Control(CONTROLTYPE type);
	virtual ~Control();
	void InitControl(string name);

	virtual bool IsEnabled() const;
	virtual void Enable();
	virtual void Disable();

	virtual CONTROLTYPE GetType() const;
	virtual bool IsType(CONTROLTYPE type) const;

protected:
	CONTROLTYPE m_type;
	bool m_enabled;
};

#endif