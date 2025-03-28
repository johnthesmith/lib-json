#include <iostream>
#include <regex>

/* File read */
#include <streambuf>
#include <sstream>
#include <fstream>
#include <cmath>

#include "../core/utils.h"

#include "param.h"
#include "json.h"
#include "../core/utils.h"



Json::Json()
{
    paramList = ParamList::create();
}



Json::~Json()
{
    if( paramList != nullptr )
    {
        paramList -> destroy();
    }
}



Json* Json::create()
{
    return new Json();
}



void Json::destroy()
{
    delete this;
}



shared_ptr<Json> Json::shared()
{
    return make_shared <Json>();
}



/*
    Mearge paramlist to other paramList
*/
Json* Json::copyTo
(
    ParamList* a
)
{
    paramList -> copyTo( a );
    return this;
}



Json* Json::dump()
{
    paramList -> dump();
    return this;
}



/*
    Search all keys with name,
    load file from value,
    replace key value from file
*/
Json* Json::include
(
    /* Key name for including */
    string aKeyName
)
{
    getParamList() -> recursionLoop
    (
        [ &aKeyName ]
        (
            Param* item
        )
        {
            if( item -> isObject())
            {
                auto filename = item -> getObject() -> getString( Path{ aKeyName } );
                if( filename != "" )
                {
                    auto loaded = Json::create() -> fromFile( filename );

                    if( loaded -> isOk() )
                    {
                        item -> getObject() -> copyFrom( loaded -> getParamList());
                    }
                    else
                    {
                        item -> setString( loaded -> getCode() );
                    }

                    loaded -> destroy();
                }
            }
            return false;
        }
    );
    return this;
}



/*
    Search all keys with name,
    load file from value,
    replace key value from file
*/
Json* Json::overload
(
    /* Key name for including */
    string aKeyName
)
{
    getParamList() -> recursionLoop
    (
        [ &aKeyName, this ]
        (
            Param* item
        )
        {
            if( item -> isObject() )
            {
                auto pathes = item -> getObject() -> getObject( Path{ aKeyName });
                if( pathes != NULL )
                {
                    auto path = pathes -> getPath();
                    if( path.size() != 0 )
                    {

                        item -> getObject() -> copyFrom( getParamList() -> getObject( path ));
                    }
                }
            }
            return false;
        }
    );
    return this;
}




