//
// the implementation of UnixPl class
//

// won't be compiled if aren't on the correct platform
#if defined(__unix__) || (__APPLE__)

#include "platform.hpp"

#include <unistd.h>
#include <termios.h>
#include <cstdio>
#include <sys/ioctl.h>
#include <cstring>
#include <cstdlib>

UnixPl::UnixPl() : is_initialized(false)
{
    init();
}

UnixPl::~UnixPl()
{
    cleanup();
}

//
//

void UnixPl::init()
{
    if (is_initialized)
    {
        return;
    }
    if (tcgetattr(STDIN_FILENO, &orig) == -1)
    {
        const char *err = 
            "\x1b[91mFatal error\x1b[0m: unable to initialize editor!\nError code: 1. Please view the documentation on GitHub for more information\n";
        write(STDOUT_FILENO, err, strlen(err));
        exit(EXIT_FAILURE);
    }

    struct termios raw = orig;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
    }        
}

#endif
