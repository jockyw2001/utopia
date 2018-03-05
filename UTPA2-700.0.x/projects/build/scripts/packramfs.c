#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BLOCK_SIZE  32768

typedef struct _ST_INITRAMFS_HEAD
{
	unsigned int initramfs_start;
	unsigned int initramfs_end;
} ST_INITRAMFS_HEAD;

int main( int argc, char *argv[] )
{
	ST_INITRAMFS_HEAD stHead;

	FILE * fp_system_map;

	FILE * fp_in;
	FILE * fp_out;

	unsigned addr;
	char str0[32];
	char str1[32];

	void *buf;
	int offset;
	size_t tmp;

	unsigned int ker_dst_base = 0;
	unsigned int ker_dst_entry = 0;
	unsigned int ker_dst_end = 0;

	char cmd_str[512];

	if ( argc < 2 ) {
		printf( "Usage: %s [FILE]\n", argv[0] );
		return -1;
	}

	fp_out = fopen( argv[1], "wb+" );
	if ( fp_out == NULL ) {
		perror( argv[1] );
		return -1;
	}

	fp_in = fopen( "usr/initramfs_data.cpio.gz", "rb" );
	if ( fp_in == NULL ) {
		perror( "usr/initramfs_data.cpio.gz" );
		fclose( fp_out );
		return -1;
	}

#ifdef CONFIG_INITRAMFS_IMG_HDR_ADDR
	stHead.initramfs_start = CONFIG_INITRAMFS_IMG_HDR_ADDR + sizeof( ST_INITRAMFS_HEAD );
#else
	stHead.initramfs_start = 0x88000000 + sizeof( ST_INITRAMFS_HEAD );
#endif

	fseek( fp_in, 0, SEEK_END );

	offset = ftell( fp_in );
	if ( offset >= 0 ) {	// real image size
		stHead.initramfs_end = (unsigned int) (stHead.initramfs_start + offset);
	} else {
		stHead.initramfs_end = stHead.initramfs_start;
	}
	fwrite( &stHead, 1, sizeof( ST_INITRAMFS_HEAD ), fp_out );

	rewind( fp_in );
	buf = malloc( 0x400000 );
	if ( buf == NULL ) {
		perror( "malloc fail" );
		return -1;
	}

	while ( !feof( fp_in ) ) {
		tmp = fread( buf, 1, 0x400000, fp_in );
		if ( tmp <= 0 ) {
			perror( "read fail" );
			free( buf );
			fclose( fp_in );
			fclose( fp_out );
			return -1;
		}
		fwrite( buf, 1, tmp, fp_out );
		stHead.initramfs_end -= tmp;
	}

	free( buf );

	fclose( fp_in );
	fclose( fp_out );

	if ( stHead.initramfs_end < stHead.initramfs_start ) {
		printf( "cannot read overall file.\n" );
		return -1;
	}

	//! find data from system map
	fp_system_map = fopen( "System.map", "r" );
	while( fscanf( fp_system_map, "%x %s %s", &addr, str0, str1 ) > 0 ) {
		if( strcmp( str1, "_text" ) == 0 ) {
			ker_dst_base = addr;
		} else if( strcmp( str1, "kernel_entry" ) == 0 ) {
			ker_dst_entry = addr;
		} else if( strcmp( str1, "__initramfs_start" ) == 0 ) {
			ker_dst_end = addr;
		}
	}
	fclose( fp_system_map );

	printf( "[CREATE VMLINUX IMAGE]\r\n" );
	system( "cp arch/mips/boot/vmlinux.bin vmlinux.bin" );
	system( "gzip -f vmlinux.bin > vmlinux.bin.gz" );
	memset( cmd_str, 0, 512 );
	sprintf( cmd_str, "mkimage -n 'linux-3.0.20' -A mips -O linux -T kernel -C gzip -a 0x%08X -e 0x%08X -d vmlinux.bin.gz vmlinux.img", (unsigned int)ker_dst_base, (unsigned int)ker_dst_entry );
	system( cmd_str );
	system( "rm -f vmlinux.bin.gz" );
	printf( "\r\n");

	printf( "[CREATE INITRAMFS IMAGE]\r\n" );
	memset( cmd_str, 0, 512 );
	system( "chmod 644 initramfs.bin" );
	sprintf( cmd_str, "mkimage -n 'initramfs' -A mips -O linux -T ramdisk -C none -a 0x%08X -d initramfs.bin initramfs.img", stHead.initramfs_start - sizeof( ST_INITRAMFS_HEAD ) );
	system( cmd_str );
	printf( "\r\n");

	return 0;
}
