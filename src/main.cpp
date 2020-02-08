#include <asio.hpp>
#include "udp-server.hpp"

int main() {
  try {
    asio::io_service io_service;
    udp_server server(io_service, 4200);
    io_service.run();
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
