#ifndef DATAS_H
#define DATAS_H

#include "animation.h"
#include "transition.h"
#include <vector>

struct State
{
    std::vector<Transition> transitions;
    Animation animation;
};

class Datas : private std::vector<State>
{
public:
    inline static Datas & instance() {return m_instance;}
    void save(const std::string & filename);
    void load(const std::string & filename);

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

private:
    Datas() = default;
    Datas(const Datas &) = default;
    Datas & operator =(const Datas &) = default;
    static Datas m_instance;
};

#endif // DATAS_H
