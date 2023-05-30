#include <getopt.h>
#include "Analysis.h"

void Version();
void helpFile();

int main(int argc, char ** argv)
{
    UserVariables myUserVariables;

    int c;
    static struct option loptions[] =
            {

                    {"vcf",    required_argument, nullptr, 'v'},
                    {"weight", required_argument, nullptr, 'w'},
                    {"output", required_argument, nullptr, 'o'},
                    {"format", required_argument, nullptr, 'f'},
                    {"memory", required_argument, nullptr, 'm'},
                    {"help",   no_argument      , nullptr, 'h'},
                    {nullptr,0,nullptr,0}
            };

    while ((c = getopt_long(argc, argv, "v:w:o:f:m:h",loptions,nullptr)) >= 0)
    {
        switch (c) {
            case 'v': myUserVariables.VcfFileName = optarg;   break;
            case 'w': myUserVariables.WeightFileName = optarg;   break;
            case 'o': myUserVariables.OutputPrefix = optarg;   break;
            case 'f': myUserVariables.Format = optarg;   break;
            case 'm': myUserVariables.Memory = optarg;   break;
            case 'h': helpFile(); return 0;
            case '?': helpFile(); return 0;
            default: printf("[ERROR:] Unknown argument: %s\n", optarg);
        }
    }

    if(!myUserVariables.CheckValidity()) return -1;

    Version();
    Analysis myAnalysis(myUserVariables);
    myAnalysis.myUserVariables->Status();

    time_t start_time = time(nullptr);
    myAnalysis.myUserVariables->CreateCommandLine(argc,argv);
    String MySuccessStatus = myAnalysis.Run();
    if(MySuccessStatus!="Success")
        return -1;
    time_t time_tot = time(nullptr) - start_time;
    cout << "\n Analysis finished." << endl;
    cout << "   -- Analysis took " << time_tot << " seconds." << endl;
    cout << "   -- Weight file contains " << myAnalysis.InputWeight.NoMarkers << " markers." << endl;
    cout << "   -- Calculation included " << myAnalysis.NoVariantsAnalyzed << " markers." << endl;
    cout << "   -- Skipped " << myAnalysis.InputWeight.NoMarkers-myAnalysis.NoVariantsAnalyzed << " markers not found in VCF file." << endl;
    cout << "   -- PRS for " << myAnalysis.NoGenes << " traits and " << myAnalysis.NoSamples << " samples are produced." << endl;

    return 0;
}

void Version()
{
    printf("\n ------------------------------------------------------------------------------ \n");
    printf("             PRScal -- A Tool to Assemble Polygenic Risk Score    \n");
    printf(" ------------------------------------------------------------------------------\n");
    printf(" (c) 2023 - Ketian Yu, Goncalo Abecasis \n");
    cout<< " Version : " << VERSION<< ";\n Built   : " << DATE << " by " << USER << endl;
}

void helpFile()
{
    Version();
    printf( " About   : Calculate PRS from individual-level of genotypes data given weights.\n");
    printf( " Usage   : PRScal [options] \n");
    printf( " Options :\n");
    printf( "   -v [dose.vcf.gz]               // [Required] Input Dosage File\n");
    printf( "   -w [weights.gz]                // [Required] Input Weight File\n");
    printf( "   -o [OutputPrefix]              // [Required] Output Prefix\n");
    printf( "   --format [GT/DS/HDS]           // [Optional] Genotype info format (Default: DS)\n");
    printf( "   --memory [3.5]                 // [Optional] Memory Limit (Default: 3.5GB)\n");
    printf( "   -h, --help                     //  If ON, detailed help on options and usage. \n");
    printf( " Note: the weight file should contain tab-separated columns:\n");
    printf( "       \t\t#CHROM\t|\tBP\t|\tID\t|\tREF\t|\tALT\t|\tBETA\n");
    printf( "       BETA field format is: gene1=weight1;gene2=weight2;...\n");
    cout<<endl;
}
