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
    OutputScores();
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
    NoGenes = 0;
    DosageBuffer.clear();
    map_last_seen.clear();
    TempResult.clear();

    while(true)
    {
        if(!FindSamePosition())
            return true;
        if(!LoadBuffer())
            return false;
        if(!ProcessCommonVariant())
            return true;
    }

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
                if(TempResult.size() * NoSamples > memory_limit)
                    FlushTempResult();
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

bool Analysis::OpenOutputFile()
{
    FILE* OutFile = fopen(myUserVariables->OutputPrefix+".scores", "w");
    if(OutFile==nullptr)
    {
        cout << "\n ERROR !!! Program could NOT create the output file " << myUserVariables->OutputPrefix+".scores" << " !!!" << endl;
        cout <<" Please check your write permissions in the output directory\n OR maybe the output directory does NOT exist ...\n";
        cout << "\n Program Exiting ... \n\n";
        return false;
    }

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    fprintf(OutFile, "##filedate=%d.%d.%d\n",(timeinfo->tm_year + 1900),(timeinfo->tm_mon + 1) ,timeinfo->tm_mday);
    fprintf(OutFile, "##source=PRScal.v%s\n",VERSION);
    fprintf(OutFile, "##vcf=%s\n", InputDosage.FileName.c_str());
    fprintf(OutFile, "##format=%s\n", InputDosage.Format.c_str());
    fprintf(OutFile, "##weight=%s\n", InputWeight.FileName.c_str());
    fprintf(OutFile, "##PRScal_Command=%s\n", myUserVariables->CommandLine.c_str());

    fprintf(OutFile, "#Gene");
    for(int i=0; i<NoSamples; i++)
        fprintf(OutFile, "\t%s", InputDosage.SampleNames[i].c_str());
    fprintf(OutFile, "\n");
    fclose(OutFile);
    return true;
}

void Analysis::Output(const string& gene, vector<double>& scores, FILE* OutFile)
{
    fprintf(OutFile, "%s", gene.c_str());
    for(int i=0; i<NoSamples; i++)
    {
        double score = scores[i];
        if(score>-1e-4 and score<1e-4)
            fprintf(OutFile, "\t0");
        else
            fprintf(OutFile, "\t%.4f", score);
    }
    fprintf(OutFile, "\n");
    NoGenes++;
}

void Analysis::OutputEntireResult()
{
    FILE* OutFile = fopen(myUserVariables->OutputPrefix+".scores", "a");
    map<string, vector<double>>::iterator it;
    for (it = TempResult.begin(); it != TempResult.end(); it++)
        Output(it->first, it->second, OutFile);
    fclose(OutFile);
}



void Analysis::OutputScores()
{
    map<string, vector<double>>::iterator it;

    // if there is no temporary files, output all results directly in the output file
    if(chunk==0)
    {
        OutputEntireResult();
        return;
    }

    //if there are temporary files
    if(!TempResult.empty())
        FlushTempResult();

    FILE* OutFile = fopen(myUserVariables->OutputPrefix+".scores", "a");
    for(int k=1; k<=chunk; k++)
    {
        string InFileName(myUserVariables->OutputPrefix+".temp.chunk"+k+".scores");
        IFILE InFile = ifopen(InFileName.c_str(), "r");
        string line;

        while(InFile->readLine(line)>-1)
        {
            // load data for each line
            string gene;
            istringstream iss(line);
            iss >> gene;

            double score;
            vector<double> scores;
            scores.resize(NoSamples);
            for(int i=0; i<NoSamples; i++)
            {
                iss >> score;
                scores[i] = score;
            }

            it = TempResult.find(gene);

            if(it!=TempResult.end())
            {
                for(int i=0; i<NoSamples; i++)
                    it->second[i] += scores[i];
            }
            else if(map_last_seen[gene] > k)
            {
                TempResult[gene] = scores;
            }

            // if this is the last occurrence of the gene, output
            if(map_last_seen[gene]==k)
            {
                if(it!=TempResult.end())
                {
                    Output(gene, it->second, OutFile);
                    TempResult.erase(it);
                }
                else
                    Output(gene, scores, OutFile);
            }

            line = "";
        }
        ifclose(InFile);
        remove(InFileName.c_str());
    }
    fclose(OutFile);
    OutputEntireResult();
}

void Analysis::FlushTempResult()
{
    chunk++;
    FILE* OutFile = fopen(myUserVariables->OutputPrefix+".temp.chunk"+chunk+".scores", "w");

    map<string, vector<double>>::iterator it;

    for (it = TempResult.begin(); it != TempResult.end(); it++)
    {
        string gene = it->first;
        fprintf(OutFile, "%s", gene.c_str());
        for(int i=0; i<NoSamples; i++)
            fprintf(OutFile, "\t%f", it->second[i]);
        fprintf(OutFile, "\n");

        map_last_seen[gene] = chunk;
    }
    TempResult.clear();
    fclose(OutFile);
}

