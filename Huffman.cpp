#include <bitset>
#include "Huffman.h"
//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/13 17:43
// @Project: Proj5_Huffman_Compressor
//
//

HuffmanCoding::HuffmanCoding() : root(nullptr) {};

HuffmanNode *HuffmanCoding::buildTree(int *frequencyCount) {

    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, std::greater<HuffmanNode *>> pq;
    for (int i = 0; i < 256; i++) {
        if (frequencyCount[i] != 0)
//            std::cout<< frequencyCount[i] <<std::endl;

            pq.push(new HuffmanNode(i, frequencyCount[i]));
    }
    while (pq.size() != 1) {
        HuffmanNode *left = pq.top();
        pq.pop();
        HuffmanNode *right = pq.top();
        pq.pop();

        auto *newNode = new HuffmanNode(0, left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    root = pq.top();
    return root;
}

//先写一个弱智递归
void HuffmanCoding::generateCodes(HuffmanNode *root, unsigned int code, int length,
                   QMap<unsigned char, std::pair<unsigned int, int>> &huffmanCodes) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCodes[root->data] = std::make_pair(code, length);

    }

    generateCodes(root->left, (code << 1), length + 1, huffmanCodes);
    generateCodes(root->right, (code << 1) | 1, length + 1, huffmanCodes);
}

void HuffmanCoding::serializeTree(HuffmanNode* node, QFile& outfile) {
    if (!node) {
        // 用特殊标记表示空节点
        outfile.write("0", 1);
        return;
    }
    // 用 "1" 表示非空节点，然后写入字符
    outfile.write("1", 1);
    outfile.write(reinterpret_cast<const char*>(&node->data), 1);

    serializeTree(node->left, outfile);
    serializeTree(node->right, outfile);
}

// 反序列化Huffman树
HuffmanNode* HuffmanCoding::deserializeTree(QFile& infile) {
    char flag;
    infile.read(&flag, 1);
    if (flag == '0') return nullptr; // 空节点

    unsigned char data;
    infile.read(reinterpret_cast<char*>(&data), 1);
    HuffmanNode* node = new HuffmanNode(data, 0);

    node->left = deserializeTree(infile);
    node->right = deserializeTree(infile);

    return node;
}

void HuffmanCoding::makeHuffman(QString path_in, QString path_out) {

    //读入频率
    QFile infile(path_in);
    if (!infile.open(QIODevice::ReadOnly)) {
        qDebug() << "哈夫曼编码时输入文件打开失败。";
        return;
    }
    unsigned char temp;
    int frequencyCount[256] = {0};
    while (!infile.atEnd()) {
        infile.read(reinterpret_cast<char *>(&temp), 1);
        frequencyCount[temp]++;
    }
    //读完一次回到文件头
    infile.close();
    infile.open(QIODevice::ReadOnly);
    //生成树

    QMap<unsigned char, std::pair<unsigned int, int>> huffmanCodes;
    generateCodes(buildTree(frequencyCount), 0, 0, huffmanCodes);

    //输出
    QFile outfile(path_out);
    if (!outfile.open(QIODevice::WriteOnly)) {
        qDebug() << "哈夫曼编码时输出文件打开失败。";
        infile.close();
        return;
    }
    //先写序列化哈夫曼树
    serializeTree(root, outfile);
//    outfile.write((char *)frequencyCount, 256 * sizeof(int));

//    outfile.close();
//    return;

    unsigned char currentChar;
    unsigned int bitBuffer = 0;
    int bitCount = 0;

    while (infile.read(reinterpret_cast<char *>(&currentChar), 1)) {
        auto [code, length] = huffmanCodes[currentChar];
//        std::cout << std::endl;
        for (int i = length - 1; i >= 0; --i) {
            bitBuffer = (bitBuffer << 1) | ((code >> i) & 1);
//            std::cout << std::bitset<8>(bitBuffer) << " ";
            bitCount++;
            if (bitCount == 8) {
                outfile.write(reinterpret_cast<const char *>(&bitBuffer), 1);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        outfile.write(reinterpret_cast<const char *>(&bitBuffer), 1);
    }

    infile.close();
    outfile.close();
    root = nullptr;
}

void HuffmanCoding::decodeHuffman(QString path_in, QString path_out) {
    QFile infile(path_in);
    if (!infile.open(QIODevice::ReadOnly)) {
        qDebug() << "哈夫曼解码时输入文件打开失败。";
        return;
    }

    // 读取频率表
//    int frequencyCount[256] = {0};
//    infile.read(reinterpret_cast<char*>(frequencyCount), 256 * sizeof(int));

    // 使用频率表构建哈夫曼树
    QMap<unsigned char, std::pair<unsigned int, int>> huffmanCodes;
    root = deserializeTree(infile);
    generateCodes(root, 0, 0, huffmanCodes);

    // 输出文件准备
    QFile outfile(path_out);
    if (!outfile.open(QIODevice::WriteOnly)) {
        qDebug() << "哈夫曼解码时输出文件打开失败。";
        infile.close();
        return;
    }

    unsigned char bitBuffer = 0;
    HuffmanNode* currentNode = root;

    while (!infile.atEnd()) {
        infile.read(reinterpret_cast<char*>(&bitBuffer), 1);
//        std::cout << std::bitset<8>(bitBuffer) << " ";

        for (int i = 7; i >= 0; --i) {
            bool bit = (bitBuffer >> i) & 1;

            if (bit) {
                currentNode = currentNode->right;
            } else {
                currentNode = currentNode->left;
            }

            if (!currentNode->left && !currentNode->right) {
                outfile.write(reinterpret_cast<char*>(&currentNode->data), 1);
//                std::cout<< currentNode->data <<std::endl;

                currentNode = root;
            }
        }
    }

    infile.close();
    outfile.close();
    root = nullptr;
}

