#pragma once

#include <iostream>

namespace SupaRL
{
  class ConsoleLogger
  {
    public:
      void debug(const char* message)
      {
#ifdef DEBUG
        std::cout << message << std::endl;
#endif
      }

      void debug(const char* message, int value)
      {
#ifdef DEBUG
        std::cout << message << value << std::endl;
#endif
      }

  };
}
