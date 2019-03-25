#include "detect_thread.h"
#include "cow_feeding_monitoring.h"

#include <QDebug>
#include <QtSql>
#include <QSqlDriver>
#include "qsqldatabase.h"
#include <QSqlError>
#include <QPluginLoader>
#include <QQueue>
#include "raspicam_cv.h"


using namespace cv;
using namespace cv::dnn;
using namespace std;

Mat detect_confirm_image[4];
cv::dnn::Net net,classify_net ;
std::vector<String> classNamesVec,classifyclassNames;
bool bbox_x_compare(Rect rec1,Rect rec2);
int find_max_value_index(int arr[]);


int inpWidth = 224;
int inpHeight = 224;
int face_center_x[4] = {0};
int face_center_y[4] = {0};
int total_cow_num =19;
QString hist_dir_str= "";
int show_eat_img[4]={0};

typedef struct {
    int fence_num;
    int cur_fence_status ;
    int pre_fence_status ;
    int eat_status;
    int leave_status;
    char cow_name;
    QDateTime start_time;
    QDateTime leave_time;
    cv::Rect cow_position;
    int cow_id;
    double probability_indiv_cow;
    QQueue<int> fence_status;
    int voted_count ;
    int cow_id_ar[19];
    QQueue<Mat> q_cowface;
}fence;

void reset_fence_status(fence &fence_vec); //use call by reference to pass fence_vec

detect_thread::detect_thread()
{
    //Load detection model
    const cv::String modelConfiguration = "/home/pi/Cow_Feeding_Monitoring_System_v2/tiny-yolo-voc-cowface_achr_5.cfg";
    const cv::String modelBinary = "/home/pi/Cow_Feeding_Monitoring_System_v2/tiny-yolo-voc-cowface_achr_5_18000.weights";
    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelBinary);

    //Load detection label
    ifstream classNamesFile;
    classNamesFile.open("cowface.names");
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }

    //Load classification model
    const cv::String classify_model = "/home/pi/Cow_Feeding_Monitoring_System_v2/mobilenet_class18_final_graph.pb";
    classify_net = readNetFromTensorflow(classify_model);

    //Load classification label
    ifstream classNamesFile2;
    classNamesFile2.open("/home/pi/Cow_Feeding_Monitoring_System_v2/Individual_Cow_face_labels.txt");
    if (classNamesFile2.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile2, className))
            classifyclassNames.push_back(className);
    }
    else
        qDebug()<<"Cannot find classification label file.";
}
detect_thread::~detect_thread()
{
    wait();
}



void detect_thread::receiveStopCam(const bool &status)
{
    this->status = status;
}

bool detect_thread::detect_BY_YOLO( Mat frame, std::vector<Rect> &cowfaces, std::vector<float> &probability)
{

    cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1 / 255.F, cv::Size(320, 320), cv::Scalar(), true,false); //Convert Mat to batch of images
    net.setInput(inputBlob, "data");                         //set the network input
    cv::Mat detectionMat = net.forward("detection_out");     //compute output

    float confidenceThreshold = 0.6;

    for (int i = 0; i < detectionMat.rows; i++)
    {
        const int probability_index = 5;
        const int probability_size = detectionMat.cols - probability_index;
        float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
        size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;

        float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
        if (confidence > confidenceThreshold)
        {
            float x_center = detectionMat.at<float>(i, 0) * frame.cols;
            float y_center = detectionMat.at<float>(i, 1) * frame.rows;
            float width = detectionMat.at<float>(i, 2) * frame.cols;
            float height = detectionMat.at<float>(i, 3) * frame.rows;
            cv::Point p1(cvRound(x_center - width / 2), cvRound(y_center - height / 2));
            cv::Point p2(cvRound(x_center + width / 2), cvRound(y_center + height / 2));
            cv::Rect object(p1, p2);
            cowfaces.push_back(object);
            probability.push_back(confidence);
        }
    }

    if(cowfaces.size()>0)
        return true;
    else
        return false;
}

void detect_thread::classification(Net net, Mat input_frame, int &classid, float &probability)
{

    cv::Mat blob = cv::dnn::blobFromImage(input_frame,1./127.5,Size(inpWidth,inpHeight),cv::Scalar(127.5 ,127.5, 127.5));

    net.setInput(blob);
    Mat prob = net.forward();

    Point classIdPoint;
    double confidence;
    minMaxLoc(prob.reshape(1, 1), 0, &confidence, 0, &classIdPoint);
    classid= classIdPoint.x;
    probability = confidence;
}

