
#ifdef __WINDAUBE__
#   ifdef BUILD_DLL_GETOPT
#       define PUBLIC __declspec(dllexport)
#   else
#       define PUBLIC __declspec(dllimport)
#   endif
#else
#   define PUBLIC
#endif

#include <windows.h>
#include <time.h>

PUBLIC extern int optind;
// extern int opterr;
PUBLIC extern char *optarg;

PUBLIC int getopt(int argc, char *argv[], char *optstring);

#undef PUBLIC
