1. Pour ajouter/supprimer des types de blocs

- Aller dans define.h et ajouter son type à l'enum BLOCK_TYPE.
- Appeler la méthode LoadBlocTexture dans la méthode LoadResource()
  du engine.cpp et y indiquer le type de bloc et le string nom de
  fichier.
- Dans info.cpp, ajouter le bloc à la méthode GenerateBlocInfos().
  Utiliser la syntaxe suivante: m_blocInfos[type] = new BlockInfo(
  type, string nom);
- Aller dans son.cpp dans la méthode GetFootType(BlockType type),
  ajouter à la switch le nouveau type de bloc et y assigner un son
  de pas.

  Note importante!
  
  Faire attention à la méthode AddTexture de texturearray.cpp parce qu'elle
  assigne les textures par itérations au lieu de se servir de l'enum. Il faut 
  donc charger les textures dans un certain ordre.

2. Contrôles

2.1. Pour ajouter un nouveau contrôle

- Aller dans engine.h et ajouter le contrôle sous la forme d'un champ private
- Aller dans la méthode LoadResource() du fichier engine.cpp
- Après le chargement des textures, initialiser le contrôle avec le constructeur
  acceptant des paramètres (ne pas utiliser le constructeur par défaut puisque ça
  enlèverait l'utilité d'avoir un contrôle visuel personnalisé...)
- Faire attention au champ Position. C'est en fait la position du contrôle par
  rapport à son conteneur
- Ajouter le contrôle au conteneur approprié
- Incrémenter la constante PNL_name_CONTROL_NBR de 1 dans le fichier interface.h
- Si tous les contrôles ont bien été ajoutés à leur conteneur, un seul appel
  de la méthode Render() de m_pnl_screen dessinera tous les contrôles

2.2. Ajouter des propriétés à une classe dérivée

- Aller dans le fichier de la classe en question.
- Ajouter le champ dans la section private
- Créer 2 méthodes, une pour accéder et une pour changer le champ (si nécessaire)
- Ajouter le champ à la surcharge d'opérateur de la classe.
- Ne pas oublier d'initialiser la variable dans le constructeur
- Si le champ doit faire partie du contructeur, le mettre à la suite des
  autres paramètre. Aussi ajouter l'initialisation au constructeur par défaut
  avec une valeur similaire à null
- Apporter les modifications nécessaires aux éléments déjà construits afin qu'ils
  acceptent ce nouveau paramètre.

2.3. Ajouter des propriétés à la classe de base

- Aller dans le fichier control.h(cpp) et ajouter le champ dans la section protected
- Créer 2 méthodes, une pour accéder et une pour changer le champ (si nécessaire)
- Ajouter le champ à la surcharge d'opérateur de la classe de base
- Ajouter le champ dans toutes les classes qui dérivent de celle-ci.
- Initiliser le champ dans le constructeur par défaut.
- Si le champ doit faire partie du contructeur, le mettre à la suite des
  autres paramètre. Aussi ajouter l'initialisation au constructeur par défaut
  avec une valeur similaire à null.
- Aller dans les classes dérivées de celle-ci et y inclure le nouveau paramètre dans
  les contructeurs (.h et .cpp) et l'initialiser dans l'appel du constructeur de la
  classe de base.
- Apporter les modifications nécessaires aux éléments déjà construits afin qu'ils
  acceptent ce nouveau paramètre.
