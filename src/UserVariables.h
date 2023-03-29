//
// Created by Ketian Yu on 3/29/23.
//

#ifndef MAIN_CPP_USERVARIABLES_H
#define MAIN_CPP_USERVARIABLES_H

#include <string>
#include <StringBasics.h>
using namespace std;

class UserVariables {
public:
    String VcfFileName;
    string Format;
    String WeightFileName;
    String OutputPrefix;

    string CommandLine;

    UserVariables() {
        VcfFileName = "";
        WeightFileName = "";
        OutputPrefix = "";
        Format = "DS";
    };
};

#endif //MAIN_CPP_USERVARIABLES_H
