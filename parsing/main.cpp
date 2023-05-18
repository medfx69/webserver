#include "request.hpp"

int main()
{
    request r;
    client cl("txt", 0, 0, 0, 0, 0);
    cl.read_len = 0;
    cl.readed = 0;
    cl.flag = 1;
    cl.chunked = 1;
    r.handle_body(&cl, "a\r\nali bay");
    r.handle_body(&cl, "ar \r\n5\r\nali ");
    r.handle_body(&cl, "B\r\n0\r\n");
}
