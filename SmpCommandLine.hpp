//
// SmpCommandLine.hpp
// Dennis @ 2021-05

/* 
### Descriptions: SmpCommandLine

  # SmpCommandLine is a light-weight header-file-only c++ package for user command line parsing 
    and argument extraction.

    (For a qucik reference of usage, you may check the example codes in SmpCommandLine_Demo.cpp)
   
  # In command line, arguments provided by user are separated by space. In general, there are two 
    kinds of arguments in command line: Flagged Arguments and Unflagged Arguments. A Flagged 
    Argument could start with a hyphen sign and a conjuncted character (eg: -n), which is referred
    to as short flag, or, start with double hyphen signs with a string (e.g.: --image_name), 
    which is referred to as long flag. The flag is usually followed by an argument giving the value
    of the specified field (e.g.: --image_name my_photo.jpg).

  # How to use this SmpCommadnLine.hpp:

    1) Include SmpCommandLine.hpp in your project's main c++ source file (the one with the main 
       fuctcion):
       #include "SmpCommandLine.hpp"

    2) Make sure you have added "( int argc, char *argv[] )" in your main fucntion's input 
       parameter list, that is: 
       int main( int argc, char *argv[] ) 
    
    3) Create a SmpCommandLine object at the top of the main(...) function, and pass the main()'s
       'argc' and 'argv' to SmpCommandLine's constuction method, like: 
        int main( int argc, char *argv[] ) 
        {
            SmpCommandLine command(argc, argv); 
            //...
        
    4) Call SmpCommandLine's member function
           getInteger( shorFlag, longFlag, defaultValue, helpMsg )
       to extract an integer number led by a short flagged specified by 'shorFlag', or a long 
       flag specified by 'longFlag'. If none of the specified flags presents in the command line,
       the returned value of this method is set to the given 'defaultValue'.

    5) Similiarly, call below member function of SmpCommandLine to extract different types of 
       value from command line:
           getFloat( shortFlag, longFlag...), to extract a float number with specified flag
           getDouble( shortFlag, longFlag...), to extract a double number with specified flag
           getString( shortFlag, longFlag...), to extract a string with specified flag. 
           getBoolean( shortFlag, longFlag...), to extract a boolean with specified flag. 

    6) Take notes to the 'getBoolean()' method. There are two types of command line formats for 
       boolean value, one is simply using a flag (e.g. -b), the present of that flag in command
       line sets the associated boolean value to true; the other format is to use a flag followed by  
       a text boolean name, explicitly setting the boolean to yes/no (or true/false), (e.g.: -b yes, 
       -b false). By default, we use the formal format, namely CMD_FLAG_ONLY, unless it is clearly
       specified by giving a 'CMD_WITH_VALUE' (of type SmpCommandType) in the call to getBoolean().

    7) Multipy boolean flags can be combined in one shortFlag starting with a single hythen in 
       command line (e.g.: '-xzvf', which is equalvalue to '-x -z -v -f'). Note in this case, the 
       boolean value should be in CMD_FLAG_ONLY format, except for the last flag which can be 
       followed by a value argument when needed.

    8) After extracting all the flagged arguments from command line, you may call 
          getInteger( index, defaultValue, helpMsg )
          getFloat( index, defaultValue, helpMsg )
          getString( index, defaultValue, helpMsg )
          ...
       to extract unflagged arguments from fixed sequence position (specified by the fisrt 
       parameter 'index') in command line. 
       
       Sequence position of unflagged argument is the position of argument counted from '1' 
       excluding all flagged arguments (flags and associated values). e.g. for command line: 
       "$./programname --wait_time 100 input_file.jpg output_file.jpg" getArgument( 1, ... ) 
       returns 'input_file.jpg', getArgument( 2, ... ) returns 'output_file.jpg' 
      
    9) Finally, if necessary, call below piece of codes to show help message to user, when it is 
       wanted:
            if( userCommands.helpMessageWanted() )
                userCommands.showHelpMessage();
       to display help hint when 
       user type -h or --help in the command line. The help message is automatically composed from 
       the helpMsg strings provided in the above calls to getArgument(..., helpMsg ).

  # IMPORTANT NOTES: 
       Please make sure you call getXxxxx( shortFlag, longFlag, ... ) to extract all flagged 
    arguments before calling getXxxx( index, ... ) to extract unflagged arguments, or the you may 
    extract unexpected argument when calling getXxxx( index, ... ). 

    A bit more notes:
    * Flags should start with a alphabet ranging from 'a'~'z' or 'A'~'Z', they can not start with a
      number.
    * string argument cannot contain any printable characters.

  # Examples Codes could be found in SmpCommandLine_Demo.cpp
###
*/

