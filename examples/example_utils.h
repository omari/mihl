#ifdef __WINDAUBE__

#include <windows.h>
#include <conio.h>

static int
peekch( void ) 
{
    if ( kbhit() )
        return _getch();
    return -1;
}

static inline void
delay( int msec )
{
    Sleep( msec );
}

#else

#define __USE_XOPEN_EXTENDED
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static int initialized = 0;

static struct termios termattr, save_termattr;

static int
set_tty_raw( void ) 
{
    int i = tcgetattr( 0, &termattr );
    if ( i < 0 )  {
        printf( "%m\n" ); 
        fflush( stdout );
        return -1;
    }
    save_termattr = termattr;

    termattr.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    termattr.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    termattr.c_cflag &= ~(CSIZE | PARENB);
    termattr.c_cflag |= CS8 | OCRNL ;
    termattr.c_oflag &= ~(OPOST | ONOCR);

    termattr.c_cc[VMIN] = 1;  /* or 0 for some Unices*/
    termattr.c_cc[VTIME] = 0;

    i = tcsetattr( 0, TCSANOW, &termattr );
    if ( i < 0 ) {
        printf( "%m\n" ); 
        fflush( stdout );
    }

    return 0;
}



static void
bye_bye( void )
{
    if ( initialized )
        tcsetattr( 0, TCSAFLUSH, &save_termattr );
}


static int
peekch( void )
{
    if ( !initialized ) {
        initialized = 1;
        set_tty_raw( );
        atexit( bye_bye );
    }
    termattr.c_cc[VMIN] = 0;
    int i = tcsetattr( 0, TCSANOW, &termattr );
    unsigned char ch;
    ssize_t size = read( 0, &ch, 1 );
    termattr.c_cc[VMIN] = 1;
    i = tcsetattr( 0, TCSANOW, &termattr );
    if (size == 0)
        return -1;
    return ch;
}

static inline void
delay( int msec )
{
    usleep( msec*1000 );
}

#endif
