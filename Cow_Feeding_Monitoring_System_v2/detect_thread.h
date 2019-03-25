#ifndef DETECT_THREAD_H
#define DETECT_THREAD_H

#include <QThread>
#include <QLabel>
#include <QWidget>
//#include <QObject>

#include "opencv2/opencv.hpp"
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>

using namespace cv;
using namespace cv::dnn;
class detect_thread : public QThread
{
Q_OBJECT
public:
    detect_thread();
    ~detect_thread();
    void receiveStopCam(const bool &status);
    void getconfiguration(int recx0, int recx1, int recx2, int recx3, int recy, int recwidth0
                                 , int recwidth1, int recwidth2, int recwidth3, int recheight, float threshold, bool flip_vertical);
    void Hist_qualiz(cv::Mat &input_img);
    void liner_transform(cv::Mat input_img,cv::Mat &ouput_img, double alpha, double beta);

protected:
    virtual void run();

private:
    int rec_x[4] = {0};
    int rec_y[4] = {0};
    int rec_width[4] = {0};
    int rec_height[4] = {0};
    float count_threshold = 0.2;
    bool flip_status = false;
    bool status;

    //function set
    bool detect_BY_YOLO( Mat frame, std::vector<Rect> &cowfaces, std::vector<float> &probability);
    float IOU_calculation(cv::Point fence_bottomright,cv::Point fence_topleft  ,cv::Rect cow_faces_vect);

    void mkdir(QString work_dir_str, QString hist_dir_str);
    void WriteCountFile(int fence,QDateTime Cow_start_eating_time,int cow_count,int VotedCount, QDateTime datetime);
    void classification(Net net , Mat input_frame, int &classid, float &probability);
    void write_csvfile(QString save_dir,QString csv_file_name,QString str);

signals:
    void eat_image_get(QString str);
    void leave_image_get(QString str);
    void image_get(QString str1,QString str2);
    //void send_log(QString str);
};

#endif // DETECT_THREAD_H
