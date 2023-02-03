#!/usr/bin/env python3
import sys
import basf2 as b2
import modularAnalysis as ma
# import stdV0s
import flavorTagger as ft
from variables import variables as vm  # shorthand for VariableManager
import variables.collections as vc
import variables.utils as vu
import vertex as vx

# get input file number from the command line
file_name = sys.argv[1]
file_number = file_name.split("sub00/")
input_filename = file_number[1]

# get output folder name from the command line
output_foldername = sys.argv[2]
output_filename = input_filename

# weight file
weightfile = sys.argv[3]
#################################################################################

# Perform analysis.
main = b2.create_path()

ma.inputMdstList(environmentType="default",filelist=f"{file_name}",path=main)


# fill final state particle lists
ma.fillParticleList("pi+", "abs(d0)<0.2 and abs(z0)<1 and cosTheta >= -0.6", path=main)
ma.fillParticleList("K+", "abs(d0)<0.2 and abs(z0)<1 and cosTheta >= -0.6", path=main)


ma.reconstructDecay("anti-D0 -> K+ pi-",cut="1.84 < M < 1.89",path=main)
ma.matchMCTruth("anti-D0", path=main)
vx.kFit("anti-D0", conf_level=0.0, fit_type='massvertex', path=main)

ma.reconstructDecay("B+ -> [anti-D0 -> K+ pi-] pi+ pi- pi+",cut="Mbc > 5.27 and abs(deltaE) < 0.15",path=main)
ma.matchMCTruth("B+", path=main)


ma.buildRestOfEvent(target_list_name="B+", path=main)
cleanMask = ("cleanMask", "nCDCHits > 0 and useCMSFrame(p)<=3.2", "p >= 0.05 and useCMSFrame(p)<=3.2")
ma.appendROEMasks(list_name="B+", mask_tuples=[cleanMask], path=main)
ma.buildContinuumSuppression(list_name="B+", roe_mask="cleanMask", path=main) 
vx.kFit("B+", conf_level=0.0, fit_type='vertex', path=main)
vx.TagV("B+", 'breco', path=main)
ft.flavorTagger(["B+"], path=main)
ma.buildEventShape(inputListNames=["B+"], path=main)


main.add_module(
    "MVAExpert",
    listNames=["B+"],
    extraInfoName="ContinuumProbability",
    identifier=f"{weightfile}",  # name of the weightfile used
)
simpleCSVariables = [
    "DeltaZ",
    "R2",
    "thrustBm",
    "thrustOm",
    "cosTBTO",
    "cosTBz",
    # "KSFWVariables(et)",
    # "KSFWVariables(mm2)",
    # "KSFWVariables(hso00)",
    "KSFWVariables(hso01)", # has some issue
    "KSFWVariables(hso02)",
    # "KSFWVariables(hso03)", # has some issue
    # "KSFWVariables(hso04)",
    "KSFWVariables(hso10)",
    "KSFWVariables(hso12)",
    # "KSFWVariables(hso14)",
    "KSFWVariables(hso20)",
    # "KSFWVariables(hso22)",
    # "KSFWVariables(hso24)",
    "KSFWVariables(hoo0)",
    # "KSFWVariables(hoo1)",
    # "KSFWVariables(hoo2)",
    # "KSFWVariables(hoo3)",
    # "KSFWVariables(hoo4)",
    "CleoConeCS(1)",
    # "CleoConeCS(2)",
    # "CleoConeCS(3)",
    # "CleoConeCS(4)",
    # "CleoConeCS(5)",
    # "CleoConeCS(6)",
    # "CleoConeCS(7)",
    # "CleoConeCS(8)",
    # "CleoConeCS(9)",
]
vm.addAlias("ContProb", "extraInfo(ContinuumProbability)")

ma.variablesToNtuple(
    decayString="B+",
    variables=["isSignal", "ContProb", "isContinuumEvent"] + simpleCSVariables,
    filename=f"{output_foldername}/{output_filename}",
    treename="tree",
    path=main,
)

b2.process(main)