/********************************************************************************
** Form generated from reading UI file 'cow_feeding_monitoring.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COW_FEEDING_MONITORING_H
#define UI_COW_FEEDING_MONITORING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cow_Feeding_Monitoring
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *Eating_Img_label;
    QLabel *Leaving_Img_label;
    QLabel *label_7;
    QLabel *label_8;
    QWidget *tab_2;
    QLabel *Capture_Img_label;
    QLabel *Detect_Img_label;
    QLCDNumber *lcdNumber;
    QGroupBox *groupBox_2;
    QTextBrowser *textBrowser;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *StartButton;
    QPushButton *StopButton;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *webcam_radioButton;
    QRadioButton *picamera_radioButton;
    QDoubleSpinBox *Threshold_spinBox;
    QLabel *label_6;
    QWidget *tab_4;
    QRadioButton *vertical_flip_radioButton;
    QLabel *label;
    QSpinBox *recx0;
    QSpinBox *recx1;
    QSpinBox *recx2;
    QSpinBox *recx3;
    QLabel *label_4;
    QSpinBox *recwidth0;
    QSpinBox *recwidth1;
    QSpinBox *recwidth2;
    QSpinBox *recwidth3;
    QSpinBox *recy;
    QLabel *label_3;
    QLabel *label_5;
    QSpinBox *recheight;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *Capture_Btn;
    QPushButton *Test_rec_pos;
    QPushButton *Fence_par_ok_Btn;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QDoubleSpinBox *scale_factor_doubleSpinBox;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cow_Feeding_Monitoring)
    {
        if (Cow_Feeding_Monitoring->objectName().isEmpty())
            Cow_Feeding_Monitoring->setObjectName(QStringLiteral("Cow_Feeding_Monitoring"));
        Cow_Feeding_Monitoring->resize(1253, 732);
        actionExit = new QAction(Cow_Feeding_Monitoring);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(Cow_Feeding_Monitoring);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 961, 431));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(12);
        tabWidget->setFont(font);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        Eating_Img_label = new QLabel(tab);
        Eating_Img_label->setObjectName(QStringLiteral("Eating_Img_label"));
        Eating_Img_label->setGeometry(QRect(20, 50, 451, 301));
        Eating_Img_label->setAutoFillBackground(true);
        Leaving_Img_label = new QLabel(tab);
        Leaving_Img_label->setObjectName(QStringLiteral("Leaving_Img_label"));
        Leaving_Img_label->setGeometry(QRect(490, 50, 450, 301));
        Leaving_Img_label->setAutoFillBackground(true);
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(210, 0, 131, 51));
        QFont font1;
        font1.setFamily(QStringLiteral("Yu Gothic UI Semibold"));
        font1.setPointSize(19);
        font1.setItalic(false);
        label_7->setFont(font1);
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(680, 0, 131, 51));
        label_8->setFont(font1);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        Capture_Img_label = new QLabel(tab_2);
        Capture_Img_label->setObjectName(QStringLiteral("Capture_Img_label"));
        Capture_Img_label->setGeometry(QRect(20, 30, 450, 300));
        Capture_Img_label->setAutoFillBackground(true);
        Detect_Img_label = new QLabel(tab_2);
        Detect_Img_label->setObjectName(QStringLiteral("Detect_Img_label"));
        Detect_Img_label->setGeometry(QRect(490, 30, 450, 300));
        Detect_Img_label->setAutoFillBackground(true);
        tabWidget->addTab(tab_2, QString());
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(980, 40, 241, 41));
        lcdNumber->setDigitCount(8);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 450, 961, 221));
        groupBox_2->setFont(font);
        textBrowser = new QTextBrowser(groupBox_2);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 30, 941, 181));
        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(980, 90, 261, 351));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        layoutWidget = new QWidget(tab_3);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 120, 171, 27));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        StartButton = new QPushButton(layoutWidget);
        StartButton->setObjectName(QStringLiteral("StartButton"));

        horizontalLayout_3->addWidget(StartButton);

        StopButton = new QPushButton(layoutWidget);
        StopButton->setObjectName(QStringLiteral("StopButton"));

        horizontalLayout_3->addWidget(StopButton);

        layoutWidget_2 = new QWidget(tab_3);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 20, 221, 28));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        webcam_radioButton = new QRadioButton(layoutWidget_2);
        webcam_radioButton->setObjectName(QStringLiteral("webcam_radioButton"));

        horizontalLayout_4->addWidget(webcam_radioButton);

        picamera_radioButton = new QRadioButton(layoutWidget_2);
        picamera_radioButton->setObjectName(QStringLiteral("picamera_radioButton"));
        picamera_radioButton->setChecked(true);

        horizontalLayout_4->addWidget(picamera_radioButton);

        Threshold_spinBox = new QDoubleSpinBox(tab_3);
        Threshold_spinBox->setObjectName(QStringLiteral("Threshold_spinBox"));
        Threshold_spinBox->setGeometry(QRect(90, 70, 51, 26));
        Threshold_spinBox->setValue(0.2);
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 70, 71, 21));
        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        vertical_flip_radioButton = new QRadioButton(tab_4);
        vertical_flip_radioButton->setObjectName(QStringLiteral("vertical_flip_radioButton"));
        vertical_flip_radioButton->setGeometry(QRect(10, 170, 111, 23));
        vertical_flip_radioButton->setChecked(true);
        label = new QLabel(tab_4);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 21, 21));
        QFont font2;
        font2.setPointSize(14);
        label->setFont(font2);
        recx0 = new QSpinBox(tab_4);
        recx0->setObjectName(QStringLiteral("recx0"));
        recx0->setGeometry(QRect(30, 20, 51, 26));
        recx0->setMaximum(1000);
        recx0->setValue(0);
        recx1 = new QSpinBox(tab_4);
        recx1->setObjectName(QStringLiteral("recx1"));
        recx1->setGeometry(QRect(90, 20, 51, 26));
        recx1->setMaximum(2000);
        recx1->setValue(275);
        recx2 = new QSpinBox(tab_4);
        recx2->setObjectName(QStringLiteral("recx2"));
        recx2->setGeometry(QRect(150, 20, 51, 26));
        recx2->setMaximum(2000);
        recx2->setValue(540);
        recx3 = new QSpinBox(tab_4);
        recx3->setObjectName(QStringLiteral("recx3"));
        recx3->setGeometry(QRect(210, 20, 41, 26));
        recx3->setMaximum(2000);
        recx3->setValue(840);
        label_4 = new QLabel(tab_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 60, 41, 21));
        label_4->setFont(font2);
        recwidth0 = new QSpinBox(tab_4);
        recwidth0->setObjectName(QStringLiteral("recwidth0"));
        recwidth0->setGeometry(QRect(30, 60, 51, 26));
        recwidth0->setMaximum(2000);
        recwidth0->setValue(260);
        recwidth1 = new QSpinBox(tab_4);
        recwidth1->setObjectName(QStringLiteral("recwidth1"));
        recwidth1->setGeometry(QRect(90, 60, 51, 26));
        recwidth1->setMaximum(2000);
        recwidth1->setValue(260);
        recwidth2 = new QSpinBox(tab_4);
        recwidth2->setObjectName(QStringLiteral("recwidth2"));
        recwidth2->setGeometry(QRect(150, 60, 51, 26));
        recwidth2->setMaximum(2000);
        recwidth2->setValue(275);
        recwidth3 = new QSpinBox(tab_4);
        recwidth3->setObjectName(QStringLiteral("recwidth3"));
        recwidth3->setGeometry(QRect(210, 60, 41, 26));
        recwidth3->setMaximum(2000);
        recwidth3->setValue(0);
        recy = new QSpinBox(tab_4);
        recy->setObjectName(QStringLiteral("recy"));
        recy->setGeometry(QRect(30, 100, 51, 26));
        recy->setMaximum(2000);
        recy->setValue(150);
        label_3 = new QLabel(tab_4);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 100, 21, 21));
        label_3->setFont(font2);
        label_5 = new QLabel(tab_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(90, 100, 21, 21));
        label_5->setFont(font2);
        recheight = new QSpinBox(tab_4);
        recheight->setObjectName(QStringLiteral("recheight"));
        recheight->setGeometry(QRect(110, 100, 51, 26));
        recheight->setMaximum(2000);
        recheight->setValue(560);
        layoutWidget1 = new QWidget(tab_4);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 240, 231, 27));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        Capture_Btn = new QPushButton(layoutWidget1);
        Capture_Btn->setObjectName(QStringLiteral("Capture_Btn"));

        horizontalLayout_5->addWidget(Capture_Btn);

        Test_rec_pos = new QPushButton(layoutWidget1);
        Test_rec_pos->setObjectName(QStringLiteral("Test_rec_pos"));

        horizontalLayout_5->addWidget(Test_rec_pos);

        Fence_par_ok_Btn = new QPushButton(tab_4);
        Fence_par_ok_Btn->setObjectName(QStringLiteral("Fence_par_ok_Btn"));
        Fence_par_ok_Btn->setGeometry(QRect(10, 200, 231, 25));
        layoutWidget2 = new QWidget(tab_4);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 130, 113, 28));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        scale_factor_doubleSpinBox = new QDoubleSpinBox(layoutWidget2);
        scale_factor_doubleSpinBox->setObjectName(QStringLiteral("scale_factor_doubleSpinBox"));
        scale_factor_doubleSpinBox->setMaximum(2);
        scale_factor_doubleSpinBox->setSingleStep(0.1);
        scale_factor_doubleSpinBox->setValue(0.5);

        horizontalLayout->addWidget(scale_factor_doubleSpinBox);

        tabWidget_2->addTab(tab_4, QString());
        layoutWidget->raise();
        layoutWidget->raise();
        vertical_flip_radioButton->raise();
        label->raise();
        recx0->raise();
        recx1->raise();
        recx2->raise();
        recx3->raise();
        label_4->raise();
        recwidth0->raise();
        recwidth1->raise();
        recwidth2->raise();
        recwidth3->raise();
        recy->raise();
        label_3->raise();
        label_5->raise();
        recheight->raise();
        Fence_par_ok_Btn->raise();
        Cow_Feeding_Monitoring->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Cow_Feeding_Monitoring);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1253, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        Cow_Feeding_Monitoring->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Cow_Feeding_Monitoring);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Cow_Feeding_Monitoring->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Cow_Feeding_Monitoring);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Cow_Feeding_Monitoring->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(Cow_Feeding_Monitoring);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Cow_Feeding_Monitoring);
    } // setupUi

    void retranslateUi(QMainWindow *Cow_Feeding_Monitoring)
    {
        Cow_Feeding_Monitoring->setWindowTitle(QApplication::translate("Cow_Feeding_Monitoring", "Cow_Feeding_Monitoring", nullptr));
        actionExit->setText(QApplication::translate("Cow_Feeding_Monitoring", "Exit", nullptr));
        Eating_Img_label->setText(QString());
        Leaving_Img_label->setText(QString());
        label_7->setText(QApplication::translate("Cow_Feeding_Monitoring", "Feeding", nullptr));
        label_8->setText(QApplication::translate("Cow_Feeding_Monitoring", "Leaving", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Cow_Feeding_Monitoring", "Main Screen", nullptr));
        Capture_Img_label->setText(QString());
        Detect_Img_label->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Cow_Feeding_Monitoring", "Fence Test", nullptr));
        groupBox_2->setTitle(QApplication::translate("Cow_Feeding_Monitoring", "System Log", nullptr));
        StartButton->setText(QApplication::translate("Cow_Feeding_Monitoring", "Start", nullptr));
        StopButton->setText(QApplication::translate("Cow_Feeding_Monitoring", "Stop", nullptr));
        webcam_radioButton->setText(QApplication::translate("Cow_Feeding_Monitoring", "WebCam", nullptr));
        picamera_radioButton->setText(QApplication::translate("Cow_Feeding_Monitoring", "Pi Camera", nullptr));
        label_6->setText(QApplication::translate("Cow_Feeding_Monitoring", "Threshold", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("Cow_Feeding_Monitoring", "General", nullptr));
        vertical_flip_radioButton->setText(QApplication::translate("Cow_Feeding_Monitoring", "vertical_flip", nullptr));
        label->setText(QApplication::translate("Cow_Feeding_Monitoring", "x", nullptr));
        label_4->setText(QApplication::translate("Cow_Feeding_Monitoring", "w", nullptr));
        label_3->setText(QApplication::translate("Cow_Feeding_Monitoring", "y", nullptr));
        label_5->setText(QApplication::translate("Cow_Feeding_Monitoring", "h", nullptr));
        Capture_Btn->setText(QApplication::translate("Cow_Feeding_Monitoring", "Capture", nullptr));
        Test_rec_pos->setText(QApplication::translate("Cow_Feeding_Monitoring", "Test_ROI", nullptr));
        Fence_par_ok_Btn->setText(QApplication::translate("Cow_Feeding_Monitoring", "OK", nullptr));
        label_2->setText(QApplication::translate("Cow_Feeding_Monitoring", "Scale", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("Cow_Feeding_Monitoring", "Option", nullptr));
        menuFile->setTitle(QApplication::translate("Cow_Feeding_Monitoring", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cow_Feeding_Monitoring: public Ui_Cow_Feeding_Monitoring {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COW_FEEDING_MONITORING_H
