#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>

#include "alglib/stdafx.h"
#include "alglib/alglibmisc.h"

#include "most_isolated.h"


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
std::tuple<double, double, std::string> MostIsolatedFinder::parseLine(std::string line) {
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
    
    return make_tuple(x,y, name);
}

void MostIsolatedFinder::readAllInput(std::istream &is) {
    std::string line;
    std::vector<std::string> nameList;
    double i = 0;
    while (getline(is, line)) {

        auto [x, y, name] = parseLine(line);
        this->flatPoints.push_back(x);
        this->flatPoints.push_back(y);
        this->flatPoints.push_back(i);
        this->placeNames.push_back(name);

        ++i;
    }
}

template<typename T>
void printVector(std::vector<T> vec, std::ostream& os = std::cout) {
    for (const T& item : vec) {
        os << item << std::endl;
    }
}

void MostIsolatedFinder::buildKdTree(){
    const double* dataAsArr = &(this->flatPoints[0]);
    alglib::real_2d_array arr;
    arr.setcontent(flatPoints.size() / 3, 3, dataAsArr);

    alglib::ae_int_t nx = 2, ny = 1, normtype = 2; // normtype 2 is euclidian 
    alglib::kdtreebuild(arr, nx, ny, normtype, this->kdt);
}

double MostIsolatedFinder::nearestDistanceAg(alglib::real_1d_array x, alglib::kdtree& kdt){
    
    alglib::ae_int_t numResults = alglib::kdtreequeryknn(kdt, x, 2);
    if (numResults > 2) {
        throw std::runtime_error("alglib found multiple nearest neighbours");
    } else if (numResults < 2) {
        throw std::runtime_error("not enough points in grid to look for neighbours");
    }
    alglib::real_1d_array results = "[]";
    alglib::kdtreequeryresultsdistances(kdt, results);
    // if I wanted to bind this with the index in the kdt, kdtreequeryresultsy but no need
    return results[1];
}

double MostIsolatedFinder::nearestDistance(std::tuple<double, double> point, alglib::kdtree &kdt) {
    alglib::real_1d_array pointAg;
    double x,y;
    std::tie(x,y) = point;
    double pointArr[] = {x, y};
    pointAg.setcontent(2, pointArr);
    return nearestDistanceAg(pointAg, kdt);
}

std::vector<double> MostIsolatedFinder::findAllDistances() {

    std::vector<double> distances(flatPoints.size()/3);
    for (size_t i = 0; i < flatPoints.size(); i += 3) {
        std::tuple<double, double> point = std::make_tuple(flatPoints[i], flatPoints[i+1]);
        double distance = nearestDistance(point, this->kdt);
        distances[i/3] = distance;
    }

    return distances;
}

//find index of biggest element
template<typename T>
size_t argMax(std::vector<T> vec) {
    auto maxit = std::max_element(vec.begin(), vec.end());
    return std::distance(vec.begin(), maxit);
}

std::string MostIsolatedFinder::findMostIsolated() {
    std::vector<double> distances = this->findAllDistances();
    size_t iMinDist = argMax<double>(distances);
    // indices are matched in the distances list and the places list
    return placeNames[iMinDist];
}

void MostIsolatedFinder::init(std::istream& is) {
    readAllInput(is);
    buildKdTree();
}

int main() {

    MostIsolatedFinder mif;
    mif.init(std::cin);

    std::string mostIsoPlace = mif.findMostIsolated();
    std::cout << mostIsoPlace << std::endl;

    return 0;
}
