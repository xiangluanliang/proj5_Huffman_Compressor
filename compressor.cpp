//
// @Author: 杨皓然 23301142
// @E-amil: 23301142@bjtu.edu.cn
// @CreateTime: 2024/11/10 12:10
// @Project: Proj5_Huffman_Compressor
//
//
#include "compressor.h"


compressor::compressor() : QObject(nullptr),path_in(""),path_out(""){
}

void compressor::encode(QString path_in, QString path_out) {
    RLC* r = new RLC();
    QString mid = "mid_file.rlc";
    r->makeRLC(path_in, mid);

    HuffmanCoding* h = new HuffmanCoding();
    h->makeHuffman(mid, path_out);

    QFile::remove(mid);
    std::cout<< "压缩完成" <<std::endl;
    emit zip_finished();

}


void compressor::decode(QString path_in, QString path_out) {
    // 解压哈夫曼编码
    HuffmanCoding* h = new HuffmanCoding();
    QString mid = "mid_file.rlc";
    h->decodeHuffman(path_in, mid);

    // 解压游程编码
    RLC* r = new RLC();
    r->decodeRLC(mid, path_out);
    QFile::remove(mid);
    
    std::cout<< "解压完成" <<std::endl;
    emit zip_finished();
}

//修改
QString compressor::get_suffix() {
    return "txt";
}





