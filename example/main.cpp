#include <iostream>
#include <string>
#include <array>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include "types.h"
#include "helpers.h"


using namespace std;
using namespace boost::asio;
using           boost::asio::ip::tcp;
using namespace boost::asio::error;
using           boost::intrusive_ptr;
using           boost::intrusive_ref_counter;
using           boost::intrusive_from_raw;
using boost_error = boost::system::error_code;


constexpr auto max_length = size_t(8192);
const     auto message = R"(HTTP/1.1 200 OK
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


class server
{
  private:
    constexpr static auto default_port = ushort(4567);

    io_service _service;
    tcp::acceptor _acceptor;
    tcp::socket _socket;


  public:
    server(ushort port = default_port) :
      _acceptor(_service, tcp::endpoint(tcp::v4(), port)),
      _socket(_service)
    {
      accept();

      cout << "Starting tcp server at \033[34m0.0.0.0:" << port << "\033[0m" << endl
        << "Check this out with the command below" << endl
        << endl
        << " $ \033[4;34mwget http://127.0.0.1:" << port << "\033[0m" << endl
        << endl;
    }

    ~server()
    {
      cout << "Bye!" << endl;
    }

    auto run() -> void
    {
      try
      {
        _service.run();
      }
      catch (exception& e)
      {
        cerr << "\033[31mException has been thrown\033[0m" << endl;
        cerr << "\033[33m" << e.what() << "\033[0m" << endl;
        cerr << endl;
      }
    }


  private:
    auto accept() -> void
    {
      _acceptor.async_accept(_socket, [this](boost::system::error_code error)
      {
        if (error) return; // TODO : 에러처리

        spawn(_service, [&](auto yield)
        {
          auto socket = move(_socket);
          auto client = socket.remote_endpoint();

          cout << to_string(client) << " has been connected" << endl;
          cout << endl;

          array<byte, max_length> request;
          boost_error error;

          socket.async_read_some(buffer(request), yield[error]);
          if (error) return; // TODO : 에러처리

          cout << to_string(client) << " has sent a request" << endl;
          cout << "\033[33m" << string(request.begin(), request.end()) << "\033[0m" << endl;

          async_write(socket, buffer(message), yield[error]);
          if (error) return; // TODO : 에러처리

          socket.shutdown(tcp::socket::shutdown_both, error);

          cout << to_string(client) << " close her connection gracefully" << endl;
          cout << endl;
        });

        accept();
      });
    }
};


auto main() -> int
{
  server app;
  app.run();
}
