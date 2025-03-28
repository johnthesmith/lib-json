/*
    List of Params
*/

#pragma once

#include <vector>
#include <memory>       /* std::unique_ptr */

#include "../core/heap.h"
#include "../core/rnd.h"

#include "param.h"



using namespace std;



typedef vector <string> Path;


class ParamList : public Heap
{
    private:

        Param*      parent = NULL;

        ParamList*  fromBufferInternal
        (
            void*&,          /* buffer */
            const size_t,    /* size of buffer */
            bool&
        );



        /*
            Push one elemen
        */
        ParamList* push
        (
            Param* a
        );

    public:


        /*
            Constructor
        */
        ParamList();


        /*
            Destructor
        */
        ~ParamList();



        /*
            Create object
        */
        static ParamList* create();



        /*
            Create selfdestruct object
        */
        static shared_ptr<ParamList> shared();



        void destroy() override;


        /*
            Add Params from argument list to this list
        */
        ParamList* add
        (
            ParamList*
        );




        /*
            Return index by Param
        */
        int indexBy
        (
            Param*
        );




        /*
            Return paramer or null by name
        */
        Param* getByName
        (
            string
        );



        /*
            Return parameter by list of name path
        */
        Param* getByName
        (
             /* Names of parameter */
            Path,
            /* Use pointer * */
            bool = false
        );



        /*
            Return Param by index
        */
        Param* getByIndex
        (
            int
        );



        /*
            Return Param by index
        */
        ParamList* setByIndex
        (
            int,
            Param*
        );



        /*
            Resize
        */
        ParamList* resize
        (
            int
        );




        /*
            Delete Param by index
        */
        ParamList* deleteParam
        (
            int /* Index of the Param */
        );



        /*
            Clear list
            All parameters will be destroy and list will be resize
        */
        ParamList* clear();



        /*
            Return Param by name
        */
        int getIndexByName
        (
            string  /* Name of parameter */
        );



        /*
            Return index by value
        */
        int getIndexByValue
        (
            string  /* Name of parameter */
        );



        string getString
        (
            int,            /* Index of parameter */
            string  = ""    /* Dafault value */
        );



//        string getString
//        (
//            string,         /* Name of parameter */
//            string  = ""    /* Dafault value */
//        );



        ParamList* loadString
        (
            string,         /* Name of parameter */
            string&,        /* Result */
            string  = ""    /* Dafault value */
        );



        string getString
        (
            Path,/* Name of parameter */
            string  = ""    /* Dafault value */
        );




        bool getBool
        (
            int,            /* Index of parameter */
            bool = false    /* Dafault value */
        );



//        bool getBool
//        (
//            string,         /* Name of parameter */
//            bool = false    /* Dafault value */
//        );



        /*
            Get boolean value by path
        */
        bool getBool
        (
            Path,           /* Name path of parameter */
            bool = false    /* default value */
        );



        /*
            Load bool value from param to var
        */
        ParamList* loadBool
        (
            string,         /* Name of parameter */
            bool&,          /* Place for result */
            bool = false    /* Default value */
        );



        /*
            Load bool value from param to var
        */
        ParamList* loadBool
        (
            Path,           /* Path of parameters */
            bool&,          /* Place for result */
            bool = false    /* Default value */
        );



        long long int getInt
        (
            int,                /* Name of parameter */
            long long int = 0   /* Dafault value */
        );



//        long long int getInt
//        (
//            string,             /* Name of parameter */
//            long long int = 0   /* Dafault value */
//        );



        /*
            Get integer value by path
        */
        long long int getInt
        (
            Path,    /* Name of parameter */
            long long int = 0   /* default value */
        );



        ParamList* loadInt
        (
            string,             /* Name of parameter */
            int&,               /* Result */
            int = 0             /* Dafault value */
        );



        ParamList* loadInt
        (
            string,             /* Name of parameter */
            unsigned int&,      /* Result */
            unsigned int = 0    /* Dafault value */
        );



        ParamList* loadInt
        (
            string,             /* Name of parameter */
            long long int&,
            long long int = 0   /* Dafault value */
        );



        double getDouble
        (
            int,            /* Name of parameter */
            double = 0.0    /* Dafault value */
        );


//
//        double getDouble
//        (
//            string,         /* Name of parameter */
//            double = 0.0    /* Dafault value */
//        );



        /*
            Get double value by path
        */
        double getDouble
        (
            Path,  /* Name of parameter */
            double = 0        /* default value */
        );





        ParamList* getData
        (
            int,     /* Name of parameter */
            char*&,
            size_t&
        );


//
//        ParamList* getData
//        (
//            string,         /* Name of parameter */
//            char*&,
//            size_t&
//        );
//


        /*
            Get double value by path
        */
        ParamList* getData
        (
            Path,  /* Name of parameter */
            char*&,
            size_t&
        );



//        /*
//            Get object value by name
//        */
//        ParamList* getObject
//        (
//            string,             /* Name of parameter */
//            ParamList* = NULL   /* Value */
//        );
//


