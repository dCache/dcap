/*
 *   DCAP - dCache Access Protocol client interface
 *
 *   Copyright (C) 2000,2004 DESY Hamburg DMG-Division.
 *
 *   AUTHOR: Tigran Mkrtchayn (tigran.mkrtchyan@desy.de)
 *
 *   This program can be distributed under the terms of the GNU LGPL.
 *   See the file COPYING.LIB
 *
 */


/*
 * $Id: dcap_url.c,v 1.17 2005-08-15 10:05:03 tigran Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include "dcap.h"
#include "dcap_types.h"
#include "dcap_errno.h"
#include "dcap_error.h"
#include "dcap_url.h"
#include "debug_level.h"
#include "xutil.h"
#include "dcap_str_util.h"
#include "tunnelManager.h"

#define DCAP_PREFIX "dcap://"
#define PNFS_PREFIX "pnfs://"
#define DEFAULT_DOOR "dcache"


int isUrl( const char *path)
{

	return (strstr(path, DCAP_PREFIX) != NULL ) || (strstr(path, PNFS_PREFIX) != NULL ) ;

}


dcap_url* dc_getURL( const char *path )
{

	dcap_url *url;
	char *s;
	char *w;
	char *port_str;
	int host_len;
	int type = URL_NONE;
	int def_door_len;
	char *domain;
	struct servent *se;
        char *t;


	if(path == NULL) {
		dc_errno = DEURL;
		return NULL;
	}


	s = strstr(path, DCAP_PREFIX);
	if( s != NULL ) {
		type = URL_DCAP;
	}else{

		s = strstr(path, PNFS_PREFIX);
		if( s != NULL ) {
			type = URL_PNFS;
		}

	}


	if( (type != URL_DCAP) && (type != URL_PNFS) ) {
		dc_errno = DEURL;
		return NULL;
	}

	url = (dcap_url *)malloc(sizeof(dcap_url));
	if(url == NULL) {
		dc_debug(DC_ERROR, "Failed to allocate dcap_url for %s", path);
		return NULL;
	}


	url->host = NULL;
	url->file = NULL;
	url->prefix = NULL;
	url->type= type;
        url->need_brackets = 0;
        url->port = -1;


	if( s != path ) {
		url->prefix = (char *)xstrndup(path, s - path );
	}else{
		s = (char *)path;
	}

	/* now s is a pointer to url without prefix */

	s = (char *)(s + strlen(DCAP_PREFIX));


	/* w points to a first / in the path */
	w = strchr(s, '/');
	if(w == NULL) {
		free(url);
		return NULL;
	}

	url->file = strdup(w + 1);

	host_len = w-s;

    if( host_len != 0 ) {

                if (s[0] == '[') {
                    /* ipv6 address */
                    s++;
                    t = strchr(s, ']');
                    if (t == NULL || t > w) {
                        /* bad formated url */
                        dc_errno = DEURL;
                        dc_debug(DC_ERROR, "badly formed url for %s", path);
                        free(url);
                        return NULL;
                    }
                    url->need_brackets = 1;
                    url->host = xstrndup(s, t - s);
                    s = t+1;
                } else {
                    t = strchr(s, ':');
                    if (t == NULL || t > w) {
                        url->host = xstrndup(s, w - s);
                        s = w;
                    } else {
                        url->host = xstrndup(s, t - s);
                        s = t;
                    }
                }

                /* s points to ':' or '/' */

		if( s[0] == '/' ) {
			w = strchr(path, ':');
			w = xstrndup(path, w - path);
			se = getservbyname(w, "tcp");
			free(w);
			url->port = se ? ntohs(se->s_port) : DEFAULT_DOOR_PORT;
		}else{
                        s++;
                        port_str = xstrndup(s, w-s);
                        url->port = atoi(port_str);
                        free(port_str);
		}


	}else{

		if( url->type == URL_PNFS ) {
			free(url);
			return NULL;
		}

		domain = strchr(w + 1, '/');
		if( domain == NULL ) {
			free(url);
			return NULL;
		}
		domain++;
		w = strchr(domain , '/');
		if( w == NULL ) {
			w = (char *) domain + strlen(domain);
		}

		host_len = w - domain ;
		def_door_len = strlen(DEFAULT_DOOR);


		url->host = (char *)malloc( def_door_len + host_len + host_len >  0 ? 2 : 1);
		if(url->host == NULL) {
			dc_debug(DC_ERROR, "Failed to allocate hostname for %s", path);
			free(url);
			return NULL;
		}

		memcpy(url->host, DEFAULT_DOOR , def_door_len);
		if(host_len) {
			memcpy(url->host + def_door_len, ".", 1);
		}
		memcpy(url->host + def_door_len +1, domain, host_len);
		url->host[host_len + def_door_len +1] = '\0';

	}

	return url;
}
