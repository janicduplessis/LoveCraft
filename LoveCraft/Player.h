#define MOUSE_SENSIBILITY 0.1f
#define PII 3.141592654f

class Player
{
public:
	Player();
	Player(float posX, float posY, float posZ, float rotX, float rotY);
	~Player();

	void TurnLeftRight ( float value );
	void TurnTopBottom ( float value );

	void Move ( bool front , bool back , bool left , bool right , float elapsedTime );

	void ApplyRotation () const ;
	void ApplyTranslation () const ;

private:
	float m_posX;
	float m_posY;
	float m_posZ;
	float m_rotX;
	float m_rotY;
};

