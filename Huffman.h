//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/13 17:39
// @Project: Proj5_Huffman_Compressor
//
//

#ifndef PROJ5_HUFFMAN_COMPRESSOR_HUFFMAN_H
#define PROJ5_HUFFMAN_COMPRESSOR_HUFFMAN_H

#include <QFile>
#include <QString>
#include <queue>
#include <string>
#include <QFileInfo>
#include <QMap>
#include <QDebug>
#include <iostream>
#include <QDataStream>

class HuffmanNode {
public:
    unsigned char data;
    int freq;
    HuffmanNode *left, *right;
    std::string binary_coding;

    HuffmanNode(unsigned char currentChar, int freq)
            : data(currentChar), freq(freq), left(nullptr), right(nullptr) {};

    bool operator>(const HuffmanNode &other) const {
        return freq > other.freq;
    };
};

class HuffmanCoding {
public:
    HuffmanCoding();

    HuffmanNode *buildTree(int *);

    void generateCodes(HuffmanNode *root, unsigned int code, int length,
                       QMap<unsigned char, std::pair<unsigned int, int>> &huffmanCodes);

    void serializeTree(HuffmanNode *node, QFile &outfile);
    void serializeTreeBits(HuffmanNode *node, QDataStream &out);
    HuffmanNode *deserializeTreeBits(QDataStream &in);
    HuffmanNode *deserializeTree(QFile &infile);

    void makeHuffman(QString path_in, QString path_out);
    void decodeHuffman(QString path_in, QString path_out);

    static QString get_suffix(QString path_in);
private:
    HuffmanNode *root;

};

#endif //PROJ5_HUFFMAN_COMPRESSOR_HUFFMAN_H
