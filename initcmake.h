#ifndef INITCMAKE_H
#define INITCMAKE_H

#include <QDialog>
#include <QProgressBar>
#include <QDebug>
#include "projectinfo.h"
#include <QPaintEvent>
#include <QStatusBar>

QT_BEGIN_NAMESPACE
namespace Ui { class InitCMake; }
QT_END_NAMESPACE

class InitCMake : public QDialog
{
    Q_OBJECT

public:
    InitCMake(QWidget *parent = nullptr);
    bool init();
    ~InitCMake();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_pushButton_ProjectPath_clicked();

    void on_pushButton_Init_clicked();


    void on_lineEdit_ProjectName_editingFinished();
    bool createFile();
    bool touchCMakeLists();

private:
    Ui::InitCMake *ui;
    bool isOK;
    ProjectInfo* m_info;//保存信息的对象实例
    QProgressBar* m_initProgress;//创建进度
    QStatusBar* m_statusBar;
    bool readProjectPath();//获取目录

};
#endif // INITCMAKE_H
