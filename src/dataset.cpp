#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;


vector<string> split(string str, char separator){
    replace(str.begin(), str.end(), separator, ' ');
    vector<string> res;
    stringstream ss(str);
    string temp;
    while (ss >> temp)
        res.push_back(temp);
    return res;
}


struct Rating{
    unsigned int user;
    unsigned int item;
    double rating;
};


class Dataset{
private:
    string currentLine;
    ifstream fileStream;
    string inputFileName;
    char separator;
    unsigned int nusers;
    unsigned int nitems;

public:
    Dataset(string inputFileName, char separator=',');
    ~Dataset();
    void reset();
    void next();
    Rating get();
    bool end();
    unsigned int users();
    unsigned int items();
};

Dataset::Dataset(string inputFileName, char separator){
    this->inputFileName = inputFileName;
    this->separator = separator;
    fileStream.open(inputFileName);
    // Compute the number of users and items
    Rating rating;
    nusers = 0; nitems = 0;
    for(this->reset(); !this->end(); this->next()){
        rating = this->get();
        nusers = max(nusers, rating.user + 1);
        nitems = max(nitems, rating.item + 1);
    };
    this->reset();
};

Dataset::~Dataset(){
    fileStream.close();
};

void Dataset::reset(){
    fileStream.clear();
    fileStream.seekg(0);    
    next();
};

void Dataset::next(){
    std::getline(fileStream, currentLine);
};

Rating Dataset::get(){
    vector<string> line = split(currentLine, separator);
    return {static_cast<unsigned int>(stoi(line[0]) - 1),
            static_cast<unsigned int>(stoi(line[1]) - 1),
            stod(line[2])};
};

bool Dataset::end(){
    return fileStream.eof();
};

unsigned int Dataset::users(){
    return nusers;
};

unsigned int Dataset::items(){
    return nitems;
};