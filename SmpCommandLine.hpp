//
// SmpCommandLine.hpp
// Dennis @ 2021-05

/* 
### Descriptions: SmpCommandLine (V0.5)

  # SmpCommandLine is a light-weight header-file-only tool used to parse and extract arguments from 
    command line.

    You may check the example codes below for quick usage.

    There are two kinds of arguments in command line: Flagged Arguments and Unflagged Arguments.
    A Flagged Argument starts with a hyphen sign followed by one character (eg: -n, referred to as  
    short flag) or, stars with double hyphen sign followed by a word (e.g.: --image_name, which is 
    referred to as long flag). A hyphen flag is usually followed by an argument specifying the value
    of this field (e.g.: --image_name my_photo.jpg).

    All arguments are separated by spaces.

  # How to use this SmpCommadnLine.hpp:

    1) Include SmpCommandLine.hpp in your project's main c++ source file (the one with the main(...) 
       fuctcion):
       #include "SmpCommandLine.hpp"

    2) Make sure you have added "int argc, char *argv[]" in main(...) fucntion's input parameter 
       list, that is: 
       int main( int argc, char *argv[] ) 
    
    3) Create a SmpCommandLine object at the top of the main(...) function, and pass the main()'s
       'argc' and 'argv' to SmpCommandLine's constuction methed, like: 
        int main( int argc, char *argv[] ) 
        {
            SmpCommandLine command(argc, argv); 
            //...
        
    4) Call 
           SmpCommandLine::getInteger( shorFlag, longFlag, defaultValue, helpMsg )
       to extract a integer number (in type of 'int') from command line with flag of 'shorFlag' or
       'longFlag'. If none of the specified flags presents in the command line, the returned value
       of this method is set to 'defaultValue'.

    5) Similiarly, call below methods of SmpCommandLine to extract different type of value from 
       command line:
           getFloat( shortFlag, longFlag...), to extract a float number with specified flag
           getDouble( shortFlag, longFlag...), to extract a double number with specified flag
           getString( shortFlag, longFlag...), to extract a string with specified flag. 
           getBoolean( shortFlag, longFlag...), to extract a boolean with specified flag. 

    6) Take notes to the 'getBoolean()' method. There are two types of command line formats to 
       specify boolean value, one is simply using a flag (e.g. -b), the present of the flag in command
       line sets the associated boolean value to true; the other format is using a flag followed by a 
       text boolean value, explicitly set the boolean to yes/no (or true/false), (e.g.: -b yes /
       -b false). by default, we use the formal format, unless it is clearly specified by 'bFlagOnly' 
       parameter when calling getBoolean().

    7) Multipy boolean flags can be combined in one shortFlag starting with a single hythen in 
       command line (e.g.: '-xzvf', which is equalvalue to '-x -z -v -f'). Note in this case the 
       boolean value should be in FlagOnly type. The last flag can be followed by a value argument.

    8) After extracting all the flagged arguments from command line, you may call 
          getArgument( index, defaultValue, helpMsg ), or 
          getString( index, defaultValue, helpMsg )
       to extract unflagged (common) arguments from fixed sequence position (specified by the fisrt
       parameter 'index') in command line. 
       
       Sequence position of unflagged argument is the position of argument counting from '1', after 
       excluding the flagged arguments (flags and associated values). 
       e.g. for command line: "$./programname --wait_time 100 input_file.jpg output_file.jpg" 
       getArgument( 1, ... ) returns 'input_file.jpg', getArgument( 2, ... ) returns 'output_file.jpg' 

       (Note, unflagged arguments, or common arguments, are those
       arguments without leading flags who start with '-' or '--', unflagged arguments should be given
       in fixed sequence in command line).
    
    9) You may extract unflagged arguments in type other than string, like in type of integer, float, by 
       calling 
          getInteger( index, defaultValue, helpMsg )
          getFloat( index, defaultValue, helpMsg )
       Use the first parameter to specify the sequence positon of the wanted argument. 
       
       Again, please remember to extract these unflagged arguments after extracting all wanted flagged
       arguments.
       
    10) Finally, if necessary, call SmpCommandLine::showHelpMessage() to display help hint to user. the help 
       message is automatically composed from the helpMsg strings provided in the above calls to 
       getArgument(..., helpMsg ).

  # IMPORTANT NOTES: 
       Please make sure you call ::getArgument( shortFlag, longFlag, ... ) to extract all flagged 
    arguments before calling ::getArgument( index, ... ) to extract common (i.e unflagged) arguments, or
    the method ::getArgument( index, ... ) may extract unexpected argument back. 

    A bit more notes:
    * Flags should start with a alphabet ranging from 'a'~'z' or 'A'~'Z', they can not start with a number.
    * string argument can not contain any printable characters.

  # Examples Codes:
    
    // 1) The most simple way:
    //
    #include "SmpCommandLine.hpp"
    int main( int argc, char *argv[] )
    {
        SmpCommandLine commands( argc, argv );

        std::string inputFileName  = commands.getString( "i", NULL );
        std::string outputFileName = commands.getString( "o", NULL );
        int  someNumber  = commands.getInteger( "n", NULL );
        bool someFlag    = commands.getBoolean( "b", NULL );
        // ...
    }
    // then the in command line, user types, e.g.: $./yourprogramname -i original.jpg -o myphoto.jpb -n 123 -b 
    
    // 2) a bit more sophisticated use:
    //
    #include "SmpCommandLine.hpp"
    int main( int argc, char *argv[] )
    {
        SmpCommandLine userCommands( argc, argv );

        // Extract flagged arguments first:
        int index  = userCommands.getInteger( "i", "index", int(0), "specifies the index of the item" );
        float radius = userCommands.getFloat( "r", "radius", float(6750.0), "the radius of the shpere" ); 
        std::string description = userCommands.getString( "d", "description", "", "a description text string" );
        
        // Extract unflagged arguments later:
        std::string srcFileName = userCommands.getArgument( 1, NULL, "file name of the source image" );  // call this after get flagged argument
        std::string tgtFileName = userCommands.getArgument( 2, NULL, "file name of the target image" );  // call this after get flagged argument

        std::cout << "index = "  << index << std::endl;
        std::cout << "radius = " << radius << std::endl;
        std::cout << "description : " << description <<std::endl;
        std::cout << "srcFileName : " << srcFileName << std::endl;
        std::cout << "tgtFileName : " << tgtFileName << std::endl;

        if( argc < 3 ) {
            userCommands.showHelpMessage();
        }

        //....
        //....
    
    return(0);
    }
    // I this case, user types in command line: $./yourprogramname original.jpg output.jpg --index 123 --radius 6.28 --description "this is a demo app"    
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
#include "DebugMessage.h"

class SmpCommandLine 
{
  private:
    const std::string _VERSION_NUMBER_ = "SmpCommandLine V0.5, Dennis @ 2021-05"; //2021-05    
  
  protected:
    const char hyphenchar = '-';
    const std::string singleHyphen = std::string("-");
    const std::string doubleHyphen = std::string("--");
    const std::string emptyString  = std::string("");
    const bool        bQuitOnError = true;

    std::vector<std::string> arguments;
    std::vector<std::string> helpMessageQueue;
    std::string firstLineFlagMsg;
    int  maxUnflaggedArgs;
    bool bWarningHasShown;

  protected:
    std::string getFlaggedArgument( const char* shortFlag, const char* longFlag, bool bFlagOnly=false )
    {
        if( shortFlag == NULL && longFlag == NULL ) {
            ERROR_MESSAGE( "Source usage error: shortFlag and longFlag can not both be NULL!");
            
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
                ERROR_MESSAGE( "Source code error: In %s(), illegal format of long flag in calling. \
                    (use double hyphen '--flag' or no hyphen 'flag' for long flags", __FUNCTION__ );
                if( bQuitOnError ) {
                    exit(-1);
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

                if( !bFlagOnly && i < arguments.size() && !arguments[i].empty() ) 
                {
                    std::string valueString = arguments[i];
                    arguments.erase( arguments.begin()+i );
                    return( valueString );
                } 
                else 
                {
                    return(std::string(" "));
                }
                
                i--; // important!!
            }
        }
        //DEBUG_MESSAGE( "%s: flag:%s, returns empty string.\n", __FUNCTION__, 
        //               (shortFlag!=NULL? shortFlag : longFlag) );

        return( emptyString );
    };

    std::string getUnflaggedArgument( int index )
    {   
        if( index > maxUnflaggedArgs )
            maxUnflaggedArgs = index;

        for( int i = 0, pi = 0; i < arguments.size(); i++ )
        {
            if( arguments[i][0] == '-' ) 
            {
                if( !bWarningHasShown ) {
                    DEBUG_MESSAGE ("WARNING! There may be unknown flags in the command line, or in source code you have extracted\n" ); 
                    DEBUG_MESSAGE ("         unflagged arguments before extracting all the flagged ones (i.e. argument with leading\n" );
                    DEBUG_MESSAGE ("         '-' or '--' sign). Please make sure to extract common arguments after extracting \n" );
                    DEBUG_MESSAGE ("         all flagged arguments in source code.\n");
                    DEBUG_MESSAGE ("         Consult the readme description in SmpCommandLine.hpp for details.\n");
                    bWarningHasShown = true;
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

  public:

    enum {
        WITH_ARG   = 0,
        FLAG_ONLY  = 1
    };

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

        helpMessageQueue.push_back( "Usage:");
        helpMessageQueue.push_back( arguments[0] + " [argument1] ... [--flag1 arg] ..." );

        bWarningHasShown = false;
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

    // Extract FLAGGED argument in int type
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
                ERROR_MESSAGE( "Command line error: Invalid number following flag %s", 
                                (shortFlag!=NULL? shortFlag:longFlag) );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract UNFLAGGED argument in int type
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
                ERROR_MESSAGE( "Command line error: Invalid number in position %d", index );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument in flat type
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
                ERROR_MESSAGE( "Command line error: Invalid number following flag %s", 
                               (shortFlag!=NULL? shortFlag:longFlag) );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
            
        } else {
            return( defaultValue );
        }
    };

    // Extract UNFLAGGED argument in flat type
    float getFloat( int index, float defaultValue = 0.0, const char* helpMsg = "" )
    {
        addHelpMessage( index, to_string(defaultValue), helpMsg );
        
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
                ERROR_MESSAGE( "Command line error: Invalid number in position %d", index );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    DEBUG_MESSAGE( "Use default value" );
                    return( defaultValue );
                }    
            }
            
        } else {
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument in double type
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
                ERROR_MESSAGE( "Command line error: Invalid number following flag %s", 
                               (shortFlag!=NULL? shortFlag:longFlag) );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    DEBUG_MESSAGE("Use default value");
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract UNFLAGGED argument in double type
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
                ERROR_MESSAGE( "Command line error: Invalid number in position %d", index );
                if( bQuitOnError ) {
                    exit(-1);
                } else { 
                    DEBUG_MESSAGE("Use default value");
                    return( defaultValue );
                }    
            }
        } else {
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument in bealean type
    bool getBoolean( const char* shortFlag, const char* longFlag, bool bFlagOnly = true, bool defaultValue = false, const char* helpMsg = "" )
    {
        if( bFlagOnly && defaultValue )
        {
            ERROR_MESSAGE( "Source usage error: In %s(), when bFlagOnly is set to true, defaultValue must be set to false.", __FUNCTION__ );
               
            if( bQuitOnError )
                exit(-1);
            else 
                defaultValue = false;
        }

        addHelpMessage( shortFlag, longFlag, std::to_string(defaultValue), helpMsg, bFlagOnly );

        std::string valueString = getFlaggedArgument( shortFlag, longFlag, bFlagOnly );
    
        if( !valueString.empty() ) 
        {
            if( bFlagOnly )
                return( true );

            std::string argInLower;
            std::transform( valueString.begin(), valueString.end(), 
                           argInLower.begin(), [](unsigned char c){ return std::tolower(c); } );

            if( argInLower == std::string("no")   || argInLower == std::string("n") || 
                argInLower == std::string("false")|| argInLower == std::string("f") ) 
            {
                return( false );
            } else if( 
                argInLower == std::string("yes")  || argInLower == std::string("y") || 
                argInLower == std::string("true") || argInLower == std::string("t") )
            {
                return( true );
            } else {
                
                ERROR_MESSAGE( "Command line error: Invalid value for boolean flag (%s)", (shortFlag!=NULL? shortFlag:longFlag) );
                
                if( bQuitOnError ) {
                    exit(-1);
                } else {
                    DEBUG_MESSAGE( "Use default value (%d)", defaultValue );
                    return( defaultValue );
                }
            }
        } else { 
            return( defaultValue );
        }
    };

    // Extract FLAGGED argument in string type
    std::string getString( const char* shortFlag, const char* longFlag, const char* defaultValue = NULL, const char* helpMsg = "" )
    {
        addHelpMessage( shortFlag, longFlag, (defaultValue!=NULL? std::string(defaultValue) : std::string("")), helpMsg );
        
        std::string valueString = getFlaggedArgument( shortFlag, longFlag );
    
        if( !valueString.empty() && valueString != std::string(" ") ) {
            return( valueString );
        } else {
            if( defaultValue != NULL )
                return( std::string(defaultValue) );
            else 
                return( std::string("") );
        }
    };

    // Extract UNFLAGGED argument in string type
    std::string getString( int index, const char* defaultValue = NULL, const char* helpMsg = "" )
    {   
        addHelpMessage( index, (defaultValue!=NULL? std::string(defaultValue) : std::string("")), helpMsg );
        
        std::string argString = getUnflaggedArgument( index );
       
        if( !argString.empty() ) 
            return( argString );
        else if( defaultValue != NULL )
            return( std::string(defaultValue) );
        else 
            return( std::string("") );
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

    float getArgument( const char* shortFlag, const char* longFlag, float defaultValue, const char* helpMsg = "" )
    {
        return( getFloat( shortFlag, longFlag, defaultValue, helpMsg ) );
    };

    float getArgument( int index, float defaultValue, const char* helpMsg = "" )
    {
        return( getFloat( index, defaultValue, helpMsg ) );
    };

    double getArgument( const char* shortFlag, const char* longFlag, double defaultValue, const char* helpMsg = "" )
    {
        return( getDouble( shortFlag, longFlag, defaultValue, helpMsg ) );
    };

    double getArgument( int index, double defaultValue, const char* helpMsg = "" )
    {
        return( getDouble( index, defaultValue, helpMsg ) );
    };

    bool getArgument( const char* shortFlag, const char* longFlag, bool bFlagOnly, bool defaultValue = false, const char* helpMsg = "" )
    {
        return( getBoolean( shortFlag, longFlag, bFlagOnly, defaultValue, helpMsg ) );
    };

    std::string getArgument( const char* shortFlag, const char* longFlag, const char* defaultValue = NULL, const char* helpMsg = "" )
    {
        return( getString( shortFlag, longFlag, defaultValue, helpMsg ) );
    };

    std::string getArgument( int index, const char* defaultValue = NULL, const char* helpMsg = "" )
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
    };

    //  Check the validity of user inputed command line, according to the getArgument functions called
    //  previously in code.
    int checkValidity()
    {
        if( arguments.size() > 1 )
        {
            for( int i = 1; i < arguments.size(); i++ )
            {
                if( arguments[i][0] == hyphenchar ) {
                    ERROR_MESSAGE( "checkValidity(): Unknown flag %s detected in command line", arguments[i].c_str() );
                    return(-1);
                }
            }
        } 
       
        DEBUG_MESSAGE("checkValidity(): WARNING! Feature not completedly implemented.\n");
        return(0);
    };

    void showVersionInfo()
    {
        std::cout << _VERSION_NUMBER_ << std::endl;
    }
}; 

#endif //__SMP_COMMAND_LINE_HPP__
