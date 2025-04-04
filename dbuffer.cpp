//
// implementation of DBuffer (double buffer) class
//

#include "buffer.hpp"
#include "platform.hpp"

//
//

DBuffer::DBuffer(int &w, int &h)
    : b_width(w), b_height(h), cached_width(0), cached_height(0), is_winsize_changed(false)
{;
    cached_width = b_width;
    cached_height = b_height;

    front.resize(cached_height, std::string(cached_width, ' '));
    back.resize(cached_height, std::string(cached_width, ' '));
}

//
//

void DBuffer::update(const TBuffer &tbuffer, const int v_offset, const int h_offset)
{
    // detect winsize changes
    if (cached_width != b_width || cached_height != b_height)
    {
        is_winsize_changed = true;
        cached_width = b_width;
        cached_height = b_height;

        front.resize(cached_height, std::string(cached_width, ' '));
        back.resize(cached_height, std::string(cached_width, ' '));
    }
    else
    {
        is_winsize_changed = false;
    }

    const std::vector<std::string> &ls = tbuffer.getLines();

    for (int i = 0; i < cached_height; ++i)
    {
        if (v_offset + i < ls.size())
        {
            std::string l = ls.at(v_offset + i);
            std::string visible;

            if (h_offset < l.length())
            {
                visible = l.substr(h_offset, cached_width);
            }
            else
            {
                visible = "";
            }
            back.at(i) =
                visible + std::string(std::max(0, cached_width - static_cast<int>(visible.length())), ' ');
        }
        else
        {
            back.at(i) = std::string(cached_width, ' ');
        }
    }
}

//
//

void DBuffer::render()
{
    // freeze the visible caret and reset cursor position
    Pl::draw("\x1b[s\x1b[H");

    for (int i = 0; i < cached_height; ++i)
    {
        // conditions for rendering 
        if (front.at(i) != back.at(i) || is_winsize_changed)
        {
            Pl::draw(back.at(i) + "\n");
        }
        else
        {
            Pl::draw("\n");
        }
    }

    std::swap(front, back);

    // restore cursor position after rendering
    Pl::draw("\x1b[u");
}

