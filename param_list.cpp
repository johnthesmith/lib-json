#include <iostream>
#include <filesystem>
#include <cstring>
#include <sstream>
#include <map>
#include <memory>  /* std::shared_ptr */

#include "../core/console.h"
#include "../core/str.h"
#include "../core/buffer_to_hex.h"

#include "param.h"
#include "param_list.h"



using namespace std;



ParamList::ParamList()
{
}




ParamList::~ParamList()
{
    clear();
}



ParamList* ParamList::create()
{
    return new ParamList();
}



shared_ptr<ParamList> ParamList::shared()
{
    return make_shared <ParamList>();
}



void ParamList::destroy()
{
    delete this;
}


/*
    Add Params from argument list to this list
*/
ParamList* ParamList::add
(
    ParamList* a
)
{
    Heap::add(( Heap* ) a );

    return this;
}



/*
    Return index by Param
*/
int ParamList::indexBy
(
    Param* a
)
{
    return Heap::indexBy(( void* ) a );
}



/*
    Return paramer by name
*/
Param* ParamList::getByName
(
    string a
)
{
    return ( Param* )getByIndex( getIndexByName( a ));
}



/*
    Return parameter by list of name path
*/
Param* ParamList::getByName
(
    /* Names of parameter */
    Path aPath,
    /* Use pointer * item if it exists */
    bool aUsesPointer
)
{
    Param* result = NULL;
    Param* param = NULL;
    int c = aPath.size();
    int i = 0;

    if( c > 0 )
    {
        do
        {
            ParamList* iParamList = param == NULL ? this : param -> getObject();
            param = iParamList -> getByName( aPath[ i ] );

            /* Check up the pointer exists if param is null */
            if( param == NULL && aUsesPointer )
            {
                param = iParamList -> getByName( "*" );
                if( param != NULL && param -> isObject() )
                {
                    auto pointerPath = param -> getObject() -> getPath();
                    for( int j = i; j < c; j ++ )
                    {
                        pointerPath.push_back( aPath[ j ]);
                    }

                    param = getRoot() -> getByName( pointerPath, true );
                    i = c - 1;
                }
            }

            /* Find param by numeric */
            if( param == NULL && isNum( aPath[ i ]) )

            {
                param = iParamList -> getByIndex( std::atoi( aPath[ i ].c_str() ));
            }

            if( i == c - 1 )
            {
                result = param;
            }

            i++;
        } while ( i < c && param != NULL && param -> getType() == KT_OBJECT );
    }
    return result;
}



/*
    Return index by Param
*/
Param* ParamList::getByIndex
(
    int a
)
{
    return ( Param* )Heap::getByIndex( a );
}




/*
    Set value by index
*/
ParamList* ParamList::setByIndex
(
    int aIndex,
    Param* aParam
)
{
    Heap::setByIndex(aIndex, ( void* ) aParam );
    return this;
}



/*
    Resize
*/
ParamList* ParamList::resize
(
    int a
)
{
    Heap::resize( a );
    return this;
}



/*
    Clear
*/
ParamList* ParamList::clear()
{
    int c = getCount();
    for( int i = 0; i < c; i ++ )
    {
        auto p = getByIndex( i );
        p -> destroy();
    }
    Heap::resize( 0 );
    return this;
}



/*
    Return Param index by Name
*/
int ParamList::getIndexByName
(
    string a    /* Name */
)
{
    int result = -1;

    if( a != "" )
    {
        int c = getCount();
        for( int i = 0; i < c && result == -1; i++ )
        {
            if( getByIndex( i ) -> getName() == a )
            {
                result = i;
            }
        }
    }

    return result;
}



/*
    Return Param index by value
*/
int ParamList::getIndexByValue
(
    string a    /* Name */
)
{
    int result = -1;

    if( a != "" )
    {
        int c = getCount();
        for( int i = 0; i < c && result == -1; i++ )
        {
            if( getByIndex( i ) -> getString() == a )
            {
                result = i;
            }
        }
    }

    return result;
}



/******************************************************************************
    get
*/


///*
//    Get string value
//*/
//string ParamList::getString
//(
//    string aName,   /* Name of parameter */
//    string aDefault /* Value */
//)
//{
//    return getString( getIndexByName( aName ), aDefault );
//}
//



/*
    Get string value
*/
ParamList* ParamList::loadString
(
    string aName,       /* Name of parameter */
    string& aResult,    /* Place for result */
    string aDefault     /* Value */
)
{
    aResult = getString( Path{ aName }, aDefault );
    return this;
}




