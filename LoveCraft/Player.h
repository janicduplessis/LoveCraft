#define MOUSE_SENSIBILITY 0.1f
#define PII 3.141592654f

class Player
{
public:
	Player(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0);
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

