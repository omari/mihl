#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "mihl.h"

#include "../example_utils.h"

int
http_root( connexion_t *cnx, char const *tag, char const *host, void *param )
{
    mihl_add( cnx, "<html>" );
    mihl_add( cnx, "<body>" );
    mihl_add( cnx, "This is a test HTML page for MIHL.<br>" );
    mihl_add( cnx, "<br>Here is a JPEG Image:<br>" );
    mihl_add( cnx, "<img style='width: 70px; height: 72px;' alt='' src='image.jpg'><br><br>" );
    mihl_add( cnx, "<a href='nextpage.html'>Next Page<a>" );
    mihl_add( cnx, "</body>" );
    mihl_add( cnx, "</html>" );
    mihl_send( cnx,
		"Content-type: text/html\r\n" );
    return 0;
}

int
http_nextpage( connexion_t *cnx, char const *tag, char const *host, void *param )
{
    mihl_add( cnx, "<html>" );
    mihl_add( cnx, "<body>" );
    mihl_add( cnx, "This is another page...<br>" );
    mihl_add( cnx, "<a href='/'>Previous Page<a>" );
    mihl_add( cnx, "</body>" );
    mihl_add( cnx, "</html>" );
    mihl_send( cnx,
		"Content-type: text/html\r\n" );
    return 0;
}                               // http_nextpage


int
main( int argc, char *argv[] )
{
    help( );

    int vlog = MIHL_LOG_ERROR | MIHL_LOG_WARNING | MIHL_LOG_INFO | MIHL_LOG_INFO_VERBOSE;
    mihl_set_log_level( vlog );
    mihl_init( NULL, 8080, 8 );

    mihl_handle_get( "/", http_root, NULL );
    mihl_handle_file( "/image.jpg", "image.jpg", "image/jpeg", 0 );
    mihl_handle_get( "/nextpage.html", http_nextpage, NULL );

    for (;;) {
        int status = mihl_server( );
        if ( status == -2 )
            break;
        if ( peek_key( &vlog ) )
            break;
    }
    
    return 0;
}
