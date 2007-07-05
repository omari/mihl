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
void base64( char const *bin, size_t size, char *bout, size_t maxlen ) {

//    static const char cd64[]=
//        "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

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

}                               // base64
