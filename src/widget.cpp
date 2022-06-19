//
// Created by 12038 on 2022/6/15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Widget.h" resolved

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //取消菜单栏
    this->setWindowFlags(Qt::FramelessWindowHint);

    //阴影边框效果
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(10);
    shadow->setColor(Qt::black);
    shadow->setOffset(0);

    ui->shadowWidget->setGraphicsEffect(shadow);

    //父窗口透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    //最大化最小化关闭功能实现
    connect(ui->btnMax, SIGNAL(clicked()), this, SLOT(btnMaxClickedSlot()));
    connect(ui->btnMin, SIGNAL(clicked()), this, SLOT(btnMinClickedSlot()));
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(btnCloseClickedSlot()));

    ui->btnMin->setStyleSheet("border-image: url(:/png/min.png)");
    ui->btnMax->setStyleSheet("border-image: url(:/png/fullscreen3.png)");
    ui->btnClose->setStyleSheet("border-image: url(:/png/close.png)");


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
}

Widget::~Widget()
{
    renameFile->deleteLater();
    myThread->exit();
    myThread->wait(10 * 1000);
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
//    QWidget::mousePressEvent(event);
    QPoint mouseStartPoint = event->globalPos();
    QPoint windowLeftTopPoint = this->geometry().topLeft();
    this->mousePosInWindow = mouseStartPoint - windowLeftTopPoint;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
//    QWidget::mouseMoveEvent(event);
    if(this->mousePosInWindow == QPoint()) return;
    QPoint mousePoint = event->globalPos();
    QPoint windowLeftTopPoint = mousePoint - this->mousePosInWindow;
    this->move(windowLeftTopPoint);
}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
    this->mousePosInWindow = QPoint();
}

void Widget::closeEvent(QCloseEvent *event)
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

void Widget::btnMaxClickedSlot()
{
    ui->btnMax->setStyleSheet("border-image: url(:/png/fullscreen4.png)");
    if(this->isMaximized()){
        ui->layoutMain->setMargin(9);
        ui->btnMax->setStyleSheet("border-image: url(:/png/fullscreen3.png)");
        this->showNormal();
    }
    else{
        ui->layoutMain->setMargin(0);
        ui->btnMax->setStyleSheet("border-image: url(:/png/fullscreen4.png)");
        this->showMaximized();
    }
}

void Widget::btnMinClickedSlot()
{
    this->showMinimized();
}

void Widget::btnCloseClickedSlot()
{
    this->close();
}

void Widget::selectFileClickedSlot() {
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

void Widget::selectDirClickedSlot() {
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

void Widget::processSlot(const int &now, const int &total) {
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(now);
}

void Widget::oldFileNameSlot(const QString &oldName) {
    ui->listWidget->addItem(oldName);
}

void Widget::newFileNameSlot(const QString &newName) {
    ui->listWidget_2->addItem(newName);
}

void Widget::btnConfirmClickedSlot() {
    QString s = ui->lineEdit->text();
    if(s.contains('/') || s.contains('\\') || s.contains('<') || s.contains('>') || s.contains('?') || s.contains('*') || s.contains(':') || s.contains('|') || s.contains('"')){
        QMessageBox::critical(this, "错误", "文件名中不能包含/\\|<>:?*等字符");
        return;
    }
    confirmFlag = true;
}





