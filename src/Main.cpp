#include <getopt.h>
#include "UserVariables.h"

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
                    {"help",   no_argument      , nullptr, 'h'},
                    {nullptr,0,nullptr,0}
            };

    while ((c = getopt_long(argc, argv, "v:w:o:h",loptions,nullptr)) >= 0)
    {
        switch (c) {
            case 'v': myUserVariables.VcfFileName = optarg;   break;
            case 'w': myUserVariables.WeightFileName = optarg;   break;
            case 'o': myUserVariables.OutputPrefix = optarg;   break;
            case 'h': helpFile(); return 0;
            case '?': helpFile(); return 0;
            default: printf("[ERROR:] Unknown argument: %s\n", optarg);
        }
    }
//
//    if(!myUserVariables.CheckValidity()) return -1;
//
//    aggRSquareVersion();
//    RSquare myAnalysis(myUserVariables);
//    myAnalysis.myUserVariables->Status();
//
//    int start_time = time(0);
//    myAnalysis.myUserVariables->CreateCommandLine(argc,argv);
//
//    String MySuccessStatus = myAnalysis.Analyze();
//
//    if(MySuccessStatus!="Success")
//        return -1;
//
//    int time_tot = time(0) - start_time;
//    cout << "\n Analysis finished." << endl;
//    cout << "   -- Found " << myAnalysis.NoCommonVariants << " common variants." << endl;
//    cout << "   -- Analysis included " << myAnalysis.NoCommonVariantsAnalyzed << " variants." << endl;
//    cout << "   -- Analysis took " << time_tot << " seconds." << endl;
    return 0;
}


void helpFile()
{

}
