//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
// @file   apiFS.c
// @author MStar Semiconductor Inc.
// @brief  FileSystem wrapper
// @note   Most of the APIs follows POSIX Specification including: \n
//                  - Files and Directories [POSIX Section 5] \n
//                  - Input and Output [POSIX Section 6]
// @note   With some limitations: \n
//                  - Doesn't support symolic link, user/group/other ID, ...
// @note   Plz reference Open Group's Single Unix Specification V3 for detailed POSIX API descriptions: \n
//                  - http://www.opengroup.org/onlinepubs/009695399/ \n
///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (MSOS_TYPE_LINUX)

#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>
#include "MsCommon.h"
#include "MsFS.h"
#if (defined (MSOS_TYPE_LINUX) && !defined(ANDROID))
#include <sys/statvfs.h>
#endif
#if defined (ANDROID)
#include <sys/vfs.h>
#define statvfs statfs
#endif
#include <string.h>

// extern struct dirent *readdir(DIR *dir);

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MAX_CHARACTER_SIZE    8
#define MAX_FILENAME_LENGTH    512

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
static int _bUnicode=0;

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
static unsigned char * _MsFS_UnicodeStrToUTF8Str (const unsigned short * unicode_str,unsigned char * utf8_str, int utf8_str_size);



//-------------------------------------------------------------------------------------------------
/// File system API initiatiion
/// @param  bUnicode \b IN: 0 support ASCII only, otherwise it supports unicode
/// @return 0 : no error
/// @return otherwise : error
//-------------------------------------------------------------------------------------------------
int MsFS_Init( int bUnicode)
{
    _bUnicode= bUnicode;
    return 0;
}

//
// Mount/Umount operations
//
//-------------------------------------------------------------------------------------------------
/// Mount a filesystem
/// @param  devname \b IN: name of hardware device:     eg. ""      , "/dev/fd0/"   , ...
/// @param  dir     \b IN: name of mount point:             "/ram"  , "/floppy"     , ...
/// @param  fsname  \b IN: name of implementing filesystem: "ramfs" , "fatfs"       , ...
/// @return 0 : no error
/// @return otherwise : error
//-------------------------------------------------------------------------------------------------
int MsFS_Mount( const char *devname, const char *dir, const char *fsname, unsigned long mountflags, const void* data )
{
    return mount( devname, dir, fsname, mountflags, data);
}

//-------------------------------------------------------------------------------------------------
/// Unmount a filesystem
/// @param  dir     \b IN: name of mount point:         eg.   "/ram"  , "/floppy"     , ...
/// @return 0 : no error
/// @return otherwise : error
//-------------------------------------------------------------------------------------------------
int MsFS_Umount( const char *dir)
{
    return umount( dir );
}

//
// File/Directory operations
//
//-------------------------------------------------------------------------------------------------
/// Open a file
/// @param  path    \b IN: the pathname of the file
/// @param  oflag   \b IN: bitwise-OR flags of O_RDONLY, O_WRONLY, O_RDWR, O_APPEND, O_CREAT, O_EXCL, O_TRUNC, O_NONBLOCK
/// @param  ...     \b IN: not support
/// @return >=0 : the file descriptor associated with the file
/// @return <0 : errno set to indicate the error
/// @note   creat() is redundant and can be replaced by open( path, O_WRONLY | O_CREAT | O_TRUNC, mode );
/// @note   The maximum number of open files allowed is determined by the CYGNUM_FILEIO_NFILE
/// @note   Standard C Library's counterparts: \n
///         - fopen()   is equivalent to    open();
//-------------------------------------------------------------------------------------------------
int MsFS_Open( const char *path, int oflag, ... )
{
    char u8path[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        return open(u8path,oflag);
    }
    return open( path, oflag );
}

