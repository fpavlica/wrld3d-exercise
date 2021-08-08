#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "alglib/stdafx.h"
#include "alglib/alglibmisc.h"


// with the given data, all place names start with the word place followed by an int
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


alglib::kdtree* buildKdTree(std::vector<double> &flatData) {

    const double* dataAsArr = &flatData[0];
    alglib::real_2d_array arr;
    arr.setcontent(flatData.size() / 3, 3, dataAsArr);

    alglib::ae_int_t nx = 2, ny = 1, normtype = 2; // normtype 2 is euclidian 
    // TODO use smart pointers here
    alglib::kdtree *kdt = new alglib::kdtree;
    alglib::kdtreebuild(arr, nx, ny, normtype, *kdt);
    return kdt;
}

alglib::real_2d_array nearestNeighbourAgRaw(alglib::real_1d_array x, alglib::kdtree& kdt){
    alglib::ae_int_t numResults = alglib::kdtreequeryknn(kdt, x, 2);
    if (numResults != 2) {
        throw std::runtime_error("alglib found multiple nearest neighbours");
    }
    alglib::real_2d_array results = "[[]]";
    alglib::kdtreequeryresultsx(kdt, results);
    return results;
}

std::vector<double> nearestNeighbourCpp(std::vector<double> point, alglib::kdtree& kdt){
    alglib::real_1d_array pointAg;
    pointAg.setcontent(2, &point[0]);

    auto resultsRaw = nearestNeighbourAgRaw(pointAg, kdt);
    std::vector<double> resultCpp(2);
    resultCpp[0] = resultsRaw[1][0];
    resultCpp[1] = resultsRaw[1][1];

    return resultCpp;
}

double nearestDistance(alglib::real_1d_array x, alglib::kdtree& kdt){
    
    alglib::ae_int_t numResults = alglib::kdtreequeryknn(kdt, x, 2);
    if (numResults != 2) {
        throw std::runtime_error("alglib found multiple nearest neighbours");
    }
    alglib::real_1d_array results = "[]";
    alglib::kdtreequeryresultsdistances(kdt, results);
    return results[1];
}

double nearestDistance(std::vector<double> point, alglib::kdtree &kdt) {
    alglib::real_1d_array pointAg;
    pointAg.setcontent(2, &point[0]);
    return nearestDistance(pointAg, kdt);
}

std::vector<double> allDistances(std::vector<double>& flatData) {

    auto *kdt = buildKdTree(flatData);

    std::vector<double> distances(flatData.size()/3);
    for (size_t i = 0; i < flatData.size(); i += 3) {
        std::vector<double> point(2);
        point[0] = flatData[i];
        point[1] = flatData[i+1];
        double distance = nearestDistance(point, *kdt);
        distances[i/3] = distance;
    }

    return distances;
}

//find index of smallest element
template<typename T>
size_t argMin(std::vector<T> vec) {
    auto minit = std::min_element(vec.begin(), vec.end());
    return std::distance(vec.begin(), minit);
}

//find index of biggest element
template<typename T>
size_t argMax(std::vector<T> vec) {
    auto maxit = std::max_element(vec.begin(), vec.end());
    return std::distance(vec.begin(), maxit);
}

std::string findMostIsolated(std::vector<double> flatData) {
    std::vector<double> distances = allDistances(flatData);
    size_t iMinDist = argMax<double>(distances);
    //probs instead have an array of names rather than this hack
    std::stringstream conv;
    conv << "place" << static_cast<long>(flatData[iMinDist*3 + 2]); //casting to long to prevent the case of exp notation
    return conv.str();
}

int main() {
    std::string line;
    std::vector<double> flatData = readPointsFlat();
    // printVector(flatData);

    std::string mostIsoPlace = findMostIsolated(flatData);
    std::cout << mostIsoPlace << std::endl;
    return 0;
}
