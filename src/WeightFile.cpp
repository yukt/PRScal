//
// Created by Ketian Yu on 3/29/23.
//

#include <fstream>
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

inline bool ValidChrom(string chr)
{
    std::vector<string> ValidChromList {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16",
                                        "17","18","19","20","21","22","23","X","Y", "MT",
                                        "chr1","chr2","chr3","chr4","chr5","chr6","chr7","chr8","chr9","chr10",
                                        "chr11","chr12","chr13","chr14","chr15","chr16","chr17","chr18","chr19",
                                        "chr20","chr21","chr22","chr23","chrX","chrY", "chrMT"};
    for(string valid_chr : ValidChromList)
        if(chr == valid_chr)
            return true;
    return false;
}

bool ValidNumber(const string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    if(!s.empty() && it == s.end())
    {
        int number = stoi(s);
        if(number>0)
            return true;
    }
    return false;
}

bool WeightFile::CheckValidity()
{
    InputWeightStream = ifopen(FileName, "r");
    if(InputWeightStream==NULL)
    {
        cout << "\n ERROR !!! Program could NOT open Weight file : " << FileName << " !!! " << endl;
        cout << "\n Program Exiting ... \n\n";
        return false;
    }

    string line;

    while(line.length()==0 || line.find("#") == 0)
    {
        line = "";
        if(InputWeightStream->readLine(line) < 0)
        {
            cout << "\n ERROR !!! Weight " << FileName << " does not contain valid information !!! " << endl;
            cout << "\n Program Exiting ... \n\n";
            return false;
        }
    }
    ifclose(InputWeightStream);

    string chr, bp, rsid, ref, alt, weights, beta;
    istringstream iss(line);
    iss >> chr >> bp >> rsid  >> ref >> alt >> beta;

    if(chr.empty() || bp.empty() || rsid.empty() || ref.empty() || alt.empty() || beta.empty())
    {
        cout << "\n ERROR !!! Insufficient number of columns in weight file !!! " << endl;
        cout << "\n Program Exiting ... \n\n";
        return false;
    }

    if(!ValidChrom(chr))
    {
        cout << "\n ERROR !!! " << FileName << " contains invalid chromosome " << chr << " !!! " << endl;
        cout << "\n Program Exiting ... \n\n";
        return false;
    }

    if(!ValidNumber(bp))
    {
        cout << "\n ERROR !!! " << FileName << " contains invalid base pair position " << bp << " !!! " << endl;
        cout << "\n Program Exiting ... \n\n";
        return false;
    }


    return true;

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
    string chr, rsid, ref, alt;
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

