#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UFScanner.h"


#define TEMPLATE_SIZE 1024

char m_strError[128];
unsigned char* imageData;
int pic_width;
int pic_height;
unsigned char atemp[TEMPLATE_SIZE];

void OnInit()
{
    UFS_STATUS ufs_res;
    int nScannerNumber;
    HUFScanner hScanner;
    ufs_res = UFS_Init();

    ufs_res = UFS_GetScannerNumber(&nScannerNumber);
    if (ufs_res == UFS_OK) {
    }


    ufs_res = UFS_GetScannerNumber(&nScannerNumber);
    if (ufs_res == UFS_OK) {
        printf("UFS_SetScannerCallback: OK\r\n");
    } else {
        UFS_GetErrorString(ufs_res, m_strError);
        printf("UFS_SetScannerCallback: %s\r\n", m_strError);
        return;
    }

    ufs_res = UFS_GetScannerHandle(0, &hScanner);
    if (ufs_res == UFS_OK) {
        printf("UFS_GetScannerHandle: SUCCEEDED\r\n");
    } else {
        UFS_GetErrorString(ufs_res, m_strError);
        printf("UFS_GetScannerHandle: ERROR: %s\r\n", m_strError);
        return;
    }

    int value = 500; // 5sec
    //ufs_res = UFS_SetParameter(hScanner, UFS_PARAM_TIMEOUT, &value);
    value = TEMPLATE_SIZE;
    ufs_res = UFS_SetParameter(hScanner, UFS_PARAM_TEMPLATE_SIZE, &value);
    value = 0;
    ufs_res = UFS_SetParameter(hScanner, UFS_PARAM_DETECT_CORE, &value);

    ufs_res = UFS_ClearCaptureImageBuffer(hScanner);
    if(ufs_res != UFS_OK) {
        return;
    }

    ufs_res = UFS_CaptureSingleImage(hScanner);
    if (ufs_res == UFS_OK) {
        printf("UFS_GETCaptureImageInfo: SUCCEEDED\r\n");
    } else {
        UFS_GetErrorString(ufs_res, m_strError);
        printf("UFS_GetScannerHandle: ERROR: %s\r\n", m_strError);
        return;
    }

  char fileName[128];
    int resolution;
    ufs_res = UFS_GetCaptureImageBufferInfo(hScanner, &pic_width, &pic_height, &resolution);
    if (ufs_res == UFS_OK) {
        printf("UFS_GETCaptureImageInfo: SUCCEEDED\r\n");
    } else {
        UFS_GetErrorString(ufs_res, m_strError);
        printf("UFS_GetScannerHandle: ERROR: %s\r\n", m_strError);
        return;
    }
    imageData = (uchar*)malloc(pic_width * pic_height * sizeof (uchar));

    ufs_res = UFS_GetCaptureImageBuffer(hScanner, imageData);
    if (ufs_res == UFS_OK) {
        printf("UFS_GETCaptureImage: SUCCEEDED\r\n");
    } else {
        UFS_GetErrorString(ufs_res, m_strError);
        printf("UFS_GetScannerHandle: ERROR: %s\r\n", m_strError);
        return;
    }

    unsigned char atemp[TEMPLATE_SIZE];
    int nTemplateSize, nEnrollQuality;
    ufs_res = UFS_ExtractEx(hScanner, TEMPLATE_SIZE, atemp, &nTemplateSize, &nEnrollQuality);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OnInit();

    QImage img = QImage(imageData, pic_width, pic_height, QImage::Format_Grayscale8);

   // img.loadFromData(imageData,pic_width*pic_height, "JPG");

    QPixmap px = QPixmap::fromImage(img);

    ui->picture->setPixmap(px);
}

MainWindow::~MainWindow()
{
    delete ui;
}