float detect_thread::IOU_calculation(cv::Point fence_bottomright,cv::Point fence_topleft  ,cv::Rect cow_faces_vect)
{

    int cow_faces_topleft_x = cow_faces_vect.x;
    int cow_faces_topleft_y = cow_faces_vect.y;
    int cow_faces_bottomright_x = cow_faces_vect.x + cow_faces_vect.width;
    int cow_faces_bottomright_y = cow_faces_vect.y + cow_faces_vect.height;

    if(cow_faces_bottomright_y < fence_topleft.y || cow_faces_topleft_y > fence_bottomright.y || cow_faces_bottomright_x <
            fence_topleft.x ||cow_faces_topleft_x > fence_bottomright.x)
        return 0.0; //No intersection
    else{
        //int fence_area = (fence_bottomright.x-fence_topleft.x) * (fence_bottomright.y- fence_topleft.y);
        int cow_area_detection = (cow_faces_bottomright_x-cow_faces_topleft_x+1) * (cow_faces_bottomright_y-cow_faces_topleft_y+1);

        int xx1 = max(fence_topleft.x,cow_faces_topleft_x);
        int yy1 = max(fence_topleft.y,cow_faces_topleft_y);
        int xx2 = min(fence_bottomright.x,cow_faces_bottomright_x);
        int yy2 = min(fence_bottomright.y,cow_faces_bottomright_y);

        int h = max(0,yy2-yy1+1);
        int w = max(0,xx2-xx1+1);
        int intersection = w*h;
        float iou = (float)intersection/cow_area_detection;    //It's not real IOU!!
        return iou;
    }
}
void detect_thread::Hist_qualiz(cv::Mat &input_img)
{
    cv::Mat imageRGB[3];
    cv::split(input_img, imageRGB);
    for (int i = 0; i < 3; i++)
    {
        cv::equalizeHist(imageRGB[i], imageRGB[i]);
    }
    merge(imageRGB, 3, input_img);
}

void detect_thread::liner_transform(cv::Mat input_img,cv::Mat &ouput_img, double alpha, double beta)
{
    input_img.convertTo(ouput_img,-1,alpha,beta);
}


