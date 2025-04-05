//
// the implementation of UnixPl class
//

// won't be compiled if aren't on the correct platform
#if defined(__unix__) || defined(__APPLE__)

#include "platform.hpp"

#include <unistd.h>
#include <termios.h>
#include <cstdio>
#include <sys/ioctl.h>
#include <string>
#include <cstdlib>

UnixPl::UnixPl() : is_initialized(false)
{
    memset(&orig, 0, sizeof(orig));
}

UnixPl::~UnixPl()
{
    cleanup();
}

//
//

void UnixPl::init()
{
    Pl::draw("Init called");
    if (is_initialized)
    {
        Pl::draw("Checking reinit");
        return;
    }
    Pl::draw("Getting terminal state");

    if (!isatty(STDIN_FILENO))
    {
        Pl::draw("\x1b[91mFatal error\x1b[0m: stdin is not a terminal device.");
        exit(EXIT_FAILURE);
    }
    if (tcgetattr(STDIN_FILENO, &orig) == -1)
    {
        Pl::draw("\x1b[91mFatal error\x1b[0m: unable to initialize editor! Error code: 1.");
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
        Pl::draw("\x1b[91mFatal error\x1b[0m: unable to initialize edtior! Error code: 2.");
        exit(EXIT_FAILURE);
    }

    // enable alternate screen buffer
    Pl::draw("\x1b[?1049h");
    // enable mouse support
    Pl::draw("\x1b[1000h\x1b[1002h\x1b[1006h\x1b[1007h");

    is_initialized = true;
}

//
//

void UnixPl::cleanup()
{
    if (!is_initialized)
    {
        return;
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);

    Pl::draw("\x1b[?1049l");
    Pl::draw("\x1b[1000l\x1b[1002l\x1b[1006l\x1b[1007l");

    is_initialized = false;
}

//
//

void UnixPl::getWinsize(int &width, int &height)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 || ws.ws_col == 0)
    {
        // default value if failed getting terminal window size
        width = 80;
        height = 24;
    }
    else
    {
        width = ws.ws_col;
        height = ws.ws_row;
    }
}

//
//

void UnixPl::setCursorPos(int row, int col)
{
    // 1 based
    Pl::draw("\x1b[" + std::to_string(row + 1) + ";" + std::to_string(col + 1) + "H");
}

#endif

