#!/bin/bash
# This file selectes the signal event from charged file
# To run this we need PYROOT file named "my_signal.py" in same directory
path=$(pwd)

# path of steering file code
pyrt_file="$path/my_scale_lumino.py"                      
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
declare -a options=("signal" "uubar" "ddbar" "ssbar" "ccbar" "charged")
in6out6=""
for opt in "${options[@]}"
do
    in6out6+="$input_path/train_$opt.root "
done
for opt in "${options[@]}"
do
    in6out6+="$output_path/train_$opt.root "
done

# Command for scaling
# echo "$(pyroot my_signal.py $in6out6)"       #For own system
echo "$(basf2 $pyrt_file $in6out6)"        #For BELLE II server

# Combining signal and charged file
h_output_file="$output_path/train_bbbar.root"
combine="$output_path/train_signal.root $output_path/train_charged.root"
echo "$(hadd ${h_output_file} ${combine})"