void detect_thread::run()
{
    raspicam::RaspiCam_Cv Camera;
    cv::Mat Capture_image;

    //Setting Pi Camera parameters
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC3 );


    //Open the RPI Camera
    if (!Camera.open()) {
        qDebug()<<"Error opening the camera";
    }
    else
    {

        //--------------------New Parameters---------------------
        int Nodenumber = 26;
        QString location = "NTU";
        float IOU_thrsh = 0.5;

        bool eating_status = false;
        bool leaving_status = false;
        cv::Mat eating_image;
        cv::Mat leaving_image;

        int eating_time;

        QDateTime datetime = datetime.currentDateTime();
        QTime time = time.currentTime();
        QElapsedTimer hourtimer,timer_detection;
        int expired_time_for_detection  = 5000;  //5 sec
        QDir work_dir;
        QString work_dir_str = "";

        time.start();
        hourtimer.start();
        timer_detection.start();
        int cow_total_eating[24] = {0};


        //MySQL Datebase Setting
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("140.112.94.123");
        db.setPort(10000);
        db.setUserName("root");
        db.setPassword("ntubime405");
        db.setDatabaseName("dairy_cow405");
        QString farm="NTU";

        if (!db.isValid()){
            QSqlError err = db.lastError();
            qDebug() << endl << "!DB ERROR: "<< err.text();

        }

        //Initalize the fence vector
        std::vector<fence> fence_vec;
        for(int i=0;i<4;i++)
        {
            fence fence0;
            fence0.pre_fence_status = 0;
            fence0.cur_fence_status = 0;
            fence0.eat_status = 0;
            fence0.leave_status = 0;
            fence0.cow_position.x = 0;
            fence0.cow_position.y = 0;
            fence0.cow_position.width = 0;
            fence0.cow_position.height = 0;
            fence0.cow_id = -1;
            fence0.voted_count = 0;
            fence0.fence_status.enqueue(0);
            fence0.fence_status.enqueue(0);
            fence0.fence_status.enqueue(0);
            for(int k=0;k<total_cow_num;k++)
            {
                fence0.cow_id_ar[k] =0;
            }
            fence_vec.push_back(fence0);
            fence0.q_cowface.clear();
        }
        //Start the detection loop
        this->status = true;
        while(status)
        {
            //------------------------------ Require the current datatime and time -----------------------------------
            datetime = datetime.currentDateTime();
            time = time.currentTime();

            //-------------------------------- Set Working directory -------------------------------------------------
            work_dir_str ="/home/pi/Cow_Feeding_Monitoring_System_v2/"+ datetime.toString("yyyy_MM_dd");
            hist_dir_str = "/home/pi/Cow_Feeding_Monitoring_System_v2/"+ datetime.toString("yyyy_MM_dd")+"/histogram";
            work_dir.setPath(work_dir_str);
            if(!work_dir.exists())
                this->mkdir(work_dir_str,hist_dir_str);

            //-------------------------------- csv. file for saving eating frequency----------------------------------
            QDir::currentPath();
            QFile Eating_frequency_file,Duration_file;
            Duration_file.setFileName(hist_dir_str+"/Cow_eating_duration"+".csv");
            Eating_frequency_file.setFileName(hist_dir_str+"/Eating_frequency_file"+".csv");

            QFile Start_time_log_file;
            Start_time_log_file.setFileName(hist_dir_str+"/Start_time_log"+".csv");
            Start_time_log_file.open(QIODevice::Append);
            QTextStream out_Start_time_log_file(&Start_time_log_file);

            QFile Elapsed_time_file;
            Elapsed_time_file.setFileName(hist_dir_str+"/Elapsed_time_log"+".csv");
            Elapsed_time_file.open(QIODevice::Append);
            QTextStream out_Elapsed_time_file(&Elapsed_time_file);

            QFile Eating_time_file_0;
            Eating_time_file_0.setFileName(hist_dir_str+"/Eating_time_0_log"+".csv");
            Eating_time_file_0.open(QIODevice::Append);
            QTextStream out_Eating_time_file_0(&Eating_time_file_0);

            QFile Eating_time_file_1;
            Eating_time_file_1.setFileName(hist_dir_str+"/Eating_time_1_log"+".csv");
            Eating_time_file_1.open(QIODevice::Append);
            QTextStream out_Eating_time_file_1(&Eating_time_file_1);

            QFile Eating_time_file_2;
            Eating_time_file_2.setFileName(hist_dir_str+"/Eating_time_2_log"+".csv");
            Eating_time_file_2.open(QIODevice::Append);
            QTextStream out_Eating_time_file_2(&Eating_time_file_2);

            QFile Eating_time_file_3;
            Eating_time_file_3.setFileName(hist_dir_str+"/Eating_time_3_log"+".csv");
            Eating_time_file_3.open(QIODevice::Append);
            QTextStream out_Eating_time_file_3(&Eating_time_file_3);



            //--------------------------------- Detect if there any cows are eating ------------------------------------
            //detect the cow in fix interval

            //if(timer_detection.hasExpired(expired_time_for_detection-4000))
            if(timer_detection.hasExpired(250))
            {
                std::vector<Rect> cowfaces;
                std::vector<float> detected_probability;

                QTime startTime = QTime::currentTime();
                Camera.grab();
                Camera.retrieve (Capture_image);


                qDebug()<<"Current Time:"<<time.toString("hh-mm-ss");
                if(flip_status)
                    cv::flip(Capture_image,Capture_image,-1);


                if(Nodenumber==21)
                {
                    if((startTime.hour()>=0) && (startTime.hour()<=3))
                    {
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }else if(startTime.hour()>=18)
                    {
                        //this->Hist_qualiz(Capture_image);
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }
                }else if(Nodenumber==22)
                {
                    if((startTime.hour()>=0) && (startTime.hour()<=4))
                    {
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }else if((startTime.hour()>=19))
                    {
                        //this->Hist_qualiz(Capture_image);
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }
                }else if(Nodenumber==23)
                {
                    if((startTime.hour()>=0) && (startTime.hour()<=4))
                    {
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }else if((startTime.hour()>=19))
                    {
                        //this->Hist_qualiz(Capture_image);
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }
                }else if(Nodenumber==24)
                {
                    if((startTime.hour()>=0) && (startTime.hour()<=4))
                    {
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }else if((startTime.hour()>=18))
                    {
                        //this->Hist_qualiz(Capture_image);
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }
                }else if(Nodenumber==25)
                {
                    if((startTime.hour()>=0) && (startTime.hour()<=4))
                    {
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }else if((startTime.hour()>=18))
                    {
                        //this->Hist_qualiz(Capture_image);
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }
                }else if(Nodenumber==26||Nodenumber==27||Nodenumber==28||Nodenumber==29||Nodenumber==30)
                {
                    if((startTime.hour()>=0) && (startTime.hour()<=4))
                    {
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }else if((startTime.hour()>=17))
                    {
                        //this->Hist_qualiz(Capture_image);
                        this->liner_transform(Capture_image,Capture_image,1.5,15);
                    }
                }
                //--------------------Save fence image for confirmation----------------------------------------
                //imwrite(work_dir_str.toStdString()+"/fence/"+(datetime.toString("yyyy_MM_dd_hh_mm_ss")+".jpg").toStdString(),Capture_image);


                //-----------------------------Check if there any fence has cow---------------------------------------------

                int max_fence_index = 0;
                int cow_face_num = 0;
                float max_IOU = 0.01;
                bool check[4] ={false};

                //debug
                qDebug()<<"fence status:";
                if(detect_BY_YOLO(Capture_image,cowfaces,detected_probability))
                {
                    bool check_leave_flag = false;
                    std::stable_sort(cowfaces.begin(), cowfaces.end(),bbox_x_compare ); //Sort detection bbox by

                    //Save raw detected image for confirmation
//                    QString confirm_filename = datetime.toString("yyyy_MM_dd_hh_mm_ss")+".jpg";
//                    confirm_filename = work_dir_str+("/confirm/"+confirm_filename);
//                    qDebug()<<"confir_image resize";
//                    cv::Mat show_eating_img;
//                    cv::resize(Capture_image,show_eating_img,cv::Size(640,480));
//                    qDebug()<<"confir_image resize success";
//                    cv::imwrite(confirm_filename.toStdString(),show_eating_img);
//                    Cow_Feeding_Monitoring::image_send(confirm_filename,Nodenumber,location,"confirm");


                    cv::Mat Detected_image = Capture_image.clone();
                    eating_image = Capture_image.clone();

                    for(int i=0; i<fence_vec.size(); i++)
                    {
                        cv::Point fence_topleft(static_cast<int>(rec_x[i]),static_cast<int>(rec_y[i]));
                        cv::Point fence_bottomright(static_cast<int>((rec_x[i]+rec_width[i])),static_cast<int>((rec_y[i]+rec_height[i])));
                        cv::rectangle(eating_image,fence_topleft,fence_bottomright, cv::Scalar(200,0,125),2);

                        max_fence_index = i;

                        for(int j=0; j<cowfaces.size(); j++)
                        {
                            if(!check[j]){
                                float IOU = IOU_calculation(fence_bottomright,fence_topleft,cowfaces.at(j));
                                if(IOU >= max_IOU)
                                {
                                    max_IOU = IOU;
                                    cow_face_num = j;
    //                                if(IOU >= 0.7){
    //                                    check[j] = true;
    //                                    //cowfaces.erase(cowfaces.begin()+j);
    //                                    break;
    //                                }
                                }
                            }else
                                continue;
                        }
                        //------------------------Update the current fence[i] status-----------------------------------
                        //if the detect object is not highly intersect with fence[i] then set fence[i] status as 0 otherwis set as 1.
                        if(max_IOU < IOU_thrsh)
                        {
                            fence_vec[i].cur_fence_status = 0;
                            if(fence_vec[i].fence_status.size() == 3)
                                fence_vec[i].fence_status.dequeue();
                            fence_vec[i].fence_status.enqueue(0);
                        }else{
                            /*record current fence[i] status and record the cowface position*/
                            fence_vec[i].cur_fence_status = 1;
                            if(fence_vec[i].fence_status.size() == 3)
                                fence_vec[i].fence_status.dequeue();
                            fence_vec[i].fence_status.enqueue(1);
                            fence_vec[i].cow_position = cowfaces.at(cow_face_num); //set the detected cow face position in fence[i]
                            //fence_vec[i].cow_position = cowfaces.at(cow_face_num);
                            //if(fence_vec[i].eat_status)


                            check[cow_face_num] = true; //set a check flag, it means this cowface is belong to a fence[i]
                            if(!Detected_image.empty()){
                                QString confirm_filename = datetime.toString("yyyy_MM_dd_hh_mm_ss")+".jpg";
                                confirm_filename = work_dir_str+("/confirm/"+confirm_filename);
                                cv::Mat show_eating_img;
                                cv::putText(Detected_image,datetime.toString("yyyy_MM_dd_hh_mm_ss").toStdString(),Point(620,50),0, 1, Scalar(255,255,255),2);
                                //cv::resize(Detected_image,show_eating_img,cv::Size(640,480));
                                //cv::imwrite(confirm_filename.toStdString(),show_eating_img);
                                cv::imwrite(confirm_filename.toStdString(),Detected_image);
                                emit image_get(confirm_filename,"confirm");
                            }
                        }
                        //---------------------------------------------------------------------------------------------

                        if(fence_vec[i].fence_status.at(0)==0 &&fence_vec[i].fence_status.at(1)==1 &&fence_vec[i].fence_status.at(2)==1 &&fence_vec[i].eat_status==0)
                        {
                            show_eat_img[i]=1;
                            fence_vec[i].eat_status = 1;
                            fence_vec[i].start_time = datetime;

                        }
                        else if(fence_vec[i].fence_status.at(0)==1 &&fence_vec[i].fence_status.at(1)==0 &&fence_vec[i].fence_status.at(2)==0 &&fence_vec[i].eat_status==1)
                        {
                            fence_vec[i].leave_status = 1;
                            //fence_vec[i].eat_status = 0;
                            fence_vec[i].leave_time = datetime;
                        }

                        if(fence_vec[i].eat_status==1)
                        {
                            
                            if(fence_vec[i].voted_count<5){
                                //classification
                                int classId = -1;
                                float class_probability = 0.0;

                                //Check if cropping image is out of window, if so modify cropping image range
                                int x = fence_vec[max_fence_index].cow_position.x;
                                int y = fence_vec[max_fence_index].cow_position.y;
                                int width = fence_vec[max_fence_index].cow_position.width ;
                                int height = fence_vec[max_fence_index].cow_position.height;

                                fence_vec[max_fence_index].cow_position.x = x>0 ? x:0;
                                fence_vec[max_fence_index].cow_position.y = y>0 ? y:0;
                                fence_vec[max_fence_index].cow_position.width = width<(Capture_image.cols-fence_vec[max_fence_index].cow_position.x) ? width:(Capture_image.cols-fence_vec[max_fence_index].cow_position.x-1);
                                fence_vec[max_fence_index].cow_position.height = height<(Capture_image.rows-fence_vec[max_fence_index].cow_position.y) ? height:(Capture_image.rows-fence_vec[max_fence_index].cow_position.y-1);

//                                if((fence_vec[max_fence_index].cow_position.x+fence_vec[max_fence_index].cow_position.width)>1279)
//                                    fence_vec[max_fence_index].cow_position.width = 1279 - fence_vec[max_fence_index].cow_position.x;
//                                if((fence_vec[max_fence_index].cow_position.y+fence_vec[max_fence_index].cow_position.height)>959)
//                                    fence_vec[max_fence_index].cow_position.height = 959 - fence_vec[max_fence_index].cow_position.y;
//                                if(fence_vec[max_fence_index].cow_position.x<0)
//                                    fence_vec[max_fence_index].cow_position.x = 0;
//                                if(fence_vec[max_fence_index].cow_position.y<0)
//                                    fence_vec[max_fence_index].cow_position.y = 0;
                                //qDebug()<<"fence:"<<max_fence_index<<",x:"<<fence_vec[max_fence_index].cow_position.x<<"w"<<fence_vec[max_fence_index].cow_position.width;

                                Rect rec(fence_vec[max_fence_index].cow_position.x,
                                         fence_vec[max_fence_index].cow_position.y,
                                         fence_vec[max_fence_index].cow_position.width,
                                         fence_vec[max_fence_index].cow_position.height);

                                Mat crop_cowface = Capture_image(rec);
                                fence_vec[i].q_cowface.push_back(crop_cowface); //push cropping cowface in fence[i]
                                cv::resize(crop_cowface,crop_cowface,cv::Size(inpWidth,inpHeight));

                                //Predict class
                                this->classification(classify_net,crop_cowface,classId,class_probability);

                                //record cow id
                                //if probability < 0.7 then set cow_id = unknown, it means unknown
                                if(class_probability>=0.7){

                                    fence_vec[max_fence_index].cow_id_ar[classId]++;
                                }
                                else{
                                    fence_vec[max_fence_index].cow_id_ar[total_cow_num-1]++;
                                }

                                fence_vec[i].voted_count++;

                                if(fence_vec[i].voted_count == 5)
                                {

                                    //qDebug()<<"voted!!!!!!";
                                    int max_index = find_max_value_index(fence_vec[i].cow_id_ar);
                                    if((fence_vec[i].cow_id_ar[max_index]>=3)&&(max_index!=total_cow_num-1)){
                                        show_eat_img[i] = 1;
                                        fence_vec[i].cow_id = max_index;
//                                        int k = 0;
//                                        while(!fence_vec[i].q_cowface.isEmpty()){
//                                            Mat img = fence_vec[i].q_cowface.dequeue();
//                                            QString cow_id = QString::number(fence_vec[i].cow_id);
//                                            QString qstr_img = work_dir_str+"/Cow/Cow"+cow_id+"/Cow"+cow_id+"_"+QString::number(k)+datetime.toString("_yyyy_MM_dd_hh_mm_ss");
//                                            imwrite(qstr_img.toStdString(),img);
//                                            k++;
//                                        }
                                    }
                                    else{
                                        fence_vec[i].cow_id = total_cow_num-1;
                                        fence_vec[i].q_cowface.clear();
                                    }
                                    fence_vec[i].voted_count++;
                                }

                            }

                        }

                        //console output
                        qDebug()<<"fence"+QString::number(i)<<"cow_id:"<<fence_vec[i].cow_id<<"voted_count"<<fence_vec[i].voted_count<<"status:"<<fence_vec[i].fence_status.at(0)<<fence_vec[i].fence_status.at(1)<<fence_vec[i].fence_status.at(2)
                                <<"Start Time:"<<fence_vec[i].start_time.toString("hh_mm_ss") <<"Pos:"<<fence_vec[i].cow_position.x<<","<<fence_vec[i].cow_position.y
                                <<"Leave Time:"<<fence_vec[i].leave_time.toString("hh_mm_ss");
                        if(fence_vec[i].leave_status)
                        {

                            if(fence_vec[i].cow_id != (total_cow_num-1) && fence_vec[i].cow_id !=-1)
                            {
                                if (db.open()){
                                    QSqlQuery query;

                                    query.prepare("INSERT INTO ntu_feeding_node_indiv(time_start,time_end,node,fence,cow_id,farm)"
                                                   "VALUES (:time_start, :time_end, :node, :fence, :cow_id, :farm)");
                                    query.bindValue(":time_start", fence_vec[i].start_time);
                                    query.bindValue(":time_end", fence_vec[i].leave_time);
                                    query.bindValue(":node", Nodenumber);
                                    query.bindValue(":fence", i);
                                    query.bindValue(":cow_id", fence_vec[i].cow_id);
                                    query.bindValue(":farm", farm);

                                    query.exec();
                                    qDebug()<<"query executed";

                                    //write data to csv file
                                    QString str = "start_time,"+fence_vec[i].start_time.toString("yyyy_MM_dd_hh_mm_ss")+","+"leaving_time,"+fence_vec[i].leave_time.toString("yyyy_MM_dd_hh_mm_ss");
                                    QString csv_file_name = datetime.toString("yyyy_MM_dd_")+"cow_"+QString::number(fence_vec[i].cow_id);
                                    this->write_csvfile(hist_dir_str,csv_file_name,str);

                                }
                                else
                                    qDebug() << "error opening database" << endl;
                            }

                            if(!Capture_image.empty() && !check_leave_flag){
                                cv::Mat show_capture_img;
                                cv::resize(Capture_image,show_capture_img,cv::Size(640,480));
                                QString leave_filename = "leave_"+datetime.toString("yyyy_MM_dd_hh_mm_ss")+".jpg";
                                leave_filename =  work_dir_str+("/leaving/"+leave_filename);
                                //cv::imwrite(leave_filename.toStdString(),show_capture_img);
                                //emit image_get(leave_filename,"leaving");
                                //emit leave_image_get(leave_filename);
                                check_leave_flag = true;
                            }
                            //clear the eating status
                            reset_fence_status(fence_vec[i]);
                        }

                        //if cow id is unknow then restart the voted
                        if(fence_vec[i].cow_id == total_cow_num-1){
                            fence_vec[i].cow_id = -1;
                            fence_vec[i].voted_count = 0;
                            for(int k=0;k<total_cow_num;k++)
                            {
                                fence_vec[i].cow_id_ar[k] =0;
                            }
                        }
                        max_fence_index = 0;
                        cow_face_num = 0;
                        max_IOU = 0.01;

                    }// out of for loop
                    cowfaces.clear();
                    detected_probability.clear();
                    check[4] ={false};
                    check_leave_flag = false;
                }
                else //detect nothing
                {
                    bool check_leave_flag = false;
                    for(int i=0; i<fence_vec.size(); i++)
                    {
                        fence_vec[i].cur_fence_status = 0;

                        if(fence_vec[i].fence_status.size() == 3)
                            fence_vec[i].fence_status.dequeue();
                        fence_vec[i].fence_status.enqueue(0);

                        if(fence_vec[i].fence_status.at(0)==1 &&fence_vec[i].fence_status.at(1)==0 &&fence_vec[i].fence_status.at(2)==0 && fence_vec[i].eat_status==1)
                        {
                            fence_vec[i].leave_status = 1;
                            fence_vec[i].eat_status = 0;
                            fence_vec[i].leave_time = datetime;

                        }
                        qDebug()<<"fence"+QString::number(i)<<"cow_id:"<<fence_vec[i].cow_id<<"voted_count"<<fence_vec[i].voted_count<<"status:"<<fence_vec[i].fence_status.at(0)<<fence_vec[i].fence_status.at(1)<<fence_vec[i].fence_status.at(2)
                                <<"Start Time:"<<fence_vec[i].start_time.toString("hh_mm_ss") <<"Pos:"<<fence_vec[i].cow_position.x<<","<<fence_vec[i].cow_position.y
                                <<"Leave Time:"<<fence_vec[i].leave_time.toString("hh_mm_ss");
                        if(fence_vec[i].leave_status)
                        {
                            if(fence_vec[i].cow_id != (total_cow_num-1) && fence_vec[i].cow_id !=-1)
                            {
                                if (db.open()){
                                    QSqlQuery query;

                                    query.prepare("INSERT INTO ntu_feeding_node_indiv(time_start,time_end,node,fence,cow_id,farm)"
                                                   "VALUES (:time_start, :time_end, :node, :fence, :cow_id, :farm)");
                                    query.bindValue(":time_start", fence_vec[i].start_time);
                                    query.bindValue(":time_end", fence_vec[i].leave_time);
                                    query.bindValue(":node", Nodenumber);
                                    query.bindValue(":fence", i);
                                    query.bindValue(":cow_id", fence_vec[i].cow_id);
                                    query.bindValue(":farm", farm);

                                    query.exec();
                                    qDebug()<<"query executed";

                                    //write data to csv file
                                    QString str = "start_time,"+fence_vec[i].start_time.toString()+","+"leaving_time,"+fence_vec[i].leave_time.toString();
                                    QString csv_file_name = datetime.toString("yyyy_MM_dd_")+"cow_"+fence_vec[i].cow_id;
                                    this->write_csvfile(hist_dir_str,csv_file_name,str);

                                }
                                else
                                    qDebug() << "error opening database" << endl;
                            }
                            if(!Capture_image.empty() && !check_leave_flag){
                                cv::Mat show_capture_img;
                                cv::resize(Capture_image,show_capture_img,cv::Size(640,480));
                                QString leave_filename = "leave_"+datetime.toString("yyyy_MM_dd_hh_mm_ss")+".jpg";
                                leave_filename =  work_dir_str+("/leaving/"+leave_filename);
                                //cv::imwrite(leave_filename.toStdString(),show_capture_img);
                                //emit image_get(leave_filename,"leaving");
                                //emit leave_image_get(leave_filename);
                                check_leave_flag = true;
                            }
                            //clear the eating status
                            reset_fence_status(fence_vec[i]);
                        }
                    }
                    check_leave_flag = false ;
                 }
                
                /*Draw bounding box*/
                for(int i=0;i<fence_vec.size();i++)
                {
                    if(fence_vec[i].eat_status==1)
                    {
                        //Draw cowface bounding box
                        QString show_time = fence_vec[i].start_time.toString("hh:mm:ss");
                        putText(eating_image,show_time.toStdString(),Point(fence_vec[i].cow_position.x+10,fence_vec[i].cow_position.y), 0, 1,Scalar(0,215,255),2);
                        cv::rectangle(eating_image, fence_vec[i].cow_position, cv::Scalar(0,0,255),2);
                    }
                    if((fence_vec[i].cow_id!=-1) && (fence_vec[i].cow_id!=total_cow_num-1))
                    {
                        //draw cow_id on eating_image
                        String output = format("%s", classifyclassNames[fence_vec[i].cow_id].c_str());
                        cv::Point p(fence_vec[i].cow_position.x+10,fence_vec[i].cow_position.y
                                         +fence_vec[i].cow_position.height/2);
                        cv::putText(eating_image, output,p,FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,69,255),2);
                    }
                }
                
                if(show_eat_img[0]||show_eat_img[1]||show_eat_img[2]||show_eat_img[3])
                {
                    if(!eating_image.empty()){
                        cv::Mat show_eating_img;
                        //cv::putText(eating_image,datetime.toString("yyyy_MM_dd_hh_mm_ss").toStdString(),Point(eating_image.cols/2,50),0, 1, Scalar(255,255,255),2);
                        cv::resize(eating_image,show_eating_img,cv::Size(640,480));
                        QString eat_filename = "eat_"+datetime.toString("yyyy_MM_dd_hh_mm_ss")+".jpg";
                        eat_filename =  work_dir_str+("/eating/"+eat_filename);
                        qDebug()<<eat_filename;
                        cv::imwrite(eat_filename.toStdString(),show_eating_img);
                        //emit image_get(eat_filename,"eating");
                        qDebug()<<"eat img send !";
                        //emit eat_image_get(eat_filename);
                    }
                    
                }
                for(int k=0;k<4;k++)
                {
                    show_eat_img[k]=0;
                }
                //----------------------------Show the current status------------------------------------------------------------
                QTime stopTime = QTime::currentTime();
                int  elapsed = startTime.msecsTo(stopTime);
                qDebug()<<"elapsed time ="<<QString::number((float)elapsed/1000)<<"s";

                out_Elapsed_time_file<<"Current Time"<<","<<time.currentTime().toString("hh_mm_ss")<<","<<"Elapsed Time"<<","<<QString::number((float)elapsed/1000)<<"\n";
                Elapsed_time_file.close();


//                eating_status = false;
//                leaving_status = false;
                timer_detection.start();   //Restart the timer_detection for detection in every 0.5 sec
            }

            if(datetime.toString("mm") == "59" && (datetime.toString("ss").toInt()>=49))
            {
                Eating_frequency_file.open(QIODevice::Append);
                QTextStream out_Eating_frequency_file(&Eating_frequency_file);

                out_Eating_frequency_file <<"Total detection cows"<<","<<datetime.toString("hh")<<","
                    <<QString::number(cow_total_eating[datetime.time().hour()])<<"\n";
                Eating_frequency_file.close();
                QThread::sleep(1);
            }

         }

    }
}



