#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mihl.h"

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
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n" );
    return 0;
}

int
http_nextpage( connexion_t *cnx, char const *tag, char const *host, void *param )
{
    mihl_add( cnx, "<html>" );
    mihl_add( cnx, "<body>" );
    mihl_add( cnx, "This is another page...<br>" );
    mihl_add( cnx, "<a href=''>Previous Page<a>" );
    mihl_add( cnx, "</body>" );
    mihl_add( cnx, "</html>" );
    mihl_send( cnx,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n" );
    return 0;
}

int
main( int argc, char *argv[] )
{
    mihl_init( 8080 );

    mihl_handle_get( "/", http_root, NULL );
    mihl_handle_file( "/image.jpg", "image.jpg", "image/jpeg", 0 );
    mihl_handle_get( "/nextpage.html", http_nextpage, NULL );

    for (;;) {
        int status = mihl_server( );
        if ( status == -2 )
            break;
    }
    
    return 0;
}