/*
    Convert string to Json
*/
Json* Json::fromString
(
    const string a
)
{
    int c = a.length();

    /* Cteate ROOT object */
    auto obj = JsonObject::create();

    obj -> name = "ROOT";
    obj -> pairBegin();
    obj -> nameEnd();
    obj -> pairPart = PP_VALUE;

    for( index = 0; index < c && isOk(); index++ )
    {
        switch( a[ index ] )
        {
            case '[':
                if( !obj -> fStringBegin )
                {
                    switch( obj -> pairPart )
                    {
                        case PP_NAME:
                            error( "Unexpected [ in name", obj );
                        break;
                        case PP_UNKNOWN:
                        case PP_VALUE:
                            if( !obj -> fValueBegin )
                            {
                                obj = JsonObject::create( obj );
                                obj -> fArray = true;
                                obj -> nameEnd();
                            }
                            if( obj -> fValueBegin || obj -> fValueEnd )
                            {
                                error( "Unexpected [ in value", obj );
                            }
                        break;
                    }
                }
                else
                {
                    obj -> addChar( a[ index ] );
                }
            break;

            case ']':
                if( !obj -> fStringBegin )
                {
                    if( !obj -> fArray )
                    {
                        error( "Unexpected ] without name", obj );
                    }
                    else
                    {
                        switch( obj -> pairPart )
                        {
                            case PP_UNKNOWN:
                            case PP_VALUE:
                                if( obj -> prevJsonObject != NULL )
                                {
                                    obj -> pairEnd();
                                    obj = obj -> deleteObject( false );
                                }
                                else
                                {
                                    error( "Unexpected ] and heracly error", obj );
                                }
                            break;
                            default:break;
                        }
                    }
                }
                else
                {
                    obj -> addChar( a[ index ] );
                }
            break;


            case '{':
                if( !obj -> fStringBegin )
                {
                    switch( obj -> pairPart )
                    {
                        case PP_NAME:
                            error( "Unexpected { in name", obj );
                        break;
                        case PP_UNKNOWN:
                        case PP_VALUE:
                            if( !obj -> fValueBegin )
                            {
                                obj = JsonObject::create( obj );
                            }
                            if( obj -> fValueBegin || obj -> fValueEnd )
                            {
                                error( "Unexpected { in value", obj );
                            }
                        break;
                    }
                    obj -> pairBegin();
                }
                else
                {
                    obj -> addChar( a[ index ] );
                }
            break;

            case '}':
                if( !obj -> fStringBegin )
                {
                    if( !obj -> fPairBegin )
                    {
                        error( "Unexpected } without name", obj );
                    }
                    else
                    {
                        switch( obj -> pairPart )
                        {
                            case PP_NAME:
                                if( obj -> fNameBegin )
                                {
                                    error( "Unexpected } in name", obj );
                                }
                                else
                                {
                                    /* Close ROOT case */
                                    if( obj -> prevJsonObject != NULL )
                                    {
                                        if( obj -> name != "")
                                        {
                                            obj -> pairEnd();
                                        }
                                        obj = obj -> deleteObject( false );
                                    }
                                }
                            break;
                            case PP_UNKNOWN:
                            case PP_VALUE:
                                if( obj -> prevJsonObject != NULL )
                                {
                                    if( obj -> name != "")
                                    {
                                        obj -> pairEnd();
                                    }
                                    obj = obj -> deleteObject( false );
                                }
                                else
                                {
                                    error( "Unexpected } and heracly error", obj );
                                }
                            break;
                        }
                    }
                }
                else
                {
                    obj -> addChar( a[ index ] );
                }
            break;

            case '"':
                if( !obj -> fStringBegin )
                {
                    obj -> fStringBegin = true;
                    obj -> fStringEnd = false;

                    if( obj -> fArray )
                    {
                        switch( obj -> pairPart )
                        {
                            case PP_UNKNOWN:
                                if( !obj -> fValueBegin && !obj -> fValueEnd )
                                {
                                    obj -> nameEnd();
                                    obj -> valueBegin();
                                }
                                else
                                {
                                    error( "Unexpected \" in value part", obj );
                                }
                            break;
                            default:break;
                        }
                    }
                    else
                    {
                        switch( obj -> pairPart )
                        {
                            case PP_UNKNOWN:
                            case PP_NAME:
                                if( !obj -> fNameBegin && !obj -> fNameEnd )
                                {
                                    obj -> nameBegin();
                                    obj -> pairBegin();
                                }
                                else
                                {
                                    error( "Unexpected \" in name", obj );
                                }
                            break;
                            case PP_VALUE:
                                if( !obj -> fValueBegin && !obj -> fValueEnd )
                                {
                                    obj -> valueBegin();
                                }
                                else
                                {
                                    error( "Unexpected \" in value part", obj );
                                }
                            break;
                        }
                    }
                }
                else
                {
                    if( !obj -> fEscape )
                    {
                        if( obj -> fValueBegin && obj -> fStringBegin )
                        {
                            obj -> valueEnd();
                        }

                        if( obj -> fNameBegin )
                        {
                            obj -> nameEnd();
                            if( obj -> name == "" )
                            {
                                error( "NameIsEmpty", obj );
                            }
                        }

                        obj -> fStringBegin = false;
                        obj -> fStringEnd = true;
                    }
                    else
                    {
                        obj -> addChar( a[ index ] );
                    }
                }
            break;

            case '\\':
                if( obj -> fStringBegin )
                {
                    obj -> addChar( a[ index ] );
                    obj -> fEscape = true;
                }
                else
                {
                    error( "Unexpected \\", obj );
                }
            break;

            case ':':
                if( !obj -> fStringBegin )
                {
                    if( obj -> fArray )
                    {
                        error( "Unexpected : in value part", obj );
                    }
                    else
                    {
                        if( obj -> pairPart == PP_NAME )
                        {
                            obj -> fStringBegin = false;
                            obj -> fStringEnd = false;
                            obj -> pairPart = PP_VALUE;
                        }
                        else
                        {
                            error( "Unexpected : in value part", obj );
                        }
                    }
                }
                else
                {
                    obj -> addChar( a[ index ] );
                }
            break;

            case ' ':
                if( !obj -> fStringBegin )
                {
                    if( obj -> fValueEnd && obj -> pairPart == PP_VALUE )
                    {
//                        obj -> pairEnd();
                    }
                }
                else
                {
                    obj -> addChar( a[ index ] );
                }
            break;

            case ',':
                if( !obj -> fStringBegin )
                {
                    if( obj -> pairPart == PP_VALUE || obj -> pairPart == PP_UNKNOWN )
                    {
                        obj -> pairEnd();
                    }
                }
                else
                {
                    obj -> addChar( a[ index ] );
                }
            break;

            case '\n':
                if( obj -> fStringBegin )
                {
                    obj -> addChar( a[ index ] );
                }
                else
                {
                    line ++;
                }
            break;

            default:

                if( obj -> fArray && !obj -> fValueEnd )
                {
                    obj -> valueBegin();
                }

                if( obj -> fValueEnd )
                {
                    error( "Unexpected character after value", obj );
                }

                if( !obj -> fNameBegin && !obj -> fNameEnd )
                {
                    error( "Unexpected character before name", obj );
                }

                if( obj -> fNameEnd && obj -> pairPart == PP_NAME )
                {
                    error( "Unexpected character after name", obj );
                }

                if( isOk() )
                {
                    obj -> addChar( a[ index ]);
                }
            break;
        }
    }


    /* Check heracly */
    if( isOk() )
    {
        if( obj -> prevJsonObject != NULL )
        {
            error( "HeraclyError", obj );
        }
        else
        {
            obj -> pairEnd();
            /* obj -> paramList ROOT must contin the KT_OBJECT */
            if
            (
                obj -> paramList -> getByIndex( 0 ) != NULL &&
                obj -> paramList -> getByIndex( 0 ) -> getType() == KT_OBJECT
            )
            {
                /* Destroy JSON paramlist */
                paramList -> destroy();
                /* Set first element Paramlist of root as Json Paramlist */
                paramList = (ParamList*) (obj -> paramList -> extractByIndex( 0 ));

                /* Remove objects from object */
                obj -> paramList -> resize( 0 );
                /* Destroy ROOT paramlist */
                obj -> paramList -> destroy();
            }
            else
            {
                error( "UnknownFormat", NULL );
            }
        }
    }

    /* Clear stack */
    while( obj != NULL )
    {
        obj = obj -> deleteObject( !isOk() );
    }

    return this;
}



