/**
 *
 * @file mihl.h
 *
 * HTTP embedded server library
 * Copyright (C) 2006-2007  Olivier Singla
 * http://mihl.sourceforge.net/
 *
 */

#ifndef __USE_XOPEN_EXTENDED
#   define __USE_XOPEN_EXTENDED
#endif
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>

typedef int SOCKET;

typedef struct mihl_ctx mihl_ctx_t;

typedef struct mihl_cnx mihl_cnx_t;

typedef int (mihl_pf_handle_get_t)(mihl_cnx_t *, char const *, char const *, void *);

typedef int (mihl_pf_handle_post_t)(mihl_cnx_t *, char const *, char const *, int, char **, char **, void *);

mihl_ctx_t *mihl_get_ctx( mihl_cnx_t *cnx );

int mihl_log( mihl_ctx_t *, unsigned level, const char *fmt, ... );

mihl_ctx_t *mihl_init( char const *bind_addr, int port, int maxnb_cnx, unsigned log_level );

int mihl_end( mihl_ctx_t * );

int mihl_server( mihl_ctx_t * );

int mihl_add( mihl_cnx_t *cnx, char const *fmt, ... );

int mihl_send( mihl_cnx_t *cnx, char const *fmt_header, ... );

int mihl_handle_get( mihl_ctx_t *ctx, char const *tag, mihl_pf_handle_get_t *pf, void *param );

int mihl_handle_post( mihl_ctx_t *ctx, char const *tag, mihl_pf_handle_post_t *pf, void *param );

int mihl_handle_file( mihl_ctx_t *ctx, char const *tag, char const *filename, 
    char const *content_type, int close_connection );

typedef struct {
	struct sockaddr_in client_addr; // Client IP address
    time_t time_started;            // When did the connexion started
    time_t time_last_data;          // When was last time we go incoming data ? (used for timedout enx closing)
    char *last_request;             // Last HTTP request (strdup)
    char *host;                     // 'Host:' (strdup)
    char *user_agent;               // 'User-Agent:' (strdup)
} mihl_cnxinfo_t;

int mihl_info( mihl_ctx_t *ctx, int maxnb_cnxinfos, mihl_cnxinfo_t *infos );

#define MIHL_LOG_ERROR          0x01
#define MIHL_LOG_WARNING        0x02
#define MIHL_LOG_INFO           0x04
#define MIHL_LOG_INFO_VERBOSE   0x08
#define MIHL_LOG_DEBUG          0x10
void mihl_set_log_level( mihl_ctx_t *, unsigned level );

unsigned mihl_get_log_level( mihl_ctx_t * );

int mihl_dump_info( mihl_ctx_t * );

int mihl_dump_info_handlers( mihl_ctx_t *ctx );

int send_file( mihl_cnx_t *cnx, char *tag, char *filename, char *content_type, int close_connection );
