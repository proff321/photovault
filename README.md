#Photo Vault#
**An application centered around the idea that manageing photos should be easy and what is archived today should be readable in years to come.**

##Introduction##
As a user's collection of images grows in time, it becomes quite overwhelming to manage that collection.  Especially when the user does not have the time to catelog and tag all of the images captured on a daily basis.  Well, that's where PhotoVault comes in.

Basically, the user uploads Etheir images to a *watch folder* of sorts and then runs the applicaiton.  Once complete, the photos are then sorted into folders based on their capture date.  The folder structure follows the format `/Year/Month/Date/ImageName.jpg`.  This makes it super easy for a user to locate images based on what we all undestand, time.

Right now the application is in its minimum state.  As time goes on, more features will be added.  You are encouraged to read through the [Development Plan](#developmentPlan) to get an idea of what the future of PhotoVault looks like.

##Current Features (Version 0.0.0)
1. Place images in appropriate folder based on date taken (capture date).  Images are stored in folders that start with the year, and then the month, and then the day of the month.  The final structure is `/Year/Month/Day/ImageName.jpg`.
1. Images without a capture date, are placed into a seperate folder where the user can do what they will with them.
1. Takes in command line arguments that specify the locations for the various required folders.  See [Configuration Properties](#configurationProperties) for more information.

##<a id="developmentPlan"></a>Development Plan

**Version 0.0.1**

1. Place images into the appropiate file structure based on their creation date if no capture date can be found.
1. Hash the images to give each image a unique name.
1. Update program to take arguments from a config file instead of command line argument.

##<a id="configurationProperties"></a>Configuration Poperties

**workingDir** - This is the absolute path to the directory where the application can store images that are being processed.  This directory does not require a large amount of space as only one image is stored at a time.  

**dumpDir** - This is the directory from which photos (and other files) are parsed for sorting and manipulation.  The typical setup is to make this a network share which users can *dump* their media into for the application to then sort.

**archiveDir** - This is the directory where the photos with a capture date are stored.  This should be a `read-only` directory as it is intended to function as an archive over time.

**errorDir** - This is the directory where the photo whithout a capture date are stored.  Basically, if the file (normally a photo) does not have the necessary EXIF data, it is moved to this directory for the user to what (s)he will with it.

**help** - Displays generic help text describing each of the accepted commands.
