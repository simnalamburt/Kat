#include <iostream>
#include <string>
#include <array>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include "types.h"
#include "helpers.h"


using namespace std;
using namespace boost::asio;
using           boost::asio::ip::tcp;
using namespace boost::asio::error;
using boost_error = boost::system::error_code;


constexpr auto port = ushort(4567);
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


auto main() -> int
{
  io_service service;
  auto acceptor = tcp::acceptor(service, tcp::endpoint(tcp::v4(), port));
  auto socket = tcp::socket(service);

  spawn(service, [&](auto yield)
  {
    while (true)
    {
      boost_error error;
      acceptor.async_accept(socket, yield[error]);
      if (error) continue; // TODO : 에러처리

      spawn(service, [&](auto yield)
      {
        auto client = move(socket);
        auto info = to_string(client.remote_endpoint());

        cout << info << " has been connected" << endl;
        cout << endl;

        array<byte, max_length> request;
        boost_error error;

        client.async_read_some(buffer(request), yield[error]);
        if (error) return; // TODO : 에러처리

        cout << info << " has sent a request" << endl;
        cout << "\033[33m" << string(request.begin(), request.end()) << "\033[0m" << endl;

        async_write(client, buffer(message), yield[error]);
        if (error) return; // TODO : 에러처리

        client.shutdown(tcp::socket::shutdown_both, error);

        cout << info << " close her connection gracefully" << endl;
        cout << endl;
      });
    }
  });

  cout << "Starting tcp server at \033[34m0.0.0.0:" << port << "\033[0m" << endl
    << "Check this out with the command below" << endl
    << endl
    << " $ \033[4;34mwget http://127.0.0.1:" << port << "\033[0m" << endl
    << endl;

  service.run();

  cout << "Bye!" << endl;
}
