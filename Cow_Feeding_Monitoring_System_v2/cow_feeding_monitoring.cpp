#include "cow_feeding_monitoring.h"
#include "ui_cow_feeding_monitoring.h"

#include <QTimer>
#include <QString>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include "raspicam_cv.h"
#include "raspicam.h"
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
using namespace cv;
using namespace std;
Mat input_image;
int Nodenumber = 26;
Cow_Feeding_Monitoring::Cow_Feeding_Monitoring(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cow_Feeding_Monitoring)
{
    ui->setupUi(this);

    //Start the timer
    QTimer *ClockTimer = new QTimer(this);
    ClockTimer->start(1000); //以1000毫秒為周期起動定時器
    connect(ClockTimer,SIGNAL(timeout()),this,SLOT(showTime())); //Update the date time of LCD

    //Initialize the detect Thread
    detect_thread1 = new detect_thread();
    ui->textBrowser->append("Cow feeding monitoring system");

    //connect
    connect(detect_thread1, SIGNAL(eat_image_get(QString)), this,SLOT(show_eating_image(QString)));

    connect(detect_thread1, SIGNAL(leave_image_get(QString)), this,SLOT(show_leaving_image(QString)));

    connect(detect_thread1, SIGNAL(image_get(QString,QString)), this,SLOT(show_image(QString,QString)));
}
Cow_Feeding_Monitoring::~Cow_Feeding_Monitoring()
{
    delete ui;
}

void Cow_Feeding_Monitoring::showTime()
{
    QTime time = QTime::currentTime();
    QString stime=time.toString("hh:mm:ss"); //設定顯示時間格式
    ui->lcdNumber->display(stime);
}
void Cow_Feeding_Monitoring::show_console(QString str)
{
    ui->textBrowser->append(str);
}

QImage Cow_Feeding_Monitoring::Mat2QImage(const cv::Mat src)
{
    if(src.type()==CV_8UC1)
    {
         QVector<QRgb> colorTable;
         for (int i = 0; i < 256; i++)
             colorTable.push_back(qRgb(i,i,i));
         const uchar *qImageBuffer = (const uchar*)src.data;
         QImage img(qImageBuffer, src.cols, src.rows, src.step, QImage::Format_Indexed8);
         img.setColorTable(colorTable);
         return img;
    }
    if(src.type()==CV_8UC3)
    {
        const uchar *qImageBuffer = (const uchar*)src.data;
        QImage img(qImageBuffer, src.cols, src.rows, src.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "Error! Can't convert image type.";
         return QImage();
    }
 }

//Capture the image from pi camera for fence testing
void Cow_Feeding_Monitoring::on_Capture_Btn_clicked()
{
    raspicam::RaspiCam_Cv Camera;
    Mat show_image;

    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );

    //Open camera
    if (!Camera.open()) {
        ui->textBrowser->append("Error opening the camera");
    }
    else
    {
        //Capture the image
        Camera.grab();
        Camera.retrieve (input_image);
        if(ui->vertical_flip_radioButton->isChecked())
            cv::flip(input_image,input_image,-1);
        ui->textBrowser->append("The size of capture image: "+QString::number(input_image.cols)+" "+QString::number(input_image.rows));

        //Enhacement image contrast
        detect_thread1->liner_transform(input_image,input_image,1.5,15);

        // Resize the image to fit the display screen
        cv::resize(input_image, show_image, cv::Size(ui->Detect_Img_label->width(), ui->Detect_Img_label->height()));
        //Display
        ui->Capture_Img_label->setPixmap(QPixmap::fromImage(this->Mat2QImage(show_image)));

    }
    Camera.release(); //Release Camera
}

