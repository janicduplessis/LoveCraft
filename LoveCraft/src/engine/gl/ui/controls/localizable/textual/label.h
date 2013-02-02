#ifndef LABEL_H_
#define LABEL_H_

#include "../textual.h"

/**
* Obient Définit la valeur de
*/

class Label : public Textual
{
public:
	Label();
	virtual ~Label();
	virtual void Init(Docking dock, Point offset = Point(0, 0));
	virtual void Render();

	virtual void SetDocking(Docking dock);
	virtual Docking GetDocking() const;
	virtual bool IsDocking(Docking dock) const;

	virtual void SetOffset(Point offset);
	virtual Point GetOffset() const;
	virtual bool IsOffset(Point &offset) const;

	virtual void UseNextDocking();

	virtual Point AbsolutePosition() const;
protected:
	virtual void DrawingBindTexture();
	Docking m_docking;
	Point m_offset;
};

#endif