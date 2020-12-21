#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QObject>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class ProjectInfo : public QObject
{
    Q_OBJECT
public:
    explicit ProjectInfo(QObject *parent = nullptr);
    QString projectName() const;
    void setProjectName(const QString &projectName);

    QString projectPath() const;
    void setProjectPath(const QString &projectPath);

    QString sourcePath() const;
    void setSourcePath(const QString &sourcePath);

    QString execPath() const;
    void setExecPath(const QString &execPath);

    QString libPath() const;
    void setLibPath(const QString &libPath);

    QString cMakeVersion() const;
    void setCMakeVersion(const QString &cMakeVersion);
    void printInfo(ProjectInfo &r);
    bool m_createFile(QString s);
    void setConfigPath(const QString &configPath);
    bool touchDemo();
//private:
    QString m_projectName;//Project Name
    QString m_projectPath;// 项目目录
    QString m_sourcePath;//src
    QString m_execPath;//buid
    QString m_libPath;//LIBRARY_OUTPUT_PATH
    QString m_cMakeVersion;
    QString m_configPath;
    friend ostream &operator<<(ostream &os, const ProjectInfo &r);

};
ostream &operator<<(ostream &os, const ProjectInfo &r);

#endif // PROJECTINFO_H
