#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <string>
#include "define.h"

class BlockInfo
{
    public:
    BlockInfo(T type, const std::string& nom);
    ~BlockInfo();

    T GetType() const;

    void SetDurabilite(int durabilite);
    int GetDurabilite() const;

    void Afficher() const;

    private:
    T m_type;
    std::string m_nom;
    int m_durabilite;

};

#endif // BLOCKINFO_H__
