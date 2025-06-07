//
// SmpCommandLine.hpp
// Dennis @ 2021-2025, V2.1

/* 
### Descriptions: SmpCommandLine

  # SmpCommandLine is a light-weight header-file-only c++ tool for user command line parsing 
    and argument extraction.

  # NOTE: For a qucik reference of usage, you may directly check the example codes in 
    SmpCommandLine_Demo.cpp
   
  # Background & Terminologies: 
    Command Line is a simple way for user to provide input commands to launch and control computer
	programs through character interface like a Terminal. For example, below command line launches
	a program named 'my_program' with a serial of arguments:
	
	   ./my_program input_photo.jpg -o output_photo.jpg --filter gaussion -w 1024 -h 768 -s 
	   
	In command line, arguments provided by user are separated by space. In general, there are two 
	kinds of arguments, namely, Unflagged Arguments and Flagged Arguments. An Unflagged Argument
	is just a textual parameter (a textual string or a number) directly given in the command line. 
	e.g., "input_photo.jpg" in the above example. 

	A Flagged Argument specifies the argument name by a flag and then the parameter. There are two
	commonly used versions of Flagged Arguments: concised(short) version and full(long) version. 
	A concised version of Flagged Argument starts with a hyphen sign followed by one conjuncted
	character, and then a parameter. e.g., "-o output_photo.jpg" in the above example. A full 
	version of Flagged Argument starts with a double-hyphen sign followed by a flag name (a word), 
	and then the parameter, e.g., "--filter guassion" in above example.
	
	If a Flagged Argument is used to provide a boolean argument, the parameter can be obmitted. 
	e.g., "-s" in above example.

  # Usage of SmpCommadnLine.hpp:

    1) Include SmpCommandLine.hpp in your project's main c++ source file (the one with the main 
       fuctcion), and make sure you have added the input argument phrase in the main function's
	   argument list, like:

        #include "SmpCommandLine.hpp"
        //...
		int main( int argc, char *argv[] )
		//...

    2) Create a SmpCommandLine object at the top of the main(...) function, with the 'argc' and
	   'argv' as constuctor's parameter, that is:
       
        int main( int argc, char *argv[] ) 
        {
            SmpCommandLine command(argc, argv); 
            //...
        
    3) Call SmpCommandLine's member function to extract different type of command line arguments,
       for examples:

           int imageWidth = getInteger(  "w", "--width", 1024,   "specifies the image width" );

       The prototype of this method is: 
           
            int SmpCommandLine::getInteger( const char* shortflag, const char* longflag, 
                                            int defaultValue, std::string helpMessage );
       
       This method extracts an integer number led by a short flagged (i.e. concised version of flag,
	   in this case, '-w'), or a long flag (i.e. full version of flag, int this case, --width). If
	   none of the specified flags presents in the user's command line, the returned value is set to
	   the given 'defaultValue'.

    4) Similiarly, you may call below member function of SmpCommandLine to extract other types of 
       value from command line:

           getFloat( shortFlag, longFlag...), to extract a float number with specified flag
           getDouble( shortFlag, longFlag...), to extract a double number with specified flag
           getString( shortFlag, longFlag...), to extract a string with specified flag. 
           getBoolean( shortFlag, longFlag...), to extract a boolean with specified flag. 

    5) A special note to the getBoolean( shortFlag, longFlag,...) method: In this version, boolean 
       arguments are "flag-only" argument, which means that, in user command line, boolean arguments
       consist of a flag only, but with no parameter follows behind. for example, below are boolean 
       arguments in user command line:

            ./some_program -x -z -v

       when calls 
            bool x = getBoolean( "x", NULL, "" );
    
       The boolean value 'x' will be set to true(yes). Please also note that, the default value of 
	   boolean argument is always false.


    6) After extracting all the flagged arguments from command line, you may call 

            getInteger( index, defaultValue, helpMsg )
            getFloat( index, defaultValue, helpMsg )
            getString( index, defaultValue, helpMsg )
            ...
       to extract unflagged arguments from fixed sequence position (specified by the fisrt 
       parameter 'index') in command line. 
       
       Sequence position of unflagged argument is the position of argument counted from '1' 
       excluding all flagged arguments (flags and associated values). e.g. for command line: 

            ./my_program --wait_time 100 input_file.jpg output_file.jpg 
       
       getString( 1, ... ) returns 'input_file.jpg', and
       getString( 2, ... ) returns 'output_file.jpg' 
      
    7) Finally, if necessary, call below piece of codes to show help message to user, when it is 
       wanted:

            if( userCommands.helpMessageWanted() )
                userCommands.showHelpMessage();
       
       to display help hint when user type -h or --help in the command line. The help message is 
       automatically composed from the helpMsg strings provided in the above calls to 
       getArgument(..., helpMsg ).

  # IMPORTANT NOTES: 

    1) Please make sure you call getXxxxx( shortFlag, longFlag, ... ) to extract ALL Flagged 
       Arguments before calling getXxxx( index, ... ) to extract any Unflagged Arguments, or the
	   you may extract unexpected argument when calling getXxxx( index, ... ). 

    2) (For user who type command lines), in this version, when typing command line, please always
       add a space to sperate a flag and its following value. e.g., this is good: 
            -w 1920
       And, this is not good: 
            -w1920

    A bit more notes:
    * Flags should start with a alphabet ranging from 'a'~'z' or 'A'~'Z', please don't use number to 
	  as start a flag.
    * string argument cannot contain any non-printable characters.

  # Examples of Usage
   	Example codes could be found in SmpCommandLine_Demo.cpp
###
*/

