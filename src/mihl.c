
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>

#ifdef __WINDAUBE__
#   define _WIN32_WINNT 0x0500
#   include <winsock2.h>
#   include <Mswsock.h>
#   include <windows.h>
#   include <io.h>
#else
#   include <unistd.h>
#   include <sys/select.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   include <netdb.h>
#endif

#define BUILD_DLL_MIHL
#include "mihl.h"

#define __MAIN__
#include "glovars.h"

#include "tcp_utils.h"


static void
add_new_connexion( SOCKET sockfd, struct sockaddr_in *client_addr )
{

    // Find a new slot to store the new active connexion
    if ( nb_connexions == MAXNB_CONNEXIONS ) {
        assert( 0 );
    }
    connexion_t *cnx = NULL;
    for ( int ncnx = 0; ncnx < MAXNB_CONNEXIONS; ncnx++ ) {
        cnx = &connexions[ncnx];
        if ( !cnx->active )
            break;
    }                           // for (connexions)
    assert( cnx != NULL );
    nb_connexions++;

    cnx->active = 1;    
    cnx->sockfd = sockfd;
    memmove( &cnx->client_addr, client_addr, sizeof(struct sockaddr_in) );
    cnx->time_started = time( NULL );
    cnx->time_last_data = cnx->time_started;
    cnx->host = NULL;                     // 'Host:'
    cnx->user_agent = NULL;               // 'User-Agent:'
    cnx->keep_alive = 300;                // Default timeout
    cnx->html_buffer_len = 0;                               // Current length
    cnx->html_buffer_sz = 8192;                             // Length allocated (8K increment)
    cnx->html_buffer = (char*)malloc(cnx->html_buffer_sz);  // HTML output buffer (mihl_add, mihl_send)
    strcpy( cnx->html_buffer, "" );

    printf( "\nAccepted a connexion from %s, socket=%d\n",
		  inet_ntoa( cnx->client_addr.sin_addr ), sockfd );
    fflush( stdout );

}                               // add_new_connexion


static void
delete_connexion( connexion_t *cnx )
{
    printf( "Delete connexion for socket %d\n", cnx->sockfd );
    fflush( stdout );
	shutdown( cnx->sockfd, SHUT_RDWR );	    // Close the connection
    closesocket( cnx->sockfd );
    cnx->active = 0;
    nb_connexions--;
}                               // delete_connexion


static int
bind_and_listen( )
{

#ifdef __WINDAUBE__
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if ( iResult != NO_ERROR ) {
        printf( "Error at WSAStartup()\n" );
        exit( -1 );
    }
#endif

    // Create sockets for the telnet and http connections
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sockfd < 0 ) {
		printf( "Unable to open a socket!\n" );
        exit( -1 );
    }

    // Set the option SO_REUSEADDR
    const int flag = 1;
    if ( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&flag, sizeof( flag ) ) < 0 ) {
		printf( "Unable to setsockopt a socket!\n" );
        exit( -1 );
    }

    // Non blocking Socket
#ifdef __WINDAUBE__
    unsigned long cmd = 1;
    if ( ioctlsocket( sockfd, FIONBIO, &cmd ) != 0 ) {
		printf( "Unable to fcntl a socket:FIONBIO !\n" );
        fflush( stdout );
        exit( -1 );
    }
#else
    fcntl( sockfd, F_SETFL, fcntl( sockfd, F_GETFL, 0 ) | O_NONBLOCK );
#endif

    // Bind the socket 
	struct sockaddr_in server_addr;
	memset( &server_addr, 0, sizeof( server_addr ) );
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	server_addr.sin_port = htons( mihl_port );
	int status = bind( sockfd, ( struct sockaddr * ) &server_addr, sizeof( struct sockaddr_in ) );
    if ( (status == SOCKET_ERROR) && (ERRNO == EADDRINUSE) ) {
		printf( "%s %d\n\tUnable to bind, port %d is already in use\n",
		   __FILE__, __LINE__, 
		   mihl_port );
		if ( closesocket( sockfd ) == -1 )
			printf( "Error %d while closing socket %d\n", 
				errno, sockfd );
		return -1;
	}
	if ( status < 0 ) {
		printf( "%s %d\nUnable to bind - port %d - status=%d errno=%s\n",
			__FILE__, __LINE__,
			mihl_port,
			status, strerror( errno ) );
		if ( closesocket( sockfd ) == -1 )
			printf( "Error %d while closing socket %d\n", 
				errno, sockfd );
		return -1;
	}

	return listen( sockfd, 8 );
    
}                               // bind_and_listen


