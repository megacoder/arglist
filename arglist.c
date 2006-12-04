/*
 *------------------------------------------------------------------------
 * vim: ts=8 sw=8
 *------------------------------------------------------------------------
 * Author:   tf135c (James Reynolds)
 * Filename: arglist.c
 * Created:  2006-12-04 13:48:13
 *------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <alloca.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <gcc-compat.h>

static	char *		me = "arglist";

static	void				_sentinel0
process(
	char const * const		path,
	...
)
{
	va_list				ap;
	size_t				argc;
	char *				argv[ 512 ];
	pid_t				pid;

	puts( path );
	va_start( ap, path );
	for( argc = 0; argc < DIM( argv ); ++argc )	{
		argv[ argc ] = va_arg( ap, char * );
		if( !argv[ argc ] )	{
			break;
		}
		printf( "  argv[ %3d ] = '%s'\n", argc, argv[ argc ] );
	}
	va_end( ap );
	argv[ argc ] = NULL;
	switch( (pid = fork()) )	{
	default:
		/* I am the parent					*/
		{
			pid_t		obit;
			int		status;

			obit = waitpid( pid, &status, 0 );
			if( (obit != pid) && !WIFEXITED( status ) )	{
				fprintf( stderr, "%s bogus '%s'.\n", me, path );
			}
		}
		break;
	case -1:
		perror( "some forking error" );
		exit( 1 );
		/*NOTREACHED*/
	case 0:
		/* I am the child					*/
		execvp( path, argv );
		perror( path );
		exit( 1 );
	}
}

/*
 *------------------------------------------------------------------------
 * main: central control logic
 *------------------------------------------------------------------------
 */

int
main(
	int		argc		_unused,
	char * *	argv		_unused
)
{
	char *		bp;

	/* Figure out process name					*/
	me = argv[ 0 ];
	if( (bp = strrchr( me, '/' )) != NULL )	{
		me = bp + 1;
	}
	/*								*/
	process(
		"/bin/echo",
		"echo",
		"this",
		"sucks",
		NULL
	);
	/*								*/
	return( 0 );
}