/*
    Get string value
*/
string ParamList::getString
(
    int aIndex,   /* Name of parameter */
    string aDefault /* Value */
)
{
    auto p = getByIndex( aIndex );
    return
    p == NULL || p -> getType() == KT_UNKNOWN
    ? aDefault
    : p -> getString();
}



/*
    Get string value by full path
*/
string ParamList::getString
(
    Path aName,  /* Name of parameter */
    string aDefault         /* default value */
)
{
    Param* result = getByName( aName, true );
    return result == NULL ? aDefault : result -> getString();
}



/*
    get boolean value by index
*/
bool ParamList::getBool
(
    int aIndex,     /* Name of parameter */
    bool aDefault   /* Value */
)
{
    auto p = getByIndex( aIndex );
    return
    p == NULL || p -> getType() == KT_UNKNOWN
    ? aDefault
    : p -> getBool();
}



///*
//    Set boolean value by name
//*/
//bool ParamList::getBool
//(
//    string aName,   /* Name of parameter */
//    bool aDefault   /* Value */
//)
//{
//    return getBool( getIndexByName( aName ), aDefault );
//}




/*
    Get boolean value by path
*/
bool ParamList::getBool
(
    Path aName,  /* Name of parameter */
    bool aDefault           /* default value */
)
{
    Param* result = getByName( aName, true );
    return result == NULL ? aDefault : result -> getBool();
}



/*
    Load bool value from param to var
*/
ParamList* ParamList::loadBool
(
    string aName,   /* Name of parameter */
    bool& aResult,  /* Place for result */
    bool aDefault   /* Default value */
)
{
    aResult = getBool( Path{ aName }, aDefault );
    return this;
}



/*
    Load bool value from param to var
*/
ParamList* ParamList::loadBool
(
    Path aName,     /* Name of parameter */
    bool& aResult,  /* Place for result */
    bool aDefault   /* Default value */
)
{
    aResult = getBool( aName, aDefault );
    return this;
}



///*
//    Get integer value by name
//*/
//long long int ParamList::getInt
//(
//    string aName,   /* Name of parameter */
//    long long int aDefault    /* Value */
//)
//{
//    return getInt( getIndexByName( aName ), aDefault );
//}




/*
    get integer value by index
*/
long long int ParamList::getInt
(
    int aIndex,             /* Index of parameter */
    long long int aDefault  /* Value */
)
{
    auto p = getByIndex( aIndex );
    return
    p == NULL || p -> getType() == KT_UNKNOWN
    ? aDefault
    : p -> getInt();
}



/*
    Get integer value by path
*/
long long int ParamList::getInt
(
    Path aName,  /* Name of parameter */
    long long int aDefault  /* default value */
)
{
    Param* result = getByName( aName, true );
    return result == NULL ? aDefault : result -> getInt();
}



/*
    Get integer value
*/
ParamList* ParamList::loadInt
(
    string aName,       /* Name of parameter */
    int& aResult,       /* Result */
    int aDefault        /* Default value */
)
{
    aResult = (int) getInt( Path{ aName }, aDefault );
    return this;
}



/*
    Get string value
*/
ParamList* ParamList::loadInt
(
    string aName,           /* Name of parameter */
    long long int& aResult, /* Result */
    long long int aDefault  /* Default value */
)
{
    aResult = getInt( Path{ aName }, aDefault );
    return this;
}



/*
    Get string value
*/
ParamList* ParamList::loadInt
(
    string aName,           /* Name of parameter */
    unsigned int& aResult,  /* Result */
    unsigned int aDefault   /* Default value */
)
{
    aResult = getInt( Path{ aName }, aDefault );
    return this;
}




///*
//    Get double value by name
//*/
//double ParamList::getDouble
//(
//    string aName,   /* Name of parameter */
//    double aDefault    /* Value */
//)
//{
//    return getDouble( getIndexByName( aName ), aDefault );
//}



/*
    Get double value by index
*/
double ParamList::getDouble
(
    int aIndex,         /* Name of parameter */
    double aDefault     /* Value */
)
{
    auto p = getByIndex( aIndex );
    return
    p == NULL || p -> getType() == KT_UNKNOWN
    ? aDefault
    : p -> getDouble();
}



/*
    Get double value by path
*/
double ParamList::getDouble
(
    Path aName,  /* Name of parameter */
    double aDefault         /* default value */
)
{
    Param* result = getByName( aName, true );
    return result == NULL ? aDefault : result -> getDouble();
}





