/**
 * @file example1.c
 *
 */

/**
 * @mainpage MIHL: Minimal Httpd Library - Example #1
 * 
 * HTTP embedded server library \n
 * Copyright (C) 2006-2007  Olivier Singla \n
 * http://mihl.sourceforge.net/ \n\n
 *
 * Simple use of GET.
 * Display a page with a JPEG image and a link to a 2nd page. 
 * The 2nd page contains a link to the 1st page.
 * There is also a link to a non existent page.
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "mihl.h"

#include "example_utils.h"

/**
 * GET Handler for the URL: /
 * 
 * @param cnx opaque context structure as returned by mihl_init()
 * @param tag TBD
 * @param host TBD
 * @param param TBD
 * @return 0
 */
int http_root( mihl_cnx_t *cnx, char const *tag, char const *host, void *param ) {
    mihl_add( cnx, "<html>" );
    mihl_add( cnx, "<body>" );
    mihl_add( cnx, "This is a test HTML page for MIHL.<br>" );
    mihl_add( cnx, "<br>Here is a JPEG Image:<br>" );
    mihl_add( cnx, "<img style='width: 70px; height: 72px;' alt='' src='image.jpg'><br><br>" );
    mihl_add( cnx, "host= [%s]<br><br>", host );
    mihl_add( cnx, "<a href='nextpage.html'>Next Page<a><br><br>" );
    mihl_add( cnx, "<a href='unknown.html'>Non-Existent Page<a><br><br>" );
    mihl_add( cnx, "</body>" );
    mihl_add( cnx, "</html>" );
    mihl_send( cnx,
		"Content-type: text/html\r\n" );
    return 0;
}

/**
 * TBD
 * 
 * @param cnx opaque context structure as returned by mihl_init()
 * @param tag URL of the non existent page
 * @param host TBD
 * @param param TBD
 * @return TBD
 */
static int http_page_not_found( mihl_cnx_t *cnx, char const *tag, char const *host, void *param ) {
    mihl_add(  cnx, "<html>" );
    mihl_add(  cnx, "<head>" );
    mihl_add(  cnx, "</head>" );
    mihl_add(  cnx, "<br>" );
    mihl_add(  cnx, " THIS PAGE IS NOT FOUND " );
    mihl_add(  cnx, "<br>" );
    mihl_add(  cnx, "The tag is [%s]", tag );
    mihl_add(  cnx, "<br>" );
    mihl_add(  cnx, "</body>" );
    mihl_add(  cnx, "</html>" );
    return mihl_send( cnx,
		"Content-type: text/html\r\n" );
}                               // http_page_not_found

/**
 * TBD
 * 
 * @param cnx opaque context structure as returned by mihl_init()
 * @param tag TBD
 * @param host TBD
 * @param param TBD
 * @return 0
 */
int http_nextpage( mihl_cnx_t *cnx, char const *tag, char const *host, void *param ) {
    mihl_handle_get( mihl_get_ctx(cnx), NULL, http_page_not_found, NULL );
    mihl_add( cnx, "<html>" );
    mihl_add( cnx, "<body>" );
    mihl_add( cnx, "This is another page...<br>" );
    mihl_add( cnx, "<a href='/'>Previous Page<a><br><br>" );
    mihl_add( cnx, "<a href='another_unknown.html'>Non-Existent Page<a><br><br>" );
    mihl_add( cnx, "</body>" );
    mihl_add( cnx, "</html>" );
    mihl_send( cnx,
		"Content-type: text/html\r\n" );
    return 0;
}                               // http_nextpage

/**
 * Program entry point
 * 
 * @param argc Number of arguments
 * @param argv Arguments given on the command line
 * @return
 * 	- 0 if OK
 * 	- or -1 if an error occurred (errno is then set).
 */
int main( int argc, char *argv[] ) {
    help( );

    mihl_ctx_t *ctx = mihl_init( NULL, 8080, 8, 
        MIHL_LOG_ERROR | MIHL_LOG_WARNING | MIHL_LOG_INFO | MIHL_LOG_INFO_VERBOSE );
    if ( !ctx )
    	return -1;

    mihl_handle_get( ctx, "/", http_root, NULL );
    mihl_handle_file( ctx, "/image.jpg", "../image.jpg", "image/jpeg", 0 );
    mihl_handle_get( ctx, "/nextpage.html", http_nextpage, NULL );

    for (;;) {
        int status = mihl_server( ctx );
        if ( status == -2 )
            break;
        if ( peek_key( ctx ) )
            break;
    }
    
    return 0;
}
