1. Pour ajouter/supprimer des types de blocs

- Aller dans define.h et ajouter son type � l'enum BLOCK_TYPE.
- Appeler la m�thode LoadBlocTexture dans la m�thode LoadResource()
  du engine.cpp et y indiquer le type de bloc et le string nom de
  fichier.
- Dans info.cpp, ajouter le bloc � la m�thode GenerateBlocInfos().
  Utiliser la syntaxe suivante: m_blocInfos[type] = new BlockInfo(
  type, string nom);
- Aller dans son.cpp dans la m�thode GetFootType(BlockType type),
  ajouter � la switch le nouveau type de bloc et y assigner un son
  de pas.

  Note importante!
  
  Faire attention � la m�thode AddTexture de texturearray.cpp parce qu'elle
  assigne les textures par it�rations au lieu de se servir de l'enum. Il faut 
  donc charger les textures dans un certain ordre.

2. Contr�les

2.1. Pour ajouter un nouveau contr�le

- Aller dans engine.h et ajouter le contr�le sous la forme d'un champ private
- Aller dans la m�thode LoadResource() du fichier engine.cpp
- Apr�s le chargement des textures, initialiser le contr�le avec le constructeur
  acceptant des param�tres (ne pas utiliser le constructeur par d�faut puisque �a
  enl�verait l'utilit� d'avoir un contr�le visuel personnalis�...)
- Faire attention au champ Position. C'est en fait la position du contr�le par
  rapport � son conteneur
- Ajouter le contr�le au conteneur appropri�
- Incr�menter la constante PNL_name_CONTROL_NBR de 1 dans le fichier interface.h
- Si tous les contr�les ont bien �t� ajout�s � leur conteneur, un seul appel
  de la m�thode Render() de m_pnl_screen dessinera tous les contr�les

2.2. Ajouter des propri�t�s � une classe d�riv�e

- Aller dans le fichier de la classe en question.
- Ajouter le champ dans la section private
- Cr�er 2 m�thodes, une pour acc�der et une pour changer le champ (si n�cessaire)
- Ajouter le champ � la surcharge d'op�rateur de la classe.
- Ne pas oublier d'initialiser la variable dans le constructeur
- Si le champ doit faire partie du contructeur, le mettre � la suite des
  autres param�tre. Aussi ajouter l'initialisation au constructeur par d�faut
  avec une valeur similaire � null
- Apporter les modifications n�cessaires aux �l�ments d�j� construits afin qu'ils
  acceptent ce nouveau param�tre.

2.3. Ajouter des propri�t�s � la classe de base

- Aller dans le fichier control.h(cpp) et ajouter le champ dans la section protected
- Cr�er 2 m�thodes, une pour acc�der et une pour changer le champ (si n�cessaire)
- Ajouter le champ � la surcharge d'op�rateur de la classe de base
- Ajouter le champ dans toutes les classes qui d�rivent de celle-ci.
- Initiliser le champ dans le constructeur par d�faut.
- Si le champ doit faire partie du contructeur, le mettre � la suite des
  autres param�tre. Aussi ajouter l'initialisation au constructeur par d�faut
  avec une valeur similaire � null.
- Aller dans les classes d�riv�es de celle-ci et y inclure le nouveau param�tre dans
  les contructeurs (.h et .cpp) et l'initialiser dans l'appel du constructeur de la
  classe de base.
- Apporter les modifications n�cessaires aux �l�ments d�j� construits afin qu'ils
  acceptent ce nouveau param�tre.
