#pragma once

//
// class definition for TBuffer (text buffer) and DBuffer (double buffer) class
//

#include <vector>
#include <string>
#include <optional>
#include <memory>

//
//

enum class Scroll
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

//
//

class TBuffer
{
  private:
    std::vector<std::string> lines;

    int v_offset;
    int h_offset;

    int &b_width, &b_height;

  public:
    TBuffer(int &w, int &h);
    ~TBuffer() = default;

    // return error message in std::string, so it's optional
    // the core (Mn) class will handle empty filename, so there won't be default value here
    [[nodiscard]] std::optional<std::string> load(const std::string &fn);
    [[nodiscard]] std::optional<std::string> save(const std::string &fn);

    void insert(int row, int col, char c);
    void remove(int row, int col);

    void insert(int row, int col, const std::string &s);
    void remove(int row);

    void join(int row);
    void split(int row, int col)

    void scroll(Scroll d);

    [[nodiscard]] const std::vector<std::string> &getLines() const { return lines; }
    [[nodiscard]] const std::string &getLine(int row) const;
    [[nodiscard]] int getLineLength(int row) const;
    [[nodiscard]] int getLineCount() const;

    [[nodiscard]] int getHorizontalOffset() const;
    [[nodiscard]] int getVerticalOffset() const;
};

//
//

class DBuffer
{
  private:
    std::vector<std::string> front;
    std::vector<std::string> back;

    int &b_width, &b_height;
    int cached_width, cached_height;
    bool is_winsize_changed;

  public:
    DBuffer(int &w, int &h);
    ~DBuffer() = default;

    // these two methods should be called together in a loop
    void update(const TBuffer &tbuffer, int v_offset, int h_offset);
    void render();
};

