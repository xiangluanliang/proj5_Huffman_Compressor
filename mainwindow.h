//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/10 10:50
// @Project: Proj5_Huffman_Compressor
//
//
#ifndef PROJ5_HUFFMAN_COMPRESSOR_MAINWINDOW_H
#define PROJ5_HUFFMAN_COMPRESSOR_MAINWINDOW_H
#include <QThread>
#include <QMainWindow>
#include <QMessageBox>
#include "compressor.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMessageBox msgBox;
    compressor* zip;

//    bool path_in_cor;
//    bool path_out_cor;
    int isCompressor;
    QThread *zip_thread; // 在这里使用了多线程
    void zip_finished();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

signals:
    void make_zip();
    void process_zip();

private:
    Ui::MainWindow *ui;
};
#endif //PROJ5_HUFFMAN_COMPRESSOR_MAINWINDOW_H
