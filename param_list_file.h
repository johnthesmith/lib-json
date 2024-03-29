/*
    List of Params with files work
*/

#pragma once

#include "param_list.h"
#include "../core/result.h"



using namespace std;



struct ParamListFile : public ParamList, public Result
{
    /*
        Create and return the ParamListFile object
    */
    static ParamListFile* create();


    /*
        Destroy the this object
    */
    void destroy() override;



    /*
        Load structure from json file
    */
    ParamListFile* fromJsonFile
    (
        string
    );



    ParamListFile* clear();
};


