#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <asio.hpp>
#include "webcam.hpp"

class udp_server {
  public:
		udp_server();
		udp_server(asio::io_service& io_service, int port);

  private:
    void start_receive();
    void handle_receive(const asio::error_code &error, std::size_t bytes_transferred);
    void handle_send(const std::string &message, const asio::error_code &error, std::size_t bytes_transferred);

    int m_port;
    std::vector<char> m_recv_buffer;
    webcam m_webcam{0};
    asio::ip::udp::socket m_socket;
    asio::ip::udp::endpoint m_endpoint;
};

#endif
