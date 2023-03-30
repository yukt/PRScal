//
// Created by Ketian Yu on 3/29/23.
//

#include "Analysis.h"
#include<fstream>

String Analysis::Run()
{
    if(!SanityCheck())
        return "Input.VCF.Error";
    if(!CheckWeightFile())
        return "Weight.File.Error";
    if(!OpenOutputFile())
        return "File.Write.Error";

    OpenStreamInputFiles();
    CalculateScore();
    CloseStreamInputFiles();
    if(!OutputScores())
        return "File.Write.Error";
    return "Success";
}

bool Analysis::CheckWeightFile()
{
    if(!InputWeight.CheckValidity())
        return false;
    return true;
}

bool Analysis::SanityCheck()
{
    if(!InputDosage.CheckValidity())
        return false;
    NoSamples = InputDosage.NoSamples;
    return true;
}

void Analysis::OpenStreamInputFiles()
{
    InputDosage.OpenStream();
    InputWeight.OpenStream();
}

bool Analysis::CalculateScore()
{
    NoVariantsAnalyzed = 0;
    DosageBuffer.clear();

    while(true)
    {
        if(!FindSamePosition())
            return true;
        if(!LoadBuffer())
            return false;
        if(!ProcessCommonVariant())
            return true;
    }

    return true;
}

bool Analysis::FindSamePosition()
{
    if(InputDosage.CurrentVariantName=="No:More:Variant:Period")
        return false;
    if(InputWeight.CurrentVariantName=="No:More:Variant:Period")
        return false;

    while(true)
    {
        while(InputDosage.CurrentChr != InputWeight.CurrentChr)
        {
            while(InputDosage.CurrentChr < InputWeight.CurrentChr)
            {
                if(!InputDosage.ReadRecord())
                    return false;
            }
            while(InputDosage.CurrentChr > InputWeight.CurrentChr)
            {
                if(!InputWeight.ReadRecord())
                    return false;
            }
        }

        while(InputDosage.CurrentChr == InputWeight.CurrentChr && InputDosage.CurrentBp != InputWeight.CurrentBp)
        {
            if(InputDosage.CurrentBp < InputWeight.CurrentBp)
            {
                if(!InputDosage.ReadRecord())
                    return false;
            }
            else
            {
                if(!InputWeight.ReadRecord())
                    return false;
            }
        }

        if(InputDosage.CurrentChr == InputWeight.CurrentChr && InputDosage.CurrentBp == InputWeight.CurrentBp)
        {
            return true;
        }
    }
}

bool Analysis::LoadBuffer()
{
    DosageBuffer.clear();
    BufferBp = InputDosage.CurrentBp;

    while (InputDosage.CurrentBp == BufferBp)
    {
        Dosage CurrentDosage(InputDosage.CurrentVariantName, InputDosage.Format);

        if(CurrentDosage.LoadDosage(*(InputDosage.GenotypeInfo)))
            DosageBuffer.push_back(CurrentDosage);
        if(!InputDosage.ReadRecord())
            break;
    }
    return true;
}

bool Analysis::ProcessCommonVariant()
{
    if (DosageBuffer.empty())
        return true;
    while(InputWeight.CurrentBp == BufferBp)
    {
        int NoBufferVariants = DosageBuffer.size();
        for (int i=0; i<NoBufferVariants; i++)
        {
            if(InputWeight.CurrentVariantName == DosageBuffer[i].VariantName)
            {
                NoVariantsAnalyzed++;
                vector<double>& dose = DosageBuffer[i].dose;
                ParseWeight(InputWeight.CurrentBetaLine);

                map<string, double>::iterator it;

                for (it = CurrentWeight.begin(); it != CurrentWeight.end(); it++)
                {
                    string gene = it->first;
                    double weight = it->second;

                    map<string,vector<double>>::iterator it_rst = TempResult.find(gene);
                    if(it_rst != TempResult.end())
                    {
                        for(int j=0; j<NoSamples; j++)
                            it_rst->second[j] += weight * dose[j];
                    }
                    else
                    {
                        vector<double> score;
                        score.resize(NoSamples);
                        for(int j=0; j<NoSamples; j++)
                            score[j] = weight * dose[j];
                        TempResult[gene] = score;
                    }

                }
                break;
            }
        }
        if(!InputWeight.ReadRecord())
            return true;
    }
    return true;
}

void Analysis::ParseWeight(string line)
{
    CurrentWeight.clear();
    string term;
    istringstream ss(line);
    while( getline(ss, term, ';') )
    {
        size_t pos = term.find("=");
        string gene = term.substr(0, pos);
        double beta = stod(term.substr(pos+1));
        CurrentWeight[gene] = beta;
    }
}

void Analysis::CloseStreamInputFiles()
{
    InputDosage.CloseStream();
    InputWeight.CloseStream();
}

bool Analysis::OpenOutputFile() const
{
    ofstream OutFile(myUserVariables->OutputPrefix+".scores");
    if(!OutFile.is_open())
    {
        cout << "\n ERROR !!! Program could NOT create the output file " << myUserVariables->OutputPrefix+".scores" << " !!!" << endl;
        cout <<" Please check your write permissions in the output directory\n OR maybe the output directory does NOT exist ...\n";
        cout << "\n Program Exiting ... \n\n";
        return false;
    }
    OutFile.close();
    return true;
}

bool Analysis::OutputScores()
{
    FILE* OutFile = fopen(myUserVariables->OutputPrefix+".scores", "w");
    fprintf(OutFile, "Gene");
    for(int i=0; i<NoSamples; i++)
        fprintf(OutFile, "\t%s", InputDosage.SampleNames[i].c_str());
    fprintf(OutFile, "\n");

    map<string, vector<double>>::iterator it;

    for (it = TempResult.begin(); it != TempResult.end(); it++)
    {
        fprintf(OutFile, "%s", it->first.c_str());
        for(int i=0; i<NoSamples; i++)
            fprintf(OutFile, "\t%f", it->second[i]);
        fprintf(OutFile, "\n");
    }
    fclose(OutFile);
    return true;

}

