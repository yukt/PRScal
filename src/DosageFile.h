//
// Created by Ketian Yu on 3/29/23.
//

#ifndef PRSCAL_DOSAGEFILE_H
#define PRSCAL_DOSAGEFILE_H

#include <string>
#include <StringBasics.h>
#include "VcfFileReader.h"
#include "VcfHeader.h"

using namespace std;

class Dosage
{
public:
    string VariantName;
    string Format;
    vector<double> dose;

    Dosage(string &name, string &format)
    {
        VariantName = name;
        Format = format;
    }

    bool LoadDosage(VcfRecordGenotype& GenotypeInfo);
};

class DosageFile
{
public:
    String FileName;
    string Format;
    int NoMarkers, NoSamples;
    vector<string> SampleNames;
    string ChrId;

    VcfFileReader* InputDosageStream;
    VcfRecord*     CurrentRecord;
    VcfRecordGenotype* GenotypeInfo;
    int CurrentChr, CurrentBp;
    string CurrentVariantName;

    DosageFile(String &filename, string &format)
    {
        FileName = filename;
        Format = format;
    }

    // Check Validity
    bool CheckValidity();
    bool ValidFileType();
    bool ValidSampleInfo();
    void clearSampleName() { SampleNames.clear();}

    // Read Records
    void OpenStream();
    void CloseStream();
    bool ReadRecord();
    double GetDosage(int SampleId);
    double GetNumGeno(int SampleId);

};


#endif //PRSCAL_DOSAGEFILE_H
