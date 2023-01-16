#!/bin/bash
# this file reconstruct from all the folder
# To run this we need steering file named "b2d.py" in same directory
# WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.

path=$(pwd)

# path of steering file code
steering_file="$path/b2d.py"                      
echo "Steering file is $steering_file"

# Strating reconstruction
declare -a options=("charged" "mixed" "uubar" "ddbar" "ssbar" "ccbar") #"mixed" 

for opt in "${options[@]}"
do
    # path of output files
    output_folder_name="$path/data/$opt"        
    if [ -d "$output_folder_name" ]
    then
        echo "$output_folder_name already exists."
    else 
        $(mkdir -p ${output_folder_name})
        echo "$output_folder_name has been created"
    fi
    echo "Name of the folder of output file is $output_folder_name"

    # path of input files
    input_datapath='/group/belle2/dataprod/MC/MC15ri'
    input_folder_name="$input_datapath/$opt/sub00"    
    echo "Name of the folder of Input file is $input_folder_name"

    ###########################################################
    # Reading file name in given folder
    input_filelist=$(ls ${input_folder_name}/*.root)
    input_file_count=$(ls ${input_folder_name}/*.root | wc -l)
    echo "Number of input file in $input_folder_name is $input_file_count"

    #Storing filename in array from long single string
    declare -a input_file_array=()
    let count=0
    for l in ${input_filelist}
    do
        input_file_array[$count]="$l"
        ((count++))
    done
    length_input_file_array=${#input_file_array[@]}

    #checking whether array dimension is ok
    if [ $input_file_count -eq $count ]
    then
        if [ $input_file_count -eq $length_input_file_array ]
        then
            echo "Input file count and input file array dimension are same."
        else
            echo "There is some discrepancy between Input file count and input file array dimension"
            echo "Because Input file count is $input_file_count but array dimension is $length_input_file_array"

        fi
    fi

    # Submiting Jobs
    for((i=0; i<$length_input_file_array; i++))
    do
        # input_file="$input_folder_name/${input_file_array[$i]}"
        # WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
        echo "$(bsub -q s "basf2 $steering_file ${input_file_array[$i]} $output_folder_name")"
    done
done