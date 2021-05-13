// SmpCommandLine_Demo.cpp
// Dennis @ 2021-05
// Codes demostrate how to use SmpCommandLine.hpp
// Compile: $/g++ SmpCommandLine_Demo.cpp
// Run: $./a.out ...

#include "SmpCommandLine.hpp"

int main( int argc, char *argv[] )
{
    SmpCommandLine userCommands( argc, argv );

    // Firstly extract all flagged argumants (i.e. argument identified by a leading hyphen flag)
    int  index = userCommands.getInteger( "i", "index", int(0), "specifies the index of the item" );
    double radius = userCommands.getDouble( "r", "radius", double(6750.0), "the radius of the shpere" ); 
    bool bShowImage = userCommands.getBoolean( "s", "show_image", CMD_FLAG_ONLY, false, "whether display the image during processing" );
    std::string description = userCommands.getString( "d", "description", "", "a description text string" );
 
    // Then, extract unflagged arguments:
    std::string srcFileName = userCommands.getArgument( 1, "", "file name of the source image" );
    std::string tgtFileName = userCommands.getArgument( 2, "", "file name of the target image" );

    // Check whether need to show help message:
    if( userCommands.helpMessageWanted() || argc < 3 ) {
        userCommands.showHelpMessage();
    }

    //
    std::cout << "index = "  << index << std::endl;
    std::cout << "radius = " << radius << std::endl;
    std::cout << "bShowImage = " << bShowImage << std::endl;
    std::cout << "description : " << description <<std::endl;
    std::cout << "srcFileName : " << srcFileName << std::endl;
    std::cout << "tgtFileName : " << tgtFileName << std::endl;

return(0);
}
