#!/usr/bin/env python3
import sys
import basf2 as b2
import modularAnalysis as ma
# import stdV0s
# import stdPi0s
import flavorTagger as ft
from variables import variables as vm  # shorthand for VariableManager
import variables.collections as vc
import variables.utils as vu
import vertex as vx

# get input file number from the command line
file_name = sys.argv[1]
if "sub00/" in file_name :
    file_number = file_name.split("sub00/")
else :
    file_number = file_name.split("sub01/")
input_filename = file_number[1]

# get output folder name from the command line
output_foldername = sys.argv[2]
output_filename = input_filename

# weight file
weightfile = sys.argv[3]
#################################################################################

# set analysis global tag (needed for flavor tagging) [S10]
b2.conditions.prepend_globaltag(ma.getAnalysisGlobaltag())  # [E10]

# Perform analysis.
main = b2.create_path()

ma.inputMdstList(environmentType="default",filelist=f"{file_name}",path=main)


# fill final state particle lists
ma.fillParticleList("pi+:myPions2", "abs(d0)<0.2 and abs(z0)<1 and cosTheta >= -0.6", path=main)
ma.fillParticleList("pi+:myPions1", "abs(d0)<0.2 and abs(z0)<1", path=main)
ma.fillParticleList("K+", "abs(d0)<0.2 and abs(z0)<1 and cosTheta >= -0.6", path=main)

#D0 reconstruction
ma.reconstructDecay("anti-D0 -> K+ pi-:myPions1",cut="1.84 < M < 1.89",path=main)
ma.variablesToExtraInfo("anti-D0", variables={'M': 'M_before_fit'}, path=main)
vx.kFit("anti-D0", conf_level=0.0, fit_type='massvertex', path=main)
ma.matchMCTruth("anti-D0", path=main)

#B reconstruction
ma.reconstructDecay("B+ -> anti-D0 pi+:myPions2 pi-:myPions2 pi+:myPions2",cut="5.2 < Mbc < 5.3 and abs(deltaE) < 0.3",path=main)
ma.matchMCTruth("B+", path=main)

ma.buildRestOfEvent(target_list_name="B+", path=main)
cleanMask = ("cleanMask", "nCDCHits > 0 and useCMSFrame(p)<=3.2", "p >= 0.05 and useCMSFrame(p)<=3.2")
ma.appendROEMasks(list_name="B+", mask_tuples=[cleanMask], path=main)
ma.buildContinuumSuppression(list_name="B+", roe_mask="cleanMask", path=main) 

# ############### D* veto starts here #########################
# roe_path = b2.create_path()                                                                                                                  
# deadEndPath = b2.create_path()                             
# ma.signalSideParticleFilter('B+', '', roe_path, deadEndPath)

# vm.addAlias("goodFWDGamma", "passesCut(clusterReg == 1 and clusterE > 0.075)")
# vm.addAlias("goodBRLGamma", "passesCut(clusterReg == 2 and clusterE > 0.05)")
# vm.addAlias("goodBWDGamma", "passesCut(clusterReg == 3 and clusterE > 0.1)")
# vm.addAlias("goodGamma", "passesCut(goodFWDGamma or goodBRLGamma or goodBWDGamma)")
# ma.fillParticleList(decayString="gamma:roe", cut="isInRestOfEvent == 1 and goodGamma", path=roe_path)
# # stdPi0s.stdPi0s(path=roe_path)
# ma.fillSignalSideParticleList(outputListName='anti-D0:sig', decayString='B+ -> ^anti-D0 pi+ pi- pi+',path=roe_path)     

# ma.reconstructDecay(decayString='D0*:veto ->  anti-D0:sig gamma:roe', cut='',path=roe_path)          
# # ma.reconstructDecay(decayString='D0*:veto ->  anti-D0:sig pi0:eff60_May2020Fit', cut='',path=roe_path)          
# rankByLowest(particleList='D0*:veto', variable='massDifference(0)',numBest=1, path=roe_path)  #############COMPLIMENT
# variableToSignalSideExtraInfo(particleList='D0*:veto', varToExtraInfo={'massDifference(0)': 'veto'}, path=roe_path)
# # execute roe_path for each RestOfEvent in the event                  
# main.for_each('RestOfEvent', 'RestOfEvents', roe_path)
# vm.addAlias('dm', 'extraInfo(veto)')
# #ma.printVariableValues('B+', ['dm'], path=main)
# #ma.applyCuts('B+','dm > 0.15',path=main) #############COMPLIMENT
# ################ D* veto ends here ############################

