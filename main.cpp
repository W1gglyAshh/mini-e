#include "platform.hpp"
#include "buffer.hpp"

#include <memory>
#include <unistd.h>

int main(int argc, char **argv)
{
    std::unique_ptr<Pl> pl = Pl::createPlInstance();

    int w, h;
    pl->getWinsize(w, h);
    std::unique_ptr<TBuffer> tb = std::make_unique<TBuffer>(w, h);
    std::unique_ptr<DBuffer> db = std::make_unique<DBuffer>(w, h);

    if (tb->load(argv[1]).has_value())
    {
        Pl::draw("Bye!");
        return -1;
    }
    bool is_running = true;

    while(is_running)
    {
        db->update(*tb, tb->getVerticalOffset(), tb->getHorizontalOffset());
        db->render();

        char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n > 0)
        {
            switch (c)
            {
            case 'w':
                tb->scroll(Scroll::UP);
                break;
            case 's':
                tb->scroll(Scroll::DOWN);
                break;
            case 'a':
                tb->scroll(Scroll::LEFT);
                break;
            case 'd':
                tb->scroll(Scroll::RIGHT);
                break;
            case 'q':
                is_running = false;
                break;
            }
        }
    }
    return 0;
}
