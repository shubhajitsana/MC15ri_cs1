#!/bin/bash
# This file selectes the signal event from charged file
# To run this we need PYROOT file named "my_signal.py" in same directory
path=$(pwd)

# path of steering file code
pyrt_file="$path/my_signal_bcs.py"                      
echo "Steering file is $pyrt_file"

# Input Directory
input_path="$path/cs/test/prescale_combine"
output_path=$input_path
# Preparing string for piping in pyroot code
in2out1=""
in2out1+="$input_path/test_charged.root "
in2out1+="$input_path/test_signal.root "
in2out1+="$output_path/test_bbbar.root "

# Command for scaling
# echo "$(pyroot my_signal.py $in2out1)"       #For own system
echo "$(basf2 $pyrt_file $in2out1)"        #For BELLE II server