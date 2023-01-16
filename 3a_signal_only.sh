#!/bin/bash
# This file selectes the signal event from charged file
# To run this we need PYROOT file named "my_signal.py" in same directory
path=$(pwd)

# path of steering file code
pyrt_file="$path/my_signal.py"                      
echo "Steering file is $pyrt_file"

# Input Directory
input_path="$path/cs/train"


# Create Output Directory to save combined root files
output_path="$path/cs/train/signal_scaled"
if [ -d "$output_path" ]
then
    echo "$output_path already exists."
else 
    $(mkdir -p ${output_path})
    echo "$output_path has been created"
fi

# Preparing string for piping in pyroot code
in4out4=""
in4out4+="$input_path/train_charged.root "
in4out4+="$output_path/train_charged.root "

# Command for scaling
# echo "$(pyroot my_signal.py $in4out4)"       #For own system
echo "$(basf2 my_signal.py $in4out4)"        #For BELLE II server