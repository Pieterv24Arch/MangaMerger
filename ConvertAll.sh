#!/bin/bash
#
#	This is a basic script to run mangamerger for all directories in the same directory ass 	#  #	ConvertAll is run
#	
#	How it works:
#	Run <thisScript> <outputDirectory>
#
currentDir=$PWD;
output=$1;

mkdir -p "$output";
cd "$output";
output=$PWD;
cd "$currentDir";

for d in */; do
	inputDir="$currentDir/$d";
    outputFile="$output/${d%/}.pdf";
    if [ "$inputDir" != "$output/" ]; then
		echo "Input $inputDir"
		#echo "Output $outputDir"
	    MangaMerger "$inputDir" "$outputFile";
	fi
done
