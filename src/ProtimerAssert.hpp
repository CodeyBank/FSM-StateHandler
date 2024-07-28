#include <iostream>
#include <stdexcept>

// Custom assert macro
#define PROTIMER_ASSERT(cond, msg)                                             \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::cerr << "Assertion failed: " << #cond << "\n"                       \
                << "Message: " << msg << "\n"                                  \
                << "File: " << __FILE__ << "\n"                                \
                << "Line: " << __LINE__ << "\n";                               \
      std::terminate();                                                        \
    }                                                                          \
  } while (0)
