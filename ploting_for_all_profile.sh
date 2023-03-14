#!/bin/bash

# path=$(pwd)

# # path of steering file code
# steering_file="$path/my_anyfile_anyvariable_bin_range.py"                      
# # steering_file="$path/b2d_bcs_veto.py"                      
# echo "Steering file is $steering_file"

# Strating reconstruction
declare -a options=("all" "charged" "mixed" "uubar" "ddbar" "ssbar" "ccbar")  # "signal" 

for opt in "${options[@]}"
do
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root DstrminusroeD_md 500 0 2.2 Difference/in/mass/of/D^{*-}/and/#bar{D}^{0}")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root D_str0_pi0D_md 500 0 0.15 Difference/in/mass/of/#bar{D}^{*0}/and/#bar{D}^{0}")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root D_str0_gammaD_md 500 0 0.25 Difference/in/mass/of/#bar{D}^{*0}/and/#bar{D}^{0}")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvMD0and1stpi 500 1.9 5.1 Invariant/mass/of/#bar{D}^{0}/and/1^{st}/pion")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvMD0and2ndpi 500 1.9 5.1 Invariant/mass/of/#bar{D}^{0}/and/2^{nd}/pion")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvMD0and3rdpi 500 1.9 5.1 Invariant/mass/of/#bar{D}^{0}/and/3^{rd}/pion")"
    echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvM1stand2ndpi 200 0.48 0.51 Invariant/mass/of/1^{st}/and/2^{nd}/pion")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvM1stand3rdpi 500 0 3.2 Invariant/mass/of/1^{st}/and/3^{rd}/pion")"
    echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvM2ndand3rdpi 200 0.48 0.51 Invariant/mass/of/2^{nd}/and/3^{rd}/pion")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvM1st2nd3rdpi 500 0 3.7 Invariant/mass/of/1^{st}/2^{nd}/and/3^{rd}/pion")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvMD0and1st2ndpi 500 2.2 5.3 Invariant/mass/of/#bar{D}^{0}/and/1^{st}/and/2^{nd}/pion")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvMD0and2nd3rdpi 500 2.2 5.3 Invariant/mass/of/#bar{D}^{0}/2^{nd}/and/3^{rd}/pion")"
    # echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$opt.root InvMD0and1st3rdpi 500 2.2 5.3 Invariant/mass/of/#bar{D}^{0}/1^{st}/and/3^{rd}/pion")"
done

# Strating reconstruction
# declare -a combo=("ch_m_cc" "m_cc")
# for item in "${combo[@]}"
# do 
#     echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$item.root InvMD0and1stpi 100 1.95 2.15 Invariant/mass/of/#bar{D}^{0}/and/1^{st}/pion")"
#     echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$item.root InvMD0and2ndpi 100 1.95 2.15 Invariant/mass/of/#bar{D}^{0}/and/2^{nd}/pion")"
#     echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/$item.root InvMD0and3rdpi 100 1.95 2.15 Invariant/mass/of/#bar{D}^{0}/and/3^{rd}/pion")"
# done
echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/m_cc.root InvMD0and1stpi 150 2 2.02 Invariant/mass/of/#bar{D}^{0}/and/1^{st}/pion")"
echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/m_cc.root InvMD0and3rdpi 150 2 2.02 Invariant/mass/of/#bar{D}^{0}/and/3^{rd}/pion")"
echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/ch_m_cc.root InvMD0and2ndpi 150 2 2.02 Invariant/mass/of/#bar{D}^{0}/and/2^{nd}/pion")"
echo "$(bsub -q s "basf2 my_anyfile_anyvariable_bin_range.py data/combined/ch_m_cc.root DstrminusroeD_md 150 0.1 0.2 Difference/in/mass/of/D^{*-}/and/#bar{D}^{0}")"