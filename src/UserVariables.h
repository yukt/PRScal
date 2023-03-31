//
// Created by Ketian Yu on 3/29/23.
//

#ifndef MAIN_CPP_USERVARIABLES_H
#define MAIN_CPP_USERVARIABLES_H

#include <string>
#include <StringBasics.h>
using namespace std;

class UserVariables {
public:
    String VcfFileName;
    string Format;
    String WeightFileName;
    String OutputPrefix;

    string CommandLine;

    UserVariables() {
        VcfFileName = "";
        WeightFileName = "";
        OutputPrefix = "";
        Format = "DS";
    };

    void Status()
    {
        cout << " Command Line Options: " << endl;
        printf( "   --vcf    [%s]\n", VcfFileName.c_str());
        printf( "   --weight [%s]\n", WeightFileName.c_str());
        printf( "   --output [%s]\n", OutputPrefix.c_str());
        printf( "   --format [%s]\n", Format.c_str());
    }

    void CreateCommandLine(int argc, char ** argv)
    {
        int len = 0;

        for (int i=0; i<argc; i++)
            len += strlen(argv[i]) + 1;

        char MyCommandLine[len];
        strcpy(MyCommandLine,argv[0]);

        for (int i=1; i<argc; i++)
        {
            strcat(MyCommandLine, " ");
            strcat(MyCommandLine, argv[i]);
        }
        CommandLine=MyCommandLine;
    }

    bool CheckValidity() const
    {
        if(VcfFileName == ""){
            cout<< " Missing -v [--vcf], a required parameter.\n\n";
            cout<< " Try -h [--help] for usage ...\n\n";
            cout<< " Program Exiting ...\n\n";
            return false;
        }
        if(WeightFileName == ""){
            cout<< " Missing -w [--weight], a required parameter.\n\n";
            cout<< " Try -h [--help] for usage ...\n\n";
            cout<< " Program Exiting ...\n\n";
            return false;
        }
        if(OutputPrefix == ""){
            cout<< " Missing -o [--output], a required parameter.\n\n";
            cout<< " Try -h [--help] for usage ...\n\n";
            cout<< " Program Exiting ...\n\n";
            return false;
        }
        if(Format!="DS" and Format!="GT" and Format!="HDS")
        {
            printf("ERROR !!! Invalid argument --format %s !!!\n", Format.c_str());
            return false;
        }

        return true;
    }
};

#endif //MAIN_CPP_USERVARIABLES_H
