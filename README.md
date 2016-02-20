svt-qa
======

Quality assurance and analysis framework for the Heavy Photon Search Silicon Vertex Tracker



Instructions for loading calibration conditions to the conditions database 

Hey Team HPS SVT DAQ! 

All of my conditions stuff can be accessed from clonpc11 and is in the directory:

/home/omoreno/conditions

Once you generate a baseline file (not a threshold file!), you can load them into the conditions data base using the command: 

java -cp <hps-java-jar> org.hps.conditions.svt.SvtConditionsLoader -r <run_number> -p <conditions_property_file> -c <calibrations.xml>

This will load the conditions and create a conditions record starting at run <run_number>.  
