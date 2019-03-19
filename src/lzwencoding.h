#ifndef LZWENCODING_H
#define LZWENCODING_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "vector.h"
#include <unordered_map>
#include "bitstream.h"

using namespace std;

const int INITIAL_DICT_SIZE = 256;
const int SIZE_OF_LZW_CHAR = 12;
const int MAX_DICT_SIZE = 4095;

Vector<int> encodeData(istream& intput);
string convertToBinary(int toConvert);
string decodeData(ibitstream& input);
void compress (ifstream& input, ofbitstream& output);
void decompress (ifbitstream& input, ofstream& output);

#endif // LZWENCODING_H
