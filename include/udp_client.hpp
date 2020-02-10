#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <asio.hpp>

class udp_client {
  public:
		udp_client();
		udp_client(asio::io_service& io_service, const std::string &address, int port);
    std::vector<char> get_recv_buffer();

  private:
    void start_send();
    void start_receive();
    void handle_receive(const asio::error_code &error, std::size_t bytes_transferred);
    void handle_send(const std::string &message, const asio::error_code &error, std::size_t bytes_transferred);

    int m_port;
    std::vector<char> m_recv_buffer;
    std::mutex m_recv_buffer_mutex;
    const std::string m_address;
    asio::ip::udp::socket m_socket;
    asio::ip::udp::endpoint m_endpoint;
};

#endif
