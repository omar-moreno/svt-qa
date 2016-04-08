svt-qa
======

Quality assurance and analysis framework for the Heavy Photon Search Silicon Vertex Tracker

### Calculating Thresholds

After taking a baseline run, the following command can be executed in a terminal to generate a file containing the thresholds

    svt_qa -i <EVIO file> -e -t <threshold level>

The '-e' flag is indicating that the input file format is EVIO.  The threshold level is in units of RMS noise.  Typically, this will be a value between 1 and 4. 


