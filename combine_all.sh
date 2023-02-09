#!/bin/bash
# This file combines all files after reconstruction
# WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
# SO WE DON'T NEED TO RUN "FOR" LOOP TO ADD PATH NAME
path=$(pwd)

# Create Directory to save combined root files
output_path="$path/data/combined"
if [ -d "$output_path" ]
then
    echo "$output_path already exists."
else 
    $(mkdir -p ${output_path})
    echo "$output_path has been created"
fi

all_combine_input=""    #To store all the the output of below for loop
all_combine_output="$output_path/all.root"


##the root file number in one job
let nRootFile=300

#combining for charged folders
declare -a options=("charged" "mixed" "uubar" "ddbar" "ssbar" "ccbar" "signal") 

for opt in "${options[@]}"
do
    # OUtput files
    output_file="$output_path/$opt.root"
    if [ -f "$output_file" ]
    then
        echo "$output_file already exists."
        $(rm -f ${output_file})
        echo "So $output_file has been deleted and a new file will be generated."
    fi

    # Input files
    if [[ $opt == "uubar" || $opt == "ccbar" ]]
    then
        input_path_1="$path/data/$opt/sub00"
        input_path_2="$path/data/$opt/sub01"
        input_path="$input_path_1 and $input_path_2"
        echo "Name of input folder are $input_path_1 and $input_path_2"

        input_files=$(ls ${input_path_1}/*.root)
        input_files+=" "
        input_files+=$(ls ${input_path_2}/*.root)

        input_file_count_1=$(ls ${input_path_1}/*.root | wc -l)
        input_file_count_2=$(ls ${input_path_2}/*.root | wc -l)
        input_file_count=$(($input_file_count_1 + $input_file_count_2))
        echo "Number of input file in $input_path_1 and $input_path_2 is $input_file_count"
    else
        input_path="$path/data/$opt"
        input_files=$(ls ${input_path}/*.root)
        input_file_count=$(ls ${input_path}/*.root | wc -l)
        echo "Number of input files in $input_path is $input_file_count"
    fi


    ###########################start combining##################
    echo "Please wait for combination of $nRootFile input file from $input_path folder....."

    if [ $input_file_count -lt $nRootFile ]
    then
        echo "In $input_path folder the given number($input_file_count) of files to be combined is less than $nRootFile."
        echo "So hadd is being applying just once."
        echo "$(hadd ${output_file} ${input_files})"
    else
        combine=""  # Empty array to store files
        intermediate_files=""   # To store the name of intermediate combinded files
        let nRootFilesFlag=0    # To take counts and combine when it takes value n*$nRootFile
        let newFileNo=0;
        #Combining files in a bunch containing 300 files
        for l in ${input_files}
        do 
            combine+="$l "
            ((nRootFilesFlag++))

            if [ $nRootFilesFlag -eq $nRootFile ]
            then
                ((newFileNo++))
                temp="${output_path}/${opt}_${newFileNo}.root"
                echo "$(hadd ${temp} ${combine})"
                intermediate_files+="$temp "
                nRootFilesFlag=0
                combine=""
            fi
        done

        ##Leftover root file
        if [ -z "$combine" ]        # Checking whether the string is empty
        then
            echo "No root file is leftover after running for loop $newFileNo*$nRootFile times"
        else
            ((newFileNo++))
            echo "Please wait for combination of last bunch of input file from $input_path folder....."
            temp="${output_path}/${opt}_${newFileNo}.root"
            echo "$(hadd ${temp} ${combine})"
            intermediate_files+="$temp "
        fi


        # Combining all files
        echo "$(hadd ${output_file} ${intermediate_files})" 
                                        # the 2nd arfument of "hadd" should be "STRING" only
        all_combine_input+="$output_file " 
        echo "Deleting $intermediate_files ......"
        $(rm -rf ${intermediate_files})
    fi
done

# Combining all file into one file
if [ -f "$all_combine_output" ]
then
    echo "$all_combine_output already exists."
    $(rm -f ${all_combine_output})
    echo "So $all_combine_output has been deleted and a new file will be generated."
fi

echo "Please wait for WHOLE Combinded output file....."
echo "$(hadd ${all_combine_output} ${all_combine_input})"