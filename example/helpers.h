#pragma once
#include <string>
#include <boost/asio.hpp>

namespace std
{
  auto to_string(const boost::asio::ip::tcp::endpoint& endpoint) -> string
  {
    return "\033[32m" + endpoint.address().to_string() + ':' + to_string(endpoint.port()) + "\033[0m";
  }
}
