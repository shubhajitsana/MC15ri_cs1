#!/bin/bash
# This file just copies the signalMC to "signal_scaled" folder
path=$(pwd)

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

echo "$(cp $input_path/train_signal.root $output_path/train_signal.root)"
        #it's just copying a file from one folder to another folder with same name