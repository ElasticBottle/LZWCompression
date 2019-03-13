#include "lzwencoding.h"

unordered_map<string, int> buildCompressionDict() {
    unordered_map<string, int> dictionary;
    for (int i = 0; i < INITIAL_DICT_SIZE; ++i) {
        dictionary[string(1, i)] = i;
    }
    return dictionary;
}

Vector<int> encodeData(istream& input) {
    int dictSize = INITIAL_DICT_SIZE;
    unordered_map<string, int> dictionary = buildCompressionDict();

    string w = "";
    char ch;
    Vector<int> output;
    while (input.get(ch)) {
        if (dictionary.count(w + ch)) {
            w = w + ch;
        } else {
            dictionary[w + ch] = dictSize ++;
            output.push_back(dictionary[w]);
            w = ch;
        }
    }
    output.push_back(dictionary[w]);
    output.push_back(0);
    return output;
}

string convertToBinary(int toConvert) {
    string binary = "";
    while (toConvert > 0) {
        binary += to_string(toConvert % 2);
        toConvert /= 2;
    }
    while (binary.length() < 12) {
        binary += "0";
    }
    string toReturn = "";
    for (int i = 0; i < binary.length(); ++i) {
        toReturn += binary[binary.length() - 1 - i];
    }
    return toReturn;
}

void writeToFile(Vector<int> compressed, obitstream& output) {
    for (int i = 0; i < compressed.size(); ++i) {
        string toOutput = convertToBinary(compressed[i]);
        for (string::iterator it = toOutput.begin(); it != toOutput.end(); ++ it) {
            switch (*it) {
            case '1':
                output.writeBit(1);
                break;
            case '0':
                output.writeBit(0);
                break;
            }
        }
    }
}

void compress (ifstream& input, ofbitstream& output) {
    Vector<int> encodedData = encodeData(input);
    //writeToFile(encodedData, output);
    output << encodedData;
}

unordered_map<int, string> buildDecompressionDict() {
    unordered_map<int, string> dictionary;
    for (int i = 0; i < INITIAL_DICT_SIZE; ++i) {
        dictionary[i] = string(1, i);
    }
    return dictionary;
}

int getCode(ibitstream& input) {
    int code = 0;
    for (int i = 0; i < SIZE_OF_LZW_CHAR; ++i) {
        int bit = input.readBit();
        code += bit * pow(2, 11 - i);
    }
    return code;
}

string decodeData(ibitstream& input) {
    unordered_map<int, string> dictionary = buildDecompressionDict();
    int dictSize = INITIAL_DICT_SIZE;

    string output;
    int code = 0;
    int prevCode = getCode(input);

    output += dictionary[prevCode];

    while (true) {
        code = getCode(input);
        if (code == 0) break;
        if (dictionary.count(code)) {
            dictionary[dictSize ++] = dictionary[prevCode] + dictionary[code][0];
            output += dictionary[code];
        } else {
            dictionary[dictSize ++] = dictionary[prevCode] + dictionary[prevCode][0];
            output += dictionary[dictSize - 1];
        }
        prevCode = code;
    }
    return output;
}


void decompress (ifbitstream& input, ofstream& output) {
    output << decodeData(input);
}
