#!/bin/bash
#
#	This is a basic script to run mangamerger for all directories in the same directory ass 	#  #	ConvertAll is run
#	
#	How it works:
#
#	For this to work make sure MangaMerger is build and is contained in a /bin folder
#	Run <thisScript> <outputDirectory>
#
currentDir=$PWD;
output=$1;

mkdir -p $output;
cd $output;
output=$PWD;
cd $currentDir;

for d in */; do
	inputDir="$currentDir/$d";
    outputFile="$output/${d%/}.pdf";
    if [ "$inputDir" != "$output/" ]; then
    	#echo "$inputDir";
    	#echo "$outputFile";
	    MangaMerger "$inputDir" "$outputFile";
	fi
done

