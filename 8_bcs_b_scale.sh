#!/bin/bash
# This file scales the event from uubar,ddbar,ssbar,ccbar file
# To run this we need PYROOT file named "my_scale.py" in same directory
path=$(pwd)

# path of steering file code
# pyrt_file="$path/my_scale_bcs_lumino.py"    # To take data according to overall cross section
# pyrt_file="$path/my_scale_bcs_lumino_50_50.py"    # To take data like 50% as signal and 50% as continnum event
pyrt_file="$path/my_scale_bcs_lumino_50_50_after_all_cut.py"    # To take data like 50% as signal and 50% as continnum event
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
declare -a options=("bbbar" "uubar" "ddbar" "ssbar" "ccbar")
in5out5=""
for opt in "${options[@]}"
do
    in5out5+="$input_path/test_deltaz_filtered_$opt.root "
done
for opt in "${options[@]}"
do
    in5out5+="$output_path/test_deltaz_filtered_$opt.root "
done

# Command for scaling
# echo "$(pyroot my_scale.py $in5out5)"         #For own system
echo "$(basf2 $pyrt_file $in5out5)"          #For BELLE II server

# To take data like 50% as signal and 50% as continnum event
# in_bbbar="$input_path/test_deltaz_filtered_bbbar.root"
# out_bbbar="$output_path/test_deltaz_filtered_bbbar.root"
# echo "$(cp ${in_bbbar} ${out_bbbar})"