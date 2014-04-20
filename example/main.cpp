#include <iostream>
#include <string>
#include <boost/asio.hpp>


using namespace std;
using namespace boost::asio;
using           boost::asio::ip::tcp;


int main()
{
  const unsigned short port = 4567;

  cout << "Starting tcp server at \033[34m0.0.0.0:" << port << "\033[0m" << endl;
  cout << "Check this out with the command below" << endl;
  cout << endl;
  cout << " $ \033[4;34mnc 127.0.0.1 " << port << "\033[0m" << endl;
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

      write(socket, buffer("Hello, World!\n"));
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