ParamList* ParamList::getData
(
    int aIndex,     /* Name of parameter */
    char*& aBuffer,
    size_t& aSize
)
{
    auto p = getByIndex( aIndex );
    if( p != NULL && p -> getType() != KT_UNKNOWN )
    {
        aBuffer = p -> getValue();
        aSize = p -> getSize();
    }
    return this;
}



//
//ParamList* ParamList::getData
//(
//    string aName,     /* Name of parameter */
//    char*& aBuffer,
//    size_t& aSize
//)
//{
//    return getData( getIndexByName( aName ), aBuffer, aSize );
//}



/*
    Get double value by path
*/
ParamList* ParamList::getData
(
    Path aName,  /* Name of parameter */
    char*& aBuffer,
    size_t& aSize
)
{
    Param* p = getByName( aName, true );
    if( p != NULL )
    {
        aBuffer = p -> getValue();
        aSize = p -> getSize();
    }
    return this;
}



///*
//    Get object value by name
//*/
//ParamList* ParamList::getObject
//(
//    string aName,   /* Name of parameter */
//    ParamList* aDefault    /* Value */
//)
//{
//    return getObject( getIndexByName( aName ), aDefault );
//}
//


/*
    Get object value by index
*/
ParamList* ParamList::getObject
(
    int aIndex,             /* Name of parameter */
    ParamList* aDefault     /* Value */
)
{
    auto p = getByIndex( aIndex );
    return
    p == NULL || p -> getType() == KT_UNKNOWN
    ? aDefault
    : p -> getObject();
}



/*
    Get object value by path
*/
ParamList* ParamList::getObject
(
    Path aName,      /* Name of parameter */
    ParamList* aDefault         /* default value */
)
{
    Param* result = getByName( aName, true );

    return
    result == NULL || !result -> isObject()
    ? aDefault
    : result -> getObject();
}



/*
    Get object value by path and attribute value
*/
ParamList* ParamList::getObject
(
    Path aPath,
    string aNameAttr,
    string aValue
)
{
    ParamList* result = NULL;
    auto section = getObject( aPath, NULL );
    if( section != NULL )
    {
        section -> loop
        (
            [ &aValue, &aNameAttr, &result ]
            ( Param* iParam )
            {
                bool stop = false;
                auto params = iParam -> getObject();

                if
                (
                    params != NULL &&
                    params -> getString( Path{ aNameAttr }) == aValue
                )
                {
                    stop = true;
                    result = params;
                }

                return stop;
            }
        );
    }
    return result;
}



/*
    Return object by path or root prm list
*/
ParamList* ParamList::selectObject
(
    Path aPath
)
{
    return getObject( aPath, this );
}




/*
    Push one elemen
*/
ParamList* ParamList::push
(
    Param* a
)
{
    a -> setParent( this );
    Heap::push( (void*)a );
    return this;
}





/*
    Push vector of string values
*/
ParamList* ParamList::pushVector
(
    vector<string> aValues /* Values */
)
{
    for( auto value:aValues )
    {
        pushString( value );
    }
    return this;
}




/*
    Push string value
*/
ParamList* ParamList::pushString
(
    string aValue   /* Default value */
)
{
    /* Create new string param */
    push( Param::create() -> setString( aValue ));
    return this;
}



/*
    Push string value
*/
ParamList* ParamList::pushInt
(
    long long int aValue   /* Default value */
)
{
    /* Create new string param */
    push( Param::create() -> setInt( aValue ));
    return this;
}



/*
    Push bool value
*/
ParamList* ParamList::pushBool
(
    bool aValue   /* Default value */
)
{
    /* Create new string param */
    push( Param::create() -> setBool( aValue ));
    return this;
}



/*
    Push double value
*/
ParamList* ParamList::pushDouble
(
    double aValue   /* Default value */
)
{
    /* Create new string param */
    push( Param::create() -> setDouble( aValue ));
    return this;
}



/*
    Push object value
*/
ParamList* ParamList::pushObject
(
    ParamList* aValue   /* Default value */
)
{
    /* Create new param */
    push( Param::create() -> setObject( aValue ));
    return this;
}



/*
    Push object
*/
ParamList* ParamList::pushObject
(
    /* Path for object */
    Path aPath,
    /* Value object */
    ParamList* aValue
)
{
    auto node = createParam( aPath );
    if( !node -> isObject() )
    {
        node -> setObject( ParamList::create() );
    }

    node -> getObject() -> push
    (
        Param::create() -> setObject( aValue )
    );

    return this;
}