#ifndef __SMP_COMMAND_LINE_HPP__
#define __SMP_COMMAND_LINE_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // for to_lower
#include <cctype>

#ifndef DEBUG_MESSAGE 
#define _DEBUG_MESSAGE( x, ... )    printf( x, ##__VA_ARGS__ )
#define _ERROR_MESSAGE( x, ... )    { printf( "ERROR!: "); printf( x, ##__VA_ARGS__ ); }
#else   
#define _DEBUG_MESSAGE DEBUG_MESSAGE
#define _ERROR_MESSAGE ERROR_MESSAGE
#endif

typedef enum{
    CMD_WITH_VALUE = 0,
    CMD_FLAG_ONLY = 1,
} SmpCommandType;

class SmpCommandLine 
{
  private:
    const std::string _VERSION_NUMBER_ = "SmpCommandLine V0.9, Dennis @ 2021-05";    
 
  public:

    SmpCommandLine( int argc, char *const argv[] )
    {
        for( int i = 0; i < argc; i++ )
        {
            arguments.push_back( argv[i] );
        }
        
        // Expand the combined single flags (e.g.: '-xzvf' expland to -x -z -v -f)
        for( int i = 0; i < arguments.size(); i++ ) 
        {
            if( arguments[i][0] == hyphenchar && arguments[i][1] != hyphenchar &&
               (arguments[i][1] >= 'a' && arguments[i][1] <= 'z' || 
                arguments[i][1] >= 'A' && arguments[i][1] <= 'Z' ) &&
                arguments[i].size() > 2 )
            {
                for( int j = 1; j < arguments[i].size(); j++ )
                {
                    std::string newflag("-x");
                    newflag[1] = arguments[i][j];
                    arguments.insert( arguments.begin()+i, newflag );
                    i++;
                }
                arguments.erase( arguments.begin()+i );
            }
        }

        helpMessageQueue.push_back( "Help Message: Usage of " + arguments[0]);
        helpMessageQueue.push_back( arguments[0] + " [argument1] ... [--flag1 arg] ..." );

        mbWarningHasShown = false;
        mbHelpMsgHasShown = false;
        mbUnflaggedApiCalled = false;
        maxUnflaggedArgs = 0;

        // Bonus extension: show version info of SmpCommandLine(this module, not client software)
        for( int i = 0; i < arguments.size(); i++ ) 
        {
            if( arguments[i] == "--SmpCommandLineVersion" ) {
                showVersionInfo();
                arguments.erase( arguments.begin()+i );
                break;
            }
        }
    };