vx.kFit("B+", conf_level=0.0, fit_type='vertex', path=main)
vx.TagV("B+", 'breco', path=main)
ft.flavorTagger(["B+"], path=main)
ma.buildEventShape(inputListNames=["B+"], path=main)

main.add_module("MVAExpert", listNames=["B+"], extraInfoName="ContinuumProbability", identifier=f"{weightfile}")  # name of the weightfile used

############################### Best candidate selection Start#########################
vm.addAlias('BCS3', 'formula(((Mbc - 5.27933)/0.0026) ** 2  + ((daughter(0,InvM) - 1.86483)/0.0040276) ** 2)')
ma.rankByLowest(particleList='B+', variable='BCS3', outputVariable='BCS3_rank', path=main)
vm.addAlias('BCS3_rank', 'extraInfo(BCS3_rank)')
ma.applyCuts('B+','BCS3_rank==1',path=main)
############################### Best candidate selection Stop#########################


# Create list of variables to save into the output file
simpleCSVariables = [
    "sphericity",
    "DeltaZ",  #must    #significance
    "R2",  #must    #significance
    "thrustBm",  #must    #significance
    "thrustOm",  #must    #significance
    "cosTBTO",  #must    #significance
    "cosTBz",  #must    #significance
    "KSFWVariables(et)",    #significance
    # "KSFWVariables(mm2)",
    # "KSFWVariables(hso00)",
    # "KSFWVariables(hso01)",
    "KSFWVariables(hso02)",  #must    #significance
    # "KSFWVariables(hso03)",
    "KSFWVariables(hso04)",    #significance
    "KSFWVariables(hso10)",    #significance
    "KSFWVariables(hso12)",  #must    #significance
    "KSFWVariables(hso14)",    #significance
    "KSFWVariables(hso20)",    #significance
    # "KSFWVariables(hso22)",
    # "KSFWVariables(hso24)",
    "KSFWVariables(hoo0)",  #must    #significance
    # "KSFWVariables(hoo1)",
    "KSFWVariables(hoo2)",    #significance
    # "KSFWVariables(hoo3)",
    "KSFWVariables(hoo4)",    #significance
    # "CleoConeCS(1)",
    # "CleoConeCS(2)",
    # "CleoConeCS(3)",
    # "CleoConeCS(4)",
    # "CleoConeCS(5)",
    # "CleoConeCS(6)",
    # "CleoConeCS(7)",
    # "CleoConeCS(8)",
    # "CleoConeCS(9)",
]
vm.addAlias("D0M_BF", "extraInfo(M_before_fit)")
vm.addAlias("ContProb", "extraInfo(ContinuumProbability)")
vm.addAlias('PID_bin_kaon', 'ifNANgiveX(pidPairProbabilityExpert(321, 211, ALL), 0.5)')
vm.addAlias('chiSqrd', 'extraInfo(chiSquared)')
vm.addAlias('fitNdf', 'extraInfo(ndf)')


b_vars = []
other_var = ['p', 'E', 'isSignal', 'M', 'InvM', 'Mbc', 'deltaE', 'chiProb']
b_vars += other_var
b_vars += simpleCSVariables
track_variables = ['PID_bin_kaon', 'pionID', 'kaonID']
b_vars += vu.create_aliases_for_selected(
    track_variables,
    "B+ -> [anti-D0 -> ^K+ pi-] pi+ pi- pi+",
    prefix=["Kp"],
)

# D_var = vc.inv_mass + vc.deltae_mbc + vc.mc_truth + vc.vertex + vc.mc_vertex + ['chiSqrd', 'fitNdf']
D_var = ['M', 'D0M_BF', 'InvM', 'Mbc', 'deltaE', 'isSignal', 'dr', 'dz',
        'mcDecayVertexX', 'mcDecayVertexY', 'mcDecayVertexZ', 'x', 'y', 'z',
        'chiProb', 'chiSqrd', 'fitNdf']
b_vars += vu.create_aliases_for_selected(
    D_var,
    "B+ -> [^anti-D0 -> K+ pi-] pi+ pi- pi+",
    prefix=["D0_bar"],
)


ma.variablesToNtuple(
    decayString="B+",
    variables=b_vars +["ContProb", "isContinuumEvent"],
    filename=f"{output_foldername}/{output_filename}",
    treename="tree",
    path=main,
)

b2.process(main)

# print out the summary
print(b2.statistics)