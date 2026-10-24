
#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_APP_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_APP_H

#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include <string>

namespace j_arb::learn_cpp::object_oriented_programming {

  class App {

  public:
    OBJECT_ORIENTED_PROGRAMMING_API App();
    OBJECT_ORIENTED_PROGRAMMING_API virtual ~App();
    OBJECT_ORIENTED_PROGRAMMING_API virtual void set_title(
      std::string const & title
    );
    OBJECT_ORIENTED_PROGRAMMING_API virtual void set_title(
      char const * title
    );
    OBJECT_ORIENTED_PROGRAMMING_API virtual std::string get_title() const;

  private:
    std::string m_title;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/App.cpp"
#endif

#endif