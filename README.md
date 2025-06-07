# SmpCommandLine V2.1.0
### Descriptions: 
  SmpCommandLine is a light-weight header-file-only c++ tool for user command line parsing and 
  argument extraction.

  #### NOTE: 
  For a qucik reference of usage, you may directly check the example codes in 
  SmpCommandLine_Demo.cpp
   
  ### Background & Terminologies: 
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

  ### Usage of SmpCommadnLine.hpp:

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

  ### !IMPORTANT NOTES: 

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

  ### Examples of Usage
   	Example codes could be found in SmpCommandLine_Demo.cpp
  
