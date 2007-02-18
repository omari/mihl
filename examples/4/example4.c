#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "mihl.h"

#include "../example_utils.h"

// FFMPEG includes
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>

static AVFormatContext *format_ctx;

int
http_info( mihl_cnx_t *cnx, char const *tag, char const *host, void *param )
{

    // Try to the AVI file
    format_ctx = NULL;
    char *fname = (char *)&tag[6];
printf( "\n[%s]\n", fname );
fflush( stdout );
    if ( av_open_input_file( &format_ctx, fname, NULL, 0, NULL ) != 0 ) {
        mihl_add( cnx, "-1" );      // 1st param: status
        goto done;
    }

    mihl_add( cnx, "0" );           // 1st param: status

done:
    mihl_send( cnx,
		"Content-type: text/xml\r\n" );
    return 0;
}

// myname1=AAA&myname2=BBB&myname3=CCC]
// myname1=A+++B+C&myname2=HELLO&myname3=BONJOUR%2BMONDE]
int
http_root_post( mihl_cnx_t *cnx, char const *tag, char const *host,
    int nb_variables, char **vars_names, char **vars_values,
    void *param )
{
    printf( "\nnb_variables=%d\n", nb_variables );
    
    char *toto = malloc(1024);
    memmove( toto, vars_values[0], 1023 );
    toto[1023]=0;
    printf( "[%s]\n", toto );
    printf( "\n" );
    fflush( stdout );
    return 0;
}                               // http_root_post


int
main( int argc, char *argv[] )
{
    av_register_all( );     //FFMPEG library initializations

    help( );

    mihl_ctx_t *ctx = mihl_init( NULL, 8080, 8, 
        MIHL_LOG_ERROR | MIHL_LOG_WARNING | MIHL_LOG_INFO /* | MIHL_LOG_INFO_VERBOSE */ );

    mihl_handle_post( ctx, "/toto1", http_root_post, NULL );
    mihl_handle_get( ctx, "/Info/*", http_info, NULL );
    mihl_handle_file( ctx, "/index.html", "index.html", "text/html", 0 );
    mihl_handle_file( ctx, "/prototype.js", "prototype.js", "text/javascript", 0 );

    for (;;) {
        int status = mihl_server( ctx );
        if ( status == -2 )
            break;
        if ( peek_key( ctx ) )
            break;
    }
    
    return 0;
}
