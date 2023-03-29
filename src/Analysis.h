//
// Created by Ketian Yu on 3/29/23.
//

#ifndef PRSCAL_ANALYSIS_H
#define PRSCAL_ANALYSIS_H
#include "UserVariables.h"
#include "DosageFile.h"

class Analysis
{
public:
    UserVariables* myUserVariables;
    DosageFile InputDosage;
    IFILE Weight;

    Analysis(UserVariables &ThisUserVariables)
    :myUserVariables(&ThisUserVariables), InputDosage(ThisUserVariables.VcfFileName, ThisUserVariables.Format)
    {

    }

    String Run();
    void OpenStreamInputFiles();

};

#endif //PRSCAL_ANALYSIS_H
