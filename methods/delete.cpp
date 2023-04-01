#include "./http_TcpServer.hpp"

void  delete(Parsed *data)
{
  ifstream file;

  file.open(data->req->absoluteURI);
  if (file)
  {
    int status;
    if(!remove(file))
      std::cout << "file exist 200 (OK)" << std::endl;
    else
      std::cout << "202 (Accepted, deletion is not a single operation)." << std::endl;
  }
  else
    std::cout << "file doesn't exist 204" << std::endl;
}

int main()
{
}