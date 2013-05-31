#include "picSorter.h"

namespace PhotoVault{


PicSorter::PicSorter(std::string originalFilePathString, std::string archiveDirString, std::string errorDirString):currentFilePath(originalFilePathString),archiveDirPath(archiveDirString),errorDirPath(errorDirString)
{
	
	// Logging information
	std::cout << "Creating new PicSorter object." << '\n';
	std::cout << "File Path:  " << currentFilePath << '\n';
	std::cout << "Archive Directory:  " << archiveDirPath << '\n';
	std::cout << "Error Directory:  " << errorDirPath << '\n'; 

}


void PicSorter::MoveImage()
{
	// Get the date for the current file
	std::string fileDate = GetImageDate();

	// Move the file based on its date
	CopyToDateFolder(fileDate);

}

// Get's the image information
std::string PicSorter::GetImageDate()
{
	// Check and make sure that the image is of a supported type
	try
	{
	
		// Create a constant that refers to the image locations for the EXIV libarary
		const std::string& imageLocationString = currentFilePath.native();
		
		// Open the image
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(imageLocationString);
		
		// Read exif data from image
		image->readMetadata();
		
		// Get the exif data from the image
		Exiv2::ExifData &exifData = image->exifData();
		
		// Return the date	
		return exifData["Exif.Image.DateTime"].toString();
		
	}
	catch (Exiv2::AnyError& e)
	{
			std::cout << "INFO:  File " << currentFilePath.native() << " is not a supported type." << '\n';
			std::cout << "Error:  " << e.what() << '\n';
			
			// Return an error
			return "ERROR";
			
	}

}

void PicSorter::CopyToDateFolder(std::string imageDate)
{
	
	if(imageDate.size() < 10)
	{
		std::cerr << "Error on date retrieval" << std::endl;
		return;
	}

	// Work through the date and get the year, month and day
	std::string year =  imageDate.substr(0,4);
	std::string month = imageDate.substr(5,2);
	std::string day = imageDate.substr(8,2);
	
	std::cout << "Month:  " << month << " Day:  " << day << " Year:  " << year << '\n';
	
	boost::filesystem::path newImagePath ("");
	
	// Start at the archive base
	newImagePath /= archiveDirPath;
	
	// Append the year and check to see if it exists
	boost::filesystem::path yearPath (year);
	newImagePath /= yearPath;
	CheckOrMake(newImagePath);
	
	// Append the month and check to see if it exists
	boost::filesystem::path monthPath (month);
	newImagePath /= monthPath;
	CheckOrMake(newImagePath);
	
	// Append the day and check to see if it exists
	boost::filesystem::path dayPath (day);
	newImagePath /= dayPath;
	CheckOrMake(newImagePath);
	
	// Copy the image to the new location
	
	boost::filesystem::path tempPath(newImagePath);
	boost::filesystem::path fileName(currentFilePath.filename());
	tempPath /= fileName;
	boost::filesystem::copy_file(currentFilePath, tempPath);
	

}

void PicSorter::CheckOrMake(boost::filesystem::path directory)
{
	// Check and see if the directory exists and if it doesn't then make it
	if(!boost::filesystem::exists(directory))
	{
		std::cout << "Creating directory: " << directory.native() << '\n';
		
		boost::filesystem::create_directory(directory);
	}
	else
	{
		std::cout << "ERROR:  Count not create directory:  " << directory.native() << '\n';
	}
}


// End Namespace
}
