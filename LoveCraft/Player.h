#define MOUSE_SENSIBILITY 0.1f
#define MOUVEMENT_SPEED 0.2f
#define MOUVEMENT_SPEED_RUN 0.35f
#define MOUVEMENT_SPEED_BACKWARD 0.1f
#define PII 3.141592654f

class Player
{
public:
	/**
	* DESCRIPTION
	*
	* Constructeur par d�faut de la classe. Initialise tous les champs � 0
	*
	*/
	Player();
	/**
	* DESCRIPTION
	*
	* Constructeur de la classe.
	* 
	* @param posX Position initiale en X du joueur
	* @param posY Position initiale en Y du joueur
	* @param posZ Position initiale en Z du joueur
	* @param rotX Angle de rotation initial en X du joueur
	* @param rotY Angle de rotation initial en Y du joueur
	*/
	Player(float posX, float posY, float posZ, float rotX, float rotY);
	/**
	* DESCRIPTION
	*
	* Destructeur par d�faut de la classe
	*
	*/
	~Player();
	/**
	* DESCRIPTION
	*
	* Applique les valeurs de l'angle de rotation sur l'axe des Y
	* 
	* @param value Valeur de l'angle � modifier
	* @return void
	*
	*/
	void TurnLeftRight ( float value );
	/**
	* DESCRIPTION
	*
	* Applique les valeurs de l'angle de rotation sur l'axe des X
	* 
	* @param value Valeur de l'angle � modifier
	* @return void
	*
	*/
	void TurnTopBottom ( float value );
	/**
	* DESCRIPTION
	*
	* Fait bouger le personnage en fonction des touches enfonc�es
	* 
	* @param front Si la touche pour avancer est enfonc�e
	* @param back Si la touche pour reculer est enfonc�e
	* @param left Si la touche pour strafer � gauche est enfonc�e
	* @param right  Si la touche pour strafer � droite est enfonc�e
	/ @param run Si la touche pour courir est enfonc�e
	* @param elapsedTime Temps �coul� depuis le dernier loop de jeu
	* @return void
	*
	*/
	void Move ( bool front , bool back , bool left , bool right , bool run, bool ghost, float elapsedTime );
	/**
	* DESCRIPTION
	*
	* Applique la rotation de la cam�ra en fonction des donn�es de
	* positionnement et d'angle du joueur. � appeler apr�s la d�claration
	* de la matrice identit�e principale
	*
	*/
	void ApplyRotation () const ;
	/**
	* DESCRIPTION
	*
	* Applique la translation du joueur en fonction des donn�es de
	* positionnement et d'angle du joueur. � appeler apr�s la d�claration
	* de la matrice identit�e principale
	*
	*/
	void ApplyTranslation () const ;

private:
	/**
	* DESCRIPTION
	*
	* Position en X du joueur
	*
	*/
	float m_posX;
	/**
	* DESCRIPTION
	*
	* Position en Y du joueur
	*
	*/
	float m_posY;
	/**
	* DESCRIPTION
	*
	* Position en Z du joueur
	*
	*/
	float m_posZ;
	/**
	* DESCRIPTION
	*
	* Angle de rotation en X du joueur
	*
	*/
	float m_rotX;
	/**
	* DESCRIPTION
	*
	* Angle de rotation en Y du joueur
	*
	*/
	float m_rotY;
	/**
	* DESCRIPTION
	*
	* Derni�re coordonn�e de la souris en X.
	*
	*/
	int m_lastX;
	/**
	* DESCRIPTION
	*
	* Derni�re coordonn�e de la souris en Y.
	*
	*/
	int m_lastY;
};