static int
page_not_found( connexion_t *cnx, char const *tag, void *param )
{
    mihl_add(  cnx, "<html>" );
    mihl_add(  cnx, "<head>" );
    mihl_add(  cnx, "</head>" );
    mihl_add(  cnx, "<br>" );
    mihl_add(  cnx, " THIS PAGE IS NOT FOUND " );
    mihl_add(  cnx, "<br>" );
    mihl_add(  cnx, "</body>" );
    mihl_add(  cnx, "</html>" );
    mihl_send( cnx,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n" );
    return 0;
}                               // page_not_found


int
mihl_init( int port )
{

    mihl_port = port;

    nb_connexions = 0;          // Number of current connexions
    for ( int ncnx = 0; ncnx < MAXNB_CONNEXIONS; ncnx++ ) {
        connexion_t *cnx = &connexions[ncnx];
        cnx->active = 0;
    }                           // for (connexions)

    read_buffer_maxlen = 64*1024;
    read_buffer = (char *)malloc( read_buffer_maxlen );

    nb_handles = 0;                 // TBD
    handles = NULL;                 // TBD
    mihl_handle_get( NULL, page_not_found, NULL );

    bind_and_listen( );

    return 0;

}                               // mihl_init


static int
send_file( connexion_t *cnx, char *tag, char *filename, 
    char *content_type, int close_connection )
{

    char *file;
    int length;
    if ( read_file( filename, &file, &length ) == -1 ) {
        return -1;
    }

    //  Header to send
    time_t now = time( NULL );
    struct tm *tm = gmtime( &now );
    char date[80];
    strftime( date, sizeof(date)-1, "%c", tm );
	char msg1[1024];
	int len = sprintf( msg1, 
		"HTTP/1.1 200 OK\r\n"
        "Accept-Ranges: bytes\r\n"
		"Content-Length: %d\r\n"
        "Date: %s\r\n"
        "Content-Type: %s\r\n"
        "Connection: %s\r\n"
		"\r\n",
			length, date, content_type,
            (close_connection) ? "close" : "keep-alive" );
	int dcount = tcp_write( cnx->sockfd, msg1, len );
    if ( dcount == -1 ) {
        printf( "\n*** %s %d: OOPS - %m!!!!!\n", __FILE__, __LINE__ );
        fflush( stdout );
        return -1;
    }

    char *p = file;
    int rem = length;
    for (;;) {
        int count = MIN( rem, 16384 ); 
	    dcount = tcp_write( cnx->sockfd, (const char *)p, count );
        if ( dcount == -1 ) {
            printf( "\n*** %s %d: - %m!!!!!\n", __FILE__, __LINE__ );
            fflush( stdout );
            return -1;
        }
        rem -= count;
        if ( rem == 0 )
            break;
        p += count;
    }

    free( file );
    return 0;
}                               // send_file


static int
search_for_handle( connexion_t *cnx, uint32_t type, char *tag,
    int nb_variables, char **vars_names, char **vars_values )
{
    mihl_handle_t *handle_nfound = NULL;
    for ( int n = 0; n < nb_handles; n++ ) {
        mihl_handle_t *handle = &handles[n];
        if ( !handle->tag )
            handle_nfound = handle;
        if ( handle->tag && !strcmp( tag, handle->tag ) ) {
            if ( (type == 'GET') && handle->pf_get )
                return handle->pf_get( cnx, tag, handle->param );
            if ( (type == 'POST') && handle->pf_post )
                return handle->pf_post( cnx, tag, nb_variables, vars_names, vars_values, handle->param );
            return send_file( cnx, tag, handle->filename, handle->content_type, handle->close_connection );
        }
    }
    if ( handle_nfound )
        return handle_nfound->pf_get( cnx, tag, handle_nfound->param );
    return 0;
}                               // search_for_handle


static int
manage_new_connexions( )
{
    for (;;) {
	    socklen_t client_addr_len = sizeof( struct sockaddr_in );
	    struct sockaddr_in client_addr;
	    SOCKET sockfd_accept = accept( sockfd,
	       (struct sockaddr *)&client_addr, &client_addr_len );

	    if ( sockfd_accept == -1 ) {
            if ( (ERRNO == EAGAIN) || (ERRNO == EWOULDBLOCK) )
                return 0;
		    if ( errno == EINTR ) {	// A signal has been applied
                printf( "!!! %d !!!\n", __LINE__ );
//              Sleep( 500 );
			    continue;
            }
		    printf( "Socket non accepted - errno=%d\n", ERRNO );
		    if ( closesocket( sockfd ) == -1 ) {
			    printf( "Error %d while closing socket %d\n", errno, sockfd );
            }
            fflush( stdout );
		    exit( -1 );
        }                       // if

        add_new_connexion( sockfd_accept, &client_addr );
        return 1;
	}                           // for (;;)
}                               // manage_new_connexions


static int
got_data_for_active_connexion( connexion_t *cnx )
{

    int len = tcp_read( cnx->sockfd, read_buffer, read_buffer_maxlen );
    printf( "\n%d:[%s]\n", cnx->sockfd, read_buffer );
    fflush( stdout );

    if ( len == 0 ) {
        cnx->time_last_data = 0;    // Force closing the connection on manage_timedout_connexions()
        return -1;
    }
    cnx->time_last_data = time( NULL );

    /*
     */
    int version, subversion;
    char tag[1024];
    strcpy( tag, "" );
    if ( (len >= 14) && (!strncmp( read_buffer, "GET", 3 )) ) {
        char _tag[1024];
		int status = sscanf( read_buffer, "GET %s HTTP/%d.%d",
			_tag, &version, &subversion );
        if ( status == 3 )
            strncpy( tag, _tag, sizeof(tag)-1 );
    }
       
    /*
     *  Decode Key/Value pairs
     */
    int nb_options;
    char *options_names[50], *options_values[50];
    int nb_variables;
    char *vars_names[50], *vars_values[50];
    decode_keys_values( cnx, read_buffer, 
        &nb_options, options_names, options_values, 50,
        &nb_variables, vars_names, vars_values, 50 );

    search_for_handle( cnx, 'GET', tag, 0, NULL, NULL );

    // Clean-up keys/values pairs
    for ( int n = 0; n < nb_options; n++ ) {
        free( options_names[n] );
        free( options_values[n] );
    }

    return 1;
}                               // got_data_for_active_connexion


static int
manage_existent_connexions( )
{

    if ( nb_connexions == 0 )
        return 0;

    SOCKET last_sockfd = -1;
	fd_set ready;
	FD_ZERO( &ready );
    for ( int ncnx = 0; ncnx < MAXNB_CONNEXIONS; ncnx++ ) {
        connexion_t *cnx = &connexions[ncnx];
        if ( !cnx->active )
            continue;
		FD_SET( cnx->sockfd, &ready );
        if ( cnx->sockfd > last_sockfd )
            last_sockfd = cnx->sockfd;
    }

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int status = select( (int)last_sockfd+1, &ready, NULL, NULL, &tv );
#ifdef __WINDAUBE__
    if ( status == 0 )
        return 0;
#else
    if ( status == 0 )
        return 0;
#endif
	assert( status != -1 );

    for ( int ncnx = 0; ncnx < MAXNB_CONNEXIONS; ncnx++ ) {
        connexion_t *cnx = &connexions[ncnx];
        if ( !cnx->active )
            continue;
	    if ( FD_ISSET( cnx->sockfd, &ready ) ) {
            got_data_for_active_connexion( cnx );
	    }
    }                           // for (connexions)
    
    return 1;

}                               // manage_existent_connexions


static int
manage_timedout_connexions( )
{
    time_t now = time( NULL );
    for ( int ncnx = 0; ncnx < MAXNB_CONNEXIONS; ncnx++ ) {
        connexion_t *cnx = &connexions[ncnx];
        if ( !cnx->active )
            continue;
        int t = (int)difftime( now, cnx->time_last_data );
        if ( t >= cnx->keep_alive ) 
            delete_connexion( cnx );
    }                           // for (connexions)

    return 0;
}                               // manage_timedout_connexions


int
mihl_handle_get( char const *tag, pf_handle_get_t *pf, void *param )
{
    if ( handles == NULL ) {
        handles = (mihl_handle_t *)malloc( sizeof(mihl_handle_t) );
    }
    else {
        handles = (mihl_handle_t *)realloc( handles, sizeof(mihl_handle_t) * (nb_handles+1) );
    }
    mihl_handle_t *handle = &handles[nb_handles++];
    if ( tag )
        handle->tag = strdup( tag );
    else
        handle->tag = NULL;
    handle->pf_get = pf;
    handle->pf_post = NULL;
    handle->filename = NULL;
    handle->content_type = NULL;
    handle->close_connection = 0;
    return nb_handles;
}                               // mihl_handle_get


int
mihl_handle_post( char const *tag, pf_handle_post_t *pf, void *param )
{
    if ( tag == NULL )
        return -1;
    if ( handles == NULL ) {
        handles = (mihl_handle_t *)malloc( sizeof(mihl_handle_t) );
    }
    else {
        handles = (mihl_handle_t *)realloc( handles, sizeof(mihl_handle_t) * (nb_handles+1) );
    }
    mihl_handle_t *handle = &handles[nb_handles++];
    handle->tag = strdup( tag );
    handle->pf_get = NULL;
    handle->pf_post = pf;
    handle->filename = NULL;
    handle->content_type = NULL;
    handle->close_connection = 0;
    return nb_handles;
}                               // mihl_handle_post


int
mihl_handle_file( char const *tag, char const *filename, 
    char const *content_type, int close_connection )
{
    if ( handles == NULL ) {
        handles = (mihl_handle_t *)malloc( sizeof(mihl_handle_t) );
    }
    else {
        handles = (mihl_handle_t *)realloc( handles, sizeof(mihl_handle_t) * (nb_handles+1) );
    }
    mihl_handle_t *handle = &handles[nb_handles++];
    handle->tag = strdup( tag );
    handle->pf_get = NULL;
    handle->pf_post = NULL;
    handle->filename = strdup( filename );
    handle->content_type = strdup( content_type );
    handle->close_connection = close_connection;
    return nb_handles;
}                               // mihl_handle_file


int
mihl_server( )
{
    manage_new_connexions( );
    manage_existent_connexions( );
    manage_timedout_connexions( );
    return 1;
}                               // mihl_server
