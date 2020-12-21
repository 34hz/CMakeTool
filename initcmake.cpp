#include "initcmake.h"
#include "./ui_initcmake.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
InitCMake::InitCMake(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InitCMake)
    ,isOK(false),m_info(new ProjectInfo)
{
    ui->setupUi(this);
}

bool InitCMake::init()
{
    ui->lineEdit_ProjectPath->setEnabled(false);

    m_initProgress=new QProgressBar(this);
    m_initProgress->setOrientation(Qt::Horizontal);// 水平方向
    m_initProgress->setMaximum(100);// 最大值
    m_initProgress->setMinimum(0);// 最小值
    m_initProgress->setValue(1);// 当前进度


    char *buffer;
    //也可以将buffer作为输出参数
    if((buffer = getcwd(NULL, 0) )== NULL)
    {
        perror("getcwd error");
    }
    else
    {
        m_info->setProjectPath(buffer);
        string s =buffer;
        ui->lineEdit_ProjectPath->setText(QString::fromStdString(s));
        int pos1=s.find_last_of("/");
        s=s.substr(pos1+1);
        ui->lineEdit_ProjectName->setText(QString::fromStdString(s));
        std::cout<<s<<std::endl;
        free(buffer);
    }
    m_statusBar = new QStatusBar(this);
    m_statusBar->showMessage("getcwd");
    return true;
}

InitCMake::~InitCMake()
{
    delete ui;
}

void InitCMake::paintEvent(QPaintEvent *event)
{
    //是否激活开始按钮[GO]
    if(ui->lineEdit_ProjectName->text().isEmpty()||ui->lineEdit_ProjectPath->text().isEmpty())
    {
        ui->pushButton_Init->setEnabled(false);
    }
    else {
         ui->pushButton_Init->setEnabled(true);
    }
    m_initProgress->resize(this->size().width(),20);
    m_statusBar->resize(this->size().width(),20);
    m_initProgress->move(0,this->size().height()-50);
    m_statusBar->move(0,this->size().height()-30);

}



bool InitCMake::readProjectPath(){//获取目录
    if(ui->lineEdit_ProjectPath->text().isEmpty()){
       ui->lineEdit_ProjectPath->clear();
    }
    QString path=QFileDialog::getExistingDirectory(this,"获取项目根目录","./");
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();
    if(list.count()>0){
        int ret;
        ret = QMessageBox::warning(this,"Warning","目录非空,重新选择",QMessageBox::Ok|QMessageBox::Cancel);
        switch (ret){
        case QMessageBox::Ok:
            return false;
            break;
        case QMessageBox::Cancel:
            return true;
            break;
        }
    }else{
        //qDebug()<<path;
        m_info->setProjectPath(path);
        ui->lineEdit_ProjectPath->setText(path);
        return true;
    }
    return true;
}
void InitCMake::on_pushButton_ProjectPath_clicked()
{
    while(!readProjectPath());

}
void InitCMake::on_pushButton_Init_clicked()
{
    QString path= m_info->projectPath();
    m_info->setLibPath(path+"/lib");
    m_info->setExecPath(path+"/build");
    m_info->setSourcePath(path+"/Src");
    m_info->setCMakeVersion("0.11");
    m_info->printInfo(*m_info);
    m_info->setConfigPath(path+"/CMakeLists.txt");
    if(createFile()){
        QMessageBox::warning(this,"Warning","成功创建",QMessageBox::Ok);
    }
    else{
        QMessageBox::warning(this,"Warning","创建失败",QMessageBox::Ok);
    }
    ui->pushButton_Init->setEnabled(true);
}

void InitCMake::on_lineEdit_ProjectName_editingFinished()
{
    m_info->setProjectName(ui->lineEdit_ProjectName->text());
}

bool InitCMake::createFile()
{
    if(m_info->m_createFile(m_info->sourcePath())){
        m_initProgress->setValue(30);
        m_statusBar->showMessage("成功创建Src目录");
    }
    else {
        m_statusBar->showMessage("创建Src目录错误");
        return false;
    }
    if(m_info->m_createFile(m_info->libPath())){
        m_initProgress->setValue(60);
        m_statusBar->showMessage("成功创建lib目录");
    }
    else {
        m_statusBar->showMessage("创建lib目录错误");
        return false;
    }
    if(m_info->m_createFile(m_info->execPath())){
        m_initProgress->setValue(90);
        m_statusBar->showMessage("成功创建build目录");
    }
    else {
        m_statusBar->showMessage("创建build目录错误");
        return false;
    }
    if(touchCMakeLists()){
        m_initProgress->setValue(99);
        m_statusBar->showMessage("成功创建CMakeLists.txt");
    }
    else {
        m_statusBar->showMessage("创建CMakeLists.txt错误");
        return false;
    }
    if(m_info->touchDemo()){
        m_initProgress->setValue(100);
        m_statusBar->showMessage("成功创建示例文件");
    }
    else{
        m_statusBar->showMessage("创建示例文件错误");
        return false;
    }
    return true;
}

bool InitCMake::touchCMakeLists()
{
    ofstream file;
    string name ="test123";
    string projectName="project("+m_info->m_projectName.toStdString()+")";
    const char* project=projectName.c_str();
    const char* cmake_minimum_required="cmake_minimum_required(VERSION 3.0.0)";
    const char* aux_source_directory="aux_source_directory(. DIR_SRCS)";
    const char* add_executable="add_executable(${PROJECT_NAME} ${DIR_SRCS})";
    const char* set1="set(CPACK_PROJECT_NAME ${PROJECT_NAME})";
    const char* set2="set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR})";
    const char* set3="set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)";
    file.open(m_info->m_configPath.toStdString());
    if(file.is_open()){
        file
        <<cmake_minimum_required <<"\n"
        <<project<<"\n"
        <<aux_source_directory<<"\n"
        <<add_executable<<"\n"
        <<set1<<"\n"
        <<set2<<"\n"
        <<set3<<"\n"
        <<"##########打印相关信息\n"
        <<"message(STATUS \"CMake ProjectName: ${PROJECT_NAME}\")\n"
        <<"message(STATUS \"CMake version: ${CMAKE_VERSION}\")\n"
        <<"message(STATUS \"EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}\")\n"
        <<"message(STATUS \"LIBRARY_OUTPUT_PATH: ${LIBRARY_OUTPUT_PATH}\")\n"
        ;
        file.close();
        return true;
    }
    else {
        QMessageBox::warning(this,"Warning","CMakeLists创建失败",QMessageBox::Ok);
        return false;
    }
}