/*
    Add and return new node
*/
ParamList* ParamList::addObject
(
    string aName
)
{
    auto result = ParamList::create();
    if( aName == "" )
    {
        /* Create new param */
        push( Param::create() -> setObject( result ));
    }
    else
    {
        /* Add param by name */
        setObject( aName, result );
    }
    return result;
}





/*
    Set string value
*/
ParamList* ParamList::setValue
(
    string aName,   /* Name of parameter */
    string aValue   /* Setting value */
)
{
    switch( getType( aValue ))
    {
        case KT_DOUBLE:
            setDouble( aName, toDouble( aValue ));
        break;
        case KT_INT:
            setInt( aName, toInt( aValue ));
        break;
        case KT_BOOL:
            setBool( aName, toBool( aValue ));
        break;
        default:
            setString( aName, aValue );
        break;
    }
    return this;
}



/*
    Create or find and return Param by path
*/
Param* ParamList::createParam
(
    Path aName   /* Names of parameter */
)
{
    Param* result = NULL;
    ParamList* current = this;
    int c = aName.size();
    for( int i = 0; i < c; i++ )
    {
        Param* param = current -> getByName( aName[ i ] );
        if( i < c - 1 )
        {
            /* Not a last element */
            if( param == NULL || param -> getType() != KT_OBJECT )
            {
                auto iParamList = ParamList::create();
                current -> setObject( aName[ i ], iParamList );
                current = iParamList;
            }
            else
            {
                current = param -> getObject();
            }
        }
        else
        {
            if( param == NULL )
            {
                param = Param::create() -> setName( aName[ i ] );
                current -> push( param );
            }
            result = param;
        }
    }
    return result;
}



/*
    Create path from vector
    Warning!!! this is not a pair for getPath
*/
ParamList* ParamList::setPath
(
    Path aName   /* Names of parameter */
)
{
    ParamList* result = this;
    int c = aName.size();
    for( int i = 0; i < c; i++ )
    {
        Param* param = result -> getByName( aName[ i ] );
        if( param == NULL || param -> getType() != KT_OBJECT )
        {
            auto iParamList = ParamList::create();
            result -> setObject( aName[ i ], iParamList );
            result = iParamList;
        }
        else
        {
            result = param -> getObject();
        }
    }
    return result;
}



/*
    Convert ParamList contains the array of string
    in to the Path vector
    Warning!!! this is not a pair for setPath
*/
Path ParamList::getPath
(
    Path aName  /* Path to value with ParamList */
)
{
    Path result;

    auto paramList = aName.size() == 0 ? this : getObject( aName );

    if( paramList != NULL )
    {
        paramList -> loop
        (
            [ &result ]
            ( Param* param )
            {
                result.push_back( param -> getString());
                return false;
            }
        );
    }

    return result;
}





/*
    Set bool value
*/
ParamList* ParamList::setBool
(
    Path aPath, /* Name of parameter */
    bool aValue /* Value */
)
{
    createParam( aPath ) -> setBool( aValue );
    return this;
}



/*
    Set bool value
*/
ParamList* ParamList::setBool
(
    string aName,   /* Name of parameter */
    bool aValue     /* Value */
)
{
    createParam( Path{ aName } ) -> setBool( aValue );
    return this;
}



/*
    Set string value
*/
ParamList* ParamList::setString
(
    Path    aPath,   /* Path of parameter */
    string  aValue   /* Default value */
)
{
    createParam( aPath ) -> setString( aValue );
    return this;
}



/*
    Set string value
*/
ParamList* ParamList::setString
(
    string aName,   /* Name of parameter */
    string aValue   /* Default value */
)
{
    createParam( Path { aName } ) -> setString( aValue );
    return this;
}


/*
    Set intg value
*/
ParamList* ParamList::setInt
(
    Path    aPath,   /* Path of parameter */
    long long int aValue   /* Default value */
)
{
    createParam( aPath ) -> setInt( aValue );
    return this;
}


/*
    Set intg value
*/
ParamList* ParamList::setInt
(
    string aName,   /* Name of parameter */
    long long int aValue   /* Default value */
)
{
    createParam( Path{ aName }) -> setInt( aValue );
    return this;
}



/*
    Set double value
*/
ParamList* ParamList::setDouble
(
    string aName,   /* Name of parameter */
    double aValue   /* Value */
)
{
    createParam( Path{ aName } ) -> setDouble( aValue );
    return this;
}