/*
    Convert json to string
*/
string Json::toString
(
    bool aCompact
)
{
    auto text = Text::create() -> setCompact( aCompact );
    toStringInternal( text, paramList );
    auto result = text -> getString();
    text -> destroy();
    return result;
}



/*
    Convert json to string
*/
Json* Json::toStringInternal
(
    Text* aResult,
    ParamList* aParamList
)
{
    /* get count of items */
    auto items = aParamList -> getItems();
    int c = aParamList -> getCount();

    /* Array detection */
    bool array = true;
    for( int i = 0; i < c; i++ )
    {
        auto prm = ( Param*) items[ i ];
        array = array && prm -> getName() == "";
    }

    /* Build values */

    aResult -> begin( array ? "[" : "{" );

    for( int i = 0; i < c; i++ )
    {
        aResult -> lineBegin();

        auto prm = ( Param*) items[ i ];

        if( prm -> isObject() )
        {
            if( !array )
            {
                aResult -> add( prm -> getName(), "\"" ) -> add( ":" );
            }
            toStringInternal( aResult, prm -> getObject() );
        }
        else
        {
            if( !array )
            {
                aResult -> add( prm -> getName(), "\"" ) -> add( ":" );
            }
            aResult -> add
            (
                prm -> getString(),
                prm -> getType() == KT_STRING ||
                (
                    prm -> getType() == KT_DOUBLE &&
                    isnan( prm -> getDouble())
                ) ? "\"" : ""
            );
        }
        aResult -> add( i == c - 1 ? "" : "," );
    }

    aResult -> end( array ? "]" : "}" );

    return this;
}



