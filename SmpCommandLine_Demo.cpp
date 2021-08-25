// SmpCommandLine_Demo.cpp
// Dennis @ 2021-05
// Codes demostrate how to use SmpCommandLine.hpp
// Compile: $ g++ SmpCommandLine_Demo.cpp -o my_program
// Run: $./my_program ...

#include "SmpCommandLine.hpp"  

using namespace std;

int main( int argc, char *argv[] ) 
{
    SmpCommandLine userCommands( argc, argv );

    // Firstly extract all flagged argumants (i.e. argument identified by a leading hyphen flag)
    int    index      = userCommands.getInteger( "i", "index", 0, "specifies the index of the item" );
    double radius     = userCommands.getDouble( "r", "radius", 6750.0, "the radius of the shpere" ); 
    string filterName = userCommands.getString( "f", "filter", "", "specifies an image effect filter" ); 
    bool   bShowImage = userCommands.getBoolean( "s", "show_image", "whether display the image during processing" );
 
    // Then, extract unflagged arguments:
    std::string srcFileName = userCommands.getString( 1, "", "file name of the source image" );
    std::string tgtFileName = userCommands.getString( 2, "", "file name of the target image" );


    // Check whether need to show help message:
    if( userCommands.helpMessageWanted( 3 ) ) {  
        userCommands.showHelpMessage();
    }

    // Print the extracted arguments:
    cout << "index = "  << index << endl;
    cout << "radius = " << radius << endl;
    cout << "bShowImage = " << bShowImage << endl;
    cout << "filter name : " << filterName << endl;
    cout << "srcFileName : " << srcFileName << endl;
    cout << "tgtFileName : " << tgtFileName << endl;

return(0);
}
