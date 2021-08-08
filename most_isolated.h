#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>


#include "alglib/stdafx.h"
#include "alglib/alglibmisc.h"

class MostIsolatedFinder {
    alglib::kdtree kdt;
    std::vector<double> flatPoints;
    std::vector<std::string> placeNames;

    std::tuple<double, double, std::string> parseLine(std::string line);
    double nearestDistanceAg(alglib::real_1d_array x, alglib::kdtree& kdt);
    

public:
    void readAllInput(std::istream &is = std::cin);
    void buildKdTree();
    double nearestDistance(std::tuple<double, double> point, alglib::kdtree &kdt);
    std::vector<double> findAllDistances();

    std::string findMostIsolated();
    void init(std::istream &is = std::cin);


};