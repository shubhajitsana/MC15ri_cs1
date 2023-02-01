#!/bin/bash
# This file combines files for training(mailny used for scaling)
# WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
# SO WE DON'T NEED TO RUN "FOR" LOOP TO ADD PATH NAME
path=$(pwd)

# Create Output Directory to save combined root files
output_path="${path}/cs/test/prescale_combine"
if [ -d "$output_path" ]
then
    echo "$output_path already exists."
else 
    $(mkdir -p ${output_path})
    echo "$output_path has been created"
fi

#combining for different folder
declare -a options=("charged" "uubar" "ddbar" "ssbar" "ccbar" "signal") #"mixed" 

for opt in "${options[@]}"
do
    # Ouput file name
    output_file="$output_path/test_$opt.root"
    if [ -f "$output_file" ]
    then
        echo "$output_file already exists."
        $(rm -f ${output_file})
        echo "So $output_file has been deleted."
    fi

    # Input Directory
    input_path="$path/cs/test/$opt"
    input_files=$(ls ${input_path}/*.root)
    input_file_count=$(ls ${input_path}/*.root | wc -l)
    echo "Number of input files in $input_path is $input_file_count"

    # Combining files
    if [[ $opt == "signal" ]]
    then
        echo "$(cp $input_files $output_file)" 
                #it's just copying a file from one folder to another folder with different name
    else
        echo "Please wait for Combinded output file from $input_path folder....."
        # echo "$(hadd ${output_path}/train_$opt.root ${path_in_string})" 
        # WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
        echo "$(hadd ${output_file} ${input_files})" 
                                        # the 2nd arfument of "hadd" should be "STRING" only
    fi
done