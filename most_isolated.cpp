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
    double placeInt = 0;
    if (!(nameS >> placeInt)){
        throw std::runtime_error("unexpected place name");
    }
    return placeInt;
}

// returns line info in a vector with [0] = x, [1] = y, [2] = placeNameInt
std::vector<double> parseLine(std::string line) {
    std::stringstream lineS(line);

    std::string name; 
    double x = 0, y = 0;
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
    
    double placeInt = getPlaceNameInt(name);
    std::vector<double> outVec(3);
    outVec[0] = x;
    outVec[1] = y;
    outVec[2] = placeInt;
    return outVec;
}

std::vector<double> readPointsFlat(std::istream& is = std::cin) {
    std::string line;
    std::vector<double> flatData;
    while (getline(is, line)) {

        std::vector<double> lineInfo3 = parseLine(line);

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


alglib::kdtree* buildKdTree(std::vector<double> flatData) {

    std::cout<<"here" <<std::endl;
    const double* dataAsArr = &flatData[0];
    alglib::real_2d_array arr;
    arr.setcontent(flatData.size() / 3, 3, dataAsArr);
    std::cout<<"here2" <<std::endl;

    alglib::ae_int_t nx = 2, ny = 1, normtype = 2; // normtype 2 is euclidian 
    // TODO use smart pointers here
    alglib::kdtree *kdt = new alglib::kdtree;
    alglib::kdtreebuild(arr, nx, ny, normtype, *kdt);
    std::cout<<"here5" <<std::endl;
    return kdt;
}

void nearestNeighbour(alglib::real_1d_array x, alglib::kdtree& kdt){
    alglib::ae_int_t numResults = alglib::kdtreequeryknn(kdt, x, 2);
    if (numResults != 2) {
        throw std::runtime_error("alglib found multiple nearest neighbours");
    }
    alglib::real_2d_array results = "[[]]";
    alglib::kdtreequeryresultsx(kdt, results);
    std::cout << results.tostring(1) << std::endl;
}

int main() {
    std::string line;
    std::vector<double> flatData = readPointsFlat();
    // printVector(flatData);
    auto *kdt = buildKdTree(flatData);
    nearestNeighbour("[1312573, 8418602]", *kdt);
    
    return 0;
}
