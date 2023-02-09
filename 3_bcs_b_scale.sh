#!/bin/bash
# This file scales the event from uubar,ddbar,ssbar,ccbar file
# To run this we need PYROOT file named "my_scale.py" in same directory
path=$(pwd)

# path of steering file code
pyrt_file="$path/my_scale_bcs_lumino.py"                      
echo "Steering file is $pyrt_file"

# Input Directory
input_path="$path/cs/train"


# Create Output Directory to save combined root files
output_path="$input_path/signal_scaled"
if [ -d "$output_path" ]
then
    echo "$output_path already exists."
else 
    $(mkdir -p ${output_path})
    echo "$output_path has been created"
fi

# Preparing string for piping in pyroot code
declare -a options=("uubar" "ddbar" "ssbar" "ccbar" "bbbar")
in5out5=""
for opt in "${options[@]}"
do
    in5out5+="$input_path/train_$opt.root "
done
for opt in "${options[@]}"
do
    in5out5+="$output_path/train_$opt.root "
done

# Command for scaling
# echo "$(pyroot my_scale.py $in5out5)"         #For own system
echo "$(basf2 $pyrt_file $in5out5)"          #For BELLE II server