/*
    Set intg value
*/
ParamList* ParamList::setDouble
(
    Path    aPath,   /* Path of parameter */
    double aValue   /* Default value */
)
{
    createParam( aPath ) -> setDouble( aValue );
    return this;
}



/*
    Set param list
*/
ParamList* ParamList::setObject
(
    string aName,       /* Name of parameter */
    ParamList* aValue   /* Value */
)
{
    auto i = getIndexByName( aName );
    Param* p = NULL;

    if( i < 0 )
    {
        /* Create new object */
        p = Param::create() -> setName( aName );
        push( p );
    }
    else
    {
        /* Parameter exists */
        p = getByIndex( i );
        /* And it is not a string */
        p -> destroy();
        p = Param::create() -> setName( aName );
        setByIndex( i, p );
    }

    /* Set value */
    p -> setObject( aValue );

    return this;
}


/*
    Set ParamList in to key
    !!! Without copy of param list.
*/
ParamList* ParamList::setObject
(
    Path aPath,         /* Path of parameter */
    ParamList* aValue   /* Value */
)
{
    createParam( aPath ) -> setObject( aValue );
    return this;
}








/*
    Set param list
*/
ParamList* ParamList::setObject
(
    int aIndex,         /* Index of parameter */
    ParamList* aValue   /* Value */
)
{
    Param* p = getByIndex( aIndex );

    if( p != NULL )
    {
        /* And it is not a string */
        p -> destroy();
        p = Param::create();
        setByIndex( aIndex, p );
        /* Set value */
        p -> setObject( aValue );
    }

    return this;
}



/*
    Copy object to key
*/
ParamList* ParamList::copyFrom
(
    string aKey,         /* Parameter name */
    ParamList* aValue    /* Value */
)
{
    setObject( aKey, ParamList::create() -> copyFrom( aValue ));
    return this;
}


/*
    Set value
*/
ParamList* ParamList::setData
(
    string  aName,  /* Name of parameter */
    char*   aValue, /* Value */
    size_t  aSize,  /* Size of value */
    bool    aCopy   /* Copy of value for true */
)
{
    auto i = getIndexByName( aName );
    Param* p = NULL;

    if( i < 0 )
    {
        /* Create new object */
        p = Param::create() -> setName( aName );
        push( p );
    }
    else
    {
        /* Parameter exists */
        p = getByIndex( i );
        /* And it is not a string */
        p -> destroy();
        p = Param::create() -> setName( aName );
        setByIndex( i, p );
    }

    /* Set value */
    p -> setData( aValue, aSize, aCopy );

    return this;
}



/*
    Set param list
*/
ParamList* ParamList::setData
(
    int     aIndex,   /* Name of parameter */
    char*   aValue,   /* Value */
    size_t  aSize,    /* Size of value */
    bool    aCopy     /* Copy of value */
)
{
    Param* p = getByIndex( aIndex );

    if( p != NULL )
    {
        /* And it is not a string */
        p -> destroy();
        p = Param::create();
        setByIndex( aIndex, p );
        /* Set value */
        p -> setData( aValue, aSize, aCopy );
    }

    return this;
}



/*
    Set data in to path
*/
ParamList* ParamList::setData
(
    Path    aPath,  /* Name of parameter */
    char*   aValue, /* Buffer */
    size_t  aSize,  /* Size of buffer */
    bool    aCopy   /* Copy of value */
)
{
    createParam( aPath ) -> setData( aValue, aSize, aCopy );
    return this;
}




ParamList* ParamList::dump()
{
    cout << toString();
    return this;
}



ParamList* ParamList::toStringInternal
(
    stringstream& aResult,
    int depth
)
{
    int c = getCount();
    for( int i = 0; i < c; i++ )
    {
        auto p = getByIndex( i );
        string tab = "";
        tab.resize( 4 * depth, ' ' );

        aResult
        << tab
        << INK_GREY
        << p -> getNameOfType()
        << " "
        << INK_WHITE
        << p -> getName()
        ;

        switch( p -> getType() )
        {
            case KT_OBJECT:
                aResult
                << "\n";
                if( p -> getObject() != NULL )
                {
                    p -> getObject() -> toStringInternal( aResult, depth + 1 );
                }
            break;
            case KT_DATA:
            {
                auto b  = p -> getData();
                auto s  = p -> getSize();
                aResult
                << " "
                << "size: "
                << s
                << "\n"
                << bufferToHex( b, s, "", " | ", "\n", 4 , 32 )
                << "\n";
            }
            break;
            default:
                aResult
                << " "
                << INK_MAGENTA
                << getString( i )
                << INK_DEFAULT
                << "\n";
            break;
        }


    };
    return this;
}



