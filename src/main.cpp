#include "core/application/application.hpp"

#include <iostream>
#include <exception> 


auto main() -> int {

  try {
    Core::Application application(
      800,
      600,
      "GTC"
    );

    return application.run();
  }
  catch (const std::exception& error)
  {
    std::cerr
        << "Fatal error: "
        << error.what()
        << '\n';

    return 1;
  }
  catch(...)
  {
    std::cerr
      << "Fatal error: unknown exception\n";

    return 1;
  }
  return 0;
}

