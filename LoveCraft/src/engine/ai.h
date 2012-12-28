#ifndef AI_H__
#define AI_H__

class AI
{
public:
	AI();
	virtual ~AI();

	bool StateChanged() const;
	virtual void Process();

protected:
	virtual void CheckCollision();
	virtual void CheckPlayer();
};

#endif