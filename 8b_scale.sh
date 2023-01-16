#!/bin/bash
# This file scales the event from uubar,ddbar,ssbar,ccbar file
# To run this we need PYROOT file named "my_scale.py" in same directory
path=$(pwd)

# path of steering file code
pyrt_file="$path/my_scale.py"                      
echo "Steering file is $pyrt_file"

# Input Directory
input_path="$path/cs/test/prescale_combine"


# Create Output Directory to save combined root files
output_path="$path/cs/test/signal_scaled"
if [ -d "$output_path" ]
then
    echo "$output_path already exists."
else 
    $(mkdir -p ${output_path})
    echo "$output_path has been created"
fi

# Preparing string for piping in pyroot code
declare -a options=("uubar" "ddbar" "ssbar" "ccbar")
in4out4=""
for opt in "${options[@]}"
do
    in4out4+="$input_path/test_$opt.root "
done
for opt in "${options[@]}"
do
    in4out4+="$output_path/test_$opt.root "
done

# Command for scaling
# echo "$(pyroot my_scale.py $in4out4)"         #For own system
echo "$(basf2 my_scale.py $in4out4)"            #For BELLE II server