void detect_thread::mkdir(QString work_dir_str,QString hist_dir_str)
{
    QDir work_dir;

    //Set Working directory
    work_dir.setPath(work_dir_str);
    if(!work_dir.exists())
    {
        if(work_dir.mkdir(work_dir_str))
            qDebug()<<"Dairy's directory created successfully";
        else
            qDebug()<<"Failed to create dairy's directory";
    }
    QString work_dir_detect = work_dir_str+"/eating";
    work_dir.setPath(work_dir_detect);
    if(!work_dir.exists())
    {
        work_dir.mkdir(work_dir_detect);
        qDebug()<<"Create work_dir_eating successfully";
    }

    QString work_dir_confirm = work_dir_str+"/confirm";
    work_dir.setPath(work_dir_confirm);
    if(!work_dir.exists())
    {
        work_dir.mkdir(work_dir_confirm);
        qDebug()<<"Create work_dir_confirm successfully";
    }

    QString work_dir_leave= work_dir_str+"/leaving";
    work_dir.setPath(work_dir_leave);
    if(!work_dir.exists())
    {
        work_dir.mkdir(work_dir_leave);
        qDebug()<<"Create work_dir_leave successfully";
    }

    QString work_dir_cow_img = work_dir_str+"/Cow";
    work_dir.setPath(work_dir_cow_img);
    if(!work_dir.exists())
    {
        work_dir.mkdir(work_dir_cow_img);
        qDebug()<<"Create work_dir_cow_img successfully";
    }
    for(int i=0;i<20;i++){
        QString work_dir_indiv_cow_img= work_dir_str+"/Cow/Cow"+QString::number(i);
        work_dir.setPath(work_dir_indiv_cow_img);
        if(!work_dir.exists())
            work_dir.mkdir(work_dir_indiv_cow_img);
    }

    QString word_dir_histogram = work_dir_str+"/histogram";
    work_dir.setPath(word_dir_histogram);
    if(!work_dir.exists())
    {
        work_dir.mkdir(word_dir_histogram);
        qDebug()<<"Create word_dir_histogram successfully";
    }
}

