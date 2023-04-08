#!/bin/bash
# this file reconstruct from all the folder
# To run this we need steering file named "b2d.py" in same directory
# WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.

path=$(pwd)

# path of steering file code
steering_file="$path/b2d_bcs_myveto_1pi.py"                      
# steering_file="$path/b2d_bcs_veto.py"                      
echo "Steering file is $steering_file"

# Strating reconstruction
declare -a options=("signal" "uubar" "ddbar" "ssbar" "ccbar") #  "mixed" "charged" 

for opt in "${options[@]}"
do
    # path of output files
    if [[ $opt == "uubar" || $opt == "ccbar" ]] # making separet directory for files more than 1k
    then
        output_folder_name_1_to_1000="$path/data/$opt/sub00"        
        if [ -d "$output_folder_name_1_to_1000" ]
        then
            echo "$output_folder_name_1_to_1000 already exists."
        else 
            $(mkdir -p ${output_folder_name_1_to_1000})
            echo "$output_folder_name_1_to_1000 has been created"
        fi
        output_folder_name_1001_2000="$path/data/$opt/sub01"        
        if [ -d "$output_folder_name_1001_2000" ]
        then
            echo "$output_folder_name_1001_2000 already exists."
        else 
            $(mkdir -p ${output_folder_name_1001_2000})
            echo "$output_folder_name_1001_2000 has been created"
        fi
        echo "Name of the folder of output file are $output_folder_name_1_to_1000 and $output_folder_name_1001_2000"
    else
        output_folder_name="$path/data/$opt"        
        if [ -d "$output_folder_name" ]
        then
            echo "$output_folder_name already exists."
        else 
            $(mkdir -p ${output_folder_name})
            echo "$output_folder_name has been created"
        fi
        echo "Name of the folder of output file is $output_folder_name"
    fi

    # Setting different input folder for 
    # Separately reconstructing from signal MC and generic MC
    # path of input files and     # Reading file name in given folder
    if [[ $opt == "signal" ]]
    then
        input_datapath='/home/belle2/ssana/Official_signalMC'
        input_folder_name="$input_datapath/sub00"
        echo "Name of input folder is $input_folder_name"

        input_filelist=$(ls ${input_folder_name}/*.root)
        input_file_count=$(ls ${input_folder_name}/*.root | wc -l)
        echo "Number of input file in $input_folder_name is $input_file_count"
    elif [[ $opt == "uubar" || $opt == "ccbar" ]]
    then
        input_datapath='/group/belle2/dataprod/MC/MC15ri'
        input_folder_name_1="$input_datapath/$opt/sub00"
        input_folder_name_2="$input_datapath/$opt/sub01"
        echo "Name of input folder are $input_folder_name_1 and $input_folder_name_2"

        input_filelist=$(ls ${input_folder_name_1}/*.root)
        input_filelist+=" "
        input_filelist+=$(ls ${input_folder_name_2}/*.root)

        input_file_count_1=$(ls ${input_folder_name_1}/*.root | wc -l)
        input_file_count_2=$(ls ${input_folder_name_2}/*.root | wc -l)
        input_file_count=$(($input_file_count_1 + $input_file_count_2))
        echo "Number of input file in $input_folder_name_1 and $input_folder_name_2 is $input_file_count"
    else
        input_datapath='/group/belle2/dataprod/MC/MC15ri'
        input_folder_name="$input_datapath/$opt/sub00"
        echo "Name of input folder is $input_folder_name"

        input_filelist=$(ls ${input_folder_name}/*.root)
        input_file_count=$(ls ${input_folder_name}/*.root | wc -l)
        echo "Number of input file in $input_folder_name is $input_file_count"
    fi
    
    ###########################################################
    # Reading file name in given folder
    # input_filelist=$(ls ${input_folder_name}/*.root)
    # input_file_count=$(ls ${input_folder_name}/*.root | wc -l)
    # echo "Number of input file in $input_folder_name is $input_file_count"

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
    if [[ $input_file_count == $count && $input_file_count == $length_input_file_array ]]
    then
        echo "Input file count and input file array dimension are same."
    else
        echo "There is some discrepancy between Input file count and input file array dimension"
        echo "Because Input file count is $input_file_count but array dimension is $length_input_file_array"
    fi

    # Submiting Jobs
    for((i=0; i<$length_input_file_array; i++))
    do
        if [[ $opt == "uubar" || $opt == "ccbar" ]] # saving files (after 1k) in separet directory for uubar and ccbar
        then
            # if [[ $i < 1000 ]]    # checks less than greater than in "ASCII alphabetical order"
            # if [[ (($i < 1000)) ]]    # This also doesn't act as mathematical operation
            if [ $i -lt 1000 ]
            then
                output_folder_name=$output_folder_name_1_to_1000
            else
                output_folder_name=$output_folder_name_1001_2000
            fi
            echo "$(bsub -q s "basf2 $steering_file ${input_file_array[$i]} $output_folder_name")"
        else
            echo "$(bsub -q s "basf2 $steering_file ${input_file_array[$i]} $output_folder_name")"
        fi
    done
done