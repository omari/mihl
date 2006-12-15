
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mihl.h"

int
http_root( connexion_t *cnx, char const *tag, char const *host, void *param )
{
    mihl_add( cnx, "<!DOCTYPE html PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN'>" );
    mihl_add( cnx, "<html>" );
    mihl_add( cnx, "<head>" );
    mihl_add( cnx, "  <meta content='text/html; charset=ISO-8859-1'" );
    mihl_add( cnx, " http-equiv='content-type'>" );
    mihl_add( cnx, "  <title>MIHL Test page</title>" );
    mihl_add( cnx, "</head>" );
    mihl_add( cnx, "<body>" );
    mihl_add( cnx, "This is a test HTML page for MIHL.<br>" );
    mihl_add( cnx, "<br>" );
    mihl_add( cnx, "Here is a JPEG Image:<br>" );
    mihl_add( cnx, "<img style='width: 70px; height: 72px;' alt=''" );
    mihl_add( cnx, " src='image.jpg'><br>" );
    mihl_add( cnx, "<br>" );
    mihl_add( cnx, "<form method='post' action='toto1'>" );
    mihl_add( cnx, "<table style='text-align: left; width: 100%;' border='1' cellpadding='2'" );
    mihl_add( cnx, " cellspacing='2'>" );
    mihl_add( cnx, "  <tbody>" );
    mihl_add( cnx, "    <tr>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>Name<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>" );
    mihl_add( cnx, "       <input name='myname1' type='text'>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "    </tr>" );
    mihl_add( cnx, "    <tr>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>City<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>" );
    mihl_add( cnx, "      <input name='myname2' type='text'>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "    </tr>" );
    mihl_add( cnx, "    <tr>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>Zip<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>" );
    mihl_add( cnx, "      <input name='myname3' type='text'>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "    </tr>" );
    mihl_add( cnx, "  </tbody>" );
    mihl_add( cnx, "</table>" );
    mihl_add( cnx, "Press <input type='submit' value='here'> to submit your query." );
    mihl_add( cnx, "</form>" );
    mihl_add( cnx, "<br>" );
    mihl_add( cnx, "</body>" );
    mihl_add( cnx, "</html>" );
    mihl_send( cnx,
		"Content-type: text/html\r\n" );
    return 0;
}                               // http_root


// myname1=AAA&myname2=BBB&myname3=CCC]
// myname1=A+++B+C&myname2=HELLO&myname3=BONJOUR%2BMONDE]
int
http_root_post( connexion_t *cnx, char const *tag, char const *host,
    int nb_variables, char **vars_names, char **vars_values,
    void *param )
{
    return 0;
}                               // http_root_post


int
main( int argc, char *argv[] )
{
    mihl_init( NULL, 8080, 8 );

    mihl_handle_get( "/", http_root, NULL );
    mihl_handle_file( "/image.jpg", "image.jpg", "image/jpeg", 0 );
    mihl_handle_post( "/toto1", http_root_post, NULL );

    for (;;) {
        int status = mihl_server( );
        if ( status == -2 )
            break;
    }
    
    return 0;
}                               // main
