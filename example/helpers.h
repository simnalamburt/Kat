#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/intrusive_ptr.hpp>

namespace boost
{
  template <typename T>
  auto intrusive_from_raw(T* ptr) -> intrusive_ptr<T>
  {
    return intrusive_ptr<T>(ptr);
  }
}

namespace std
{
  auto to_string(const boost::asio::ip::tcp::endpoint& endpoint) -> string
  {
    return "\033[32m" + endpoint.address().to_string() + ':' + to_string(endpoint.port()) + "\033[0m";
  }
}
