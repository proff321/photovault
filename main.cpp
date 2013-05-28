
#include <iostream>
#include <exiv2/exiv2.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include <iomanip>
#include <cassert>

using namespace std;
using namespace boost::filesystem;

// Declerations
bool verifyWorkingDirectory(path);
bool verifyMasterDirectory();
bool copyImages(path, path);
string getImageInformation(path);
bool moveImage(path, path, string);
void checkOrMake(path);

int main(int argc, char* argv[]){

	cout << "Staring PicServe!" << '\n';

	// Directory information
	path workingDirec ("/tmp/PicServe/UploadedImages");
	path imageDirec ("/srv/samba/picShare");
	
	// Run through a couple of checks and then the main loop
	if(verifyMasterDirectory())
	{

		if(verifyWorkingDirectory(workingDirec))
		{
			//cout << "It Worked!!!" << '\n';
			copyImages(imageDirec, workingDirec);
		}
	
	}

	return 0;

}


// Verifies that the working directory is availalbe and if not tries to create it.
bool verifyWorkingDirectory(path workingDirec){
	
	if(!exists(workingDirec))
	{
		// Make the directory
		if (create_directory(workingDirec))
		{
			cout << "Created working directory." << '\n';
			return true;
		}
		else
		{
			cout << "ERROR:  Failed to create working directory." << '\n';
			return false;
		}
	}
	else
	{
		// Directory exists
		return true;
	}
}

// Verifies that there is a 'PicServe' foler in the /tmp 
bool verifyMasterDirectory()
{
	// Master directory path
	path masterDirec ("/tmp/PicServe");
	
	if(!exists(masterDirec))
	{
		// Make the directory
		if (create_directory(masterDirec))
		{
			cout << "Created master directory." << '\n';
			return true;
		}
		else
		{
			cout << "ERROR:  Failed to create master directory." << '\n';
			return false;
		}
	}
	else
	{
		// Directory exists
		return true;
	}

}

// Copies images from the shared Samba folder to the local working directory
bool copyImages(path imageDirec, path workingDirec)
{
	// Create the directory iterator
	directory_iterator dit(imageDirec), eod;

	// Use the boost 'for each' function to loop through the directory
	BOOST_FOREACH(path const &imagePath, std::make_pair(dit, eod))
	{
		std::cout << "Moving File:  " << imagePath << '\n';

		// Append file name to working directory path
		path tempPath(workingDirec);
		path fileName(imagePath.filename());
		tempPath /= fileName;

		// Copy the file
		copy_file(imagePath, tempPath, copy_option::overwrite_if_exists);
	
		// Remove the old file
		remove(imagePath);
		
		// Get the image information
		string imageDate = getImageInformation(tempPath);
		
		// Move the image 
		if(!imageDate.empty() && imageDate != "ERROR")
		{
				cout << "Image Date:  " << imageDate << '\n';
				
				path archive ("/home/eric/Projects/PicServe/Archive");
				
				moveImage(tempPath, archive, imageDate);
		}
		else
		{
			// Move the image to the un-processable folder on the Samba share
			
			cout << "Moving image " << tempPath.native() << " to import error archive ";
			
			path archive ("/home/eric/Projects/PicServe/Archive");
			
			path errorArchive ("Import Errors");
			
			archive /= errorArchive;
			
			path errorPath(archive);
			path fileName(tempPath.filename());
			errorPath /= fileName;
			
			cout << errorPath.native() << '\n';
			copy_file(tempPath, errorPath, copy_option::overwrite_if_exists);
				
			// Remove the working image file
			remove(tempPath);
			
			
		}
	}

}

// Get's the image information
string getImageInformation(path imageLocation)
{
	// Check and make sure that the image is of a supported type
	try
	{
	
		// Create a constant that refers to the image locations for the EXIV libarary
		string const imageLocationString = imageLocation.native();
		
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
			cout << "INFO:  File " << imageLocation.native() << " is not a supported type." << '\n';
			cout << "Error:  " << e.what() << '\n';
			
			// Return an error
			return "ERROR";
			
	}

}

bool moveImage(path imageDirec, path archiveDirec, string imageDate)
{
	
	// Work through the date and get the year, month and day
	string year =  imageDate.substr(0,4);
	string month = imageDate.substr(5,2);
	string day = imageDate.substr(8,2);
	
	cout << "Month:  " << month << " Day:  " << day << " Year:  " << year << '\n';
	
	path newImagePath ("");
	
	// Start at the archive base
	newImagePath /= archiveDirec;
	
	// Append the year and check to see if it exists
	path yearPath (year);
	newImagePath /= yearPath;
	checkOrMake(newImagePath);
	
	// Append the month and check to see if it exists
	path monthPath (month);
	newImagePath /= monthPath;
	checkOrMake(newImagePath);
	
	// Append the day and check to see if it exists
	path dayPath (day);
	newImagePath /= dayPath;
	checkOrMake(newImagePath);
	
	// Copy the image to the new location
	
	path tempPath(newImagePath);
	path fileName(imageDirec.filename());
	tempPath /= fileName;
	copy_file(imageDirec, tempPath, copy_option::overwrite_if_exists);
	
	// Remove the working image file
	remove(imageDirec);
	
	return true;
}

void checkOrMake(path directory)
{
	// Check and see if the directory exists and if it doesn't then make it
	if(!exists(directory))
	{
		cout << "Creating directory: " << directory.native() << '\n';
		
		create_directory(directory);
	}
}
