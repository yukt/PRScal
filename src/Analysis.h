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

    int NoSamples, NoGenes;
    int NoVariantsAnalyzed;

    vector<Dosage> DosageBuffer;
    int BufferBp;

    map<string, double> CurrentWeight;
    map<string, vector<double>> TempResult;

    // The primary memory usage is for TempResult
    // a limit of memory 10GB allows storing 1e9 double numbers.
    unsigned int memory_limit = 1e9;
    int chunk = 0;
    map<string, int> map_occurrence;

    Analysis(UserVariables &ThisUserVariables)
    :myUserVariables(&ThisUserVariables),
    InputDosage(ThisUserVariables.VcfFileName, ThisUserVariables.Format),
    InputWeight(ThisUserVariables.WeightFileName)
    {

    }

    String Run();
    bool SanityCheck();
    bool CheckWeightFile();
    bool OpenOutputFile();
    void OpenStreamInputFiles();
    bool CalculateScore();
    bool FindSamePosition();
    bool LoadBuffer();
    bool ProcessCommonVariant();
    void ParseWeight(string line);
    void CloseStreamInputFiles();
    void OutputScores();

    void FlushTempResult();
    void OutputEntireResult();

};

#endif //PRSCAL_ANALYSIS_H
