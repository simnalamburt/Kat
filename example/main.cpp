#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>


using namespace std;
using namespace boost::asio;
using           boost::asio::ip::tcp;
using namespace boost::asio::error;


using byte = unsigned char;
using sbyte = signed char;
//    char
using wchar = wchar_t;
//    short
using ushort = unsigned short;
//    int
using uint = unsigned int;
using intptr = intptr_t;
using uintptr = uintptr_t;
//    long
using ulong = unsigned long;
using llong = long long;
using ullong = unsigned long long;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using int128 = __int128_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using uint128 = __uint128_t;


const auto port = ushort(4567);
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


auto main() -> int
{
  cout.sync_with_stdio(false);
  cout << "Starting tcp server at \033[34m0.0.0.0:" << port << "\033[0m" << endl;
  cout << "Check this out with the command below" << endl;
  cout << endl;
  cout << " $ \033[4;34mwget http://127.0.0.1:" << port << "\033[0m" << endl;
  cout << endl;

  try
  {
    io_service service;
    auto acceptor = tcp::acceptor(service, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
      auto socket = tcp::socket(service);
      acceptor.accept(socket);

      thread([&](auto socket)
      {
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
      }, move(socket)).detach();
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
