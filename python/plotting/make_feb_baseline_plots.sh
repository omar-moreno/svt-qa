#! /bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#FEB: 9 Hybrid: 0 - Mean Baseline
#FEB: 9 Hybrid: 0 - Noise
echo "input file $1"
if [ -z "$2" ]; then
	python $DIR/make_feb_plots.py -i $1 --name "baseline_plots" -r ".*FEB\:\s\d.*Baseline.*"
	python $DIR/make_feb_plots.py -i $1 --name "noise_plots" -r ".*FEB\:\s\d.*Noise.*"
else
    if ! [ -z "$3" ]; then
	if [ -z "$4" ]; then	    
	    python $DIR/make_feb_plots.py -i $1 --name "baseline_plots_$3" -r ".*FEB\:\s\d.*Baseline.*"
	    python $DIR/make_feb_plots.py -i $1 --name "noise_plots_$3" -r ".*FEB\:\s\d.*Noise.*"
	else
	    python $DIR/make_feb_plots.py -i $1 --name "baseline_plots_$3_ref_$4" -r ".*FEB\:\s\d.*Baseline.*" -c $2 -l "$3 $4"
	    python $DIR/make_feb_plots.py -i $1 --name "noise_plots_$3_ref_$4" -r ".*FEB\:\s\d.*Noise.*" --c $2 -l "$3 $4"
	fi
    fi	
fi