#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // for to_lower
#include <cctype>

// for debug:
#ifndef DEBUG_MESSAGE 
#define _DEBUG_MESSAGE( x, ... )    printf( x, ##__VA_ARGS__ )
#define _ERROR_MESSAGE( x, ... )    { printf( "ERROR!: "); printf( x, ##__VA_ARGS__ ); }
#else   
#define _DEBUG_MESSAGE DEBUG_MESSAGE
#define _ERROR_MESSAGE ERROR_MESSAGE
#endif


class SmpCommandLine 
{
  private:
    const std::string _VERSION_NUMBER_ = "SmpCommandLine V2.1.0, Dennis @ 2021-2025";
  
  protected:
    const char kHyphenchar = '-';
    const std::string kSingleHyphen = std::string("-");
    const std::string kDoubleHyphen = std::string("--");
    const std::string kEmptyString  = std::string("");
    const std::string kSpaceString  = std::string(" ");
    const bool        bQuitOnError  = false;

    std::vector<std::string> arguments;
    std::vector<std::string> helpMessageQueue;
    std::string firstLineFlagMsg;

    int  mArgCount;
    int  maxUnflaggedArgs;
    bool mbWarningHasShown;
    bool mbHelpMsgHasShown;
    bool mbUnflaggedApiCalled;
 
  // Below are Public Methods:
  public:

    SmpCommandLine( int argc, char *const argv[] )
    {
        mArgCount = argc;

        for( int i = 0; i < mArgCount; i++ )
        {
            arguments.push_back( argv[i] );
        }
        
        // Split the combined single flags (e.g.: '-xzvf' expland to -x -z -v -f)
        for( int i = 0; i < arguments.size(); i++ ) 
        {
            if( arguments[i][0] == kHyphenchar && arguments[i][1] != kHyphenchar &&
               (arguments[i][1] >= 'a' && arguments[i][1] <= 'z' || 
                arguments[i][1] >= 'A' && arguments[i][1] <= 'Z' ) &&
                arguments[i].size() > 2 )
            {
                for( int j = 1; j < arguments[i].size(); j++ )
                {
                    if( (arguments[i][j] >= 'a' && arguments[i][j] <= 'z' || 
                         arguments[i][j] >= 'A' && arguments[i][j] <= 'Z' ) ) 
                    {
                        std::string newflag = std::string("-")+arguments[i][j];
                        arguments.insert( arguments.begin()+i, newflag );
                        i++;
                    } else {

                        std::string newflag = arguments[i].substr(j);
                        arguments.insert( arguments.begin()+i, newflag );
                        i++;
                        break;
                    }
                }
                arguments.erase( arguments.begin()+i );
                i--;
            }
        }

        helpMessageQueue.push_back( std::string("Usage of ") + arguments[0] + ":");
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

        ////
        // Debug: show the final (splited) argument list:
        //_DEBUG_MESSAGE( "List of argumanets:\n" );
        //for( int i = 0; i < arguments.size(); i++ ) 
        //{
        //    _DEBUG_MESSAGE( "\t%s\n", arguments[i].c_str() );
        //}
    };