//-------------------------------------------------------------------------------------------------
/// Unlink/Remove a file
/// @param  path    \b IN: the pathname of the file
/// @return =0 : the file descriptor associated with the named file
/// @return -1 : errno set to indicate the error
/// @note   Remove a link to a file; When the file's link count becomes 0 and no process has the file open,
///         the space occupied by the file will be freed.
/// @note   Cannot unlink directories; use rmdir() instead
//-------------------------------------------------------------------------------------------------
int MsFS_Unlink( const char *path )
{
    char u8path[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        return unlink(u8path);
    }
    return unlink( path );
}

//-------------------------------------------------------------------------------------------------
/// Make a directory
/// @param  path    \b IN: the pathname of the directory
/// @param  mode    \b IN: not support
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
int MsFS_MkDir( const char *path, mode_t mode )
{
    char u8path[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        return mkdir( u8path, mode );
    }

    return mkdir( path, mode );
}

//-------------------------------------------------------------------------------------------------
/// Remove a directory
/// @param  path    \b IN: the pathname of the directory
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
int MsFS_RmDir( const char *path )
{
    char u8path[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        return rmdir( u8path);
    }

    return rmdir( path );
}

//-------------------------------------------------------------------------------------------------
/// Change current working directory
/// @param  path    \b IN:  the pathname of the new working directory
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
int MsFS_ChDir(const char *path)
{
    char u8path[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        return chdir( u8path);
    }

    return chdir( path );
}

//-------------------------------------------------------------------------------------------------
/// Rename a file/directory
/// @param  path1    \b IN: the old pathname of the file
/// @param  path2    \b IN: The new pathname of the file
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
int MsFS_Rename( const char *path1, const char *path2 )
{
    char u8path1[MAX_FILENAME_LENGTH] = {0};
    char u8path2[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path1,(unsigned char *)u8path1,MAX_FILENAME_LENGTH);
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path2,(unsigned char *)u8path2,MAX_FILENAME_LENGTH);
        return rename( u8path1, u8path2 );
    }

    return rename( path1, path2 );
}

//-------------------------------------------------------------------------------------------------
/// Create a link (directory entry) from an existing file (path1) to a new one (path2)
/// @param  path1    \b IN: the pathname of an existing file
/// @param  path2    \b IN: the pathname of the new directory entry
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
/// @note   RAM filesystem supports this API but FAT filesystem doesn't
//-------------------------------------------------------------------------------------------------
int MsFS_Link( const char *path1, const char *path2 )
{
    char u8path1[MAX_FILENAME_LENGTH] = {0};
    char u8path2[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path1,(unsigned char *)u8path1,MAX_FILENAME_LENGTH);
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path2,(unsigned char *)u8path2,MAX_FILENAME_LENGTH);
        return link( u8path1, u8path2 );
    }

    return link( path1, path2 );
}

//-------------------------------------------------------------------------------------------------
/// Get file status
/// @param  path    \b IN:  the pathname of the file
/// @param  buf     \b IN:  a pointer to a stat structure as defined in <sys/stat.h>
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
/// @note   equivalent to MsFS_FStat
//-------------------------------------------------------------------------------------------------
int MsFS_Stat( const char *path, struct stat *buf )
{
    char u8path[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        return stat( u8path, buf );
    }

    return stat( path, buf );
}

//-------------------------------------------------------------------------------------------------
/// Get file status
/// @param  fd      \b IN:  tthe file descriptor associated with the file
/// @param  buf     \b IN:  a pointer to a stat structure as defined in <sys/stat.h>
/// @return =0 : succeed
/// @return -1 : errno set to indicate the error
/// @note   equivalent to   MsFS_Stat
//-------------------------------------------------------------------------------------------------
int MsFS_FStat( int fd, struct stat *buf )
{
    return fstat( fd, buf );
}

//-------------------------------------------------------------------------------------------------
/// Get configurable pathname variables
/// @param  path    \b IN:  the pathname of the file or directory
/// @param  name    \b IN:  variable in <limits.h> to be queried relative to that file or directory
/// @return current variable value for the file or directory
/// @return -1 : errno set to indicate the error
/// @note   equivalent to   MsFS_FPathConf
//-------------------------------------------------------------------------------------------------
long MsFS_PathConf( const char *path, int name )
{
    return pathconf( path, name );
}

