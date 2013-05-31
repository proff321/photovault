
#include <iostream>

// File system manipulation library
#include <boost/filesystem.hpp>

// Used to itterate over directories
#include <boost/foreach.hpp>

// Moves images to folders based on date
#include "picSorter.h"

// Program options manager
#include <boost/program_options.hpp>


// Shorten namespace to 'po' (Program Options)
namespace po = boost::program_options;

using namespace std;

int main(int argc, char* argv[]){

	cout << "Staring PicServe!" << '\n';

	// Define configuration options
	po::options_description desc("Allowed options");
    desc.add_options()
            ("workingDir", po::value<string>(), "Absolute path to the working directory for the PhotoVault application.")
            ("dumpDir", po::value<string>(), "Absolute path to the directory where images are being dumped and awaiting processing.")
            ("archiveDir", po::value<string>(), "Absolute path to the directory where images are archived once processed.")
            ("errorDir", po::value<string>(), "Absolute path to the directory where images that could not be processed are stored.")
            ("help", "Shows general help output.")
        ;

    // Create a variable to hold the confiration options
	po::variables_map vm;        

	// Get the configuration options from the command line
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    // Check to see if the user is requesting help.  If so, output the generic boost defult help.
    if (vm.count("help")) {
        cout << desc << "\n";
        return 0;
    }

    // Check to make sure the user has supplied mandatory parameters
    if(!(vm.count("workingDir") && vm.count("dumpDir") & vm.count("archiveDir") & vm.count("errorDir"))){

    	cout << "You must include parameters 'workingDir' 'dumpDir' 'archiveDir' and 'errorDir' in order for the program to run";
    	return 0;
    }

	// Loop through the files in the dump directory and check to see if it is a supported format.
	// If the image is a supported format, move the current image to the working directory
	
	// Create the directory iterator
	boost::filesystem::directory_iterator dit(vm["dumpDir"].as<string>()), eod;

	// Use the boost 'for each' function to loop through the directory
	BOOST_FOREACH(boost::filesystem::path const &currentFilePath, std::make_pair(dit, eod))
	{
		std::cout << "Working on file: " << currentFilePath.filename() << '\n';

		std::cout << "Moving File:  " << currentFilePath.filename() << '\n';
			
		// The file is a supported format, so move it to the workign directory	

		// Create a temporary path that points to the working directory.
		// Create another temporary path that holds just the file name.
		// Putting the two paths together crates a new location for the file currently
		// being worked on.
		boost::filesystem::path workingFilePath(vm["workingDir"].as<string>());
		boost::filesystem::path fileName(currentFilePath.filename());
		workingFilePath /= fileName;

		// Copy the file to the working directory
		// TODO:  Remove the overwrite feature once the files are getting properly processed
		copy_file(currentFilePath, workingFilePath, boost::filesystem::copy_option::overwrite_if_exists);
	
		// Remove the file from the Photo Dump location
		remove(currentFilePath);
		
		// Send the file to the photo sorter so it can copy the file to the appropriate location
		// Pass the file's current location as well as the archive location and the error directory locations
		PhotoVault::PicSorter pSorter(workingFilePath.native(), vm["archiveDir"].as<string>(), vm["errorDir"].as<string>());

		// Tell photo sorter to sort the file 
		pSorter.MoveImage();
		// Get the image information
		//string imageDate = getImageInformation(tempPath);

	}


	// Finish the application
	return 0;

}








