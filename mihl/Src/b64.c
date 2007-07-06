/**
 *
 * @file b64.c
 *
 * HTTP embedded server library
 * Copyright (C) 2006-2007  Olivier Singla
 * http://mihl.sourceforge.net/
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "mihl.h"

#include "glovars.h"

#include "tcp_utils.h"
#include "b64.h"

/**
 *  Utility function for Content-Transfer-Encoding standard described in RFC1113
 * 
 *  @param[in] in input buffer
 *  @param[out] out output buffer
 *  @param len of the block  
 */
static void encodeblock( unsigned char in[3], unsigned char out[4], int len ) {
    static const char cb64[]=
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}								// encodeblock

/**
 *  Content-Transfer-Encoding standard described in RFC1113
 * 
 *  @param[in] bin Input buffer
 *  @param size Length of the input buffer
 *  @param[out] bout Output buffer
 *  @param maxlen Size of the output buffer (to prevent buffer overflow)
 */
void mihl_base64_encode( char const *bin, size_t size, char *bout, size_t maxlen ) {

    memset( bout, 0, maxlen );
    int bi = 0;
    for ( unsigned index = 0; index < size; ) {
        unsigned char in[3], out[4];
        int len = 0;
        for( int i = 0; i < 3; i++ ) {
            in[i] = (unsigned char) bin[index++];
            if( index <= size )
                len++;
            else
                in[i] = 0;
        }
        if( len ) {
            encodeblock( in, out, len );
            for( int i = 0; i < 4; i++ )
                bout[bi++] = out[i];
        }
    }

}                               // mihl_base64_encode

/*
** decodeblock
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
static void decodeblock( unsigned char in[4], unsigned char out[3] ) {   
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

void mihl_base64_decode( char const *bin, size_t size, char *bout, size_t maxlen ) {
	static const char cd64[]=
		"|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";
    memset( bout, 0, maxlen );
    int bi = 0;
    for ( unsigned index = 0; index < size; ) {
	    unsigned char in[4], out[3];
	    for( int i = 0; i < 4; i++, index++ ) {
            if ( index < size ) {
		    	unsigned char v = (unsigned char) bin[index];
		    	v = ((v < 43) || (v > 122)) ? 0 : cd64[v-43];
		    	if ( v )
		    		v = (v == '$') ? 0 : v-61;
		    	in[i] = (v) ? v-1 : 0;
            }
            else {
            	in[i] = 0;
            }
	    }
       	decodeblock( in, out ); 
       	for( int i = 0; i < 3; i++ )
       		bout[bi++] = out[i];
    }							// for (index)
    bout[bi++] = 0;
}								// mihl_base64_encode
