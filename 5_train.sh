#!/bin/bash
# This file trains FBDT and gives us weightfile
# To run this code we need steering file named "my_train.py" in same directory
path=$(pwd)

# path of steering file code
steering_file="$path/my_train_signal.py"                      
# steering_file="$path/my_train.py"                      
echo "Steering file is $steering_file"

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
echo "$(basf2 $steering_file $input_file $output_file)"