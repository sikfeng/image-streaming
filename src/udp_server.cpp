#include "udp_server.hpp"

udp_server::udp_server(asio::io_service& io_service, int port)
  : m_port(port), m_socket(io_service, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
  m_recv_buffer.resize(1, 0);
  start_receive();
}

void udp_server::start_receive() {
  /*
  asio::async_read(
    m_socket, asio::buffer(m_recv_buffer, 1), asio::transfer_exactly(1),
    std::bind(
      &udp_server::handle_receive, this,
      std::placeholders::_1, std::placeholders::_2
    )
  );
  */
  m_socket.async_receive_from(
    asio::buffer(m_recv_buffer, 1), m_endpoint,
    std::bind(
      &udp_server::handle_receive, this,
      std::placeholders::_1, std::placeholders::_2
    )
  );
  /*
  std::size_t bytes_transferred = m_socket.receive_from(
    asio::buffer(m_recv_buffer, 1), m_endpoint
  );
  std::cout << "Received " << bytes_transferred << " bytes: " << std::string(m_recv_buffer.begin(), m_recv_buffer.end()) << std::endl;

  std::string message = "0123456789";
  m_socket.async_send_to(
    asio::buffer(message), m_endpoint,
    std::bind(
      &udp_server::handle_send, this, message,
      std::placeholders::_1, std::placeholders::_2
    )
  );
  start_receive();
  */
}

void udp_server::handle_receive(const asio::error_code &error, std::size_t bytes_transferred) {
  if (!error) {
    std::cout << "Received " << bytes_transferred << " bytes: " << std::string(m_recv_buffer.begin(), m_recv_buffer.end()) << std::endl;

    std::string message = "0123456789";
    m_socket.async_send_to(
      asio::buffer(message), m_endpoint,
      std::bind(
        &udp_server::handle_send, this, message,
        std::placeholders::_1, std::placeholders::_2
      )
    );
    start_receive();
  }
}

void udp_server::handle_send(const std::string &message, const asio::error_code &error, std::size_t bytes_transferred) {
  if (!error) {
    std::cout << "Sent " << bytes_transferred << " bytes: " << message << std::endl;
  }
  return;
}
