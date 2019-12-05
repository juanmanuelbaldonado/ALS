#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>


std::vector<std::string> split(std::string str, char separator){
    std::replace(str.begin(), str.end(), separator, ' ');
    std::vector<std::string> res;
    std::stringstream ss(str);
    std::string temp;
    while (ss >> temp)
        res.push_back(temp);
    return res;
}


class CSVReader{
private:
    std::string currentLine;
    std::ifstream fileStream;
    std::string inputFileName;
    char separator;

public:
    CSVReader(std::string inputFileName, char separator=',');
    void next(); /* Go to next line */
    std::vector<std::string> get(); /* Get currrent line */
    bool end();
    ~CSVReader();
};

CSVReader::CSVReader(std::string inputFileName, char separator){
    this->inputFileName = inputFileName;
    this->separator = separator;
    fileStream.open(inputFileName);
    // Load first line.
    next();
}

CSVReader::~CSVReader(){
    fileStream.close();
};

void CSVReader::next(){
    std::getline(fileStream, currentLine);
}

std::vector<std::string> CSVReader::get(){
    return split(currentLine, separator);
}

bool CSVReader::end(){
    return fileStream.eof();
}
