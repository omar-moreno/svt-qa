#! /bin/bash
#FEB: 9 Hybrid: 0 - Mean Baseline
#FEB: 9 Hybrid: 0 - Noise
echo "input file $1"
if [ -z "$2" ]; then
    python make_feb_plots.py -i $1 --name "baseline_plots" -r ".*FEB\:\s9.*Baseline.*"
    python make_feb_plots.py -i $1 --name "noise_plots" -r ".*FEB\:\s9.*Noise.*"
else
    echo "Reference file $2"
    python make_feb_plots.py -i $1 --name "baseline_plots" -r ".*FEB\:\s9.*Baseline.*" -c $2
    python make_feb_plots.py -i $1 --name "noise_plots" -r ".*FEB\:\s9.*Noise.*" --c $2 
fi