void detect_thread::getconfiguration(int recx0, int recx1, int recx2, int recx3, int recy, int recwidth0
                                    , int recwidth1, int recwidth2, int recwidth3, int recheight, float threshold,bool flip_vertical)
{

    rec_x[0] = recx0;
    rec_x[1] = recx1;
    rec_x[2] = recx2;
    rec_x[3] = recx3 ;
    rec_width[0] = recwidth0;
    rec_width[1] = recwidth1;
    rec_width[2] = recwidth2;
    rec_width[3] = recwidth3;

    flip_status = flip_vertical;

    for(int i=0; i<4; i++)
    {
      rec_y[i] = recy;
      rec_height[i] = recheight;
    }
    count_threshold = threshold;
    qDebug() << "update the configuration ";
}
void detect_thread::WriteCountFile(int fence,QDateTime Cow_start_eating_time,int cow_count,int VotedCount, QDateTime datetime)
{
    QFile cow_count_file;
    cow_count_file.setFileName("./"+datetime.toString("yyyy_MM_dd")+"/detect/detect"+QString::number(fence)+"/"+Cow_start_eating_time.toString("yyyy-MM-dd")+"_Cow_eating_count_detect_"+QString::number(fence)+".csv");
    cow_count_file.open(QIODevice::Append);
    QTextStream out(&cow_count_file);

    out <<"Time"<<","<<Cow_start_eating_time.toString("hh")<<","<<Cow_start_eating_time.toString("mm")<<","
        <<Cow_start_eating_time.toString("ss")<<","<<QString::number(cow_count)<<","<<QString::number(VotedCount)<<"\n";

    cow_count_file.close();
}

