

#ifdef __WINDAUBE__
#   ifdef BUILD_DLL_MIHL
#       define PUBLIC __declspec(dllexport)
#   else
#       define PUBLIC __declspec(dllimport)
#   endif
#else
#   define PUBLIC
#endif

#ifdef __WINDAUBE__
#else
    typedef int SOCKET;
#endif

typedef struct connexion connexion_t;

typedef int (pf_handle_get_t)(connexion_t *, char const *, void *);

typedef int (pf_handle_post_t)(connexion_t *, char const *, int, char **, char **, void *);

PUBLIC int mihl_init( int port );

PUBLIC int mihl_server( );

PUBLIC int mihl_add( connexion_t *cnx, char const *fmt, ... );

PUBLIC int mihl_send( connexion_t *cnx, char const *fmt_header, ... );

PUBLIC int mihl_handle_get( char const *tag, pf_handle_get_t *pf, void *param );

PUBLIC int mihl_handle_post( char const *tag, pf_handle_post_t *pf, void *param );

PUBLIC int mihl_handle_file( char const *tag, char const *filename, 
    char const *content_type, int close_connection );

#undef PUBLIC
