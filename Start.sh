# Description: 
#  
#  It merges all Data map of that day. 

# Directory name
name="Aug3"
dir="${name}#" 
# Prefix 
pfx="Times" 
# Location of dir:s
currantLocal="./AugAllRap/"
# How many directories (and files) are there with $dir
cd ${currantLocal} 
range=$(find . -type d -name "*${dir}*" | wc -l) 

# Rename all file so that don´t match each other.
for (( i=1; i<$[$range+1]; i++)); do
    for file in $dir$i/*.dat; do
	source=$file
	file=$(basename $file)
	basename=${file:0:15}
	dat=${file##*.}
	mv "$source" "$dir$i/$basename$i.$dat"
    done
done 
cd ..

echo "The script starts now. $range"

cd ${currantLocal} 
if [ "$( ls -lR | grep ^d | wc -l )" -gt 2 ]; then 
    cd ..
    # Merges Maps containing data-files
    for (( i=1; i<$[$range+1]; i++)) 
    do 
	nrMap="$dir$i"
	cd ${currantLocal}
	echo "Renaming files"
	##################################################
	for file in $nrMap/*.dat; do
	    source=$file
	    dest_dir=$dir$[$i+1]

	    file=$(basename $file)
	    # basename=${file%.*}
	    basename=${file:0:15}
	    dat=${file##*.}

	    if [[ ! -e "$dest_dir/$basename.$dat" ]]; then
		# file does not exist in the destination directory
		[ -e $dest_dir ] && mv "$source" "$dest_dir" 
		echo "Den filen finns inte: $dest_dir/$basename.$dat"
	    else
		num=0
		echo "1 New file Name: $dest_dir/$basename$num.$dat"
		while [[ -e "$dest_dir/$basename$num.$dat" ]]; do
		    (( num++ ))
		done
		[ -e $dest_dir ] && mv "$source" "$dest_dir/$basename$num.$dat"
		echo "2 New file Name: $dest_dir/$basename$num.$dat" 
	    fi 
	done
	echo "Moved"
	##############################################
	cd .. 

	sleep 1 
    done
    cd ${currantLocal}  
else 
    echo "Already done  $( ls -lR | grep ^d | wc -l )"
fi


# Remove all directories that are empty. 
find . -type d -empty -delete

if [ -e Normalized$name ]; 
then 
    echo "Normalization is done"
    cd ..
else 
    # Copy the unnormalized data-files to a other Map (One map should exist by this point) 
    mergedMap="$(ls -dp *$dir*)"
    cp -r $mergedMap Normalized$name/
    echo ${currantLocal}$mergedMap 
    cd .. 
    ./NormalizationNew ${currantLocal}Normalized$name/
    wait
fi

################# # Renaming by removing jobNr #####################
for file in ${currantLocal}Normalized$name/*.dat; do
    source=$file
    dest_dir=${currantLocal}Normalized$name

    file=$(basename $file)
    basename=${file:0:15}
    dat=${file##*.}
    mv "$source" "$dest_dir/$basename.$dat"
done
##############################################

./PlotGnuplotNew ${currantLocal}Normalized$name/ 3 X
wait
cd ${currantLocal}Normalized$name/
gnuplot> load 'GC.txt'
wait 

cd ..
cd ..

./PlotGnuplotNew ${currantLocal}Normalized$name/ 2 X
wait
cd ${currantLocal}Normalized$name/
gnuplot> load 'GC.txt'
wait 
cd .. 
cd ..