void detect_thread::write_csvfile(QString save_dir,QString csv_file_name,QString str)
{
    QFile qcsvfile;
    qcsvfile.setFileName(save_dir+"/"+csv_file_name+".csv");
    qcsvfile.open(QIODevice::Append);

    QTextStream stream( &qcsvfile );
    stream << str << "\n";

    qcsvfile.close();


}

bool bbox_x_compare(Rect rec1, Rect rec2)
{
    return rec1.x < rec2.x;
}

int find_max_value_index(int arr[])
{
    int max = arr[0];
    int max_index = 0;
    for(int i=0; i<total_cow_num;i++)
    {
        if(arr[i]>max){
            max = arr[i];
            max_index = i ;
        }
    }
    return max_index;
}

void reset_fence_status(fence &fence_vec)
{
    //clear the eating status
    fence_vec.eat_status = 0;
    fence_vec.leave_status = 0;
    fence_vec.cow_position.x = 0;
    fence_vec.cow_position.y = 0;
    fence_vec.cow_position.width = 0;
    fence_vec.cow_position.height = 0;
    fence_vec.cow_id = -1;
    fence_vec.voted_count = 0;
    fence_vec.start_time = QDateTime();
    fence_vec.leave_time = QDateTime();
    //fence0.fence_status.clear();
    for(int k=0;k<total_cow_num;k++)
    {
        fence_vec.cow_id_ar[k] =0;
    }
    fence_vec.q_cowface.clear();
}
