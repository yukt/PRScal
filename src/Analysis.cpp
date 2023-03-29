//
// Created by Ketian Yu on 3/29/23.
//

#include "Analysis.h"

String Analysis::Run()
{
    OpenStreamInputFiles();
    return "Success";
}

void Analysis::OpenStreamInputFiles()
{
    InputDosage.OpenStream();
    Weight = ifopen(myUserVariables->WeightFileName, "r");
}