string ParamList::toString()
{
    /* Create string stream */
    stringstream result;
    toStringInternal( result );
    return result.str();
}




ParamList* ParamList::setParam
(
    Param* aParam
)
{
    auto name = aParam -> getName();
    switch( aParam -> getType() )
    {
        case KT_NULL:
        case KT_UNKNOWN:
        case KT_ARRAY:
        break;
        case KT_DATA:
            setData( name, aParam -> getValue(), aParam -> getSize(), true );
        break;
        case KT_BOOL:
            setBool( name, aParam -> getBool() );
        break;
        case KT_STRING:
            setString( name, aParam -> getString() );
        break;
        case KT_INT:
            setInt( name, aParam -> getInt() );
        break;
        case KT_DOUBLE:
            setDouble( name, aParam -> getDouble() );
        break;
        case KT_OBJECT:
            auto valueParamList = aParam -> getObject();
            if( valueParamList != NULL )
            {
                auto newParamList = ParamList::create();
                valueParamList -> copyTo( newParamList );
                setObject( name, newParamList );
            }
        break;
    }
    return this;
}



/*
    Copy param list from other param list
*/
ParamList* ParamList::copyTo
(
    ParamList* aDestination
)
{
    loop
    (
        [ &aDestination ]
        ( Param* iParam )
        {
            aDestination -> setParam( iParam );
            return false;
        }
    );
    return this;
}



/*
    Copy param list from other param list
*/
ParamList* ParamList::copyFrom
(
    ParamList* aSource
)
{
    if( aSource != NULL )
    {
        aSource -> copyTo( this );
    }
    return this;
}



bool ParamList::contains
(
    string a
)
{
    bool result = false;

    loop
    (
        [ &result, &a ]
        (
            Param* iParam
        )
        {
            result = iParam -> getString() == a;
            return result;
        }
    );

    return result;
}



/*
    Copy params to buffer
    Example:
        void* buffer = NULL;
        size_t size = 0;
        params -> toBuffer( buffer, size );
        params::clearBuffer( buffer, size );
*/
ParamList* ParamList::toBuffer
(
    void*&  aBuffer,    /* buffer */
    size_t& aSize       /* size of buffer */
)
{
    aSize = 0;
    calcSize( aSize );

    aBuffer = ::operator new( aSize );

    size_t pos = 0;
    fillBuffer( aBuffer, pos );

    return this;
}



/*
    Fill params from buffer
*/
ParamList* ParamList::fromBuffer
(
    void*&  aBuffer,    /* buffer */
    size_t  aSize       /* size of buffer */
)
{
    bool stop = false;
    fromBufferInternal( aBuffer, aSize, stop );
    return this;
}



