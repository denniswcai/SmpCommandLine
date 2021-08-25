# SmpCommandLine V2.0.0
###   Date:2021/08/25

# Descriptions: 

  ## SmpCommandLine is a light-weight header-file-only c++ tool for user command line parsing 
    and argument extraction.

  ## NOTE: For a quick reference of usage, you may directly check the example codes in 
    SmpCommandLine_Demo.cpp
   
  ## Terminology: In command line, arguments provided by user are separated by space. In general, 
    there are two kinds of arguments in command line, namely, Flagged Arguments and Unflagged 
    Arguments. A Flagged Argument starts either with a hyphen sign and one conjuncted character 
    (eg: -n), which is referred to as short flag, or, starts with double hyphen signs and a string 
    (e.g.: --image_name), which is referred to as long flag. In most of case, a flag is followed 
    by an argument which gives the value of the specified field (e.g.: --image_name my_photo.jpg),
    Except for boolean argument, which comes with a flag only and no value part followed.

    Below gives an example of command line:

        ./my_program inputphoto.jpg -o outputphoto.jpg --filter gaussion -w 1024 -h 768 -s 
    
    In the above examples, 'inputphoto.jpg' is an unflagged argument, the rest in the command line
    are flagged argument, where, '--filter' are long flag, others are short flag. And, '-s' is a 
    boolean argument.

  ## How to use this SmpCommadnLine.hpp:

    1) Include SmpCommandLine.hpp in your project's main c++ source file (the one with the main 
       fuctcion), like:

        #include "SmpCommandLine.hpp"
        //...

    2) Make sure you have added the input argument phrase "( int argc, char *argv[] )" in your main
       fucntion's argument list. And, create a SmpCommandLine object at the top of the main(...) 
       function, with the 'argc' and 'argv' as constuctor's parameter, that is:
       
        int main( int argc, char *argv[] ) 
        {
            SmpCommandLine command(argc, argv); 
            //...
        
    3) Call SmpCommandLine's member function to extract different type of command line arguments,
       for examples:

           int imageWidth = getInteger(  "w", "--width", 1024,   "specifies the image width" );

       The prototype of this method is: 
           
            int SmpCommandLine::getInteger( const char* shortflag, const char * longflag, 
                                            int defaultValue, std::string helpMessage);
       
       This method extracts an integer number led by a short flagged (in this case, '-w'), or a 
       long flag (int this case, --width). If none of the specified flags presents in the user 
       command line, the returned value is set to the given 'defaultValue'.

    4) Similiarly, you may call below member function of SmpCommandLine to extract other types of 
       value from command line:

           getFloat( shortFlag, longFlag...), to extract a float number with specified flag
           getDouble( shortFlag, longFlag...), to extract a double number with specified flag
           getString( shortFlag, longFlag...), to extract a string with specified flag. 
           getBoolean( shortFlag, longFlag...), to extract a boolean with specified flag. 

    5) A special note to the getBoolean( shortFlag, longFlag,...) method: In this version, boolean 
       arguments are "flag-only" argument, which means that, in user command line, boolean arguments
       consist of a flag only, but with no value follows behind. for example, below are boolean 
       arguments in user command line:

            ./some_program -x -z -v

       when calls 
            bool x = getBoolean( "x", NULL, "" );
    
       The boolean value 'x' will be set to true. Please also note that, the default value of boolean 
       argument is always false.

    6) Multipy boolean flags can be combined in one shortFlag starting with a single hythen in 
       command line (e.g.: "zip -xzv", which is equalvalue to "zip -x -z -v"). 

    7) After extracting all the flagged arguments from command line, you may call 

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
      
    8) Finally, if necessary, call below piece of codes to show help message to user, when it is 
       wanted:

            if( userCommands.helpMessageWanted() )
                userCommands.showHelpMessage();
       
       to display help hint when user type -h or --help in the command line. The help message is 
       automatically composed from the helpMsg strings provided in the above calls to 
       getArgument(..., helpMsg ).

  ## IMPORTANT NOTES: 

    1) Please make sure you call getXxxxx( shortFlag, longFlag, ... ) to extract all flagged 
       arguments before calling getXxxx( index, ... ) to extract unflagged arguments, or the you may 
       extract unexpected argument when calling getXxxx( index, ... ). 

    2) (For user who type command lines), in this version, when typing command line, please always
       add a space to sperate a flag and its following value. e.g., this is good: 
            -w 1920
       And, this is not good: 
            -w1920

    A bit more notes:
    * Flags should start with a alphabet ranging from 'a'~'z' or 'A'~'Z', they can not start with a
      number.
    * string argument cannot contain any non-printable characters.

  ## Examples Codes could be found in SmpCommandLine_Demo.cpp
