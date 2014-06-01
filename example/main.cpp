#include <iostream>
#include <string>
#include <boost/asio.hpp>


using namespace std;
using namespace boost::asio;
using           boost::asio::ip::tcp;
using namespace boost::asio::error;


int main()
{
  const unsigned short port = 4567;
  const auto message = R"(HTTP/1.1 200 OK
Server: Kat
Content-Type: text/html;charset=utf-8

<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Kat!</title>
</head>
<body>
  <h1>Hello, World!</h1>
  <p>I'm Kat</p>
</body>
</html>
)"s;

  cout << "Starting tcp server at \033[34m0.0.0.0:" << port << "\033[0m" << endl;
  cout << "Check this out with the command below" << endl;
  cout << endl;
  cout << " $ \033[4;34mwget http://127.0.0.1:" << port << "\033[0m" << endl;
  cout << endl;

  try
  {
    io_service service;
    auto acceptor = tcp::acceptor(service, tcp::endpoint(tcp::v4(), port));
    for(;;)
    {
      tcp::socket socket(service);
      acceptor.accept(socket);

      const auto endpoint = socket.remote_endpoint();
      const auto client = "\033[32m" + endpoint.address().to_string() + ':' + to_string(endpoint.port()) + "\033[0m";
      cout << client << " has been connected" << endl;
      cout << endl;

      do
      {
        auto delim = "\r\n\r\n"s;

        boost::asio::streambuf request_buffer;
        boost::system::error_code error;
        read_until(socket, request_buffer, delim, error);
        if (error == eof)
          break; // Connection closed cleanly by peer.
        else if (error)
          throw boost::system::system_error(error); // Some other error.

        cout << client << " has sent a request" << endl;
        cout << "\033[33m" << string(
            buffer_cast<const char*>(request_buffer.data()),
            request_buffer.size() - delim.size()) << "\033[0m" << endl;
        cout << endl;

        write(socket, buffer(message));
      }
      while (false);

      cout << client << " closed her connection gracefully" << endl;
      cout << endl;
    }
  }
  catch (exception& e)
  {
    cerr << "\033[31mException has been thrown\033[0m" << endl;
    cerr << "\033[33m" << e.what() << "\033[0m" << endl;
    cerr << endl;
  }
  cout << "Bye!" << endl;
}