        /*
            Get object value by index
        */
        ParamList* getObject
        (
            int aIndex,         /* Name of parameter */
            ParamList* = NULL   /* Value */
        );



        /*
            Get object value by path
        */
        ParamList* getObject
        (
            Path,               /* Name of parameter */
            ParamList* = NULL   /* default value */
        );


        /*
            Get object value by path and attribute value
        */
        ParamList* getObject
        (
            Path,               /* Path for list of keys */
            string,             /* Key attribute name */
            string              /* Value for key attribute */
        );



        /*
            Return object by path or root prm list
        */
        ParamList* selectObject
        (
            Path
        );



        /*
            Copy object to key
        */
        ParamList* copyFrom
        (
            string,       /* Parameter name */
            ParamList*    /* Source parameters */
        );



        /*
            Push vector of string values
        */
        ParamList* pushVector
        (
            vector<string> /* Values */
        );




        /*
            Push string value
        */
        ParamList* pushString
        (
            string = ""     /* Value */
        );


        /*
            Push bool value
        */
        ParamList* pushBool
        (
            bool = false     /* Value */
        );


        /*
            Push integer value
        */
        ParamList* pushInt
        (
            long long int = 0     /* Value */
        );


        /*
            Push double value
        */
        ParamList* pushDouble
        (
            double = 0.0     /* Value */
        );



        /*
            Push object value
        */
        ParamList* pushObject
        (
            ParamList*    /* Value */
        );




        /*
            Add and return new node
        */
        ParamList* addObject
        (
            string = "" /* Optional name of node */
        );



        /*
            Set string value
        */
        ParamList* setValue
        (
            string,         /* Name of parameter */
            string = ""     /* Value */
        );



        /*
            Set string value
        */
        ParamList* setString
        (
            string,         /* Name of parameter */
            string = ""     /* Value */
        );



        /*
            Set string value in to path
        */
        ParamList* setString
        (
            Path,           /* Path of parameter */
            string = ""     /* Value */
        );




        /*
            Set bool value
        */
        ParamList* setBool
        (
            string,         /* Name of parameter */
            bool = false    /* Value */
        );



        /*
            Set bool value
        */
        ParamList* setBool
        (
            Path,           /* Path of parameter */
            bool = false    /* Value */
        );



        /*
            Set int value
        */
        ParamList* setInt
        (
            string,             /* Name of parameter */
            long long int = 0   /* Value */
        );



        /*
            Set int value in to path
        */
        ParamList* setInt
        (
            Path,               /* Path of parameter */
            long long int = 0   /* Value */
        );



        /*
            Set double value
        */
        ParamList* setDouble
        (
            Path,           /* Path of parameter */
            double = 0.0    /* Value */
        );



        /*
            Set double value
        */
        ParamList* setDouble
        (
            string,         /* Name of parameter */
            double = 0.0    /* Value */
        );



        /*
            Set ParamList in to key
            !!! Without copy of param list.
        */
        ParamList* setObject
        (
            string,         /* Name of parameter */
            ParamList*      /* Value */
        );



        /*
            Set ParamList in to key
            !!! Without copy of param list.
        */
        ParamList* setObject
        (
            int,            /* Index of parameter */
            ParamList*      /* Value */
        );



        /*
            Set ParamList in to key
            !!! Without copy of param list.
        */
        ParamList* setObject
        (
            Path,           /* Path of the parameter */
            ParamList*      /* Value */
        );



        /*
            Set data buffer value
        */
        ParamList* setData
        (
            string,         /* Name of parameter */
            char*,          /* Value */
            size_t,         /* Size of value */
            bool    = true  /* Copy of value */
        );



        /*
            Set data buffer value
        */
        ParamList* setData
        (
            int,            /* Name of parameter */
            char*,          /* Value */
            size_t,         /* Size of value */
            bool    = true  /* Copy of value */
        );


        /*
            Set data value to path of keys
        */
        ParamList* setData
        (
            Path,           /* Path of parameter */
            char*,          /* Value */
            size_t,         /* Size of value */
            bool    = true  /* Copy of value */
        );



        ParamList* dump();



        ParamList* toStringInternal
        (
            stringstream&,
            int = 0 /* depth */
        );



        string toString();



        /*
            Loop with lyambda
        */
        template <typename Func>
        ParamList* loop
        (
            Func callback
        )
        {
            bool stop = false;
            int c = getCount();
            auto items = getItems();

            for( int i = 0; i < c && !stop; i++ )
            {
                stop = callback(( Param*) items[ i ] );
            }

            return this;
        }




        /*
            Loop with lyambda for objects
        */
        template <typename Func>
        ParamList* objectsLoop
        (
            Func callback
        )
        {
            bool stop = false;
            int c = getCount();
            auto items = getItems();

            for( int i = 0; i < c && !stop; i++ )
            {
                auto param = ( Param*) items[ i ];
                if( param -> isObject() )
                {
                    stop = callback( param -> getObject(), param -> getName() );
                }
            }

            return this;
        }



