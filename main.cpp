#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

int main() {
    cv::VideoCapture capture(0);
    if (!capture.isOpened()) {
        return 0;
    }
    while (true) {
        auto frame_start_time = std::chrono::high_resolution_clock::now();

        auto input_start_time = std::chrono::high_resolution_clock::now();
        cv::Mat frame;
        capture >> frame;
        auto input_end_time = std::chrono::high_resolution_clock::now();
        if (frame.empty()) break;

        auto transform_start_time = std::chrono::high_resolution_clock::now();
        cv::Mat smooth_frame;
        cv::GaussianBlur(frame, smooth_frame, cv::Size(9, 9), 0);
        auto transform_end_time = std::chrono::high_resolution_clock::now();

        auto show_start_time = std::chrono::high_resolution_clock::now();
        cv::imshow("Original", frame);
        cv::imshow("Smooth", smooth_frame);
        auto show_end_time = std::chrono::high_resolution_clock::now();

        if (cv::waitKey(1) == 27) break;

        auto frame_end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> input_elapsed = input_end_time - input_start_time;
        std::chrono::duration<double> transform_elapsed = transform_end_time - transform_start_time;
        std::chrono::duration<double> show_elapsed = show_end_time - show_start_time;
        std::chrono::duration<double> total_elapsed = frame_end_time - frame_start_time;

        double input_percent = (input_elapsed.count() / total_elapsed.count()) * 100.0;
        double transform_percent = (transform_elapsed.count() / total_elapsed.count()) * 100.0;
        double show_percent = (show_elapsed.count() / total_elapsed.count()) * 100.0;

        std::cout << "FPS: " << 1.0 / total_elapsed.count() << std::endl;
        std::cout << "Input time: " << input_percent << "%" << std::endl;
        std::cout << "Transformation time: " << transform_percent << "%" << std::endl;
        std::cout << "Show time: " << show_percent << "%" << std::endl << std::endl;
    }

    capture.release();   
    cv::destroyAllWindows();
    return 0;
}
