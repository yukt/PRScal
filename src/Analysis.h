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
    double memory_limit;
    int chunk = 0;
    map<string, int> map_last_seen;

    Analysis(UserVariables &ThisUserVariables)
    :myUserVariables(&ThisUserVariables),
    InputDosage(ThisUserVariables.VcfFileName, ThisUserVariables.Format),
    InputWeight(ThisUserVariables.WeightFileName)
    {
        memory_limit = ThisUserVariables.memory * 1e8;
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
    void Output(const string& gene, vector<double>& scores, FILE* OutFile);

};

#endif //PRSCAL_ANALYSIS_H
