#if defined(WIN32) || defined(_WIN32)
  #ifdef APT_COMPILER_MSVC
    #pragma warning(disable: 4100)  // unreferenced formal parameter
    #pragma warning(disable: 4127)  // conditional expression is constant
    #pragma warning(disable: 4189)  // local variable is initialized but not referenced
    #pragma warning(disable: 4244)  // conversion from '' to '', possible loss of data
    #pragma warning(disable: 4250)  // inherits '' via dominance
    #pragma warning(disable: 4251)  // class '' needs to have dll - interface to be used by clients of class ''
    #pragma warning(disable: 4267)  // conversion from '' to '', possible loss of data
    #pragma warning(disable: 4275)  // non dll - interface class '' used as base for dll - interface class ''
    #pragma warning(disable: 4312)  // conversion from '' to '' of greater size
    #pragma warning(disable: 4456)  // declaration of '' hides previous local declaration
  #endif
  #include "win/cs-config.h"
#else defined(__linux__)
  #include "unix/cs-config.h"
#else
  #error Unrecognized platform
#endif