void Cow_Feeding_Monitoring::on_Fence_par_ok_Btn_clicked()
{
    detect_thread *a = new detect_thread();
    a->getconfiguration(ui->recx0->value(),ui->recx1->value(),ui->recx2->value(),ui->recx3->value(),
                       ui->recy->value(),ui->recwidth0->value(),ui->recwidth1->value(),ui->recwidth2->value(),
                       ui->recwidth3->value(),ui->recheight->value(),ui->Threshold_spinBox->value(),ui->vertical_flip_radioButton->isChecked());
    ui->textBrowser->append("");
    ui->textBrowser->append("Update the parameter");
    ui->textBrowser->append("--------------------------------------------------");
    if(ui->vertical_flip_radioButton->isChecked())
        ui->textBrowser->append("vertical flip is on ");
    else
        ui->textBrowser->append("vertical flip is off ");

    QString rect_pos = "x0:"+ui->recx0->text()+" x1:"+ui->recx1->text()+" x2:"+ui->recx2->text()+" x3:"+ui->recx3->text();
    QString rect_width = "w0:"+ui->recwidth0->text()+" w1:"+ui->recwidth1->text()+" w2:"+ui->recwidth2->text()+" w3:"+ui->recwidth3->text();
    QString rect_height = "y:"+ui->recy->text()+" h:"+ui->recheight->text();
    ui->textBrowser->append(rect_pos);
    ui->textBrowser->append(rect_width);
    ui->textBrowser->append(rect_height);
    ui->textBrowser->append("--------------------------------------------------");
}

void Cow_Feeding_Monitoring::on_StartButton_clicked()
{
    this->status = true;
    if(ui->picamera_radioButton->isChecked())
    {
        ui->textBrowser->append("Opened the camera successfully");

        detect_thread1->getconfiguration(ui->recx0->value(),ui->recx1->value(),ui->recx2->value(),ui->recx3->value(),
                                  ui->recy->value(),ui->recwidth0->value(),ui->recwidth1->value(),ui->recwidth2->value(),
                                  ui->recwidth3->value(),ui->recheight->value(),ui->Threshold_spinBox->value(),ui->vertical_flip_radioButton->isChecked());
        detect_thread1->start();
    }

}

void Cow_Feeding_Monitoring::on_StopButton_clicked()
{
    if(detect_thread1->isRunning())
    {
        ui->textBrowser->append("Closed the Camera...");
        detect_thread1->receiveStopCam(false);
        //thread1->stop();
    }
    else
    {
        ui->textBrowser->append("You haven't start the camera!");
    }

}

void Cow_Feeding_Monitoring::on_Test_rec_pos_clicked()
{
    Mat image_line;
    image_line = input_image.clone();


    float recx0 = ui->recx0->value();
    float recx1 = ui->recx1->value();
    float recx2 = ui->recx2->value();
    float recx3 = ui->recx3->value();
    float recy =  ui->recy->value();

    float recwidth0 = ui->recwidth0->value();
    float recwidth1 = ui->recwidth1->value();
    float recwidth2 = ui->recwidth2->value();
    float recwidth3 = ui->recwidth3->value();

    float recheight = ui->recheight->value();
    //Define the point for drawing the 4 ROI
    Point pt0s = Point(static_cast<int>(recx0),static_cast<int>(recy));
    Point pt1s = Point(static_cast<int>(recx1),static_cast<int>(recy));
    Point pt2s = Point(static_cast<int>(recx2),static_cast<int>(recy));
    Point pt3s = Point(static_cast<int>(recx3),static_cast<int>(recy));

    Point pt0e = Point(static_cast<int>(recx0+recwidth0),static_cast<int>(recy+recheight));
    Point pt1e = Point(static_cast<int>(recx1+recwidth1),static_cast<int>(recy+recheight));
    Point pt2e = Point(static_cast<int>(recx2+recwidth2),static_cast<int>(recy+recheight));
    Point pt3e = Point(static_cast<int>(recx3+recwidth3),static_cast<int>(recy+recheight));

    //Draw 4 ROI
    cv::rectangle(image_line,pt0s,pt0e,Scalar(0,0,255),3);
    cv::rectangle(image_line,pt1s,pt1e,Scalar(255,0,255),3);
    cv::rectangle(image_line,pt2s,pt2e,Scalar(255,0,0),3);
    cv::rectangle(image_line,pt3s,pt3e,Scalar(0,255,255),3);

    ui->textBrowser->append("Draw ROI for testing");
    // Resize the image to fit the display screen
    cv::resize(image_line, image_line, cv::Size(ui->Detect_Img_label->width(), ui->Detect_Img_label->height()));
    //Display
    ui->Detect_Img_label->setPixmap(QPixmap::fromImage(this->Mat2QImage(image_line)));
}