Json* Json::fromFile
(
    string aFileName
)
{
    if( isOk() )
    {
        auto t = ifstream( aFileName );
        if( t.is_open() )
        {
            /* Read file */
            stringstream b;
            b << t.rdbuf();
            /* Parsing file */
            fromString( b.str() );
        }
        else
        {
            setResult( "FileReadError" );
        }
    }

    return this;
}



Json* Json::toFile
(
    string aFileName
)
{
    if( isOk() )
    {
        auto t = ofstream( aFileName );
        if( t.is_open() )
        {
            auto text = Text::create();
            toStringInternal( text, paramList );

            /* Write file */
            t << text -> rdbuf();

            text -> destroy();
        }
        else
        {
            setResult( "FileWriteError" );
        }
    }
    return this;
}




/******************************************************************************
    Utils
*/

Json* Json::trace
(
    char c,
    JsonObject* obj
)
{
    cout << "-begin--------------------------------------\n";
    cout << "i          " << index << "\n";
    cout << "char       '" << c << "'\n";
    cout << "line       " << line << "\n";

    cout << "fArray     " << obj -> fArray << "\n";
    cout << "pairPart   " << obj -> pairPart << "\n";
    cout << "pairBegin  " << obj -> fPairBegin << "\n";
    cout << "fString    " << obj -> fStringBegin << " " << obj -> fStringEnd << "\n";
    cout << "fEscape    " << obj -> fEscape << "\n";
    cout << "fName      " << obj -> fNameBegin << " " << obj -> fNameEnd << "\n";
    cout << "fValue         " << obj -> fValueBegin << " " << obj -> fValueEnd << "\n";
    cout << "valueParamList " << obj -> valueParamList << "\n";

    cout << "valueType  " << obj -> valueType << "\n";
    cout << "Name       \"" << obj -> name << "\"\n";
    cout << "value      \"" << obj -> value << "\"\n";
    cout << "result     " << getCode() << "\n";

    cout << "-end--------------------------------------\n";

    return this;
}





ParamType Json::getType
(
    string a
)
{
    ParamType result = KT_STRING;

    if( a == "true" || a == "false" || a == "TRUE" || a == "FALSE" )
    {
        result = KT_BOOL;
    }
    else
    {
        if( a == "null" || a == "NULL" )
        {
            result = KT_NULL;
        }
        else
        {
            if( regex_match( a, regex( "[-+]?([0-9]+)" ) ))
            {
                result = KT_INT;
            }
            else
            {
                if( regex_match( a, regex( "[-+]?([0-9]*[\\.\\,][0-9]+)" ) ))
                {
                    result = KT_DOUBLE;
                }
            }
        }
    }
    return result;
}




string Json::getString
(
    string aName,
    string aDefault
)
{
    return paramList -> getString( Path{ aName }, aDefault );
}




string Json::getString
(
    Path aName,
    string aDefault
)
{
    return paramList -> getString( aName, aDefault );
}




long long int Json::getInt
(
    string aName,
    long long int aDefault
)
{
    return paramList -> getInt( Path{ aName }, aDefault );
}




long long int Json::getInt
(
    vector <string> aName,
    long long int aDefault
)
{
    return paramList -> getInt( aName, aDefault );
}



double Json::getDouble
(
    string aName,
    double aDefault
)
{
    return paramList -> getDouble( Path{ aName }, aDefault );
}




double Json::getDouble
(
    vector <string> aName,
    double aDefault
)
{
    return paramList -> getDouble( aName, aDefault );
}




