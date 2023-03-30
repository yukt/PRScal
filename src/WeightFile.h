//
// Created by Ketian Yu on 3/29/23.
//

#ifndef PRSCAL_WEIGHTFILE_H
#define PRSCAL_WEIGHTFILE_H

#include <string>
#include <StringBasics.h>
#include <map>
#include <sstream>

using namespace std;


class WeightFile
{
public:
    String FileName;
    IFILE InputWeightStream;

    int CurrentChr, CurrentBp;
    string CurrentVariantName;
    string CurrentBetaLine;

    int NoMarkers;

    WeightFile(String &filename)
    {
        FileName = filename;
    }

    bool CheckValidity();
    void OpenStream();
    bool ReadRecord();
    void CloseStream();

};

#endif //PRSCAL_WEIGHTFILE_H