//-------------------------------------------------------------------------------------------------
/// Get configurable pathname variables
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @param  name    \b IN:  variable in <limits.h> to be queried relative to that file or directory
/// @return current variable value for the file or directory
/// @return -1 : errno set to indicate the error
/// @note   equivalent to   MsFS_PathConf
//-------------------------------------------------------------------------------------------------
long MsFS_FPathConf( int fd, int name )
{
    return fpathconf( fd, name );
}

//-------------------------------------------------------------------------------------------------
/// Determine the accessibility of a file
/// @param  path    \b IN:  the pathname of the file or directory
/// @param  amode   \b IN:  the accessibility bit pattern: only existence test F_OK currently
/// @return 0: the requested access is permitted
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
int MsFS_Access( const char *path, int amode )
{
    char u8path[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        return access(u8path, amode );
    }

    return access( path, amode );
}

//-------------------------------------------------------------------------------------------------
/// Get the pathname of the current working directory
/// @param  buf     \b IN:  points to the buffer stored the absolute pathname of the current working directory
/// @param  size    \b IN:  the buffer size
/// @return buf argument
/// @return null : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
char * MsFS_GetCwd( char *buf, size_t size )
{
    return getcwd( buf, size );
}

//-------------------------------------------------------------------------------------------------
/// Open a directory stream for reading
/// @param  dirname \b IN:  the pathname of the directory
/// @return a pointer to DIR object
/// @return null : errno set to indicate the error.
/// @note   The directory stream (an ordered sequence of all the directory entries in a particular directory)
///             is positioned at the first directory entry
//-------------------------------------------------------------------------------------------------
DIR * MsFS_OpenDir( const char *dirname )
{
    char u8dirname[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)dirname,(unsigned char *)u8dirname,MAX_FILENAME_LENGTH);
        return opendir(u8dirname );
    }

    return opendir( dirname );
}

//-------------------------------------------------------------------------------------------------
/// Read a directory entry
/// @param  dirp    \b IN:  a pointer to DIR object
/// @return a pointer to dirent structure representing the directory entry at the current position in the directory
/// @return null : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
struct dirent * MsFS_ReadDir( DIR *dirp )
{
    printf("[%s][%d] MsFS_ReadDir is not supported at this stage\n", __FUNCTION__, __LINE__);
    return NULL;
    // return readdir( dirp );
}

//-------------------------------------------------------------------------------------------------
/// Reset the position of a directory stream to the beginning of a directory
/// @param  dirp    \b IN:  a pointer to DIR object
/// @return NONE
//-------------------------------------------------------------------------------------------------
void MsFS_RewindDir( DIR *dirp )
{
    rewinddir( dirp );
}

//-------------------------------------------------------------------------------------------------
/// Close a directory stream
/// @param  dirp    \b IN:  a pointer to DIR object
/// @return NONE
//-------------------------------------------------------------------------------------------------
void MsFS_CloseDir( DIR *dirp )
{
    closedir( dirp );
}


//
// File IO operations
//
//-------------------------------------------------------------------------------------------------
/// Read from a file
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @param  buf     \b OUT: read buffer
/// @param  len     \b IN:  number of bytes to be read
/// @return number of bytes actually read
/// @return -1 : errno set to indicate the error
/// @note   Standard C Library's counterparts: \n
///         - fread()   is equivalent to    read();
//-------------------------------------------------------------------------------------------------
ssize_t MsFS_Read( int fd, void *buf, size_t len )
{
    return read( fd, buf, len );
}

//-------------------------------------------------------------------------------------------------
/// Write to a file
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @param  buf     \b IN:  write buffer
/// @param  len     \b IN:  number of bytes to be written
/// @return number of bytes actually written
/// @return -1 : errno set to indicate the error
/// @note   Standard C Library's counterparts: \n
///         - fwrite()  is equivalent to    write();
//-------------------------------------------------------------------------------------------------
ssize_t MsFS_Write( int fd, const void *buf, size_t len )
{
    return write( fd, buf, len );
}

