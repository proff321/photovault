#ifndef PhotoVault_PicSorter_h
#define PhotoVault_PicSorter_h

// Filesystem maniuplation library
#include <boost/filesystem.hpp>

// Image data retrieval library
#include <exiv2/exiv2.hpp>

namespace PhotoVault{

 
class PicSorter
{

	// Path for the current file 
	boost::filesystem::path currentFilePath;
	
	// Path for the directory where the archive is stored
	boost::filesystem::path archiveDirPath;

	// Path for the directory where the files that cannot be sorted are stored
	boost::filesystem::path errorDirPath;


	public:

		// Constructor
		// It is passed three strings represeting the absolute path of:
		// Argument 1:  The location of the file for sorting
		// Argument 2:  The directory where the archive is stored
		// Argument 3:  The directory where images that cannot be sorted are stored
		PicSorter(std::string, std::string, std::string);

		// Moves the image into the appropriate folder based on the date is was taken.
		// For images that it cannot determine a date for, they are moved to the 
		// error directory for the user to figure out.
		void MoveImage();

	private:

		// Gets the image's date information
		std::string GetImageDate();

		// Moves the image to the appropriate folder based on date
		void CopyToDateFolder(std::string);

		// Creates the necessary directory if it is missing
		void CheckOrMake(boost::filesystem::path);

};

}

#endif
