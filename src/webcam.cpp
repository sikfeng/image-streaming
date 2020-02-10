#include "webcam.hpp"

webcam::webcam(): webcam(0) {}

webcam::webcam(int index) {
  m_camera.open(index);
  m_camera.set(cv::CAP_PROP_FPS, 30);
  m_camera.set(cv::CAP_PROP_BUFFERSIZE, 3);
  m_camera >> m_frame;
  m_update_frames_async = std::async(
    std::launch::async, std::bind(&webcam::update_frames, this)
  );
}

std::vector<uchar> webcam::encode() {
  try {
    std::lock_guard<std::mutex> guard(m_frame_mutex);
    std::vector<uchar> buffer;
    bool result = cv::imencode(".jpg", m_frame, buffer, m_params);
    if (!result) {
     throw;
    }
    return buffer;
  } catch (const cv::Exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void webcam::update_frames() {
  while (true) {
    if (m_terminate_update) {
      std::cout << "Terminated updating frames" << std::endl;
      return;
    }
    try {
      std::lock_guard<std::mutex> guard(m_frame_mutex);
      m_camera >> m_frame;
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}

