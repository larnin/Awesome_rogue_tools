#ifndef DATAS_H
#define DATAS_H

#include "animation.h"
#include "transition.h"
#include "ressource.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

struct State
{
    std::vector<Transition> transitions;
    Animation animation;
    sf::Vector2f pos;
};

class Datas : private std::vector<State>
{
public:
    inline static Datas & instance() {return m_instance;}
    void saveDatas(const QString & filename);
    void loadDatas(const QString & filename);

    using std::vector<State>::empty;
    using std::vector<State>::size;
    using std::vector<State>::operator [];
    using std::vector<State>::begin;
    using std::vector<State>::end;
    using std::vector<State>::cbegin;
    using std::vector<State>::cend;
    using std::vector<State>::front;
    using std::vector<State>::back;
    using std::vector<State>::push_back;
    using std::vector<State>::emplace_back;
    using std::vector<State>::pop_back;
    using std::vector<State>::erase;

    inline Texture getTexture() const {return m_texture;}

private:
    Datas() = default;
    Datas(const Datas &) = default;
    Datas & operator =(const Datas &) = default;
    static Datas m_instance;
    Texture m_texture;
};

#endif // DATAS_H
