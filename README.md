# SmpCommandLine 
## a light-weight header-file-only c++ package for user command line parsing and extraction.

    (For a qucik reference of usage, you may check the example codes in SmpCommandLine_Demo.cpp)
  
  ## In command line, arguments provided by user are separated by space. In general, there are two 
    kinds of arguments in command line: Flagged Arguments and Unflagged Arguments. A Flagged 
    Argument could start with a hyphen sign and a conjuncted character (eg: -n), which is referred
    to as short flag, or, start with double hyphen signs with a string (e.g.: --image_name), 
    which is referred to as long flag. The flag is usually followed by an argument giving the value
    of the specified field (e.g.: --image_name my_photo.jpg).
    
  ## How to use this SmpCommadnLine.hpp:
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
  
  ## IMPORTANT NOTES: 
    Please make sure you call getXxxxx( shortFlag, longFlag, ... ) to extract all flagged 
    arguments before calling getXxxx( index, ... ) to extract unflagged arguments, or the you may 
    extract unexpected argument when calling getXxxx( index, ... ). 
    A bit more notes:
    * Flags should start with a alphabet ranging from 'a'~'z' or 'A'~'Z', they can not start with a
      number.
    * string argument cannot contain any printable characters.
  
  ## Examples Codes can be found in SmpCommandLine_Demo.cpp
  
