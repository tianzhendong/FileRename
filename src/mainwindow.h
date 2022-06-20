//
// Created by 12038 on 2022/6/20.
//

#ifndef DUPLICATEFILESCHECK_MAINWINDOW_H
#define DUPLICATEFILESCHECK_MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include "rename.h"
#include <QDir>
#include <QDebug>
#include <QThread>
#include <QFileDialog>
#include <QMenu>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void fileSelectedSignal(const QString &path, const QString &strReplaced, const QString &strNew);
    void getFilesSignal(const QString &dirPath, const QString &strReplaced, const QString &strNew);
    void comboxStateChangedSignal(const QString &);

public slots:
    void processSlot(const int &, const int &);
    void oldFileNameSlot(const QString & oldName);
    void newFileNameSlot(const QString & newName);

    void delActionFeedbackSlot(bool flag);

private slots:
    void btnMaxClickedSlot();
    void btnMinClickedSlot();
    void btnCloseClickedSlot();
    void selectFileClickedSlot();
    void selectDirClickedSlot();
    void btnConfirmClickedSlot();

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    Rename * renameFile;
    QThread *myThread;

    QPoint mousePosInWindow = QPoint();
    bool confirmFlag = false;

    QAction *delAction;
    QAction *openAction;
};


#endif //DUPLICATEFILESCHECK_MAINWINDOW_H