    // Extract flagged argument of int type
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
                    _DEBUG_MESSAGE( "Use default value.\n" );
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract unflagged argument of int type
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
                    _DEBUG_MESSAGE( "Use default value.\n" );
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract flagged argument of flat type
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
                    _DEBUG_MESSAGE( "Use default value.\n" );
                    return( defaultValue );
                }    
            }
            
        } else {
            return( defaultValue );
        }
    };

    // Extract unflagged argument of flat type
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
                    _DEBUG_MESSAGE( "Use default value.\n" );
                    return( defaultValue );
                }    
            }
            
        } else {
            return( defaultValue );
        }
    };

    // Extract flagged argument of double type
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
                    _DEBUG_MESSAGE("Use default value.\n");
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract unflagged argument of double type
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
                    _DEBUG_MESSAGE("Use default value.\n");
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract flagged argument of bealean type
    bool getBoolean( const char* shortFlag, const char* longFlag, const char* helpMsg = "" )
    {
        addHelpMessage( shortFlag, longFlag, bool2String(false), helpMsg, true ); // bIsBoolean == true

        std::string valueString = getFlaggedArgument( shortFlag, longFlag, true ); // bIsBoolean == true
    
        if( !valueString.empty() ) {
            return( true );
        } else {  
            return( false );
        }
    };


    // Extract unflagged argument of boolean type
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

    // Extract flagged argument of string type
    std::string getString( const char* shortFlag, const char* longFlag, std::string defaultValue = "", const char* helpMsg = "" )
    {
        addHelpMessage( shortFlag, longFlag, defaultValue, helpMsg );
        
        std::string valueString = getFlaggedArgument( shortFlag, longFlag );
    
        if( !valueString.empty() && valueString != std::string(" ") ) 
            return( valueString );
        else 
            return( defaultValue );
    };

    // Extract unflagged argument of string type
    std::string getString( int index, std::string defaultValue = "", const char* helpMsg = "" )
    {   
        addHelpMessage( index, defaultValue, helpMsg );
        
        std::string argString = getUnflaggedArgument( index );
       
        if( !argString.empty() ) 
            return( argString );
        else 
            return( defaultValue );
    };

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

    bool helpMessageWanted( int minArgc = -1 )
    {
        // Handling help message trigers:
        bool bHelpMsgWanted = getBoolean( "h", "help", "Show this help message" );

        return( bHelpMsgWanted || (minArgc > 0 && mArgCount < minArgc) );
    }

    void showHelpMsgOnRequest()
    {
        // Handling help message trigers:
        bool bHelpMsgRequested = getBoolean( "h", "help", "Show this help message" );

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
                if( arguments[i][0] == kHyphenchar ) {
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
  // Below are private / protected Methods:
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
            std::string argInLower = boolName;

            std::transform( argInLower.begin(), argInLower.end(), argInLower.begin(), ::tolower );

            if( argInLower == std::string("no")   || argInLower == std::string("n") || 
                argInLower == std::string("false")|| argInLower == std::string("f") || 
                argInLower == std::string("off")  || argInLower == std::string("0") ) 
            {
                if( pErrorCode != NULL )
                    pErrorCode = 0;
                return( false );
            } 
            else if( argInLower == std::string("yes")  || argInLower == std::string("y") || 
                     argInLower == std::string("true") || argInLower == std::string("t") || 
                     argInLower == std::string("on")   || argInLower == std::string("1") ) 
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

    std::string getFlaggedArgument( const char* shortFlag, const char* longFlag, bool bIsBoolean=false )
    {
        if( shortFlag == NULL && longFlag == NULL ) 
        {
            _ERROR_MESSAGE( "Source usage error: shortFlag and longFlag can not both be NULL!");
            
            if( bQuitOnError ) 
                exit(-1);
            else
                return( kEmptyString );
        }

        std::string shortFlagStr;
        std::string longFlagStr;

        if( shortFlag != NULL )
        {
            if( shortFlag[0] != kHyphenchar ) {
                shortFlagStr = kSingleHyphen + shortFlag;
            } else {
                shortFlagStr = std::string(shortFlag);
            }
        }

        if( longFlag != NULL )
        {
            if( longFlag[0] != kHyphenchar ) {
                longFlagStr = kDoubleHyphen + longFlag;
            } else {
                longFlagStr = std::string(longFlag);
            }

            if( longFlag[0] == kHyphenchar && longFlag[1] != kHyphenchar ) {
                // ERROR: User has specified a wrong format of longFlag:
                _ERROR_MESSAGE( "In %s(), illegal format of long flag in calling. \
                        (use double hyphen '--flag' or no hyphen 'flag' for long flags", __FUNCTION__ );
                if( bQuitOnError ) {
                    exit(-1);
                } else {
                    return( kEmptyString );
                }
            }
        }

        if( mbUnflaggedApiCalled && bIsBoolean && shortFlagStr != std::string("-h") && longFlagStr != std::string("--help") ) 
        {   // Can we make this a compile error?!
            _ERROR_MESSAGE( "WARNING!: Please extract all flagged boolean arguments before extracting unflagged argument!\n" );
        }

        // Search in the argument list to find the specified flag:
        for( int i = 0; i < arguments.size(); i++ )
        {
            //std::cout << "i="<<i<<" "<<arguments[i] << " vs " << kSingleHyphen + shortFlag << std::endl;
            if( ( shortFlag != NULL && arguments[i] == shortFlagStr ) || 
                 ( longFlag != NULL && arguments[i] == longFlagStr ) )
            {
                // Flag found.
                arguments.erase( arguments.begin()+i );
                
                // Is this the last argument?
                if( i < arguments.size() && !arguments[i].empty() ) 
                {
                    if( bIsBoolean ) //&& arguments[i][0] == kHyphenchar ) 
                    {
                        return( kSpaceString );
                    } else {
                        std::string valueString = arguments[i];
                        arguments.erase( arguments.begin()+i );
                        return( valueString );
                    }
                } else {
                    return( kSpaceString );
                }
                i--; // important!!
            }
        }
        //_DEBUG_MESSAGE( "%s: flag:%s, returns empty string.\n", __FUNCTION__, (shortFlag!=NULL? shortFlag : longFlag) );
        return( kEmptyString );
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
                    _DEBUG_MESSAGE ("WARNING! There may be unknown flags in the command line, or in source code you have extracted\n" ); 
                    _DEBUG_MESSAGE ("         unflagged arguments before extracting all the flagged ones.\n");
                    _DEBUG_MESSAGE ("         Consult the readme description in SmpCommandLine.hpp for details.\n");
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
        return( kEmptyString );
    };

    void addHelpMessage( const char* shortFlag, const char* longFlag, const std::string defaultStr, 
                         const char* helpMsg, bool bIsBoolean = false )
    {
        // Constuct help message for this item:
        std::string helpStr;

        if( shortFlag != NULL ) {
            std::string shortFlagStr(shortFlag); 

            if( shortFlagStr[0] != kHyphenchar ) {
                shortFlagStr = kSingleHyphen + shortFlag;
            }
            helpStr = helpStr + shortFlagStr;
        }

        if( shortFlag != NULL && longFlag != NULL ) {
            helpStr = helpStr + "/";
        }

        if( longFlag != NULL ) {
            std::string longFlagStr(longFlag);

            if( longFlagStr[0] != kHyphenchar ) {
                longFlagStr = kDoubleHyphen + longFlag;
            }
            helpStr = helpStr + longFlagStr;
        }

        if( !bIsBoolean )
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
    SmpCommandLine(){
        int  mArgCount = 0 ;
        int  maxUnflaggedArgs = 0;
        bool mbWarningHasShown = false;
        bool mbHelpMsgHasShown = false;
        bool mbUnflaggedApiCalled = false;
    };
}; 


