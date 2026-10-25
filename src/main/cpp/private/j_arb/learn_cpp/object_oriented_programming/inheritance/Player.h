#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_INHERITANCE_PLAYER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_INHERITANCE_PLAYER_H

#include <cstring>
#include <utility>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Model.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"

namespace j_arb::learn_cpp::object_oriented_programming::inheritance 
{

  class Player : public encapsulation::Model
  {

    public: Player() : Player("N/a")
    {

    }

    public: Player(char const * name) : 
      encapsulation::Model(
        Player::gen_player_id(), 
        encapsulation::Type::ACTOR
      ),
      m_name(new char[std::strlen(name)+1])
    {
      std::strcpy(this->m_name, name);
    }

    public: virtual ~Player() override
    {
      delete[] this->m_name;
    }

    public: virtual Player* clone() const override 
    {
      return new Player(*this);  
    }

    protected: Player(Player const & other) :
      encapsulation::Model(other)
    {
      std::size_t len = std::strlen(other.m_name);
      this->m_name = new char[len+1];
      std::strcpy(this->m_name, other.m_name);
    }

    protected: Player& operator=(Player const & other) 
    {
      if(this != &other) {
        encapsulation::Model::operator=(other);
        this->set_name(other.m_name);
      }
      return *this;
    }

    protected: Player(Player && other) noexcept :
      encapsulation::Model(std::move(other)),
      m_name(other.m_name)
    {
      other.m_name = nullptr;
    }

    protected: Player& operator=(Player && other) noexcept 
    {
      if(this != &other) {
        encapsulation::Model::operator=(std::move(other));
        
        delete[] this->m_name;
        this->m_name = other.m_name;
        other.m_name = nullptr;
      }
      return *this;
    }

    protected: inline static char const * gen_player_id() 
    {
      //REM: TODO
      return "player_123";
    }

    public: virtual void set_name(char const * name) 
    {
      if(name == nullptr) return;

      if(std::strcmp(this->m_name, name) == 0) return;

      std::size_t len = std::strlen(name);
      char * new_buffer = new char[len+1];
      std::strcpy(this->m_name, name);
      
      delete[] this->m_name;
      this->m_name = new_buffer;
    }

    public: virtual char const * get_name() 
    {
      return this->m_name;
    }

    private: char * m_name;
  };
}

#endif