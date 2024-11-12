#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

int main() {
    cv::VideoCapture capture(0);
    if (!capture.isOpened()) {
        return 0;
    }
    int frame_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    while (true) {
        cv::Mat frame;
        capture >> frame;
        if (frame.empty()) break;
        cv::Mat smooth_frame;
        cv::GaussianBlur(frame, smooth_frame, cv::Size(9, 9), 0);
        cv::imshow("Original", frame);
        cv::imshow("Smooth", smooth_frame);

        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        double fps = 1.0 / elapsed.count();
        start_time = current_time;
        std::cout << "Frames per second: " << fps << std::endl;

        char c = (char)cv::waitKey(33);
        if (c == 27) break;
        frame_count++;
    }

    capture.release();
    cv::destroyAllWindows();
    return 0;
}