//-------------------------------------------------------------------------------------------------
/// Close a file
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @return 0: succeed
/// @return -1 : errno set to indicate the error
/// @note   Standard C Library's counterparts: \n
///         - fclose()  is equivalent to    close();
//-------------------------------------------------------------------------------------------------
int MsFS_Close( int fd )
{
    return close( fd );
}

//-------------------------------------------------------------------------------------------------
/// Seek a file
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @param  pos     \b IN:  file byte offset relative to whence
/// @param  whence  \b IN:  SEEK_SET / SEEK_CUR / SEEK_END
/// @return byte offset from the beginning of the file
/// @return -1 : errno set to indicate the error
/// @note   Standard C Library's counterparts: \n
///         - fseek()   is equivalent to    lseek();    except the return value \n
///         - rewind()  is equivalent to    lseek(fd, 0, SEEK_SET); \n
///         - ftell()   is equivalent to    lseek(fd, 0, SEEK_CUR); \n
//-------------------------------------------------------------------------------------------------
MS_U64 MsFS_Lseek( int fd, MS_U64 pos, int whence )
{
    return lseek( fd, pos, whence );
}

//-------------------------------------------------------------------------------------------------
/// File control
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @param  cmd     \b IN:  only F_DUPFD currently
/// @param  ...     \b IN:  only lowest-numbered file descriptor for F_DUPFD currently
/// @return 0: succeed
/// @return -1 : errno set to indicate the error
/// @note   fid = dup(fd);  is equivalent to  fid = fcntl(fd, F_DUPFD, 0);
/// @note   fid = dup2(fd, fd2);  is equivalent to  close(fd2); fid = fcntl(fd, F_DUPFD, fd2);
//-------------------------------------------------------------------------------------------------
int MsFS_FCntl( int fd, int cmd, ... )
{
    va_list a;
    va_start( a, cmd );
    int fda = va_arg(a, int);
    va_end(a);
    return fcntl( fd, cmd, fda);
}

//-------------------------------------------------------------------------------------------------
/// Synchronize changes to a file (all data for the open file will be transferred to the storage device)
/// @param  fd      \b IN:  the file descriptor associated with the file
/// @return 0: succeed
/// @return -1 : errno set to indicate the error
/// @note   The function will not return until the system has completed that action or until an error is detected
//-------------------------------------------------------------------------------------------------
int MsFS_FSync( int fd )
{
    return fsync( fd );
}

void MsFS_Sync(void)
{
    sync();
}


//
// Standard C Library's counterparts
//
//-------------------------------------------------------------------------------------------------
/// Open a file
/// @param  filename \b IN: the pathname of the file
/// @param  mode     \b IN:  r or rb - Open file for reading.
///                         w or wb - Truncate to zero length or create file for writing.
///                         a or ab - Append; open or create file for writing at end-of-file.
///                         r+ or rb+ or r+b - Open file for update (reading and writing).
///                         w+ or wb+ or w+b - Truncate to zero length or create file for update.
///                         a+ or ab+ or a+b Append; open or create file for update, writing at end-of-file.
/// @return the file descriptor associated with the file
/// @return NULL : errno set to indicate the error
/// @note   The maximum number of open files allowed is determined by the CYGNUM_FILEIO_NFILE
//-------------------------------------------------------------------------------------------------
FILE * MsFS_Fopen( const char *filename, const char *mode )
{
#if 0
    FILE* a = fopen(filename, mode);
    printf("[%s][%d] 0x%08x fopen %s %s\n", __FUNCTION__, __LINE__, (int)a, filename, mode);
    return a;
#else
    char u8filename[MAX_FILENAME_LENGTH] = {0};
    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)filename,(unsigned char *)u8filename,MAX_FILENAME_LENGTH);
        return fopen(u8filename, mode );
    }

    return fopen(filename, mode);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Close a file
