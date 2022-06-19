//
// Created by 12038 on 2022/6/16.
//
#include <QDateTime>
#include "Rename.h"

Rename::Rename(QObject *) {

}

Rename::~Rename() {

}

void Rename::renameFileSlot(const QString &filePath, const QString &strReplaced, const QString &strNew) {
//    filePath.lastIndexOf('/')
    renameFile(filePath, strReplaced, strNew);
}

void Rename::renameFile(const QString &filePath, const QString &strReplaced, const QString &strNew) {
    QFile file(filePath);
    QFileInfo fileInfo(filePath);
    qDebug()<<fileInfo.path();
    qDebug()<<fileInfo.absolutePath();
    qDebug()<<fileInfo.fileName();
    qDebug()<<fileInfo.suffix();
    qDebug()<<fileInfo.baseName();
    QString baseName = fileInfo.baseName();
    QString newBaseName = baseName;

    QString oldFileName = file.fileName();
    qDebug()<< "old:" <<oldFileName;
    QString newFileName = oldFileName;

    if(renameMethodFlag == "时间戳") {
//        if(!baseName.contains("flyrecord")) return "";
        QString timeStampStr = baseName.section('-', -1, -1);
        QString firstName = baseName.section('-', -2, 0);
        qint64 timeStamp = timeStampStr.toLongLong();
        qDebug() << "时间戳" << timeStamp;
        QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(timeStamp);
        if((!dateTime.isValid()) || (timeStamp == 0) || (dateTime > QDateTime::currentDateTime())){
            emit newFileNameSignal(tr("时间戳有误"));
            return;
        };
        QString time = dateTime.toString("yyyyMMddhhmmss"); //时间戳非毫秒级
        QString time1 = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        qDebug() << time1;

        newBaseName = firstName + '-' +time;
    }
    else if(renameMethodFlag == "替换"){
        qDebug()<<"mode"<<"替换";
        if(!baseName.contains(strReplaced)) {
            emit newFileNameSignal("文件名中不存在输入的字符串");
            return;
        };
        if(strNew == ""){
            emit newFileNameSignal("新替换的字符串为空");
            return;
        }
        if(strReplaced == "") {
            newBaseName = strNew;
        }
        else{
            newBaseName = baseName.replace(strReplaced, strNew);
            qDebug()<<"newBaseName:"<<newBaseName;
        }
    }
    dirFiles.clear();
    dirFiles = getFiles(fileInfo.absolutePath());
    newFileName = fileInfo.absolutePath() + '/' + newBaseName + '.' + fileInfo.suffix();
    if(dirFiles.contains(newFileName)){
        newFileName = fileInfo.absolutePath() + '/' + newBaseName + '-' + baseName +'.' + fileInfo.suffix();
    }
    qDebug()<< "new:" <<newFileName;

    file.rename(newFileName);
    emit newFileNameSignal(newFileName);
}

void Rename::getFilesSlot(const QString &filesDirPath, const QString &strReplaced, const QString &strNew) {
    //获取文件夹下所有的文件
    QStringList files= this->getFiles(filesDirPath);
//    emit oldFileNameSignal(files);
    QStringList newFiles;
    for (int i = 0; i < files.count(); i++){
        QString fileName = files.at(i);
        emit oldFileNameSignal(fileName);
        renameFile(fileName, strReplaced, strNew);
//        emit newFileNameSignal(newFile);
        emit process(i + 1, files.count());
    }
}

QStringList Rename::getFiles(const QString &filesDirPath) {
    QStringList files;
    QDir filesDir(filesDirPath);
    QList<QFileInfo> fileInfoList = filesDir.entryInfoList(QDir::Files);
    for(int i = 0; i < fileInfoList.count(); i++){
        QFileInfo fileInfo = fileInfoList.at(i);
        if(fileInfo.isDir()){
//            QStringList subFiles= getFiles(fileInfo.absoluteFilePath());
//            files.append(subFiles);
            continue;
        }
        else{
            QString fileName = fileInfo.absoluteFilePath();
            files.append(fileName);
        }
    }
    return files;
}

void Rename::checkBoxChangedSlot(int) {

}

void Rename::comboxStateChangedSlot(const QString & text) {
    renameMethodFlag = text;
    qDebug()<< renameMethodFlag;
}

