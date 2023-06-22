# Script to sample the mc file used to train the CS FBDT


import root_pandas
import ROOT
import pandas as pd
import numpy as np
from root_numpy import root2array, tree2array, array2tree, array2root
import glob
import sys
import os
import math

# Variables to save
additional_vars = ['Mbc', 'deltaE', 'isSignal','isContinuumEvent','K_kaonid', 'D0_isSignal', 'D0_genMotherPDG', 'D0_genMotherPDG_1', 'D0_mcMother_mcDaughter_1_PDG', 'K_mcPDG', 'K_genMotherPDG', 'BCS3_rank']
CS_vars = ['abs_qr','DeltaZ',"R2","thrustBm","thrustOm","cosTBTO","cosTBz","KSFWVariables__boet__bc","KSFWVariables__bomm2__bc","KSFWVariables__bohso00__bc","KSFWVariables__bohso01__bc","KSFWVariables__bohso02__bc",
"KSFWVariables__bohso03__bc","KSFWVariables__bohso04__bc","KSFWVariables__bohso10__bc","KSFWVariables__bohso12__bc","KSFWVariables__bohso14__bc","KSFWVariables__bohso20__bc",
    "KSFWVariables__bohso22__bc",
    "KSFWVariables__bohso24__bc",
    "KSFWVariables__bohoo0__bc",
    "KSFWVariables__bohoo1__bc",
    "KSFWVariables__bohoo2__bc",
    "KSFWVariables__bohoo3__bc",
    "KSFWVariables__bohoo4__bc",
    "CleoConeCS__bo1__bc",
    "CleoConeCS__bo2__bc",
    "CleoConeCS__bo3__bc",
    "CleoConeCS__bo4__bc",
    "CleoConeCS__bo5__bc",
    "CleoConeCS__bo6__bc",
    "CleoConeCS__bo7__bc",
    "CleoConeCS__bo8__bc",
    "CleoConeCS__bo9__bc"]

# spherecity is largely correlated with thrust and R2, so no need.

all_cs_vars = CS_vars + additional_vars

nEvents = 80000

# # create lists of root files, need to put correct paths
# signalFileList_1213051105 = glob.glob('/group/belle/users/ansuj/CS/signal/training/1213051105/sub00/1213051105*.root')
# signalFileList_1210030100 = glob.glob('/group/belle/users/ansuj/CS/signal/training/1213052105/sub00/1213052105*.root')

# uubarFileList = glob.glob('/group/belle/users/ansuj/CS/generic/training/uubar/sub00/uubar*.root')
# ccbarFileList = glob.glob('/group/belle/users/ansuj/CS/generic/training/ccbar/sub00/ccbar*.root')
# ddbarFileList = glob.glob('/group/belle/users/ansuj/CS/generic/training/ddbar/sub00/ddbar*.root')
# ssbarFileList = glob.glob('/group/belle/users/ansuj/CS/generic/training/ssbar/sub00/ssbar*.root')

# List of ntuple files

channels = {'1213052105': ['ansuj','signal', 0.647],
            '1213051105': ['ansuj','signal', 0.353],
            'uubar': ['ansuj','generic', 0.43168],
            'ccbar': ['ansuj','generic', 0.35745],
            'ddbar': ['ansuj','generic', 0.10785],
            'ssbar': ['ansuj','generic', 0.10301]}


files = '/group/belle/users/{}/CS/{}/training/{}/{}*.root'

continuum_samples = ['uubar', 'ddbar', 'ssbar', 'ccbar']

signal_array = list()



# Import events for each channel taking only the interesting variables
# We import events as numpy ndarray using tree2array

for i_channel in channels.keys():

    print("Taking events for channel ", i_channel)
    
    user_folder = channels[i_channel][0]
    MC_type = channels[i_channel][1]

    i_signal_chain = ROOT.TChain('kspipi')
  

    i_signal_chain.Add(files.format(user_folder, MC_type, i_channel, i_channel))
    
    events_scale = channels[i_channel][2]
   
    # Take only a fraction of the total number of events (based on cross section) to obtain 80k events
    
    #& BCS3_rank == 1 removed as its best applied after BDT cut
    
    if i_channel == "1213052105":
        cuts = " K_kaonid > 0.6 & Mbc > 5.25 & D0_isSignal == 1 & abs(D0_genMotherPDG) == 423  & abs(D0_genMotherPDG_1) == 521 & abs(D0_mcMother_mcDaughter_1_PDG) == 111 & abs(K_mcPDG) == 321 & abs(K_genMotherPDG) == 521 & abs(K_genMotherPDG) == abs(D0_genMotherPDG_1)"
        
    elif i_channel == "1213051105":
        cuts = " K_kaonid > 0.6 & Mbc > 5.25 & D0_isSignal == 1 & abs(D0_genMotherPDG) == 423  & abs(D0_genMotherPDG_1) == 521 & abs(D0_mcMother_mcDaughter_1_PDG) == 22 & abs(K_mcPDG) == 321 & abs(K_genMotherPDG) == 521 & abs(K_genMotherPDG) == abs(D0_genMotherPDG_1)"
        
    else:
        cuts = " Mbc > 5.25 "
        
    i_signal_array = tree2array(tree=i_signal_chain, branches=all_cs_vars, selection = cuts, start=0, stop=None)
    print(" There are ", i_signal_array.shape[0], " signal events. we need", events_scale*nEvents, "from", i_channel)
    
    
    
    i_signal_array.dtype.names = tuple(all_cs_vars)
       
    #signal_array.append(i_signal_array)
    
    print("---------------->",len(i_signal_array))
    
    # Create pandas dataframe from root tree
    i_signal_df = pd.DataFrame(data=i_signal_array)
    
    #i_signal_df = i_signal_df.dropna() #removing all rows that contain Nan values
    filtered_df = i_signal_df[i_signal_df[['DeltaZ', 'abs_qr']].notnull().all(1)]

    # randomly re-shuffle dataframes
    i_signal_df_train = filtered_df.sample(n=int(nEvents*events_scale))
   

    i_signal_df_train.to_root(
    '/group/belle/users/' + os.environ['USER'] +
    '/CS/training/{}_CStrain.root'.format(i_channel),
    key='kspipi')
   