/*
    Fill params from buffer
*/
ParamList* ParamList::fromBufferInternal
(
    void*&  aBuffer,    /* buffer */
    size_t  aSize,      /* size of buffer */
    bool&   aStop
)
{
    size_t      pos = 0;
    ParamType   type = KT_UNKNOWN;
    size_t      sizeOfName = 0;
    size_t      sizeOfValue = 0;
    string      name  = "";

    while( pos < aSize && !aStop )
    {
        /* Read type of value */
        aStop = pos + sizeof( type ) > aSize  || aStop;
        if( !aStop )
        {
            memcpy( &type, &( (char*) aBuffer )[ pos ], sizeof( type ));
            pos += sizeof( type );
        }

        /* Read size of name */
        aStop = pos + sizeof( size_t ) > aSize || aStop;
        if( !aStop )
        {
            memcpy( &sizeOfName, &( (char*) aBuffer )[ pos ], sizeof( size_t ));
            pos += sizeof( size_t );
        }

        /* Read size of buffer */
        aStop = pos + sizeof( sizeOfValue ) > aSize || aStop;
        if( !aStop )
        {
            memcpy( &sizeOfValue, &( (char*) aBuffer )[ pos ], sizeof( sizeOfValue ));
            pos += sizeof( sizeOfValue );
        }

        /* Read name */
        aStop = pos + sizeOfName > aSize || aStop;
        if( !aStop )
        {
            name.resize( sizeOfName );
            memcpy( &name[0], &( (char*) aBuffer )[ pos ], sizeOfName );
            pos += sizeOfName;
        }

        /* Check size of value */
        aStop = pos + sizeOfValue > aSize || aStop;
        if( !aStop )
        {
            switch( type )
            {
                case KT_NULL:
                case KT_UNKNOWN:
                case KT_ARRAY:
                break;
                case KT_OBJECT:
                {
                    auto buffer = (void*) ( &( (char*) aBuffer )[ pos ]);
                    auto value = ParamList::create()
                    -> fromBufferInternal( buffer, sizeOfValue, aStop );

                    if( !aStop )
                    {
                       setObject( name, value );
                    }
                    else
                    {
                        value -> destroy();
                    }
                    pos += sizeOfValue;
                    break;
                }
                case KT_STRING:
                {
                    string value = "";
                    value.resize( sizeOfValue );
                    memcpy( &value[0], &( (char*) aBuffer )[ pos ], sizeOfValue );
                    pos += sizeOfValue;
                    setString( name, value );
                    break;
                }
                case KT_INT:
                {
                    long long int value = 0;
                    memcpy( &value, &( (char*) aBuffer )[ pos ], sizeOfValue );
                    pos += sizeOfValue;
                    setInt( name, value );
                    break;
                }
                case KT_BOOL:
                {
                    bool value = 0;
                    memcpy( &value, &( (char*) aBuffer )[ pos ], sizeOfValue );
                    pos += sizeOfValue;
                    setBool( name, value );
                    break;
                }
                case KT_DOUBLE:
                {
                    double value = 0;
                    memcpy( &value, &( (char*) aBuffer )[ pos ], sizeOfValue );
                    pos += sizeOfValue;
                    setDouble( name, value );
                    break;
                }
                case KT_DATA:
                {
                    setData( name,  &( (char*) aBuffer )[ pos ], sizeOfValue );
                    pos += sizeOfValue;
                    break;
                }
            }
        }
    }

    return this;
}



ParamList* ParamList::calcSize
(
    size_t& aSize
)
{
    int c = getCount();
    for( int i = 0; i < c; i++ )
    {
        auto p = getByIndex( i );
        aSize +=
        sizeof( ParamType )         /* Type of parameter */
        + sizeof( size_t )          /* Size of name parameter */
        + sizeof( size_t )          /* Size of value */
        + p -> getName().length()   /* Name length */
        ;

        switch( p -> getType() )
        {
            case KT_ARRAY:
            case KT_UNKNOWN:
            case KT_NULL:
            break;
            case KT_OBJECT:
                p -> getObject() -> calcSize( aSize );
            break;
            case KT_STRING:
            case KT_INT:
            case KT_DOUBLE:
            case KT_BOOL:
            case KT_DATA:
                aSize += p -> getSize();
            break;
        }
    };

    return this;
}



ParamList* ParamList::fillBuffer
(
    void*&   aBuffer,    /* Buffer */
    size_t&  aPos        /* Position */
)
{
    int c = getCount();
    for( int i = 0; i < c; i++ )
    {
        auto p = getByIndex( i );

        /* Write value */
        switch( p -> getType() )
        {
            case KT_NULL:
            case KT_ARRAY:
            case KT_UNKNOWN:
            break;
            case KT_OBJECT:
            {
                auto value =  p -> getObject();
                size_t valueSize = 0;
                value -> calcSize( valueSize );

                fillElementInBuffer( aBuffer, aPos, p, NULL, valueSize );

                /* Recurcive write object ot buffer */
                value -> fillBuffer( aBuffer, aPos );
                break;
            }
            case KT_STRING:
            case KT_INT:
            case KT_BOOL:
            case KT_DOUBLE:
            case KT_DATA:
            {
                fillElementInBuffer( aBuffer, aPos, p, p -> getValue(), p -> getSize() );
                break;
            }
        }
    };
    return this;
}



ParamList* ParamList::fillElementInBuffer
(
    void*&      aBuffer,        /* Memory buffer for filling */
    size_t&     aPos,           /* Current position */
    Param*      aParam,
    void*       aSource,        /* Value of parameter */
    size_t      aSourceSize     /* Size of value */
)
{
    ParamType   type = aParam -> getType();
    string      name = aParam -> getName();
    size_t      sizeOfName = name.length();

    /* Write type of value */
    memcpy( &( (char*) aBuffer )[ aPos ], &type, sizeof( type ));
    aPos += sizeof( type );

    /* Write size of name */
    memcpy( &( (char*) aBuffer )[ aPos ], &sizeOfName, sizeof( size_t ));
    aPos += sizeof( size_t );

    /* Write size of buffer */
    memcpy( &( (char*) aBuffer )[ aPos ], &aSourceSize, sizeof( aSourceSize ));
    aPos += sizeof( aSourceSize );

    /* Write name */
    memcpy( &( (char*) aBuffer )[ aPos ], &name[ 0 ], sizeOfName );
    aPos += sizeOfName;

    if( aSource != NULL )
    {
        /* Recurcive write object ot buffer */
        memcpy( &( (char*) aBuffer )[ aPos ], aSource, aSourceSize );
        aPos += aSourceSize;
    }

    return this;
}



