//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/10 12:09
// @Project: Proj5_Huffman_Compressor
//
//

#ifndef PROJ5_HUFFMAN_COMPRESSOR_COMPRESSOR_H
#define PROJ5_HUFFMAN_COMPRESSOR_COMPRESSOR_H

#include <QObject>
#include <QString>
#include <QIODevice>
#include <QDebug>
#include <iostream>
#include "RLC.h"
#include "Huffman.h"
class compressor : public QObject
{
Q_OBJECT

public:
    compressor();
    void encode(QString path_in,QString path_out);
    void decode(QString path_in,QString path_out);

};
#endif //PROJ5_HUFFMAN_COMPRESSOR_COMPRESSOR_H
