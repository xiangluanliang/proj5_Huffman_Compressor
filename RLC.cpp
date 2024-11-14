#include "RLC.h"

//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/13 17:24
// @Project: Proj5_Huffman_Compressor
//
//

RLC::RLC() = default;

void RLC::makeRLC(QString path_in, QString path_out) {

    QFile infile(path_in);
    if (!infile.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开输入文件";
        return;
    }

    QFile outfile(path_out);
    if (!outfile.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开输出文件";
        infile.close();
        return;
    }

    unsigned char currentChar, previousChar = 0xFF;
    int count = 0;

    while (!infile.atEnd()) {
        if (infile.read(reinterpret_cast<char *>(&currentChar), 1) != 1) break;

        if (currentChar == previousChar) {
            count++;
            if (count == 65536) {
                outfile.putChar(0xFF);
                outfile.putChar(0x51);
                outfile.putChar(count >> 8);
                outfile.putChar(count & 0xFF);
                outfile.putChar(previousChar);
                count = 0;
            }
        } else {
            if (count >= 2) {
                outfile.putChar(0xFF);
                if (count <= 15) {
                    unsigned char countByte =
                            (highFreqChars.contains(previousChar) ? highFreqChars[previousChar] : 0x5) << 4 | count;
                    outfile.putChar(countByte);
                } else if (count <= 256) {
                    //第四位为0
                    outfile.putChar((highFreqChars.contains(previousChar) ? highFreqChars[previousChar] : 0x5) << 4);
                    outfile.putChar(static_cast<unsigned char>(count));
                } else if (count <= 65536) {
                    outfile.putChar(
                            (highFreqChars.contains(previousChar) ? highFreqChars[previousChar] : 0x5) << 4 | 1);
                    outfile.putChar(count >> 8);
                    outfile.putChar(count & 0xFF);
                } else {
                    qDebug() << "出现超出2^16的重复次数的分割错误";
                }
                outfile.putChar(previousChar);
            } else if (count == 1) {
                if (previousChar == 0xFF) {
                    outfile.putChar(0xFF);
                    outfile.putChar(0x60);  // 表示原码中出现单个FF
                } else {
                    outfile.putChar(previousChar);
                }
            }
            previousChar = currentChar;
            count = 1;
        }
    }

    // 处理文件结尾的最后一个字符或重复序列
    if (count >= 2) {
        outfile.putChar(0xFF);
        if (count <= 15) {
            unsigned char countByte =
                    (highFreqChars.contains(previousChar) ? highFreqChars[previousChar] : 0x5) << 4 | count;
            outfile.putChar(countByte);
        } else if (count <= 256) {
            outfile.putChar((highFreqChars.contains(previousChar) ? highFreqChars[previousChar] : 0x5) << 4);
            outfile.putChar(static_cast<unsigned char>(count));
        } else if (count <= 65536) {
            outfile.putChar((highFreqChars.contains(previousChar) ? highFreqChars[previousChar] : 0x5) << 4 | 1);
            outfile.putChar(count >> 8);
            outfile.putChar(count & 0xFF);
        } else {
            qDebug() << "出现超出2^16的重复次数的分割错误";
        }
        outfile.putChar(previousChar);
    } else if (count == 1) {
        if (previousChar == 0xFF) {
            outfile.putChar(0xFF);
            outfile.putChar(0x60);
        } else {
            outfile.putChar(previousChar);
        }
    }

    // 关闭文件
    infile.close();
    outfile.close();
}

void RLC::decodeRLC(QString path_in, QString path_out) {
    QFile infile(path_in);
    if (!infile.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开输入文件";
        return;
    }

    QFile outfile(path_out);
    if (!outfile.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开输出文件";
        infile.close();
        return;
    }

    unsigned char currentChar, countByte, repeatedChar;
    int count;

    while (!infile.atEnd()) {
        if (infile.read(reinterpret_cast<char *>(&currentChar), 1) != 1) break;

        if (currentChar == 0xFF) {
            if (infile.read(reinterpret_cast<char *>(&countByte), 1) != 1) break;

            int highNibble = (countByte >> 4) & 0x0F;
            int lowNibble = countByte & 0x0F;

            if (countByte == 0x60) {  // 特殊情况：单独的FF字符
                outfile.putChar(0xFF);
            } else {
                if (lowNibble >= 2 && lowNibble <= 15) {
                    count = lowNibble;
                } else if (lowNibble == 0) {
                    unsigned char countByte2;
                    if (infile.read(reinterpret_cast<char *>(&countByte2), 1) != 1) break;
                    count = countByte2;
                } else if (lowNibble == 1) {
                    unsigned char countByte2, countByte3;
                    if (infile.read(reinterpret_cast<char *>(&countByte2), 1) != 1) break;
                    if (infile.read(reinterpret_cast<char *>(&countByte3), 1) != 1) break;
                    count = (countByte2 << 8) | countByte3;
                } else {
                    qDebug() << "无效的编码格式";
                    break;
                }

                if (highNibble == 5) {
                    if (infile.read(reinterpret_cast<char *>(&repeatedChar), 1) != 1) break;
                } else {
                    repeatedChar = highFreqCharsVec[highNibble];
                }

                for (int i = 0; i < count; i++) {
                    outfile.putChar(repeatedChar);
                }
            }
        } else {
            outfile.putChar(currentChar);
        }
    }

    infile.close();
    outfile.close();

    qDebug() << "解码完成并写入输出文件。";
}


