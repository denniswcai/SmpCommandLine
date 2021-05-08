# SmpCommandLine
SmpCommandLine is a light-weight header-file-only tool used to parse and extract arguments from command line.

// SmpCommandLine.hpp
// Dennis @ 2021-05
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
