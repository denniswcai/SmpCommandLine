# SmpCommandLine V2.0.0
Date:2021/08/25

## Descriptions: 

  ### SmpCommandLine is a light-weight header-file-only c++ tool for user command line parsing and argument extraction.

  ### NOTE: For a quick reference of usage, you may directly check the example codes in 
    SmpCommandLine_Demo.cpp
   
  ### A bit Terminologies: 
    In command line, arguments provided by user are separated by spaces. And, in general, there are  
    two kinds of arguments in command line, namely, "flagged argument" and "unflagged argument". 
    
    A flagged argument could either start with a single hyphen sign followed immediately by one 
    single character specifying the name of the short flag, (eg: -n), or it could start with a 
    double hyphen followed by a string of characters (e.g.: --image_name), which is referred to as
   long flag. 
    
    In most of the cases, a flag is then followed by an argument which gives the value of that 
    specific field (e.g.: --image_name my_photo.jpg), Except for boolean argument, which comes with 
    a flag only and no value part followed.

    An unflagged argument simplely comes with a string of any characters other than hyphen(-) and 
    space.

    Below gives an example of command line:

        <code>
        ./my_program inputphoto.jpg -o outputphoto.jpg --filter gaussion -w 1024 -h 768 -b 
        </code>
    
    In the above examples, 'inputphoto.jpg' is an unflagged argument, the rest in the command line
    are flagged argument, where, '--filter' are long flag, others are short flag. And, '-b' is a 
    boolean argument with no following values.

  ### How to use this SmpCommadnLine.hpp:

    1) Include SmpCommandLine.hpp in your project's main C++ source file (the one with the main 
       fuctcion), like:

        <code>
        #include "SmpCommandLine.hpp"
        //...
        </code>

    2) Make sure you have added the input argument phrase "( int argc, char *argv[] )" in your main
       fucntion's input argument list. And then, create a SmpCommandLine object at the beginning of
       the main(...) function, with the 'argc' and 'argv' as constuctor's parameter, like,
         <code>
         int main( int argc, char *argv[] ) 
         {
            SmpCommandLine commands(argc, argv); 
            //...
         </code>
        
    3) Call SmpCommandLine's member function to extract different type of command line arguments,
       for examples:

         <code>
         int imageWidth = commands.getInteger( "w", "--width", 1024, "specifies the image width");
         </code>

       The prototype of this method is: 
           
         <code>
         int SmpCommandLine::getInteger( const char* shortflag, const char * longflag, 
                                         int defaultValue, std::string helpMessage );
         </code>
       
       This method extracts an integer number led by either a short flag (in this case, '-w'), or 
       a long flag (int this case, --width). If none of the specified flags presents in the user 
       command line, the returned value is set to the given 'defaultValue' (in this case, 1024).

    4) Similiarly, you may call below member function of SmpCommandLine to extract other types of 
       value with specified flag from command line:

         <code>
           getFloat( shortFlag, longFlag...), to extract a float number with specified flag
           getDouble( shortFlag, longFlag...), to extract a double number with specified flag
           getString( shortFlag, longFlag...), to extract a string with specified flag. 
           getBoolean( shortFlag, longFlag...), to extract a boolean with specified flag. 
         </code>

    5) A special note to the getBoolean( shortFlag, longFlag,...) method: In this version, boolean 
       arguments are "flag-only" argument, which means that, in user command line, boolean 
       arguments consist of only flag and with no value follows behind. for example, below are 
       boolean arguments in user command line:
         <code>
            ./some_program -x -z -v
         </code>

       when calls 
         <code>
            bool x = getBoolean( "x", NULL, "" );
         </code>
       The boolean value 'x' will be set to true. Please also note that, the default value of 
       boolean argument is always false.

    7) After extracting all the flagged arguments from command line, you may call 
         <code>
            getInteger( index, defaultValue, helpMsg )
            getFloat( index, defaultValue, helpMsg )
            getString( index, defaultValue, helpMsg )
            ...
         </code>
       to extract unflagged arguments from fixed sequence position (specified by the fisrt 
       parameter 'index') in command line. 
       
       Sequence position of unflagged argument is the position of argument counted from '1' 
       excluding all flagged arguments (flags and associated values). e.g. for command line: 
         <code>
            ./my_program --wait_time 100 input_file.jpg output_file.jpg 
       
         getString( 1, ... ) returns 'input_file.jpg', and
         getString( 2, ... ) returns 'output_file.jpg' 
         </code>
      
  ### <font color=#FF0000>IMPORTANT NOTES:</font> 

    8) Please always extract all flagged arguments before extract unflagged arguments, or the you 
       may extract unexpected argument when user types in mixed types of argument in command line. 

       That is, you need place the methords of below form to arguments with flags
         <code>
         getxxxx( shortFlag, longFlag...)
         </code>
       before methords of below form to receive all arguments without flag
         <code>
         getxxxx( 1|2|3, ...)
         </code>
      like,
         <code>
         int main( int argc, char *argv[] ) 
         {
            SmpCommandLine commands(argc, argv); 
            
            int imageWidth    = commands.getInteger("w", "--width", 1024, "giving the width of image");
            int imageHeight   = commands.getInteger("h", "--height", 768, "giving the height of image");
            string outputName = commands.getString( "o", "--output", output.jpg, "specify the output image filename");
            // place this type of method at after above methord:
            string inputName  = commands.getString( 1, "input.jpg, "specify the input image filename");
            //...
         </code>

    9) (For user who type command lines), in this version, when typing command line, please always
       add a space to sperate a flag and its following value. e.g., this is good: 
            -w 1920
       And, this is not good: 
            -w1920

    A bit more notes:
    * Flags should start with a alphabet ranging from 'a'~'z' or 'A'~'Z', they can not start with a
      number.
    * string argument cannot contain any non-printable characters.

  ### Examples Codes could be found in SmpCommandLine_Demo.cpp