ParamList* Json::getObject
(
    string aName,
    ParamList* aDefault
)
{
    return paramList -> getObject( Path{ aName }, aDefault );
}




ParamList* Json::getObject
(
    vector <string> aName,
    ParamList* aDefault
)
{
    return paramList -> getObject( aName, aDefault );
}



Json* Json::error
(
    /* message */
    string aMessage,
    JsonObject* aObj
)
{
    setResult
    (
        "jsonParsError",
        aMessage + " Line:" + to_string( line ) + "name:" + aObj -> name
    );
    return this;
}



/******************************************************************************
    JsonObject
*/


JsonObject* JsonObject::create
(
    JsonObject* aParent
)
{
    auto result = new JsonObject();
    result -> prevJsonObject = aParent;
    /* Create ROOT param list */
    result -> paramList = ParamList::create();
    return result;
}



JsonObject* JsonObject::deleteObject
(
    bool aDestroyParamList
)
{
    if( aDestroyParamList )
    {
        /* The object paramList must be completely destroy */
        paramList -> destroy();
    }
    else
    {
        /* ParamList using like value in parent Object */
        if( prevJsonObject != NULL )
        {
            prevJsonObject -> valueParamList = paramList;
        }
    }
    auto result = prevJsonObject;
    delete this;

    return result;
}



JsonObject* JsonObject::nameBegin()
{
    fNameBegin = true;
    fNameEnd = false;
    return this;
}



JsonObject* JsonObject::nameEnd()
{
    fNameBegin = false;
    fNameEnd = true;
    return this;
}



JsonObject* JsonObject::valueBegin()
{
    fValueBegin = true;
    fValueEnd = false;
    return this;
}



JsonObject* JsonObject::valueEnd()
{
    fValueBegin = false;
    fValueEnd = true;
    return this;
}




JsonObject* JsonObject::pairBegin()
{
    fPairBegin = true;
    pairPart = PP_NAME;
    return this;
}



JsonObject* JsonObject::pairEnd()
{
    if( fStringEnd )
    {
        fArray
        ? paramList -> pushString( value )
        : paramList -> setString( name, value );
    }
    else
    {
        if( valueParamList != NULL)
        {
            fArray
            ? paramList -> pushObject( valueParamList)
            : paramList -> setObject( name, valueParamList);
        }
        else
        {
            switch( getType( value ) )
            {
                case KT_UNKNOWN:
                case KT_OBJECT:
                case KT_ARRAY:
                case KT_DATA:
                case KT_NULL:
                    fArray
                    ? paramList -> pushInt( 0 )
                    : paramList -> setInt( name, 0 );
                break;
                case KT_STRING:
                    fArray
                    ? paramList -> pushString( value )
                    : paramList -> setString( name, value );
                break;
                case KT_BOOL:
                    fArray
                    ? paramList -> pushBool( toBool( value ))
                    : paramList -> setBool( name, toBool( value ));
                break;
                case KT_INT:
                    fArray
                    ? paramList -> pushInt( toInt( value ))
                    : paramList -> setInt( name, toInt( value ));
                break;
                case KT_DOUBLE:
                    fArray
                    ? paramList -> pushDouble( toDouble( value ))
                    : paramList -> setDouble( name, toDouble( value ));
                break;
            }
        }
    }

    fStringBegin    = false;
    fStringEnd      = false;
    fValueBegin     = false;
    fValueEnd       = false;
    fNameBegin      = false;
    fNameEnd        = fArray;
    name            = "";
    value           = "";
    valueParamList  = NULL;
    pairPart        = fArray ? PP_VALUE : PP_UNKNOWN;

    return this;
}



JsonObject* JsonObject::addChar
(
    char a
)
{
    if( fNameBegin && !fNameEnd )
    {
        name += a;
    }

    if( pairPart == PP_VALUE && !fValueBegin && !fValueEnd )
    {
        valueBegin();
    }

    if( fValueBegin && !fValueEnd )
    {
        value += a;
    }

    if( fEscape )
    {
        fEscape = false;
    }

    return this;
}



ParamList* Json::getParamList()
{
    return paramList;
}
