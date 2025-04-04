//
// implementation of TBuffer (text buffer) class
//

#include "buffer.hpp"

#include <fstream>
#include <optional>
#include <string>

//
//

TBuffer::TBuffer(int &w, int &h) : b_width(w), b_height(h), v_offset(0), h_offset(0)
{
    // ensure at least one line exist in the vector
    lines.clear();
    lines.emplace_back("");
}

//
//

std::optional<std::string> TBuffer::load(const std::string &fn)
{
    if (fn.empty())
    {
        return "Empty filename!";
    }
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        lines.clear();
        file.open(fn);

        std::string tmp_str;
        while (getline(file, tmp_str))
        {
            lines.emplace_back(tmp_str);
        }

        file.close();
    }
    catch (std::ifstream::failure &e)
    {
        return std::string("Error reading file: ") + e.what();
    }

    if (lines.empty())
    {
        // ensure at least one line exist
        lines.emplace_back("");
    }
    return {};
}

//

std::optional<std::string> TBuffer::save(const std::string &fn)
{
    if (fn.empty())
    {
        return "Empty filename!";
    }
    std::ofstream file;

    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try
    {
        file.open(fn);

        for (size_t i = 0; i < lines.size(); ++i)
        {
            file << lines.at(i);
            if (i < lines.size() - 1)
            {
                file << "\n";
            }
        }
        file << "\n";

        file.close();
    }
    catch (std::ofstream::failure &e)
    {
        return std::string("Error writing file: ") + e.what();
    }
    return {};
}

//
//

void TBuffer::insert(const int row, const int col, char c)
{
    if (row >= 0 && row < static_cast<int>(lines.size()) && col >= 0 &&
        col <= static_cast<int>(lines.at(row).length()))
    {
        lines.at(row).insert(col, 1, c);
    }
}

void TBuffer::remove(const int row, const int col)
{
    if (row >= 0 && row < static_cast<int>(lines.size()) && col >= 0 &&
        col < static_cast<int>(lines.at(row).length()))
    {
        lines.at(row).erase(col, 1);
    }
}

//
//

void TBuffer::insert(const int row, const std::string &s)
{
    if (row >= 0 && row <= static_cast<int>(lines.size()))
    {
        lines.insert(lines.begin() + row, s);
    }
}

void TBuffer::remove(const int row)
{
    if (row >= 0 && row < static_cast<int>(lines.size()))
    {
        lines.erase(lines.begin() + row);
    }
}

//
//

void TBuffer::join(const int row)
{
    if (row >= 0 && row < static_cast<int>(lines.size()) - 1)
    {
        lines.at(row) += lines.at(row + 1);
        remove(row + 1);
    }
}

void TBuffer::split(const int row, const int col)
{
    if (row >= 0 && row < static_cast<int>(lines.size()) && col >= 0 &&
        col <= static_cast<int>(lines.at(row).length()))
    {
        std::string new_l = lines.at(row).substr(col);
        lines.at(row).erase(col);
        insert(row + 1, new_l);
    }
}

//
//

const std::string &TBuffer::getLine(int row) const
{
    if (row >= 0 && row < static_cast<int>(lines.size()))
        return lines.at(row);

    static const std::string empty;
    return empty;
}

int TBuffer::getLineLength(int row) const
{
    if (row >= 0 && row < static_cast<int>(lines.size()))
    {
        return lines.at(row).length();
    }
    return 0;
}

int TBuffer::getSize() const
{
    int s = 0;
    for (size_t i = 0; i < lines.size(); i++)
    {
        s += (lines.at(i)).length();
    }
    return s;
}

//
//

void TBuffer::scroll(Scroll d)
{
    if (d == Scroll::UP)
    {
        if (v_offset > 0)
        {
            v_offset--;
        }
    }
    else if (d == Scroll::DOWN)
    {
        if (v_offset + b_height < lines.size())
        {
            v_offset++;
        }
    }
    else if (d == Scroll::LEFT)
    {
        if (h_offset > 0)
        {
            h_offset--;
        }
    }
    else if (d == Scroll::RIGHT)
    {
        size_t max_l = 0;
        for (const auto &l : lines)
        {
            max_l = std::max(max_l, l.length());
        }

        if (h_offset + b_width < max_l)
        {
            h_offset++;
        }
    }
}

