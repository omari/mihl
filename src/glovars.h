
#ifdef __MAIN__
#   define GLOBAL
#   define INIT(X) =X
#else
#   define GLOBAL extern
#   define INIT(X)
#endif

#ifdef __WINDAUBE__

#   define ERRNO WSAGetLastError()

#   define EADDRINUSE  WSAEADDRINUSE 
#   define EWOULDBLOCK WSAEWOULDBLOCK
#   define ECONNRESET  WSAECONNRESET

#   define SHUT_RDWR SD_BOTH
    typedef int socklen_t;

#   define vsnprintf _vsnprintf
#   define open _open

    typedef unsigned int uint32_t;

#   define MSG_NOSIGNAL 0

#else

#   define ERRNO errno
#   define SOCKET_ERROR -1

#   define Sleep(X) sleep(X/1000)

#   define closesocket close

#   define O_BINARY 0

#endif

#define STRNCPY( DST, SRC, LEN ) \
    strncpy( DST, SRC, LEN-1 ); \
    DST[LEN-1] = 0;

GLOBAL char mihl_bind_addr[32];     ///< HTTP bind address

GLOBAL int mihl_port;               ///< HTTP TCP port (80, 8080, etc.)

GLOBAL int mihl_maxnb_connexions;   ///< Max umer of allowed connexions

GLOBAL unsigned mihl_log_level
    INIT(MIHL_LOG_ERROR);           ///< MIHL_LOG_ERROR, etc.

GLOBAL SOCKET sockfd;               ///< TBD

struct connexion {
    int active;                         // Is this connexion active ?
    SOCKET sockfd;                      // Socked (as returned by accept)
    mihl_cnxinfo_t info;                // Exporteable information
    int keep_alive;                     // 'Keep-Alive:'
    int is_keep_alive;                  // 'Connection:'
    char *html_buffer;                  // HTML output buffer (mihl_add, mihl_send)
    int html_buffer_len;                // Current length
    int html_buffer_sz;                 // Length allocated (8K increment)
};
GLOBAL int nb_connexions;           ///< Number of current connexions
GLOBAL connexion_t *connexions;     ///< Description of the active connexions

GLOBAL int read_buffer_maxlen;      ///< TBD
GLOBAL char *read_buffer;           ///< TBD

typedef struct {
    char *tag;                      // Tag, such as "/" 
    pf_handle_get_t *pf_get;        // If not NULL, function to execute
    pf_handle_post_t *pf_post;      // If not NULL, function to execute
    char *filename;                 // If not NULL, filename to send
    char *content_type;             // Content-type, such as "image/jpeg" or "text/javascript"
    int close_connection;           // Should we close the connection after the operation ?
    void *param;                    // TBD
} mihl_handle_t;
GLOBAL int nb_handles;              ///< TBD
GLOBAL mihl_handle_t *handles;      ///< TBD

#define MIN(A,B) (((A)<(B))?(A):(B))
