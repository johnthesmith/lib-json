/*
    It is a dafault Param container
    Param has a
        Type
        Name
        Size
*/

#pragma once

#include <string>

// #include "param_list.h"
#include "../core/utils.h"




using namespace std;



class ParamList;
struct ArrayList;



class Param
{
    private:

        ParamType   type    = KT_UNKNOWN;
        string      name    = "";      /* Name of Param */
        char*       value   = NULL;
        size_t      size    = 0;

    public:

        /*
            Constructor
        */
        Param();



        /*
            Constructor with other parm
        */
        Param
        (
            Param*
        );



        /*
            Destructor
        */
        virtual ~Param();



        /*
            Create a new Param
        */
        static Param* create();




        /*
            Destroy the Param
        */
        virtual void destroy
        (
            bool = true
        );




        /*
            Return the type of the Param
        */
        virtual ParamType getType();



        /*
            Return the type of the Param
        */
        virtual string getNameOfType();



        /*
            Set Param Name
        */
        virtual Param* setName
        (
            string
        );



        /*
            Get Param name
        */
        string getName
        (
            string = ""
        );



        /******************************************************************************
            Specific methods
        */

        /*
            Return value
        */
        char* getValue();



        /*
            Set string value
                Warning!!!
                if Copy == false then source object will be destoryd by Param
        */
        Param* setValue
        (
            ParamType,
            char*,          /* Buffer */
            size_t  = 0,    /* Size of buffer */
            bool    = true  /* True for copy of buffer, false for set pointer of buffer */
        );



        /*
            Return size pointer
        */
        size_t getSize();



        /*
            Set new size
        */
        Param* resize
        (
            size_t  /* Size of buffer */
        );



        /*
            Return the value as string
        */
        string getString();



        /*
            Return the value as bool
        */
        bool getBool();



        /*
            Return the value as integer
        */
        long long int getInt();



        /*
            Return the value as float value
        */
        double getDouble();



        /*
            Return the value as object if defined
        */
        ParamList* getObject();



        /*
            Return the value as buffer if defined
        */
        char* getData();



        /*
            Set the value as string
        */
        Param* setString
        (
            string
        );



        /*
            Set the value as bool
        */
        Param* setBool
        (
            bool
        );



        /*
            Set the value as integer
        */
        Param* setInt
        (
            long long int
        );



        /*
            Set the value as float value
        */
        Param* setDouble
        (
            double
        );



        /*
            Set the value as object
        */
        Param* setObject
        (
            ParamList*
        );



        /*
            Set any data value
        */
        Param* setData
        (
            char*,      /* Buffer */
            size_t,     /* Size of buffer */
            bool = true /* Copy flag */
        );



        /*
            Create a new Param
        */
        Param* copyFrom
        (
            Param*
        );



        /*
            Create a new Param
        */
        bool isEqual
        (
            Param*
        );



        /*
            Return true if paramete is objec
        */
        bool isObject();
};
