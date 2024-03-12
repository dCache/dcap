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
 * $Id: dcap.h.in,v 1.57 2007-07-09 19:35:09 tigran Exp $
 */

#ifndef _DCAP_H_
#define _DCAP_H_

#if !defined(DCAP_THREAD_FLAG) && !defined(WIN32)
#define DCAP_THREAD_FLAG
#define DEFINED_BY_DCAP_H
#endif /* DCAP_THREAD_FLAG */

#include <stdio.h> /* needed for FILE */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h> /* needed for readv/writev */
#include <limits.h>
#ifndef IOV_MAX
#    define IOV_MAX 1024
#endif

#include <fcntl.h>
#ifndef WIN32
#    include <unistd.h>
#    include <dirent.h>
#endif /* WIN32 */
#include <dcap_errno.h>

#ifdef __APPLE__
#define stat64 stat
#define off64_t off_t
#define dirent64 dirent
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#if !defined(DCAP_BUILD) && defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64

#define dc_lseek    dc_lseek64
#define dc_stat     dc_stat64
#define dc_lstat    dc_lstat64
#define dc_fstat    dc_fstat64
#define dc_readdir  dc_readdir64
#define dc_ftello   dc_ftello64
#define dc_fseeko   dc_fseeko64
#define dc_pread    dc_pread64
#define dc_pwrite   dc_pwrite64
#define dc_fopen    dc_fopen64

#else

extern off_t    dc_lseek(int, off_t, int);
extern int      dc_stat(const char *, struct stat *);
extern int      dc_lstat(const char *, struct stat *);
extern int      dc_fstat(int, struct stat *);
#ifndef WIN32
extern struct dirent * dc_readdir(DIR *);
#endif
extern off_t    dc_ftello(FILE *fp);
extern int      dc_fseeko(FILE *, off_t, int);
extern ssize_t dc_pread(int fd, void *buff, size_t buflen, off_t offset);
extern ssize_t dc_pwrite(int fd,const void *buff, size_t buflen, off_t offset);
extern FILE *   dc_fopen(const char *, const char *);

#endif /* _FILE_OFFSET_BITS=64 */



/* POSIX like IO*/
extern int      dc_open(const char *, int, ...);
extern int      dc_creat(const char *, mode_t);
extern int      dc_close(int);
extern int      dc_close2(int);
extern ssize_t  dc_read(int, void *, size_t);
extern ssize_t  dc_readv(int, const struct iovec *, int);
extern ssize_t  dc_pread64(int, void  *,  size_t, off64_t);
extern ssize_t  dc_write(int, const void *, size_t);
extern ssize_t  dc_writev(int, const struct iovec *, int);
extern ssize_t  dc_pwrite64(int, const void  *,  size_t, off64_t);
extern off64_t  dc_lseek64(int, off64_t, int);
extern int      dc_fsync(int);
extern int      dc_dup(int);
extern int      dc_access( const char *, int);
extern int      dc_unlink( const char *);
extern int      dc_rmdir( const char *);
extern int      dc_mkdir( const char *, mode_t);
extern int      dc_chmod( const char *, mode_t);
extern int      dc_chown( const char *, uid_t, gid_t);
extern int      dc_stat64(const char *, struct stat64 *);
extern int      dc_lstat64(const char *, struct stat64 *);
extern int      dc_fstat64(int , struct stat64 *);
extern int      dc_rename(const char *, const char *);
#ifdef HAVE_ACL
extern int      dc_acl(const char *, int, int, void *);
#endif
#ifdef HAVE_FACL
extern int      dc_facl(int, int, int, void *);
#endif

#ifndef WIN32
extern DIR *    dc_opendir(const char *);
extern struct dirent64 * dc_readdir64(DIR *);
extern int      dc_closedir(DIR *);
extern off_t    dc_telldir(DIR *);
extern void    dc_seekdir(DIR *, off_t);
#endif /* WIN32 */

/* stream IO */
extern FILE *   dc_fopen64(const char *, const char *);
extern FILE *   dc_fdopen(int , const char *);
extern int      dc_fclose(FILE *);
extern size_t   dc_fwrite(const void *, size_t,  size_t, FILE *);
extern size_t   dc_fread(void *, size_t,  size_t, FILE *);
extern int      dc_fseek(FILE *, long, int);
extern long     dc_ftell(FILE *);
extern int      dc_ferror(FILE *);
extern int      dc_fflush(FILE *);
extern int      dc_feof(FILE *);
extern char *   dc_fgets(char *, int, FILE *);
extern int      dc_fgetc(FILE *);
extern off64_t  dc_ftello64(FILE *fp);
extern int      dc_fseeko64(FILE *, off64_t, int);
/* pre-stage */
extern int      dc_stage(const char *, time_t, const char *);
extern int      dc_check(const char *, const char *);

#ifndef _IOVEC2_
#define _IOVEC2_

typedef struct {
	off64_t offset;
	int len;
	char *buf;
} iovec2;

#endif
extern int     dc_readv2(int, iovec2 *, int);
/* user control */
#define onErrorRetry 1
#define onErrorFail  0
#define onErrorDefault -1

extern void dc_setOpenTimeout(time_t);
extern void dc_setCloseTimeout(unsigned int);
extern void dc_setOnError(int);

/* read ahead buffering */
extern void dc_noBuffering(int);
extern void dc_setBufferSize(int, size_t);

/* LCB (local cache block) */
extern void dc_lcb_flush(int fh);

/* write blocks conformation */
extern void dc_unsafeWrite(int);

extern void dc_setReplyHostName(const char *s);
extern void dc_setCallbackPort(unsigned short);
extern void dc_setCallbackPortRange(unsigned short, unsigned short);
/*depricated*/
extern const char * getDcapVersion();
/*Replacements for depricated method getDcapVersion*/
extern const int dc_getProtocol();
extern const int dc_getMajor();
extern const int dc_getMinor();
extern const char *dc_getPatch();

extern void dc_setTunnel( const char *);
extern void dc_setTunnelType( const char *);

extern void dc_setDebugLevel(unsigned int);
extern void dc_setStrDebugLevel(const char *);
extern void dc_debug(unsigned int, const char *, ...);
extern void dc_error(const char *); /* obsolete, use dc_perror */
extern void dc_perror(const char *);
extern const char * dc_strerror(int);

extern void dc_setTCPSendBuffer( int );
extern void dc_setTCPReceiveBuffer( int );

extern void dc_noCheckSum(int);
extern void dc_setClientActive();

/* extra options */
extern void dc_setExtraOption(char *);
extern void dc_resetExtraOption();

/* thread-safe errno hack */
#ifdef _REENTRANT
extern int *__dc_errno();
#define dc_errno (*(__dc_errno()))
#else
extern int dc_errno;
#endif /* _REENTRANT */


#ifdef  __cplusplus
}
#endif

#if defined(DCAP_THREAD_FLAG) && defined(DEFINED_BY_DCAP_H)
#undef DCAP_THREAD_FLAG
#undef DEFINED_BY_DCAP_H
#endif

#endif				/* _DCAP_H_ */
