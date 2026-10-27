
#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H

#include <memory>
#include <cstring>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_three
{

  class Model
  {

    public: explicit Model() : Model(
      Model::get_default_id()
    ) {
    }

    public: explicit Model(std::unique_ptr<char[]> id_uptr)
      : m_id_uptr(std::move(id_uptr))
    {
      if (id_uptr == nullptr) {
        this->m_id_uptr = std::move(Model::get_default_id());
      }
    }

    //REM: [x] Copy ctor
    protected: explicit Model(Model const& other)
    {
      this->set_id(other.m_id_uptr.get());
    }

    protected: explicit Model(Model&& other) noexcept = default;

    //REM: [x] Copy assignment operator
    protected: Model& operator=(Model const& other)
    {
      if (this != &other) {
        this->set_id(other.m_id_uptr.get());
      }
      return *this;
    }

    protected: Model& operator=(Model&& other) noexcept = default;

    //REM: [x] Dtor
    public: virtual ~Model() = default;

    public: virtual Model* clone() const
    {
      return new Model(*this);
    }

    public: virtual void move_into(Model& dest) noexcept
    {
      dest = std::move(*this);
    }

    public: inline void set_id(std::unique_ptr<char[]> id_uptr)
    {
      this->m_id_uptr = std::move(id_uptr);
    }

    public: inline void set_id(std::string_view id)
    {
      this->set_id(id.data());
    }

    public: virtual void set_id(char const* id_ptr)
    {
      if (id_ptr != nullptr) {

        std::size_t len = std::strlen(id_ptr);
        if (len <= 0u)
          return;

        if (std::string_view(id_ptr) == this->get_id())
          return;

        std::size_t size = len + 1;
        std::unique_ptr<char[]> new_buffer_uptr
          = std::make_unique<char[]>(size);

        std::memcpy(new_buffer_uptr.get(), id_ptr, size);
        this->m_id_uptr = std::move(new_buffer_uptr);
      }
      else if (std::string_view(this->get_id())
        != Model::get_default_id().get())
      {
        this->m_id_uptr = std::move(Model::get_default_id());
      }
    }

    public: virtual char const* get_id() const
    {
      return this->m_id_uptr.get();
    }

    public: static std::unique_ptr<char[]> get_default_id()
    {
      return std::unique_ptr<char[]>(
        new char[4] {'N', '/', 'a', '\0'}
      );
    }

    private: std::unique_ptr<char[]> m_id_uptr = nullptr;
  };
}


#endif