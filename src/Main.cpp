#include <getopt.h>
#include "Analysis.h"

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

    Analysis myAnalysis(myUserVariables);
    String MySuccessStatus = myAnalysis.Run();

    return 0;
}


void helpFile()
{

}
