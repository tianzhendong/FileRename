//
// Created by 12038 on 2022/6/16.
//

#ifndef DUPLICATEFILESCHECK_DUPLICATEFILES_H
#define DUPLICATEFILESCHECK_DUPLICATEFILES_H

#include <QObject>
#include <QHash>
#include <QDebug>
#include <QFile>
#include <QCryptographicHash>
#include <QDir>

class Rename : public QObject{
Q_OBJECT

public:
    Rename(QObject *parent = nullptr);
    ~Rename();

signals:
    void oldFileNameSignal(const QString &);
    void newFileNameSignal(const QString &);
    void process(const int &, const int &);
    void singleFileSignal(const QString &);

    void delActionFeedbackSignal(bool delFlag);



public slots:
    void getFilesSlot(const QString & filesDirPath, const QString &strReplaced, const QString &strNew);
    void renameFileSlot(const QString &filePath, const QString &strReplaced, const QString &strNew);
    void checkBoxChangedSlot(int );
    void comboxStateChangedSlot(const QString &);

    void openDirSlot();
    void delActionTriggeredSlot();
    void getTextSlot(const QString &text);

private:
    void renameFile(const QString &filePath, const QString &strReplaced, const QString &strNew);
    QStringList getFiles(const QString &);

    QStringList oldList = QStringList();
    QStringList newList = QStringList();

    QString renameMethodFlag = "替换";
    QStringList dirFiles = QStringList();

    QString currentText = "";

};


#endif //DUPLICATEFILESCHECK_DUPLICATEFILES_H
