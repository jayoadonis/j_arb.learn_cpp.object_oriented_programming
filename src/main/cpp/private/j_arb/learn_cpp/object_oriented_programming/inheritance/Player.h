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

    public: explicit Player() : Player(nullptr)
    {

    }

    public: explicit Player(char const* name) :
      encapsulation::Model(
        Player::gen_player_id(),
        encapsulation::Type::ACTOR
      ),
      m_name_ptr(new char[std::strlen(name ? name : "N/a") + 1])
    {
      std::strcpy(this->m_name_ptr, name ? name : "N/a");
    }

    public: virtual ~Player() override
    {
      delete[] this->m_name_ptr;
    }

    public: virtual Player* clone() const override
    {
      return new Player(*this);
    }

    public: virtual void move_into(
      Model& dest
    ) noexcept override {

      if (Player* derived_dest = dynamic_cast<Player*>(&dest))
      {
        *derived_dest = std::move(*this);
      }
      else
      {
        encapsulation::Model::move_into(dest);
      }
    }

    protected: explicit Player(Player const& other) :
      encapsulation::Model(other)
    {
      std::size_t len = std::strlen(other.m_name_ptr);
      this->m_name_ptr = new char[len + 1];
      std::strcpy(this->m_name_ptr, other.m_name_ptr);
    }

    protected: Player& operator=(Model const& other) override
    {
      if (this == &other) return *this;


    //   //REM: Dev/Debug mode still at Runtime (MEDIUM FAST)
    //   //REM: Only Allow exactly 'Player'
    //   assert(typeid(*this) == typeid(other) && "::: Expected to be 'Player' object.");
    //   //REM: or
    // #ifndef NDEBUG
    //   if (typeid(*this) != typeid(other))
    //   {
    //     std::cerr
    //       << "::: Excepted to be: '" << *this << "', actual: '" << other << "'\n";
    //     // std::abort(); //REM: crush
    //     // throw std::bad_cast(); //REM: Loud
    //     return *this; //REM: Silent
    //   }
    // #endif
    //   Player const* obj = static_cast<Player const*>(&other);

      //REM: ---

      //REM: Or both debug or release mode still at runtime (SLOWER)
      //REM: Allows 'Player' OR any class derived from 'Player'
      Player const* obj = dynamic_cast<Player const*>(&other);

      if (obj == nullptr)
      {
      #ifndef NDEBUG
        std::cerr << "::: Mismatch: Expected a Player or Player-derived type, actual: '" << other << "'\n";
      #endif
        // throw std::bad_cast(); //REM: Loud
        return *this; //REM: Silent
      }

      encapsulation::Model::operator=(other);
      this->set_name(obj->m_name_ptr);

      return *this;
    }

    protected: explicit Player(Player&& other) noexcept :
      encapsulation::Model(std::move(other)),
      m_name_ptr(other.m_name_ptr)
    {
      other.m_name_ptr = nullptr;
    }

    protected: Player& operator=(Player&& other) noexcept
    {
      if (this != &other) {
        encapsulation::Model::operator=(std::move(other));

        delete[] this->m_name_ptr;
        this->m_name_ptr = other.m_name_ptr;
        other.m_name_ptr = nullptr;
      }
      return *this;
    }

    protected: inline static char const* gen_player_id()
    {
      //REM: TODO
      return "player_123";
    }

    public: virtual void set_name(char const* name)
    {
      if (name == nullptr) return;

      if (std::strcmp(this->m_name_ptr, name) == 0) return;

      std::size_t len = std::strlen(name);
      char* new_buffer = new char[len + 1];
      std::strcpy(this->m_name_ptr, name);

      delete[] this->m_name_ptr;
      this->m_name_ptr = new_buffer;
    }

    public: virtual char const* get_name()
    {
      return this->m_name_ptr;
    }

    private: char* m_name_ptr;
  };
}

#endif