void Cow_Feeding_Monitoring::show_eating_image(QString str )
{

    cv::Mat img = cv::imread(str.toStdString());
    if(!img.empty()){
        // Resize the image to fit the display screen
        cv::resize(img, img, cv::Size(ui->Eating_Img_label->width(), ui->Eating_Img_label->height()));
        //Display
        ui->Eating_Img_label->setPixmap(QPixmap::fromImage(this->Mat2QImage(img)));


        image_send(str,Nodenumber,"NTU","eating");
        QFile eating_file(str);
        eating_file.remove();
    }
}
void Cow_Feeding_Monitoring::show_leaving_image(QString str )
{

    cv::Mat img = cv::imread(str.toStdString());
    if(!img.empty()){
        // Resize the image to fit the display screen
        cv::resize(img, img, cv::Size(ui->Leaving_Img_label->width(), ui->Leaving_Img_label->height()));
        //Display
        ui->Leaving_Img_label->setPixmap(QPixmap::fromImage(this->Mat2QImage(img)));

        image_send(str,Nodenumber,"NTU","leaving");
        QFile leaving_file(str);
        leaving_file.remove();
    }
}
void Cow_Feeding_Monitoring::show_image(QString img_path,QString type)
{
     cv::Mat img = cv::imread(img_path.toStdString());
     if (!img.empty()){
         if(type=="eating"){
             // Resize the image to fit the display screen
             cv::resize(img, img, cv::Size(ui->Eating_Img_label->width(), ui->Eating_Img_label->height()));
             //Display
             ui->Eating_Img_label->setPixmap(QPixmap::fromImage(this->Mat2QImage(img)));
             image_send(img_path,Nodenumber,"NTU","eating");
             QFile img_file(img_path);
             img_file.remove();
         }else if(type=="leaving"){
             // Resize the image to fit the display screen
             cv::resize(img, img, cv::Size(ui->Leaving_Img_label->width(), ui->Leaving_Img_label->height()));
             //Display
             ui->Leaving_Img_label->setPixmap(QPixmap::fromImage(this->Mat2QImage(img)));
             image_send(img_path,Nodenumber,"NTU","leaving");
             QFile img_file(img_path);
             img_file.remove();
         }else if(type=="confirm"){
            image_send(img_path,Nodenumber,"NTU","confirm");
            QFile img_file(img_path);
            img_file.remove();
         }
     }

}
bool Cow_Feeding_Monitoring::image_send(QString imagepath,int node,QString location,QString type)
{
    CURL *curl;
    CURLcode res;
    FILE *fd=NULL;

    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;
    fd = fopen(imagepath.toStdString().c_str(),"rb");
    if(!fd){
        qDebug()<<"Couldn't open image";
        return false;
    }else{
        curl_formadd(&post,&last,
                       CURLFORM_COPYNAME, "file",
                       CURLFORM_FILE, imagepath.toStdString().c_str(),
                       CURLFORM_END);
    }
    curl = curl_easy_init();
    std::stringstream ss;
    ss << node;

    string url="http://140.112.94.123:30000/DAIRY_COW/image_feed/RX_IMG_FEED.php?node=" + ss.str() + "&location=" + location.toStdString()+"&type="+type.toStdString();
    if(curl)
    {
        curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
        {
             fprintf(stderr, "curl_easy_perform() failed: %s\n",
                      curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        return true;
    }else
        return false;
    fclose(fd);

}
