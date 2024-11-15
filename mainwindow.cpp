//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/10 14:11
// @Project: Proj5_Huffman_Compressor
//
//
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), msgBox(this) // 记住父窗口，
        , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("哈夫曼RLC联合编码压缩器");
    isCompressor = 1;
//    path_in_cor = 0;
//    path_out_cor = 0;
    zip = new compressor();
    msgBox.setStandardButtons(QMessageBox::NoButton);

    zip_thread = new QThread;
    zip->moveToThread(zip_thread);
    ui->progressBar->hide();

    // 连接信号和槽，启用线程
    connect(this, &MainWindow::make_zip, zip, [=]() {
        QString path_in = ui->lineEdit->text();
        QString path_out = ui->lineEdit_2->text();
        zip->encode(path_in, path_out);
    });
    connect(this, &MainWindow::process_zip, zip, [=]() {
        QString path_in = ui->lineEdit->text();
        QString path_out = ui->lineEdit_2->text();
        zip->decode(path_in, path_out);
    });
    connect(zip, &compressor::zip_finished, this, &MainWindow::zip_finished);
    zip_thread->start();
}

MainWindow::~MainWindow() {
    delete ui;
//     调用quit()函数
    zip_thread->quit();
    // 调用wait()函数
    zip_thread->wait();
    delete zip_thread;
}

void MainWindow::on_pushButton_3_clicked() {

    if (isCompressor == 1) {
        zip->path_in = QFileDialog::getOpenFileName(this, "打开文件",
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        ui->lineEdit->setText(zip->path_in);
        if (!zip->path_in.isEmpty()) {
            QFileInfo temp(zip->path_in);
            // 获取文件的绝对路径
            QString path = temp.absoluteFilePath();
            // 获取文件的后缀
            QString suffix = temp.suffix();
            // 更改文件的后缀
            zip->path_out = path.replace(suffix, "yhr");
            ui->lineEdit_2->setText(zip->path_out);
        } else {
            ui->lineEdit_2->setText(zip->path_out);
            ui->lineEdit->setText(zip->path_in);
        }
    } else {
        zip->path_in = QFileDialog::getOpenFileName(this, "打开.yhr文件",
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                    "yhr Files (*.yhr)");
        ui->lineEdit->setText(zip->path_in);
        if (!zip->path_in.isEmpty()) {
            QFileInfo temp(zip->path_in);
            // 获取文件的绝对路径
            QString path = temp.absoluteFilePath();
            // 获取文件的后缀
            QString suffix = temp.suffix();
            // 更改文件的后缀
            zip->path_out = path.replace(suffix, zip->get_suffix());
            ui->lineEdit_2->setText(zip->path_out);
        } else {
            ui->lineEdit_2->setText(zip->path_out);
            ui->lineEdit->setText(zip->path_in);
        }
    }

}

void MainWindow::on_pushButton_4_clicked() {

    if (isCompressor == 1) {
        if (zip->path_in.isEmpty()) {
            QMessageBox::critical(this, "错误", "未选定输入的文件", QMessageBox::Ok);
            ui->lineEdit_2->setText("未选定输入的文件");
        } else {
            zip->path_out = QFileDialog::getExistingDirectory(this, "选择文件夹", QStandardPaths::writableLocation(
                    QStandardPaths::DesktopLocation));
            if (!zip->path_out.isEmpty()) {
                zip->path_out += +"/" + QFileInfo(zip->path_in).baseName() + ".yhr";
                ui->lineEdit_2->setText(zip->path_out);
            } else {
                ui->lineEdit_2->setText(zip->path_out);
                ui->lineEdit->setText(zip->path_in);
            }
        }
    } else if (isCompressor == 0) {
        if (zip->path_in.isEmpty()) {
            QMessageBox::critical(this, "错误", "未选定输入的文件", QMessageBox::Ok);
            ui->lineEdit_2->setText("未选定输入的文件");
        } else {
            // zip->path_out = QFileDialog::getExistingDirectory(this, "选择文件夹", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))+"/"+QFileInfo(zip->path_in).baseName()+"."+zip->get_suffix();
            zip->path_out = QFileDialog::getExistingDirectory(this, "选择文件夹", QStandardPaths::writableLocation(
                    QStandardPaths::DesktopLocation));
            if (!zip->path_out.isEmpty()) {
                zip->path_out += "/" + QFileInfo(zip->path_in).baseName() + "." + zip->get_suffix();
                ui->lineEdit_2->setText(zip->path_out);
            } else {
                ui->lineEdit_2->setText(zip->path_out);
                ui->lineEdit->setText(zip->path_in);
            }
        }
    }
}

void MainWindow::on_pushButton_clicked() {
    if (zip->path_in.isEmpty() || zip->path_out.isEmpty()) {
        QMessageBox::critical(this, "错误", "未选定输入或者输出的路径", QMessageBox::Ok);
        ui->lineEdit_2->setText(zip->path_out);
        ui->lineEdit->setText(zip->path_in);
        return;
    }
    if (!QFile(zip->path_in).open(QIODevice::ReadOnly)) // 当QFile匿名对象销毁的时候，qfile会自动关闭
    {
        QMessageBox::critical(this, "错误", "文件不可读", QMessageBox::Ok);
        ui->lineEdit_2->setText(zip->path_out);
        ui->lineEdit->setText(zip->path_in);
        return;
    }

    if (QFile(zip->path_in).size() > INT_MAX - 1) {
        QMessageBox::warning(this, "警告", "文件太大了，文件大小应小于2147483KB（2.1GB）！！", QMessageBox::Ok);
        ui->lineEdit_2->setText(zip->path_out);
        ui->lineEdit->setText(zip->path_in);
        return;
    }

    // 注意：这是三选
    // 如果用户取消了选择或者发生了错误，则返回QMessageBox::Cancel。因此，如果用户直接点击右上角的关闭按钮，也相当于取消了选择，所以也会返回QMessageBox::Cancel。
    if (QFile(zip->path_out).exists()) // exists()函数只能检查文件是否存在，而不能检查文件是否可写。文件是否可写取决于文件的权限和属性，以及操作,比如文件已经打开，不可写，但是存在
    {
        QMessageBox::StandardButton result = QMessageBox::warning(this, "警告", "文件已存在，是否覆盖",
                                                                  QMessageBox::Ok | QMessageBox::Cancel,
                                                                  QMessageBox::Cancel);
        if (result == QMessageBox::Cancel) {
            ui->lineEdit_2->setText(zip->path_out);
            ui->lineEdit->setText(zip->path_in);
            return;
        }
    }

    // 在这里文件已经创建，如果放在上面的上面，每次都会"警告", "文件已存在，是否覆盖"，但是上面的程序检测了if(!QFile(zip->path_out).open(QIODevice::WriteOnly))吗
    if (!QFile(zip->path_out).open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "错误", "无法写入文件", QMessageBox::Ok);
        ui->lineEdit_2->setText(zip->path_out);
        ui->lineEdit->setText(zip->path_in);
        return;
    }

    if (isCompressor == 1) {
        msgBox.show();
        // 进度条
        ui->progressBar->setMaximum(0);
        ui->pushButton->hide();
        ui->progressBar->show();
        emit make_zip();
    } else if (isCompressor == 0) {
        msgBox.show();
        ui->progressBar->setMaximum(0);
        ui->pushButton->hide();
        ui->progressBar->show();
        emit process_zip();
    }
}

