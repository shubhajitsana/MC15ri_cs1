#!/bin/bash
# This file combines files for training
path=$(pwd)

train_file="$path/cs/train/signal_scaled/train.root"
weightfile="$path/cs/test/MVAFastBDT.root"
test_file="$path/cs/test/signal_scaled/test.root"
output_report="$path/cs/evaluate.zip"

# Evaluating report
if [ -f "$output_report" ]
then
    echo "$output_report already exists."
    $(rm -f ${output_report})
    echo "So $output_report has been deleted. Plese run this bash file again."
else
    echo "$(basf2_mva_evaluate.py -id $weightfile -train $train_file -data $test_file -o $output_report)"
fi