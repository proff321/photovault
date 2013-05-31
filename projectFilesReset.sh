#!/bin/bash

echo Reseting PhotoVault Directories
echo "************************************************************************"

echo Removing files from working directory.
rm -rfv /media/PicServeStorage/WorkingDir/*

echo Removing files from photo archive.
rm -rfv /media/PicServeStorage/Archive/*

echo Removing files form phot dump directory.
rm -rfv /media/PicServeStorage/PhotoDump/*

echo Copying test files back into photo dump directory.
cp -rv /media/PicServeStorage/TestFiles/* /media/PicServeStorage/PhotoDump  

echo Creating sorting error directory.
mkdir /media/PicServeStorage/Archive/SortingErrors

echo "Reset Complete"
echo "************************************************************************"