    // Extract FLAGGED argument of int type
    int getInteger( const char* shortFlag, const char* longFlag, int defaultValue = 0, const char* helpMsg = "" )
    {
        addHelpMessage( shortFlag, longFlag, std::to_string(defaultValue), helpMsg );

        std::string valueString = getFlaggedArgument( shortFlag, longFlag );
    
        if( !valueString.empty() ) 
        {
            // TODO: check more format validity.
            if( valueString[0]=='-' || valueString[0]=='+' || (valueString[0] >= '0' && valueString[0] <= '9') )
            {
                return( atoi( valueString.c_str() ) );
            } else {
                _ERROR_MESSAGE( "Invalid number following flag %s in command line.\n", 
                                (shortFlag!=NULL? shortFlag:longFlag) );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    _DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract UNFLAGGED argument of int type
    int getInteger( int index, int defaultValue = 0, const char* helpMsg = "" )
    {   
        addHelpMessage( index, std::to_string(defaultValue), helpMsg );
        
        std::string valueString = getUnflaggedArgument( index );
       
        if( !valueString.empty() ) 
        {
            // TODO: check more format validity.
            if( valueString[0]=='-' || valueString[0]=='+' || (valueString[0] >= '0' && valueString[0] <= '9') )
            {
                return( atoi( valueString.c_str() ) );
            } else 
            {
                _ERROR_MESSAGE( "Invalid number at position %d in command line.\n", index );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    _DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument of flat type
    float getFloat( const char* shortFlag, const char* longFlag, float defaultValue = 0.0, const char* helpMsg = "" )
    {
        addHelpMessage( shortFlag, longFlag, std::to_string(defaultValue), helpMsg );
        
        std::string valueString = getFlaggedArgument( shortFlag, longFlag );
    
        if( !valueString.empty() ) {
            // TODO: check format validity.
            if( valueString[0]=='.' || valueString[0]=='-' || valueString[0]=='+' ||
               (valueString[0] >= '0' && valueString[0] <= '9') ) 
            {
                return( float(atof( valueString.c_str() )) );
            } 
            else 
            {
                _ERROR_MESSAGE( "Invalid number following flag %s in command line.\n", 
                               (shortFlag!=NULL? shortFlag:longFlag) );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    _DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
            
        } else {
            return( defaultValue );
        }
    };

    // Extract UNFLAGGED argument of flat type
    float getFloat( int index, float defaultValue = 0.0, const char* helpMsg = "" )
    {
        addHelpMessage( index, std::to_string(defaultValue), helpMsg );
        
        std::string valueString = getUnflaggedArgument( index );
       
        if( !valueString.empty() ) {
            // TODO: check format validity.
            if( valueString[0]=='.' || valueString[0]=='-' || valueString[0]=='+' ||
               (valueString[0] >= '0' && valueString[0] <= '9') ) 
            {
                return( float(atof( valueString.c_str() )) );
            } 
            else 
            {
                _ERROR_MESSAGE( "Invalid number at position %d in command line.\n", index );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    _DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
            
        } else {
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument of double type
    double getDouble( const char* shortFlag, const char* longFlag, double defaultValue = 0.0, const char* helpMsg = "" )
    {
        addHelpMessage( shortFlag, longFlag, std::to_string(defaultValue), helpMsg );
        
        std::string valueString = getFlaggedArgument( shortFlag, longFlag );
    
        if( !valueString.empty() ) {
            // TODO: check format validity.
            if( valueString[0]=='.' || valueString[0]=='-' || valueString[0]=='+' || 
               (valueString[0] >= '0' && valueString[0] <= '9') ) 
            {
                return( atof( valueString.c_str() ) );
            } 
            else 
            {
                _ERROR_MESSAGE( "Invalid number following flag %s in command line.\n", 
                               (shortFlag!=NULL? shortFlag:longFlag) );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    _DEBUG_MESSAGE("Use default value");
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract UNFLAGGED argument of double type
    double getDouble( int index, double defaultValue = 0.0, const char* helpMsg = "" )
    {
        addHelpMessage( index, std::to_string(defaultValue), helpMsg );
        
        std::string valueString = getUnflaggedArgument( index );
    
        if( !valueString.empty() ) {
            // TODO: check format validity.
            if( valueString[0]=='.' || valueString[0]=='-' || valueString[0]=='+' || 
               (valueString[0] >= '0' && valueString[0] <= '9') ) 
            {
                return( atof( valueString.c_str() ) );
            } 
            else 
            {
                _ERROR_MESSAGE( "Invalid number at position %d in command line.\n", index );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    _DEBUG_MESSAGE("Use default value");
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument of bealean type
    bool getBoolean( const char* shortFlag, const char* longFlag, SmpCommandType cmdType = CMD_FLAG_ONLY , bool defaultValue = false, const char* helpMsg = "" )
    {
        if( cmdType==CMD_FLAG_ONLY && defaultValue )
        {
            _ERROR_MESSAGE( "in calling %s(), when cmdType is set to CMD_FLAG_ONLY, defaultValue must be set to false.", __FUNCTION__ );
               
            if( bQuitOnError )
                exit(-1);
            else 
                defaultValue = false;
        }

        addHelpMessage( shortFlag, longFlag, bool2String(defaultValue), helpMsg, (cmdType==CMD_FLAG_ONLY) );

        std::string valueString = getFlaggedArgument( shortFlag, longFlag, (cmdType==CMD_FLAG_ONLY) );
    
        if( !valueString.empty() ) 
        {
            if( cmdType==CMD_FLAG_ONLY )
                return( true );
            
            int  errorCode;
            bool retValue = string2Bool( valueString, defaultValue, &errorCode );

            if( errorCode == 0 ) {
                return( retValue );
            } else {
                _ERROR_MESSAGE( "Invalid argument for boolean type in command line (flag=%s),"
                               " return default value.\n", (shortFlag!=NULL? shortFlag : longFlag) );
                return( defaultValue );
            } 
        }
        else 
        {
            return( defaultValue );
        }
    };


    // Extract UNFLAGGED argument of boolean type
    bool getBoolean( int index, bool defaultValue=false, const char* helpMsg = "" )
    {   
        addHelpMessage( index, bool2String(defaultValue), helpMsg );

        std::string argString = getUnflaggedArgument( index );
       
        if( !argString.empty() ) 
        {
            int errorCode;
            bool retValue = string2Bool( argString, defaultValue, &errorCode );

            if( errorCode == 0 ) {
                return( retValue );
            } else {
                _ERROR_MESSAGE( "Invalid argument for boolean type in command line (index=%d), return default value.\n", index );
                return( defaultValue );
            } 
        }
        else 
        {
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument of string type
    std::string getString( const char* shortFlag, const char* longFlag, std::string defaultValue = "", const char* helpMsg = "" )
    {
        addHelpMessage( shortFlag, longFlag, defaultValue, helpMsg );
        
        std::string valueString = getFlaggedArgument( shortFlag, longFlag );
    
        if( !valueString.empty() && valueString != std::string(" ") ) 
            return( valueString );
        else 
            return( defaultValue );
    };

    // Extract UNFLAGGED argument of string type
    std::string getString( int index, std::string defaultValue = "", const char* helpMsg = "" )
    {   
        addHelpMessage( index, defaultValue, helpMsg );
        
        std::string argString = getUnflaggedArgument( index );
       
        if( !argString.empty() ) 
            return( argString );
        else 
            return( defaultValue );
    };

    //// Alternative Encapsulation:
    int getArgument( const char* shortFlag, const char* longFlag, int defaultValue, const char* helpMsg = "" )
    {
        return( getInteger( shortFlag, longFlag, defaultValue, helpMsg ) );
    };

    int getArgument( int index, int defaultValue, const char* helpMsg = "" )
    {
        return( getInteger( index, defaultValue, helpMsg ) );
    };

    double getArgument( const char* shortFlag, const char* longFlag, double defaultValue, const char* helpMsg = "" )
    {
        return( getDouble( shortFlag, longFlag, defaultValue, helpMsg ) );
    };

    double getArgument( int index, double defaultValue, const char* helpMsg = "" )
    {
        return( getDouble( index, defaultValue, helpMsg ) );
    };

    bool getArgument( const char* shortFlag, const char* longFlag, SmpCommandType cmdType = CMD_FLAG_ONLY, bool defaultValue = false, const char* helpMsg = "" )
    {
        return( getBoolean( shortFlag, longFlag, cmdType, defaultValue, helpMsg ) );
    };

    std::string getArgument( const char* shortFlag, const char* longFlag, std::string defaultValue = "", const char* helpMsg = "" )
    {
        return( getString( shortFlag, longFlag, defaultValue, helpMsg ) );
    };

    std::string getArgument( int index, std::string defaultValue = "", const char* helpMsg = "" )
    {   
        return( getString( index, defaultValue, helpMsg ) );
    };
    ////

    void showHelpMessage()
    {
        // Construct the first line of help message:
        std::string firstLineMsg = arguments[0]+" ";
        
        for( int i = 0; i < maxUnflaggedArgs && i < 3; i++ )
        {
            firstLineMsg += "[argument"+std::to_string(i+1)+"] ";
        }

        if( maxUnflaggedArgs > 3 ) {
            firstLineMsg += "... ";
        }
        firstLineMsg += firstLineFlagMsg;
    
        // Update:
        helpMessageQueue[1] = firstLineMsg;

        // Show them:
        for( int i = 0; i < helpMessageQueue.size(); i++ )
        {
            if( i >= 2 ){
                std::cout << "\t";
            }
            std::cout << helpMessageQueue[i] << std::endl;
        }

        mbHelpMsgHasShown = true;
    };

    bool helpMessageWanted()
    {
        // Handling help message trigers:
        return( getBoolean( "h", "help", CMD_FLAG_ONLY, false, "Show this help message" ) );
    }

    void showHelpMsgOnRequest()
    {
        // Handling help message trigers:
        bool bHelpMsgRequested = getBoolean( "h", "help", CMD_FLAG_ONLY, false, "Show this help message" );

        if( bHelpMsgRequested && ! mbHelpMsgHasShown )
            showHelpMessage();
    }

    //  Check the validity of user inputed command line, according to the getArgument functions called
    //  previously in code.
    int checkValidity()
    {
        if( arguments.size() > 1 )
        {
            for( int i = 1; i < arguments.size(); i++ )
            {
                if( arguments[i][0] == hyphenchar ) {
                    _ERROR_MESSAGE( "checkValidity(): Unknown flag %s detected in command line", arguments[i].c_str() );
                    return(-1);
                }
            }
        } 
        _DEBUG_MESSAGE("checkValidity(): WARNING! Feature not completedly implemented.\n");
        return(0);
    };

    void showVersionInfo()
    {
        std::cout << _VERSION_NUMBER_ << std::endl;
    }

  //-----------------------------------------------------------------------------------------------
  // Below are private / protected members:
  protected:
    const char hyphenchar = '-';
    const std::string singleHyphen = std::string("-");
    const std::string doubleHyphen = std::string("--");
    const std::string emptyString  = std::string("");
    const bool        bQuitOnError = false;

    std::vector<std::string> arguments;
    std::vector<std::string> helpMessageQueue;
    std::string firstLineFlagMsg;
    int  maxUnflaggedArgs;
    bool mbWarningHasShown;
    bool mbHelpMsgHasShown;
    bool mbUnflaggedApiCalled;

  protected:

    std::string bool2String( bool torf )
    {
        if( torf )
            return( std::string("true") );
        else
            return( std::string("false") );
    };

    bool string2Bool( std::string boolName, bool defaultValue = false, int *pErrorCode = NULL )
    {
        if( !boolName.empty() ) 
        {
            std::string argInLower;

            std::transform( boolName.begin(), boolName.end(), 
                            argInLower.begin(), [](unsigned char c){ return std::tolower(c); } );

            if( argInLower == std::string("no")   || argInLower == std::string("n") || 
                argInLower == std::string("false")|| argInLower == std::string("f") ) 
            {
                if( pErrorCode != NULL )
                    pErrorCode = 0;
                return( false );
            } 
            else if( argInLower == std::string("yes")  || argInLower == std::string("y") || 
                     argInLower == std::string("true") || argInLower == std::string("t") ) 
            {
                if( pErrorCode != NULL )
                   *pErrorCode = 0;
                return( true );
            } 
            else 
            {    
                if( pErrorCode != NULL )
                   *pErrorCode = -1;
                return( defaultValue );
            }
        } 
        else 
        { 
            if( pErrorCode != NULL )
               *pErrorCode = -1;
            return( defaultValue );
        }
    };

    std::string getFlaggedArgument( const char* shortFlag, const char* longFlag, bool bFlagOnly=false )
    {
        if( mbUnflaggedApiCalled && !bFlagOnly ) {
            _ERROR_MESSAGE( "WARNING!: Please extract all flagged arguments before extracting unflagged argument!\n" );
        }

        if( shortFlag == NULL && longFlag == NULL ) {
            _ERROR_MESSAGE( "Source usage error: shortFlag and longFlag can not both be NULL!");
            
            if( bQuitOnError ) 
                exit(-1);
            else
                return( emptyString );
        }

        std::string shortFlagStr;
        std::string longFlagStr;

        if( shortFlag != NULL )
        {
            if( shortFlag[0] != hyphenchar ) {
                shortFlagStr = singleHyphen + shortFlag;
            } else {
                shortFlagStr = std::string(shortFlag);
            }
        }

        if( longFlag != NULL )
        {
            if( longFlag[0] != hyphenchar ) {
                longFlagStr = doubleHyphen + longFlag;
            } else {
                longFlagStr = std::string(longFlag);
            }

            if( longFlag[0] == hyphenchar && longFlag[1] != hyphenchar ) {
                // ERROR: User has specified a wrong format of longFlag:
                _ERROR_MESSAGE( "In %s(), illegal format of long flag in calling. \
                        (use double hyphen '--flag' or no hyphen 'flag' for long flags", __FUNCTION__ );
                if( bQuitOnError ) {
                    exit(-1);
                } else {
                    return( emptyString );
                }
            }
        }

        for( int i = 0; i < arguments.size(); i++ )
        {
            //std::cout << "i="<<i<<" "<<arguments[i] << " vs " << singleHyphen + shortFlag << std::endl;
            if( ( shortFlag != NULL && arguments[i] == shortFlagStr ) || 
                 ( longFlag != NULL && arguments[i] == longFlagStr ) )
            {
                arguments.erase( arguments.begin()+i );

                if( !bFlagOnly && i < arguments.size() && !arguments[i].empty() ) {
                    std::string valueString = arguments[i];
                    arguments.erase( arguments.begin()+i );
                    return( valueString );
                } else {
                    return(std::string(" "));
                }
                i--; // important!!
            }
        }
        //_DEBUG_MESSAGE( "%s: flag:%s, returns empty string.\n", __FUNCTION__, 
        //               (shortFlag!=NULL? shortFlag : longFlag) );
        return( emptyString );
    };

    std::string getUnflaggedArgument( int index )
    {   
        mbUnflaggedApiCalled = true;
        
        if( index > maxUnflaggedArgs )
            maxUnflaggedArgs = index;

        for( int i = 0, pi = 0; i < arguments.size(); i++ )
        {
            if( arguments[i][0] == '-' )
            {
                if( !mbWarningHasShown && arguments[i] != std::string("-h") && arguments[i] != std::string("--help") ) {
                    _ERROR_MESSAGE ("WARNING! There may be unknown flags in the command line, or in source code you have extracted\n" ); 
                    _ERROR_MESSAGE ("         unflagged arguments before extracting all the flagged ones (i.e. argument with leading\n" );
                    _ERROR_MESSAGE ("         '-' or '--' sign). Please make sure to extract common arguments after extracting \n" );
                    _ERROR_MESSAGE ("         all flagged arguments in source code.\n");
                    _ERROR_MESSAGE ("         Consult the readme description in SmpCommandLine.hpp for details.\n");
                    mbWarningHasShown = true;
                }
            } 
            else 
            {
                if( pi == index ) {
                    return( arguments[i] );
                } else { 
                    pi++;
                }
            }
        }
        return(emptyString);
    };

    void addHelpMessage( const char* shortFlag, const char* longFlag, const std::string defaultStr, 
                         const char* helpMsg, bool bFlagOnly = false )
    {
        // Constuct help message for this item:
        std::string helpStr;

        if( shortFlag != NULL ) {
            std::string shortFlagStr(shortFlag); 

            if( shortFlagStr[0] != hyphenchar ) {
                shortFlagStr = singleHyphen + shortFlag;
            }
            helpStr = helpStr + shortFlagStr;
        }

        if( shortFlag != NULL && longFlag != NULL ) {
            helpStr = helpStr + "/";
        }

        if( longFlag != NULL ) {
            std::string longFlagStr(longFlag);

            if( longFlagStr[0] != hyphenchar ) {
                longFlagStr = doubleHyphen + longFlag;
            }
            helpStr = helpStr + longFlagStr;
        }

        if( !bFlagOnly )
            helpStr = helpStr + " val";

        // Add info to the first line of help message:
        firstLineFlagMsg += "["+helpStr+"] ";
        
        helpStr = helpStr + " : ";
        helpStr = helpStr + helpMsg;
        
        if( !defaultStr.empty() )
            helpStr = helpStr + " (defualt value:" + defaultStr + ")";
        
        helpMessageQueue.push_back(helpStr);
    };

    void addHelpMessage( int index, const std::string defaultStr, const char* helpMsg )
    {
        std::string helpStr;

        helpStr = helpStr + "argument" + std::to_string(index) + ": ";
        helpStr = helpStr + helpMsg;

        if( !defaultStr.empty() )
            helpStr = helpStr + " (defualt value: " + defaultStr + ")";
        
        helpMessageQueue.push_back(helpStr);
    };

    // Forbid calling of default constructor (force user to pass argc and argv into constructor method
    // (defined below):
    SmpCommandLine(){};
}; 

#endif //__SMP_COMMAND_LINE_HPP__
