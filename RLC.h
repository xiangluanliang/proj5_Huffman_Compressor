//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/13 17:24
// @Project: Proj5_Huffman_Compressor
//
//

#ifndef PROJ5_HUFFMAN_COMPRESSOR_RLC_H
#define PROJ5_HUFFMAN_COMPRESSOR_RLC_H

#include <QString>
#include <QMap>
#include <vector>
#include <QFile>
#include <QDebug>
#include <QFileInfo>

class RLC{
public:
    RLC();
    void makeRLC(QString path_in,QString path_out);
    void decodeRLC(QString path_in,QString path_out);

private:
    QMap<unsigned char, unsigned char> highFreqChars = {
            {0x00, 0x0}, {0x20, 0x1}, {0x30, 0x2}, {0x80, 0x3}, {0x0D, 0x4}
    };
    std::vector<unsigned char> highFreqCharsVec = {0x00, 0x20,0x30, 0x80, 0x0D};
};
#endif //PROJ5_HUFFMAN_COMPRESSOR_RLC_H
