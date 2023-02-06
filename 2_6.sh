#!/bin/bash
sh 2_combine_for_train_5pm.sh
sh 3a_signal_only.sh
sh 3b_scale.sh
sh 3c_signalMC.sh
sh 4_combine_for_final_train.sh
sh 5_train.sh
sh 6_rec_for_test_5pm.sh