#pragma once
#include <string>
#include <boost/asio.hpp>

namespace kat {

auto to_string(const boost::asio::ip::tcp::endpoint& endpoint) -> std::string
{
  return "\033[32m" + endpoint.address().to_string() + ':' + std::to_string(endpoint.port()) + "\033[0m";
}

} // namespace kat
