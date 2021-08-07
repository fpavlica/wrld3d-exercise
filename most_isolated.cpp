#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "alglib/stdafx.h"
#include "alglib/alglibmisc.h"


// in our case, all place names start with the word place followed by an int
// therefore ignoring the "place" word and only using the ints for a simpler dataset
long getPlaceNameInt(std::string placeName) {
    placeName = placeName.substr(5); // 5 to end
    std::stringstream nameS(placeName);
    long placeInt = -1;
    if (!(nameS >> placeInt)){
        throw std::runtime_error("unexpected place name");
    }
    return placeInt;
}

// returns line info in a vector with [0] = x, [1] = y, [2] = placeNameInt
std::vector<long> parseLine(std::string line) {
    std::stringstream lineS(line);

    std::string name; 
    long x = 0, y = 0;
    if (!(lineS >> name)){
        throw std::runtime_error("error reading place name");
    }        
    if (!(lineS >> x)) {
        // error reading x
        std::string msg = "error reading x coordinate of ";
        throw std::runtime_error(msg + name);
    }
    if (!(lineS >> y)) {
        // error reading y
        std::string msg = "error reading y coordinate of ";
        throw std::runtime_error(msg + name);
    }
    
    long placeInt = getPlaceNameInt(name);
    std::vector<long> outVec(3);
    outVec[0] = x;
    outVec[1] = y;
    outVec[2] = placeInt;
    return outVec;
}

std::vector<long> readPointsFlat(std::istream& is = std::cin) {
    std::string line;
    std::vector<long> flatData;
    while (getline(is, line)) {

        std::vector<long> lineInfo3 = parseLine(line);

        // add input data to a flat vector
        // could maybe do some memory alloc optimisation but not needed.
        for (const auto& lineInfo1 : lineInfo3) {
            flatData.push_back(lineInfo1);
        }
    }
    return flatData;
}

template<typename T>
void printVector(std::vector<T> vec, std::ostream& os = std::cout) {
    for (const T& item : vec) {
        os << item << std::endl;
    }
}


void buildKdTree(std::vector<long> flatData) {
    const long* dataAsArr = &flatData[0];
    alglib::integer_2d_array arr;
    arr.setcontent(flatData.size() / 3, 3, dataAsArr);
}

int main() {
    std::string line;
    std::vector<long> flatData = readPointsFlat();
    printVector(flatData);
    
    return 0;
}
