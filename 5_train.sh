#!/bin/bash
# This file trains FBDT and gives us weightfile
# To run this code we need steering file named "my_train.py" in same directory
path=$(pwd)

# Output-Input Directory to save combined root files
input_path="$path/cs/train/signal_scaled"
output_path="$path/cs/test"
if [ -d "$output_path" ]
then
    echo "$output_path already exists."
else 
    $(mkdir -p ${output_path})
    echo "$output_path has been created"
fi

input_file="$input_path/train.root"
output_file="$output_path/MVAFastBDT.root"

# command for train
basf2 my_train.py $input_file $output_file