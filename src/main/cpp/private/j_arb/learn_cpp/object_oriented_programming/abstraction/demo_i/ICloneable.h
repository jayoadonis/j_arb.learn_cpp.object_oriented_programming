#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_ICLONEABLE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_ICLONEABLE_H

namespace j_arb::learn_cpp::object_oriented_programming::abstraction::demo_i {

  //REM: interface (Marker and with default member function)
  template<typename T>
  class ICloneable 
  {
    protected: explicit ICloneable() = default;
    public: virtual ~ICloneable() = 0;
    
    //REM: default member function
    public: virtual [[nodiscard]]
      std::unique_ptr<ICloneable> clone() const &
    {
      return std::unique_ptr<ICloneable>(
        new T(static_cast<T const &>(*this))
      );
    }
  };

  template<typename T>
  inline ICloneable<T>::~ICloneable() = default;
}

#endif