        /*
            Loop from existing path with lyambda for objects only
        */
        template <typename Func>
        ParamList* objectsLoop
        (
            Path aPath,
            Func aCallback
        )
        {
            auto object = getObject( aPath );
            if( object != NULL )
            {
                object -> objectsLoop( aCallback );
            }
            return this;
        }



        /*
            Recursive loop with lyambda muzzle
        */
        template <typename Func>
        ParamList* recursionLoop
        (
            Func /*<bool ( Param* )>*/ callback
        )
        {
            recursionLoopInternal( callback );
            return this;
        }



        /*
            Recursive loop internal with lyambda
            for recursionLoop method
        */
        template <typename Func>
        bool recursionLoopInternal
        (
            Func /* <bool ( Param* )>*/ callback
        )
        {
            bool stop = false;
            int c = getCount();
            auto items = getItems();

            for( int i = 0; i < c && !stop; i++ )
            {
                auto prm = ( Param*) items[ i ];

                stop = callback(( Param*) items[ i ] );
                if( !stop && prm -> isObject() )
                {
                    stop = prm -> getObject() -> recursionLoopInternal( callback );
                }
            }
            return stop;
        }





        /*
            Purge elements by lyambda
        */
        template <typename Func>
        ParamList* purge
        (
            Func /* <bool ( Param* )> */ callback
        )
        {
            int c = getCount();
            auto items = getItems();

            vector <int> list;

            /* Collect purging elements */
            for( int i = 0; i < c; i++ )
            {
                auto param = ( Param*) items[ i ];
                if( callback( param ))
                {
                    list.push_back( i );
                }
            }

            /* Remove and destroy elements */
            for( const auto& item: list )
            {
                auto param = (Param*) remove( item );
                param -> destroy();
            }

            return this;
        }



        ParamList* setParam
        (
            Param*
        );



        /*
            Copy param list from other param list
        */
        ParamList* copyTo
        (
            ParamList*
        );



        /*
            Copy param list from other param list
        */
        ParamList* copyFrom
        (
            ParamList*
        );



        bool contains
        (
            string
        );



        ParamList* toBuffer
        (
            void*&,    /* buffer */
            size_t&    /* size of buffer */
        );




        ParamList* fromBuffer
        (
            void*&,         /* buffer */
            const size_t    /* size of buffer */
        );



        ParamList* calcSize
        (
            size_t&
        );



        ParamList* fillBuffer
        (
            void*&, /* Buffer */
            size_t& /* Position */
        );



        ParamList* fillElementInBuffer
        (
            void*&,      /* Buffer */
            size_t&,     /* Position in buffer */
            Param*      aParam,
            void*,       /* Source value pointer */
            size_t       /* Size of source */
        );



        /*
            Create or find and return Param by path
        */
        Param* createParam
        (
            Path aName   /* Names of parameter */
        );



        /*
            Create path from vector
        */
        ParamList* setPath
        (
            Path /* Names of parameter */
        );


        /*
            Convert ParamList contains the array of string
            in to the Path vector
            Warning!!! this is not a pair for setPath
        */
        Path getPath
        (
            Path = {} /* Path to value with ParamList */
        );



        ParamList* setParent
        (
            Param*  /* Parent for this object */
        );



        Param* getParent();



        /*
            Return root element
        */
        ParamList* getRoot();


        /*
            Return random param if exists
        */
        Param* getRndItem
        (
            Rnd*
        );



        /*
            Return intersections with argiments
        */
        ParamList* intersect
        (
            ParamList*, /* Argument */
            ParamList*  /* Result */
        );



        /*
            Return true if intersections with argiment exists
        */
        bool isIntersect
        (
            ParamList* /* Argument */
        );



        /*
            Fill files from path to the param list
        */
        ParamList* filesFromPath
        (
            string
        );



        /*
            Deatach the value from paramlist
            Deatached valkue will be remove from this object and returns.
        */
        char* extractByIndex
        (
            int
        );


        bool exists
        (
            /* Path */
            Path,
            /* Uses pointer */
            bool = false
        );



        /*
            Compare the list of keys for this ParamList and argument ParamList
            it will return true for all keys equals.
        */
        bool isEqual
        (
            ParamList* aParamList,
            vector <string> aKeys
        );



        /*
            Return true if lyambda function return true
        */
        template <typename Func>
        Param* findFirst
        (
            Func /* <bool ( Param* )> */ aCallback
        )
        {
            Param* result = NULL;
            loop
            (
                [ &result, &aCallback ]
                ( Param* iParam )
                {
                    if( aCallback( iParam ))
                    {
                        result = iParam;
                    }
                    return result != NULL;
                }
            );
            return result;
        };



        /*
            Operations
        */

        /*
            Calculate summ of subkeys by path
        */
        double calcSum
        (
            Path    /* Path */
        );




        /*
            Return true if the value exists
        */
        bool valueExists
        (
            string  /* Value */
        );



        /*
            Return true if the value exists
        */
        bool valueExists
        (
            Path,
            string
        );



        /*
            Push object
        */
        ParamList* pushObject
        (
            /* Path for object */
            Path aPath,
            /* Value object */
            ParamList*
        );

};