ParamList* ParamList::setParent
(
    Param* a
)
{
    parent = a;
    return this;
}



Param* ParamList::getParent()
{
    return parent;
}



ParamList* ParamList::getRoot()
{
    auto result = this;
    while( result -> getParent() != NULL )
    {
        result = result -> getParent() -> getParent();
    }
    return result;
}




/*
    Return random param if exists
*/
Param* ParamList::getRndItem
(
    Rnd* aRnd
)
{
    auto c = getCount();
    return  ( c > 0 )
    ? getByIndex( aRnd -> get( 0, c - 1 ))
    : NULL;
}



/*
    Return intersections with argiments
*/
ParamList* ParamList::intersect
(
    ParamList* aArgument,
    ParamList* aResult
)
{
    if( aArgument != NULL )
    {
        auto ic = getCount();
        auto jc = aArgument -> getCount();

        for( int i = 0; i < ic; i ++)
        {
            auto pi = getByIndex( i );
            for( int j = 0; j < jc; j ++)
            {
                if( pi -> isEqual( aArgument -> getByIndex( j )))
                {
                    aResult -> push( Param::create() -> copyFrom( pi ));
                }
            }
        }
    }
    return this;
}



/*
    Return true if intersections with argiment exists
*/
bool ParamList::isIntersect
(
    ParamList* a
)
{
    auto arg = ParamList::create();
    intersect( a, arg );
    auto result = arg -> getCount() > 0;
    arg -> destroy();
    return result;
}



/*
    Fill files from path to the param list
*/
ParamList* ParamList::filesFromPath
(
    string a    /* Path */
)
{
    for( auto& p:filesystem::directory_iterator( a ))
    {
        pushString( p.path().filename().string());
    }
    return this;
}



/*
    Deatach the branch from paramlist
    Deatached branch will be remove from this object and returns.
*/
char* ParamList::extractByIndex
(
    int aIndex
)
{
    char* result = NULL;
    auto param = getByIndex( aIndex );
    if( param != NULL )
    {
        /* Parameter founded and is object */
        remove( aIndex );
        result = param -> getValue();
        /* Freed the param */
        ((ParamList*)result) -> setParent( NULL );
        param -> destroy( false );
    }
    return result;
}



bool ParamList::exists
(
    Path aPath,
    bool aUsesPointer
)
{
    return getByName( aPath, aUsesPointer ) != NULL;
}



/*
    Compare the list of keys for this ParamList and argument ParamList
*/
bool ParamList::isEqual
(
    ParamList* aParamList,
    vector <string> aKeys
)
{
    bool result = true;

    for( auto &name : aKeys )
    {
        auto param1 = getByName( name );
        auto param2 = aParamList -> getByName( name );
        result = result && param1 != NULL && param1 -> isEqual( param2 );
    }

    return result;
}



/*
    Operations
*/

/*
    Calculate summ of subkeys by path
*/
double ParamList::calcSum
(
    Path aPath /* Path */
)
{
    /* calculate sum of rnd of all mutation */
    double result = 0.0;
    loop
    (
        [ &result, &aPath ]
        ( Param* iParam )
        {
            if( iParam -> isObject() )
            {
                /* Processing mutation */
                auto item = iParam -> getObject();
                result += item -> getDouble( aPath );
            }
            return false;
        }
    );
    return result;
}



/*
    Return true if the value exists
*/
bool ParamList::valueExists
(
    string a
)
{
    auto result = false;
    recursionLoop
    (
        [ &result, &a ]
        (
            Param* item
        )
        {
            result = item -> getString() == a;
            return result;
        }
    );
    return result;
}



/*
    Return true if the value exists
*/
bool ParamList::valueExists
(
    Path aPath,
    string aValue
)
{
    auto item = getObject( aPath );
    return
    item != NULL
    ? item -> valueExists( aValue )
    : false;
}



