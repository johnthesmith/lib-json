#pragma once

#include <stack>
#include <vector>
#include <memory>       /* std::unique_ptr */

#include "param_list.h"


#include "../core/result.h"
#include "../core/text.h"


enum PairPart
{
    PP_UNKNOWN  = 0,
    PP_NAME     = 1,
    PP_VALUE    = 2
};



struct JsonObject
{
    PairPart    pairPart        = PP_UNKNOWN;
    bool        fPairBegin      = false;

    bool        fArray          = false;
    bool        fEscape         = false;

    bool        fNameBegin      = false;
    bool        fNameEnd        = false;
    string      name            = "";

    bool        fValueBegin     = false;
    bool        fValueEnd       = false;
    bool        fStringBegin    = false;
    bool        fStringEnd      = false;

    string      value           = "";

    ParamList*  valueParamList  = NULL;
    ParamList*  paramList       = NULL;

    ParamType   valueType       = KT_UNKNOWN;
    JsonObject* prevJsonObject  = NULL;

    static JsonObject* create
    (
        JsonObject* = NULL
    );

    JsonObject* nameBegin();
    JsonObject* nameEnd();
    JsonObject* valueBegin();
    JsonObject* valueEnd();
    JsonObject* pairBegin();
    JsonObject* pairEnd();
    JsonObject* deleteObject
    (
        bool /* Destroy parmList object */
    );

    JsonObject* addChar
    (
        char
    );
};



/*
    Convert string to Json
*/
class Json : public Result
{
    private:

        int                 line                = 0;    /* Current line */
        int                 index               = 0;    /* Trace index of string */
        ParamList*          paramList           = NULL;

        /*
            Convert json to string
        */
        Json* toStringInternal
        (
            Text*,
            ParamList*
        );
    public:
        Json();
        ~Json();



        /*
            Create and return new json object
        */
        static Json* create();



        /*
            Create and return new json object
        */
        virtual void destroy();




        static shared_ptr<Json> shared();


        /*
            Mearge paramlist to other paramList
        */
        Json* copyTo
        (
            ParamList*
        );



        /*
            Convert string to Json
        */
        Json* fromString
        (
            const string
        );



        /*
            Convert json to string
        */
        string toString
        (
            bool = true  /* Compact mode withot tabs ans delimiters */
        );




        Json* fromFile
        (
            /* File name */
            string
        );



        Json* toFile
        (
            /* File name */
            string
        );



        Json* dump();


        /*
            Search all keys with name,
            load file from value,
            replace key value from file
        */
        Json* include
        (
            /* Key name for including */
            string = "include"
        );



        Json* overload
        (
            /* Key name for including */
            string = "overload"
        );


        Json* trace
        (
            char c,
            JsonObject*
        );



        Json* error
        (
            /* message */
            string,
            JsonObject*
        );



        static ParamType getType
        (
            string
        );



        string getString
        (
            string,
            string = ""
        );



        string getString
        (
            Path,
            string = ""
        );



        long long int getInt
        (
            string,
            long long int = 0
        );



        long long int getInt
        (
            vector <string>,
            long long int = 0
        );



        double getDouble
        (
            string,
            double = 0.0
        );



        double getDouble
        (
            vector <string>,
            double = 0.0
        );



        ParamList* getObject
        (
            string,
            ParamList* = NULL
        );



        ParamList* getObject
        (
            vector <string>,
            ParamList* = NULL
        );



        ParamList* getParamList();
};
