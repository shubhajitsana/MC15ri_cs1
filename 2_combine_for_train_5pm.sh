#!/bin/bash
# takes command as: ./2_combine_for_train.sh 480 800 255 243 800
# This file combines files for training(mailny used for scaling)
# WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
# SO WE DON'T NEED TO RUN "FOR" LOOP TO ADD PATH NAME
# But we need "for" loop to combine root files upto certain number
path=$(pwd)

# Create Output Directory to save combined root files
output_path="${path}/cs/train"
if [ -d "$output_path" ]
then
    echo "$output_path already exists."
else 
    $(mkdir -p ${output_path})
    echo "$output_path has been created"
fi

let store_upto_charged=${1:-480}
let store_upto_uubar=${2:-800}
let store_upto_ddbar=${3:-255}
let store_upto_ssbar=${4:-243}
let store_upto_ccbar=${5:-800}
let store_upto=0


##the root file number in one job
let nRootFile=300

#combining for different folder
declare -a options=("charged" "uubar" "ddbar" "ssbar" "ccbar") #"mixed" 

for opt in "${options[@]}"
do
    input_path="$path/data/$opt"
    input_files=$(ls ${input_path}/*.root)
    input_file_count=$(ls ${input_path}/*.root | wc -l)
    echo "Number of input files in $input_path is $input_file_count"

    #Storing filename in array from long single string
    declare -a input_file_array=() #If we don't write "()" here array won't be empty in every step
    let count=0
    for l in ${input_files}
    do
        input_file_array[$count]="$l"
        ((count++))
    done
    length_input_file_array=${#input_file_array[@]}

    #checking whether array dimension is ok
    if [ $input_file_count -eq $count ]
    then
        if [ $input_file_count -eq $length_input_file_array ]
        then
            echo "Input file count and input file array dimension are same."
        else
            echo "There is some discrepancy between Input file count and input file array dimension"
            echo "Because Input file count is $input_file_count but array dimension is $length_input_file_array"
        fi
    fi

    # Finalizing the number of input files as per given parameter
    if [[ $opt == "charged" ]]
    then
        store_upto=$store_upto_charged
    elif [[ $opt == "uubar" ]]
    then
        store_upto=$store_upto_uubar
    elif [[ $opt == "ddbar" ]]
    then
        store_upto=$store_upto_ddbar
    elif [[ $opt == "ssbar" ]]
    then
        store_upto=$store_upto_ssbar
    elif [[ $opt == "ccbar" ]]
    then
        store_upto=$store_upto_ccbar
    fi

    # Storing file Upto given number
    echo "Combining $store_upto files from $input_path"
    path_in_string=""  # Empty string
    for((i=0; i<$store_upto; i++))
    do
        # path_in_string+=" $input_path/${input_file_array[i]}"
        # WHEN WE PUT "*" IN "ls" COMMAND, IT RETURNS FULL PATHNAME ALSO.
        path_in_string+=" ${input_file_array[i]}"
    done

    # Combining files
    output_file="$output_path/train_$opt.root"
    if [ -f "$output_file" ]
    then
        echo "$output_file already exists."
        $(rm -f ${output_file})
        echo "So $output_file has been deleted. Plese run this bash file again."
    else 
        echo "Please wait for combination of $nRootFile input file from $input_path folder....."

        ###########################start combining##################

        if [ $store_upto -lt $nRootFile ]
        then
            echo "In $input_path folder the given number($store_upto) of file to combine is less than $nRootFile."
            echo "So hadd is being applying just once."
            echo "$(hadd ${output_file} ${path_in_string})"
        else
            combine=""	##Empty array to store files
            intermediate_files=""   # To store the name of intermediate combinded files
            let nRootFilesFlag=0    # To take counts and combine when it takes value n*$nRootFile
            let newFileNo=0;
            #moving 1-300 file and combine
            for l in ${path_in_string}
            do 
                combine+="$l "
                ((nRootFilesFlag++))

                if [ $nRootFilesFlag -eq $nRootFile ]
                then
                    ((newFileNo++))
                    temp="${output_path}/train_${opt}_${newFileNo}.root"
                    echo "$(hadd ${temp} ${combine})"
                    intermediate_files+="$temp "
                    nRootFilesFlag=0
                    combine=""
                fi
            done

            ##Leftover root file
            if [ -z "$combine" ]        # Checking whether the string is empty
            then
                    echo "No root file is leftover after running for loop $newFileNo*$nRootFile times"
            else
                ((newFileNo++))
                echo "Please wait for combination of last bunch of input file from $input_path folder....."
                temp="${output_path}/train_${opt}_${newFileNo}.root"
                echo "$(hadd ${temp} ${combine})"
                intermediate_files+="$temp "
            fi


            # Combining all files
            echo "$(hadd ${output_file} ${intermediate_files})" 
                                            # the 2nd arfument of "hadd" should be "STRING" only
            echo "Deleting $intermediate_files ......"
            $(rm -rf ${intermediate_files})
        fi
    fi
done