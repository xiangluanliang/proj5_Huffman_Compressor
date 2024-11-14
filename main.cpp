#include <QApplication>
#include "mainwindow.h"
#include "compressor.h"
#include <iostream>

int main(int argc, char *argv[]) {
//    if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
//        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow w;
    compressor *com = new compressor();
    QString path_in = "D:\\Code\\HomeWork\\ShiXunHomework_cxd\\Proj5_Huffman_Compressor\\测试\\庆余年.txt";
    QString path_out = "D:\\Code\\HomeWork\\ShiXunHomework_cxd\\Proj5_Huffman_Compressor\\测试\\out_庆余年";
    QString path_in2 = "D:\\Code\\HomeWork\\ShiXunHomework_cxd\\Proj5_Huffman_Compressor\\测试\\庆余年2.txt";

    com->encode(path_in, path_out);
    com->decode(path_out, path_in2);

    return a.exec();
}


