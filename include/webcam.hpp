#ifndef WEBCAM_HPP
#define WEBCAM_HPP

#include <iostream>
#include <future>
#include <atomic>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

class webcam {
  public:
    webcam();
    webcam(int index);
    std::vector<uchar> encode();

  private:
    void update_frames();
    cv::VideoCapture m_camera;
    cv::Mat m_frame;
    std::vector<int> m_params{cv::IMWRITE_JPEG_QUALITY , 80};
    std::future<void> m_update_frames_async;
    std::mutex m_frame_mutex;
    std::atomic<bool> m_terminate_update{false};
};

#endif
