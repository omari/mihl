
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <unistd.h>

#include "mihl.h"

int
http_root( connexion_t *cnx, char const *tag, char const *host, void *param )
{
    mihl_add( cnx, "<!DOCTYPE html PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN'>" );
    mihl_add( cnx, "<html>" );
    mihl_add( cnx, "<head>" );
    mihl_add( cnx, "  <meta content='text/html; charset=ISO-8859-1'" );
    mihl_add( cnx, " http-equiv='content-type'>" );
    mihl_add( cnx, "  <title>blah blah</title>" );
    mihl_add( cnx, "</head>" );
    mihl_add( cnx, "<body style='color: rgb(0, 0, 0); background-color: rgb(204, 204, 204);'" );
    mihl_add( cnx, " alink='#000099' link='#000099' vlink='#990099'>" );
    mihl_add( cnx, "<div style='text-align: center; font-weight: bold;'><big>Test AJAX" );
    mihl_add( cnx, "(prototype.js)<br>" );
    mihl_add( cnx, "</big></div>" );
    mihl_add( cnx, "<br>" );
    mihl_add( cnx, "<div style='text-align: center;'><img style='width: 70px; height: 72px;'" );
    mihl_add( cnx, " alt='' src='/image.jpg'><br>" );
    mihl_add( cnx, "</div>" );
    mihl_add( cnx, "<br>" );
    mihl_add( cnx, "<table" );
    mihl_add( cnx, " style='text-align: left; background-color: rgb(208, 208, 208); margin-left: auto; margin-right: auto;'" );
    mihl_add( cnx, " border='1' cellpadding='2' cellspacing='2'>" );
    mihl_add( cnx, "  <tbody>" );
    mihl_add( cnx, "    <tr>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>Refreshed each second<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "      <td style='vertical-align: top;' id='field1'>XXXX<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "    </tr>" );
    mihl_add( cnx, "    <tr>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>Refreshed each 5 seconds<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "      <td style='vertical-align: top;' id='field2'>XXXX<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "    </tr>" );
    mihl_add( cnx, "    <tr>" );
    mihl_add( cnx, "      <td style='vertical-align: top;'>Refreshed each 30 seconds<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "      <td style='vertical-align: top;' id='field3'>XXXX<br>" );
    mihl_add( cnx, "      </td>" );
    mihl_add( cnx, "    </tr>" );
    mihl_add( cnx, "  </tbody>" );
    mihl_add( cnx, "</table>" );
    mihl_add( cnx, "<br>" );
    mihl_add( cnx, "</body>" );

    mihl_add( cnx, "<SCRIPT type='text/javascript' src='/prototype.js'> </SCRIPT>" );

    mihl_add( cnx, "<SCRIPT type='text/javascript'>" );
    mihl_add( cnx, "<!--" );

    mihl_add( cnx, "var cpt = 0;" );
    mihl_add( cnx, "function onLoadHandler( ) {" );
    mihl_add( cnx, "  setInterval( 'timerFunction1()', 1000 );" );
    mihl_add( cnx, "  setInterval( 'timerFunction5()', 5000 );" );
    mihl_add( cnx, "  setInterval( 'timerFunction30()', 30000 );" );
    mihl_add( cnx, "};" );

    mihl_add( cnx, "function timerFunction1( ) {" );
    mihl_add( cnx, "  var url = 'http://%s/data1';", host );
    mihl_add( cnx, "  var myAjax = new Ajax.Request( url, { method: 'get', onComplete: showData1 });" );
    mihl_add( cnx, "};" );

    mihl_add( cnx, "function timerFunction5( ) {" );
    mihl_add( cnx, "  var url = 'http://%s/data2';", host );
    mihl_add( cnx, "  var myAjax = new Ajax.Request( url, { method: 'get', onComplete: showData2 });" );
    mihl_add( cnx, "};" );

    mihl_add( cnx, "function timerFunction30( ) {" );
    mihl_add( cnx, "  var url = 'http://%s/data3';", host );
    mihl_add( cnx, "  var myAjax = new Ajax.Request( url, { method: 'get', onComplete: showData3 });" );
    mihl_add( cnx, "};" );

    mihl_add( cnx, "function showData1( originalRequest ) {" );
    mihl_add( cnx, "  field1.innerText = originalRequest.responseText;" );
    mihl_add( cnx, "};" );

    mihl_add( cnx, "function showData2( originalRequest ) {" );
    mihl_add( cnx, "  field2.innerText = originalRequest.responseText;" );
    mihl_add( cnx, "};" );

    mihl_add( cnx, "function showData3( originalRequest ) {" );
    mihl_add( cnx, "  field3.innerText = originalRequest.responseText;" );
    mihl_add( cnx, "};" );

    mihl_add( cnx, "-->" );
    mihl_add( cnx, "</SCRIPT>" );

    mihl_add( cnx, "<BODY onLoad='onLoadHandler();'>" );
    mihl_add( cnx, "</BODY>" );

    mihl_add( cnx, "</html>" );
    mihl_send( cnx,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n" );
    return 0;
}                               // http_root


int
http_data( connexion_t *cnx, char const *tag, char const *host, void *param )
{
    int index = (int)param;
    static int cpts[3] = { 0, 0, 0 };
    mihl_add( cnx, "cpt=%d", cpts[index]++ );
    mihl_send( cnx,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/xml\r\n" );
    return 0;
}                               // http_data


int
main( int argc, char *argv[] )
{
    mihl_init( 8080 );

    mihl_handle_get( "/", http_root, NULL );
    mihl_handle_get( "/data1", http_data, (void *)0 );
    mihl_handle_get( "/data2", http_data, (void *)1 );
    mihl_handle_get( "/data3", http_data, (void *)2 );
    mihl_handle_file( "/image.jpg", "image.jpg", "image/jpeg", 0 );
    mihl_handle_file( "/prototype.js", "prototype.js", "text/javascript", 0 );

    for (;;) {
        int status = mihl_server( );
        if ( status == -2 )
            break;
        usleep( 1000 );
//        Sleep( 0 );
    }
    
    return 0;
}                               // main
