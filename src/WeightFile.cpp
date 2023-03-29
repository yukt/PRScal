//
// Created by Ketian Yu on 3/29/23.
//

#include "WeightFile.h"

#define MAXBP 999999999

inline int chr2int(string &chr)
{
    if(chr == "chrX" or chr == "X")
        return 23;
    if(chr == "chrY" or chr == "Y")
        return 24;
    if(chr == "chrMT" or chr == "MT")
        return 25;
    if(chr[0]=='c')
        return stoi(chr.substr(3, chr.size()-1));
    return stoi(chr);
}


void WeightFile::OpenStream()
{
    InputWeightStream = ifopen(FileName, "r");
    NoMarkers = 0;
    ReadRecord();
}

bool WeightFile::ReadRecord()
{
    string line;

    while(line.length()==0 || line.find("#") == 0)
    {
        line = "";
        if(InputWeightStream->readLine(line) < 0)
        {
            CurrentChr = 99;
            CurrentBp = MAXBP;
            CurrentVariantName = "No:More:Variant:Period";
            return false;
        }
    }

    NoMarkers++;
    string chr, rsid, ref, alt, weights;
    istringstream iss(line);
    iss >> chr >> CurrentBp >> rsid  >> ref >> alt >> CurrentBetaLine;
    CurrentChr = chr2int(chr);
    CurrentVariantName = chr+":"+to_string(CurrentBp)+":"+ ref+":"+alt;

    return true;
}

void WeightFile::CloseStream()
{
    ifclose(InputWeightStream);
}

