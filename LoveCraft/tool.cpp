#include "tool.h"
#include "define.h"
#include <cassert>

void Tool::CheckTypes()
{
    // V�rifier que les types d�finis dans define.h ont vraiment la bonne taille
    // Cette v�rification se r�v�lera utile si on change de compilateur/architecture
    // et qu'on oublie de v�rifier la taille des types
    assert(sizeof(uint8) == 1);
    assert(sizeof(uint16) == 2);
    assert(sizeof(uint32) == 4);
    assert(sizeof(uint64) == 8);
    assert(sizeof(int8) == 1);
    assert(sizeof(int16) == 2);
    assert(sizeof(int32) == 4);
    assert(sizeof(int64) == 8);
}
