#ifndef COW_FEEDING_MONITORING_H
#define COW_FEEDING_MONITORING_H

#include <QMainWindow>

#include <QLabel>
#include "opencv2/opencv.hpp"
#include "detect_thread.h"
namespace Ui {
class Cow_Feeding_Monitoring;
}

class Cow_Feeding_Monitoring : public QMainWindow
{
    Q_OBJECT

public:
    Ui::Cow_Feeding_Monitoring *ui;
    QImage Mat2QImage(const cv::Mat src) ;
    static bool image_send(QString imagepath, int node, QString location, QString type);
    explicit Cow_Feeding_Monitoring(QWidget *parent = 0);

    ~Cow_Feeding_Monitoring();


private:
    detect_thread *detect_thread1 = new detect_thread ;

    bool status;


private slots:
    void showTime();
    void on_Capture_Btn_clicked();
    void on_Fence_par_ok_Btn_clicked();
    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void on_Test_rec_pos_clicked();
public slots:
    void show_eating_image(QString str);
    void show_leaving_image(QString str);
    void show_console(QString str);
    void show_image(QString img_path,QString type);
};

#endif // COW_FEEDING_MONITORING_H
