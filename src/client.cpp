#include "udp_client.hpp"

void signal_handler(int signal) {
  std::cout << "Received signal " << signal << std::endl;
  exit(0);
}

int main(int argc, char *argv[]) {
  std::signal(SIGINT, signal_handler);
  if (argc != 3) {
    std::cerr << "Usage: ./client <address> <port: 0-65535>" << std::endl;
    return 1;
  }
  int port = 0;
  std::string address = argv[1];
  try {
    port = atoi(argv[2]);
    if (port > 65535 || port < 0) {
      throw;
    }
  } catch (std::exception &e) {
    std::cerr << "Port " << argv[2] << " is not valid!" << std::endl;
    return 1;
  }
  try {
    asio::io_context io_context;
    udp_client client(io_context, address, port);
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
