#!/bin/bash
# takes command as: ./6_rec_for_test.sh 481 801 256 244 801
# this file reconstruct from all the folder
# To run this we need steering file named "b2d_test.py" in same directory
# WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
# But we need "for" loop to start reconstructing from perticular number

path=$(pwd)

# path of steering file code
steering_file="$path/b2d_test.py"                      
echo "Steering file is $steering_file"

# path of  weight file
weightfile="$path/cs/test/MVAFastBDT.root"
echo "Path of weightfile is $weightfile"

let rec_start_charged=${1:-481}-1        #as for loop starts from next numbering
let rec_start_uubar=${2:-801}-1
let rec_start_ddbar=${3:-256}-1
let rec_start_ssbar=${4:-244}-1
let rec_start_ccbar=${5:-801}-1
let rec_start=0

# Strating reconstruction
declare -a options=("charged" "uubar" "ddbar" "ssbar" "ccbar") #"mixed" 

for opt in "${options[@]}"
do
    # path of output files
    output_folder_name="$path/cs/test/$opt"        
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

    # Finalizing the starting number of array for reconstruction
    if [[ $opt == "charged" ]]
    then
        rec_start=$rec_start_charged
    elif [[ $opt == "uubar" ]]
    then
        rec_start=$rec_start_uubar
    elif [[ $opt == "ddbar" ]]
    then
        rec_start=$rec_start_ddbar
    elif [[ $opt == "ssbar" ]]
    then
        rec_start=$rec_start_ssbar
    elif [[ $opt == "ccbar" ]]
    then
        rec_start=$rec_start_ccbar
    fi

    # Submiting Jobs
    let i=0
    for((i=$rec_start; i<$length_input_file_array; i++))
    do
        # input_file="$input_folder_name/${input_file_array[$i]}"
        # WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
        echo "$(bsub -q s "basf2 $steering_file ${input_file_array[$i]} $output_folder_name $weightfile")"
    done
done