void MainWindow::on_radioButton_clicked() {
    isCompressor = 1;
    ui->pushButton->setText("压缩");
    ui->lineEdit_2->setText(zip->path_out);
    ui->lineEdit->setText(zip->path_in);
}

void MainWindow::on_radioButton_2_clicked() {
    isCompressor = 0;
    ui->pushButton->setText("解压缩");
    ui->lineEdit_2->setText(zip->path_out);
    ui->lineEdit->setText(zip->path_in);
}

void MainWindow::zip_finished() {
    // 计算压缩比
    msgBox.setWindowTitle("已完成");
    if (isCompressor == 1) {
        msgBox.setText("请查看压缩率");
        qint64 originalSize = QFile(zip->path_in).size(); // byte
        qint64 compressedSize = QFile(zip->path_out).size();
        double compressionRatio = (double) compressedSize / originalSize;
        QString percent = QString::number(compressionRatio * 100, 'f', 5) + "%";
        /*
            压缩前:originalSize byte
            压缩后:compressedSize byte
            压缩率:percent
        */

        QMessageBox messageBox;
        messageBox.setWindowTitle("Compression Result");
        messageBox.setText(
                "压缩前:" + QString::number(originalSize) + " byte\n" + "压缩后:" + QString::number(compressedSize) +
                " byte\n" + "压缩率:" + percent + "\n");
        messageBox.setIcon(QMessageBox::Information);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
    }
    msgBox.hide();
    msgBox.setText("......");
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);
    QMessageBox::information(this, "提示", " 程序已完成", QMessageBox::Ok);
    ui->progressBar->hide();
    ui->pushButton->show();
    ui->progressBar->setValue(0);
    ui->lineEdit_2->setText(zip->path_out);
    ui->lineEdit->setText(zip->path_in);
}