/// @param  stream  \b IN:  the file descriptor associated with the file
/// @return 0: succeed
/// @return EOF(-1): errno set to indicate the error
//-------------------------------------------------------------------------------------------------
int MsFS_Fclose( FILE *stream )
{
#if 0
    int a = fclose(stream);
    printf("[%s][%d] 0x%08x fclose %d\n", __FUNCTION__, __LINE__, (int)stream, a);
    return a;
#else
    return fclose(stream);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Read from a file
/// @param  data     \b OUT: read buffer
/// @param  itemsize \b IN:  item size in bytes
/// @param  nitems   \b IN:  number of items
/// @param  stream   \b IN:  the file descriptor associated with the file
/// @return number of items actually read
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
size_t MsFS_Fread( void *data, size_t itemsize, size_t nitems, FILE *stream )
{
#if 0
    size_t a = fread(data, itemsize, nitems, stream);
    printf("[%s][%d] 0x%08x read size %d\n", __FUNCTION__, __LINE__, (int)stream, a);
    return a;
#else
    return fread(data, itemsize, nitems, stream);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Write to a file
/// @param  data     \b IN: write buffer
/// @param  itemsize \b IN: item size in bytes
/// @param  nitems   \b IN: number of items
/// @param  stream   \b IN: the file descriptor associated with the file
/// @return number of items actually written
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
size_t MsFS_Fwrite( const void *data, size_t itemsize, size_t nitems, FILE *stream )
{
#if 0
    size_t a = fwrite(data, itemsize, nitems, stream);
    printf("[%s][%d] 0x%08x write %d\n", __FUNCTION__, __LINE__, (int) stream, a);
    return a;
#else
    return fwrite(data, itemsize, nitems, stream);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Truncate a file
/// @param  stream   \b IN:  the file descriptor associated with the file
/// @param  length   \b IN:  the file size to truncate to.
/// @return  0: succeed
/// @return -1: errno set to indicate the error
int MsFS_Fftruncate( FILE *stream, MS_U64 length)
{
    int fd = fileno(stream);
    return ftruncate(fd, (off_t)length);
}

//-------------------------------------------------------------------------------------------------
/// Seek a file
/// @param  stream  \b IN:  the file descriptor associated with the file
/// @param  pos     \b IN:  file byte offset relative to whence
/// @param  whence  \b IN:  SEEK_SET / SEEK_CUR / SEEK_END
/// @return 0: succeed
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
int MsFS_Fseek( FILE *stream, MS_U64 pos, int whence )
{
#if 0
    int a = fseeko(stream, pos, whence);
    printf("[%s][%d] 0x%08x write %d\n", __FUNCTION__, __LINE__, (int) stream, a);
    return a;
#else
    return fseeko(stream, pos, whence);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Reset the file position
/// @param  stream  \b IN:  the file descriptor associated with the file
/// @return NONE
//-------------------------------------------------------------------------------------------------
void MsFS_Rewind( FILE *stream )
{
    rewind(stream);
}

//-------------------------------------------------------------------------------------------------
/// Obtain current file position
/// @param  stream  \b IN:  the file descriptor associated with the file
/// @return current file position measured in bytes from the beginning of the file
/// @return -1 : errno set to indicate the error
//-------------------------------------------------------------------------------------------------
MS_U64 MsFS_Ftell( FILE *stream )
{
#if 0
    MS_U64 a = ftello(stream);
    // printf("[%s][%d] 0x%08x tell %ld\n", __FUNCTION__, __LINE__, (int) stream, a);
    return a;
#else
    return ftello(stream);
#endif
}

int MsFS_Fflush(FILE *stream)
{
    return fflush(stream);
}

MS_BOOL MApi_FS_Info(const char *path, MS_BOOL bUnicode, MApi_FsInfo* pFsInfo)
{
    struct statvfs buf;
    char u8path[MAX_FILENAME_LENGTH] = {0};

    if (!pFsInfo)
    {
        return FALSE;
    }

    if(_bUnicode)
    {
        _MsFS_UnicodeStrToUTF8Str((const unsigned short *)path,(unsigned char *)u8path,MAX_FILENAME_LENGTH);
        if (0 != statvfs(u8path, &buf))
        {
            return FALSE;
        }
    }
    else
    {
        if (0 != statvfs(path, &buf))
        {
            return FALSE;
        }
    }
    pFsInfo->u32ClusTotal = buf.f_blocks;
    pFsInfo->u32ClusFree = buf.f_bfree;
    pFsInfo->u32ClusSize = buf.f_frsize;
    return TRUE;
}


static unsigned char * _MsFS_UnicodeToUTF8( int unicode,unsigned char *p)
{
    unsigned char *e = p;
    if(e!=NULL)
    {
        if(unicode < 0x80)
        {
            *e = unicode;
            e++;
        }
        else if(unicode < 0x800)
        {

            *e = ((unicode >> 6) & 0x1f)|0xc0;
            e++;
            *e = (unicode & 0x3f)|0x80;
            e++;
        }
        else if(unicode < 0x10000)
        {

            *e = ((unicode >> 12) & 0x0f)|0xe0;
            e++;
            *e = ((unicode >> 6) & 0x3f)|0x80;
            e++;
            *e = (unicode & 0x3f)|0x80;
            e++;
        }
        else if(unicode < 0x200000)
        {

            *e = ((unicode >> 18) & 0x07)|0xf0;
            e++;
            *e = ((unicode >> 12) & 0x3f)|0x80;
            e++;
            *e = ((unicode >> 6) & 0x3f)|0x80;
            e++;
            *e = (unicode & 0x3f)|0x80;
            e++;
        }
        else if(unicode < 0x4000000)
        {

            *e = ((unicode >> 24) & 0x03)|0xf8 ;
            e++;
            *e = ((unicode >> 18) & 0x3f)|0x80;
            e++;
            *e = ((unicode >> 12) & 0x3f)|0x80;
            e++;
            *e = ((unicode >> 6) & 0x3f)|0x80;
            e++;
            *e = (unicode & 0x3f)|0x80;
            e++;
        }
        else
        {

            *e = ((unicode >> 30) & 0x01)|0xfc;
            e++;
            *e = ((unicode >> 24) & 0x3f)|0x80;
            e++;
            *e = ((unicode >> 18) & 0x3f)|0x80;
            e++;
            *e = ((unicode >> 12) & 0x3f)|0x80;
            e++;
            *e = ((unicode >> 6) & 0x3f)|0x80;
            e++;
            *e = (unicode & 0x3f)|0x80;
            e++;
        }
    }

    return e;
}


static unsigned char * _MsFS_UnicodeStrToUTF8Str (const unsigned short * unicode_str,unsigned char * utf8_str, int utf8_str_size)
{
    int idx = 0;
    int unicode = 0;
    unsigned char *e = NULL, *s = NULL;
    unsigned char utf8_ch[MAX_CHARACTER_SIZE]={0};
    s = utf8_str;
    if ((unicode_str!=NULL) && (s!=NULL))
    {
        for(idx=0, unicode=(int)unicode_str[idx];unicode!=0;idx=idx+1,unicode=(int)unicode_str[idx])
        {
            memset (utf8_ch, 0, sizeof (utf8_ch));
            e = _MsFS_UnicodeToUTF8 (unicode, utf8_ch);
            if (e > utf8_ch)
            {
                *e = '\0';

                /* Judge whether exceed the destination buffer */
                if ((int)(s - utf8_str + strlen ((const char *) utf8_ch)) >= utf8_str_size)
                {
                    return s;
                }
                else
                {
                    memcpy (s, utf8_ch, strlen ((const char *) utf8_ch));
                    s += strlen ((const char *) utf8_ch);
                    *s = '\0';
                }
            }
            else
            {
                /* Converting error occurs */
                return s;
            }
        }
    }

    return s;
}
#endif // #if defined (MSOS_TYPE_LINUX)
