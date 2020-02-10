#include "udp_client.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

void signal_handler(int signal) {
  std::cout << "Received signal " << signal << std::endl;
  exit(0);
}

//std::mutex frame_mutex;
//std::atomic<bool> terminate_display{false};

void display_frame(udp_client &client) {
  std::vector<char> raw_frame = client.get_recv_buffer();
  cv::Mat frame = cv::imdecode(raw_frame, cv::IMREAD_UNCHANGED);
  while (true) {
    //std::lock_guard<std::mutex> guard(frame_mutex);
    /*
    if (terminate_display) {
      return;
    }
    */
    try {
      raw_frame = client.get_recv_buffer();
      frame = cv::imdecode(raw_frame, cv::IMREAD_UNCHANGED);
      cv::imshow("frame", frame);
    } catch (cv::Exception &e) {
      std::cerr << e.what() << std::endl;
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
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
    std::future<void> display_frame_async = std::async(
      std::launch::async, std::bind(&display_frame, std::ref(client))
    );
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
