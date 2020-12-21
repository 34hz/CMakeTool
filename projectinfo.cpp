#include "projectinfo.h"

ProjectInfo::ProjectInfo(QObject *parent) : QObject(parent)
{

}

QString ProjectInfo::projectName() const
{
    return m_projectName;
}

void ProjectInfo::setProjectName(const QString &projectName)
{
    m_projectName = projectName;
}

QString ProjectInfo::projectPath() const
{
    return m_projectPath;
}

void ProjectInfo::setProjectPath(const QString &projectPath)
{
    m_projectPath = projectPath;
}

QString ProjectInfo::sourcePath() const
{
    return m_sourcePath;
}

void ProjectInfo::setSourcePath(const QString &sourcePath)
{
    m_sourcePath = sourcePath;
}

QString ProjectInfo::execPath() const
{
    return m_execPath;
}

void ProjectInfo::setExecPath(const QString &execPath)
{
    m_execPath = execPath;
}

QString ProjectInfo::libPath() const
{
    return m_libPath;
}

void ProjectInfo::setLibPath(const QString &libPath)
{
    m_libPath = libPath;
}

QString ProjectInfo::cMakeVersion() const
{
    return m_cMakeVersion;
}

void ProjectInfo::setCMakeVersion(const QString &cMakeVersion)
{
    m_cMakeVersion = cMakeVersion;
}

void ProjectInfo::printInfo(ProjectInfo &r)
{
    cout<<r<<endl;
}

bool ProjectInfo::m_createFile(QString s)
{

    int isCreat;
    isCreat=mkdir(s.toStdString().c_str(), S_IRWXU|S_IRWXG|S_IRWXO ); //S_IRUSR | S_IWUSR | S_IRGRP |S_IWGRP |S_IROTH |S_IWOTH);
    if(isCreat){
        perror("create file error");

        return false;
    }
    else{

        cout<<"create OK"<<endl;
        return true;
    }
}

void ProjectInfo::setConfigPath(const QString &configPath)
{
    m_configPath = configPath;
}

bool ProjectInfo::touchDemo()
{
    ofstream file;
    file.open((m_projectPath+"/"+m_projectName+".cpp").toStdString());
    if(file.is_open()){
        file
        <<"#include <iostream>\n"
        <<"int main (int argc, char* argv[]){\n"
        <<"    std::cout<<\"Hello, World\"<<std::endl;\n"
        <<"    return 0;\n"
        <<"}\n"
          ;
        file.close();
        return true;
    }else{
        return false;
    }
}
ostream &operator<<(ostream &os, const ProjectInfo &r)
{
    os <<"ProjectName: "<< r.projectName().toStdString() <<endl
      <<"CMakeVersion: "<<r.cMakeVersion().toStdString()<<endl
     <<"ProjectPath: "<<r.projectPath().toStdString()<<endl
    <<"SrcPath: "<<r.sourcePath().toStdString()<<endl
    <<"exePath: "<<r.execPath().toStdString()<<endl
    <<"libPath: "<<r.libPath().toStdString();
    return os;
}
