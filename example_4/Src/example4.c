/**
 * @file example4.c
 *
 */

/**
 * @mainpage MIHL: Minimal Httpd Library - Example #4
 * 
 * HTTP embedded server library \n
 * Copyright (C) 2006-2007  Olivier Singla \n
 * http://mihl.sourceforge.net/ \n\n
 *
 * TBD: Under development - Please ignore for now
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "mihl.h"

#include "example_utils.h"

#define UDP_PORT 9000

/**
 * GET Handler for the URL: /
 * 
 * @param cnx opaque context structure as returned by mihl_init()
 * @param tag TBD
 * @param host TBD
 * @param param TBD
 * @return 0
 */
static int http_index_sdp( mihl_cnx_t *cnx, char const *tag, char const *host, void *param ) {
	mihl_add( cnx, "v=0" );
	mihl_add( cnx, "o=- 54 1998270320 IN IP4 127.0.0.0" );
	mihl_add( cnx, "t=0 0" );
	mihl_add( cnx, "a=range:npt=now-" );
	mihl_add( cnx, "m=video %d RTP/AVP 96", UDP_PORT );
	mihl_add( cnx, "c=IN IP4 %s/15", host );
	mihl_add( cnx, "a=rtpmap:96 MP4V-ES/14980" );
	mihl_add( cnx, "a=cliprect:0,0,%d,%d", 240, 320 );
	mihl_add( cnx, "a=framesize:96 %d-%d", 320, 240 );
	mihl_add( cnx, "b=AS:1372" );
    mihl_send( cnx,
		"Content-type: text/html\r\n" );
    return 0;
}								// http_index_sdp

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

    mihl_handle_get( ctx, "/index.sdp", http_index_sdp, NULL );

    for (;;) {
        int status = mihl_server( ctx );
        if ( status == -2 )
            break;
        if ( peek_key( ctx ) )
            break;
    }
    
    return 0;
}
