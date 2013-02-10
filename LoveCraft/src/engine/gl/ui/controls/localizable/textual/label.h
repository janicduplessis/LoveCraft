#ifndef LABEL_H_
#define LABEL_H_

#include "../textual.h"
#include "util/vector2.h"
#include "util/vector3.h"

class Label : public Textual
{
public:
	Label();
	virtual ~Label();
	virtual void Init(Docking dock, Point offset = Point(0, 0));
	virtual void InitLocalizable(Point position, Container* parent, Localizable* anchor = 0);
	virtual void Render();

	virtual void SetVariableMsg(string variable);
	virtual void SetVariableMsg(float variable);
	virtual void SetVariableMsg(int variable);
	virtual void SetVariableMsg(uint32 variable);
	virtual void SetVariableMsg(bool variable);
	virtual void SetVariableMsg(Control* variable);
	virtual void SetVariableMsg(Vector3<float> variable);
	virtual void SetVariableMsg(Vector2<float> variable);
	virtual void SetVariableMsg(Vector3<int> variable);
	virtual void SetVariableMsg(Vector2<int> variable);
	virtual void SetVariableMsg(Point variable);
	virtual void SetVariableMsg(Size variable);

	virtual void SetDocking(Docking dock);
	virtual Docking GetDocking() const;
	virtual bool IsDocking(Docking dock) const;
	virtual void UseNextDocking();

	virtual void SetOffset(Point offset);
	virtual Point GetOffset() const;
	virtual bool IsOffset(Point &offset) const;

	virtual Point AbsolutePosition() const;
protected:
	Localizable* m_anchor;
	string m_variableMsg;
	string Replace();
	Docking m_docking;
	Point m_offset;
};

#endif