//
// Created by Ketian Yu on 3/29/23.
//

#ifndef PRSCAL_ANALYSIS_H
#define PRSCAL_ANALYSIS_H
#include "UserVariables.h"
#include "DosageFile.h"
#include "WeightFile.h"

class Analysis
{
public:
    UserVariables* myUserVariables;
    DosageFile InputDosage;
    WeightFile InputWeight;

    int NoSamples;
    int NoVariantsAnalyzed;

    vector<Dosage> DosageBuffer;
    int BufferBp;

    map<string, double> CurrentWeight;
    map<string, vector<double>> TempResult;

    Analysis(UserVariables &ThisUserVariables)
    :myUserVariables(&ThisUserVariables),
    InputDosage(ThisUserVariables.VcfFileName, ThisUserVariables.Format),
    InputWeight(ThisUserVariables.WeightFileName)
    {

    }

    String Run();
    bool SanityCheck();
    bool CheckWeightFile();
    bool OpenOutputFile() const;
    void OpenStreamInputFiles();
    bool CalculateScore();
    bool FindSamePosition();
    bool LoadBuffer();
    bool ProcessCommonVariant();
    void ParseWeight(string line);
    void CloseStreamInputFiles();
    bool OutputScores();

};

#endif //PRSCAL_ANALYSIS_H
