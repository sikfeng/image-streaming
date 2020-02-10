#include "udp_client.hpp"

udp_client::udp_client(asio::io_service& io_service, const std::string &address, int port)
  : m_address(address), m_port(port), m_socket(io_service), m_endpoint(asio::ip::udp::endpoint(asio::ip::address::from_string(address), port)) {
  m_recv_buffer.resize(40000, 0);
  m_socket.open(asio::ip::udp::v4());
  //m_socket.set_option(asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{500});
  start_send();
}

void udp_client::start_send() {
	std::string message = "0";
	m_socket.async_send_to(
		asio::buffer(message), m_endpoint,
		std::bind(
			&udp_client::handle_send, this, message,
			std::placeholders::_1, std::placeholders::_2
		)
	);
}

std::vector<char> udp_client::get_recv_buffer() {
  std::lock_guard<std::mutex> guard(m_recv_buffer_mutex);
  return m_recv_buffer;
}

void udp_client::handle_send(const std::string &message, const asio::error_code &error, std::size_t bytes_transferred) {
  if (!error) {
    std::cout << "Sent " << bytes_transferred 
              << " bytes: "  << message << std::endl;
    /*
    m_socket.async_receive_from(
			asio::buffer(m_recv_buffer, 10000), m_endpoint,
			std::bind(&udp_client::handle_receive, this,
				std::placeholders::_1,
				std::placeholders::_2
      )
    );
    */
    m_recv_buffer_mutex.lock();
    std::size_t bytes_transferred = m_socket.receive_from(asio::buffer(m_recv_buffer, 40000), m_endpoint);
    m_recv_buffer_mutex.unlock();
    std::cout << "Received " << bytes_transferred << " bytes" << std::endl;; 
    start_send();
  }
  return;
}

void udp_client::handle_receive(const asio::error_code &error, std::size_t bytes_transferred) {
  if (!error) {
    std::cout << "Received " << bytes_transferred << " bytes: "  
              << std::string(m_recv_buffer.begin(), m_recv_buffer.end()) 
              << std::endl;
  }
  start_send();
  return;
}
