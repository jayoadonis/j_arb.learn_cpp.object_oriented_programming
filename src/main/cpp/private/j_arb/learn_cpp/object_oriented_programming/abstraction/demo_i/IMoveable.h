#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_IMOVEABLE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_IMOVEABLE_H

namespace j_arb::learn_cpp::object_oriented_programming::abstraction::demo_i {

  //REM: interface (Marker and with default member function)
  template<typename T>
  class IMoveable
  {
    protected: explicit IMoveable() = default;
    public: virtual ~IMoveable() = 0;

    //REM: default member function
    public: virtual [[nodiscard]]
      std::unique_ptr<IMoveable> move() && noexcept
    {
      return std::unique_ptr<IMoveable>(
        new T(std::move(static_cast<T&>(*this)))
      );
    }
  };

  template<typename T>
  inline IMoveable<T>::~IMoveable() = default;
}

#endif