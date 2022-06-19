//
// Created by 12038 on 2022/6/15.
//

#ifndef DUPLICATEFILESCHECK_WIDGET_H
#define DUPLICATEFILESCHECK_WIDGET_H

#include <QMouseEvent>
#include <QWidget>
#include <QPoint>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include "Rename.h"
#include <QDir>
#include <QDebug>
#include <QThread>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void closeEvent(QCloseEvent *event);

signals:
    void fileSelectedSignal(const QString &path, const QString &strReplaced, const QString &strNew);
    void getFilesSignal(const QString &dirPath, const QString &strReplaced, const QString &strNew);
    void comboxStateChangedSignal(const QString &);

public slots:
    void processSlot(const int &, const int &);
    void oldFileNameSlot(const QString & oldName);
    void newFileNameSlot(const QString & newName);

private slots:
    void btnMaxClickedSlot();
    void btnMinClickedSlot();
    void btnCloseClickedSlot();
    void selectFileClickedSlot();
    void selectDirClickedSlot();
    void btnConfirmClickedSlot();

private:
    Ui::Widget *ui;
    Rename * renameFile;
    QThread *myThread;

    QPoint mousePosInWindow = QPoint();
    bool confirmFlag = false;


};


#endif //DUPLICATEFILESCHECK_WIDGET_H
