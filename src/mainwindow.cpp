//
// Created by 12038 on 2022/6/20.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("文件批量重命名工具-byTianZD");

    renameFile = new Rename();
    myThread = new QThread();
    renameFile->moveToThread(myThread);
    myThread->start();
    connect(renameFile, SIGNAL(destroyed(QObject *)),
            myThread, SLOT(deleteLater()));

//    ui->labelTitle->setText("      文件重命名");
    connect(ui->btnSelectFile, SIGNAL(clicked(bool)), this, SLOT(selectFileClickedSlot()));
    connect(ui->btnSelectDir, SIGNAL(clicked(bool)), this, SLOT(selectDirClickedSlot()));
    connect(this, SIGNAL(fileSelectedSignal(const QString &, const QString &, const QString &)),
            renameFile, SLOT(renameFileSlot(const QString &, const QString &, const QString &)));

    connect(this, SIGNAL(getFilesSignal(const QString &, const QString &, const QString &)),
            renameFile, SLOT(getFilesSlot(const QString &, const QString &, const QString &)));


    connect(renameFile, SIGNAL(process(const int &, const int &)),
            this, SLOT(processSlot(const int &, const int &)));
    connect(renameFile, SIGNAL(oldFileNameSignal(const QString &)),
            this, SLOT(oldFileNameSlot(const QString &)));
    connect(renameFile, SIGNAL(newFileNameSignal(const QString &)),
            this, SLOT(newFileNameSlot(const QString &)));
    connect(renameFile, SIGNAL(singleFileSignal(const QString &)),
            this, SLOT(newFileNameSlot(const QString &)));

    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)),
            renameFile, SLOT(comboxStateChangedSlot(const QString &)));

    connect(ui->btnConfirm, SIGNAL(clicked()),
            this, SLOT(btnConfirmClickedSlot()));

    ui->listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_listWidget_customContextMenuRequested(const QPoint &)));

    delAction = new QAction();
    delAction->setText("删除文件");
    openAction = new QAction();
    openAction->setText("打开文件目录");

    connect(delAction, SIGNAL(triggered(bool)),
            renameFile, SLOT(delActionTriggeredSlot()));
    connect(ui->listWidget_2, SIGNAL(currentTextChanged(const QString &)),
            renameFile, SLOT(getTextSlot(const QString &)));
    connect(openAction, SIGNAL(triggered(bool)),
            renameFile, SLOT(openDirSlot()));
    connect(renameFile, SIGNAL(delActionFeedbackSignal(bool)),
            this, SLOT(delActionFeedbackSlot(bool)));

}

MainWindow::~MainWindow() {
    renameFile->deleteLater();
    myThread->exit();
    myThread->wait(10 * 1000);
    openAction->deleteLater();
    delAction->deleteLater();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,tr("退出程序"),QString(tr("确认退出程序?")),QMessageBox::Yes|QMessageBox::No);
    if(button==QMessageBox::No)
    {
        event->ignore(); // 忽略退出信号，程序继续进行
    }
    else if(button==QMessageBox::Yes)
    {
        event->accept(); // 接受退出信号，程序退出
    }
}

void MainWindow::btnMaxClickedSlot()
{
    if(this->isMaximized()){
        this->showNormal();
    }
    else{
        this->showMaximized();
    }
}

void MainWindow::btnMinClickedSlot()
{
    this->showMinimized();
}

void MainWindow::btnCloseClickedSlot()
{
    this->close();
}

void MainWindow::selectFileClickedSlot() {
    if(!confirmFlag){
        QMessageBox::information(this, "提示", "请先确认配置信息：模式为"+ui->comboBox->currentText());
        return;
    }
    QString path = QFileDialog::getOpenFileName(
            this, "选择文件",
            "",
            "");
    if (path == "") return;

    ui->progressBar->setValue(0);
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->listWidget->addItem(path);

    emit fileSelectedSignal(path, ui->leMd5Show->text(), ui->lineEdit->text());
}

void MainWindow::selectDirClickedSlot() {
    if(!confirmFlag){
        QMessageBox::information(this, "提示", "请先确认配置信息：模式为"+ui->comboBox->currentText());
        return;
    }
    QString dirPathUrl = QFileDialog::getExistingDirectory(this, "选择文件夹", "");
    ui->lineDIrShow->setText(dirPathUrl);
    if (dirPathUrl == "") return;
    ui->progressBar->setValue(0);
    ui->listWidget->clear();
    ui->listWidget_2->clear();
//    ui->leMd5Show->clear();
    emit getFilesSignal(dirPathUrl, ui->leMd5Show->text(), ui->lineEdit->text());
}

void MainWindow::processSlot(const int &now, const int &total) {
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(now);
}

void MainWindow::oldFileNameSlot(const QString &oldName) {
    ui->listWidget->addItem(oldName);
}

void MainWindow::newFileNameSlot(const QString &newName) {
    ui->listWidget_2->addItem(newName);
}

void MainWindow::btnConfirmClickedSlot() {
    QString s = ui->lineEdit->text();
    if(s.contains('/') || s.contains('\\') || s.contains('<') || s.contains('>') || s.contains('?') || s.contains('*') || s.contains(':') || s.contains('|') || s.contains('"')){
        QMessageBox::critical(this, "错误", "文件名中不能包含/\\|<>:?*等字符");
        return;
    }
    confirmFlag = true;
}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
//    ui->listWidget_2->currentTextChanged()
    QMenu *menu = new QMenu(this);
//    menu->addAction(ui->actionAdd);
//    menu->addAction(ui->actionClear);
//    menu->addAction(ui->actionDelete);
//    menu->addAction(ui->actionInsert);

    menu->addAction(this->openAction);
    menu->addAction(this->delAction);
//    menu->addSeparator();

//    menu->addAction(ui->actionInit);
//    menu->addSeparator();
//    menu->addAction(ui->actionSelAll);
//    menu->addAction(ui->actionSelInv);
//    menu->addAction(ui->actionSelNone);
//    menu->addAction(ui->actionSelPopMenu);
    menu->exec(QCursor::pos());
    delete  menu;
}

void MainWindow::delActionFeedbackSlot(bool flag) {
    if(flag){
        qDebug()<<"remove item";
        QListWidgetItem * item = ui->listWidget_2->currentItem();
        qDebug()<<item;
        ui->listWidget_2->removeItemWidget(item);
        delete item;
    }
}




