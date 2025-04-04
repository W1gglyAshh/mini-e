#pragma once

//
// the definitions of class Platform (abstract), UnixPl and WinPl (platform specific methods)
//

#include <unistd.h>
#include <string>
#include <memory>

class Pl
{
  public:
    static void draw(const std::string &str) { write(STDOUT_FILENO, str.c_str(), str.length()); }

    virtual ~Pl() = default;

    // enter raw mode, enable alternate screen buffer and mouse control
    virtual void init() = 0;
    virtual void cleanup() = 0;

    virtual void getWinsize(int &width, int &height) = 0;
    virtual void setCursorPos(int row, int col) = 0;

    static std::unique_ptr<Pl> createPlInstance()
    
};

//
//

#if defined(__unix__) || defined(__APPLE__)

#include <termios.h>

class UnixPl : public Pl
{
  private:
    struct termios orig;

    bool is_initialized;

  public:
    UnixPl();
    ~UnixPl();

    void init() override;
    void cleanup() override;

    void getWinsize(int &width, int &height) override;
    void setCursorPos(int row, int col) override;
}

#elif defined(_WIN32) || defined(_WIN64)

// TODO: WinPl class definition

#endif

// create a pointer pointing to the correct platform child class
inline static std::unique_ptr<Pl> createPlInstance()
{
#if defined(__unix__) || defined(__APPLE__)
        return std::make_unique<UnixPl>();
#elif defined(_WIN32) || defined(_WIN64)
        return std::make_unique<WinPl>();
#else
#error "Unsupported platform!"
        return nullptr;
#endif
}
