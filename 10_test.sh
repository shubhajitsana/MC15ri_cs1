#!/bin/bash
# This file combines files for training
path=$(pwd)

train_file="$path/cs/train/signal_scaled/train.root"
weightfile="$path/cs/test/MVAFastBDT.root"
test_file="$path/cs/test/signal_scaled/test.root"
output_report="$path/cs/test/evaluate.zip"

# ################this move is required to remove "RuntimeWarning: invalid value encountered in true_divide
# #   purity = cumsignal / (cumsignal + cumbckgrd)"###################################
# train_file_moved="$path/train.root"
# weightfile_copied="$path/MVAFastBDT.root"
# test_file_moved="$path/test.root"
#                                     # Doesn't remove the error even after trying this
# $(mv ${train_file} ${train_file_moved})
# $(cp ${weightfile} ${weightfile_copied})
# $(mv ${test_file} ${test_file_moved})
# #########################################################################

# Evaluating report
if [ -f "$output_report" ]
then
    echo "$output_report already exists."
    $(rm -f ${output_report})
    echo "So $output_report has been deleted. And new report will be generated."
fi
# echo "$(basf2_mva_evaluate.py -id $weightfile -train $train_file -data $test_file -o $output_report)"
basf2_mva_evaluate.py -id $weightfile -train $train_file -data $test_file -o $output_report
            # this line finally solves runtimewarning
            # ################this move is required to remove "RuntimeWarning: invalid value encountered in true_divide
            # #   purity = cumsignal / (cumsignal + cumbckgrd)"###################################

# # Evaluate and Puting back the files at their own places again#############################
# echo "$(basf2_mva_evaluate.py -id MVAFastBDT.root -train train.root -data test.root -o evaluate.zip)"
#                                             # Doesn't remove the error unless we run this command
#                                             # directly at terminal
# $(mv ${train_file_moved} ${train_file})
# $(rm -f ${weightfile_copied})
# $(mv ${test_file_moved} ${test_file})
# output_report_need_to_move="$path/evaluate.zip"
# $(mv ${output_report_need_to_move} ${output_report})
# ##########################################################################