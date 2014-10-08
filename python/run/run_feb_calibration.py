
#--- Imports ---#
#---------------#
import pythonDaq
import time
import os
import sys
import getopt

#---------------#

# Set some default values
run_number = 0
feb_id = -1

#----------------------------------------#
#--- Parse the command line arguments ---#
#----------------------------------------#
options, remainder = getopt.getopt(sys.argv[1:], 'r:f:', 
								   ['run_number=',
                                    'feb_id=',
									'help'
								   ])

for opt, arg in options:
	if opt in ('-r', '--run_number'):
		run_number = int(arg)
	elif opt in ('-f', '--feb_id'):
		feb_id = int(arg)
	elif opt in ('--help'):
		print "\nUsage: python run_gain.py [OPTIONS]"
		print "OPTIONS:"
		print "\t-r, --run_number: Run number"
		print "\t-f, --feb_id:     Front End Board ID"
		print "\t    --help:       Display this help\n"
	  	sys.exit(0)

# If a FEB ID hasn't been specified, exit the program
if feb_id < 0:
    print "\n\tPlease specify the FEB ID of the device under test."
    print "\tUse --help option to see the usage.\n"
    sys.exit(0)

# Set the path to the DAQ
daq_home = os.environ['DAQ_HOME']
daq_data_home = os.environ['DAQ_DATA_HOME']

# Open the connection to the DAQ
pythonDaq.daqOpen("frontEndTest", 1)

#------------------------#
#-- Configure the DAQ ---#
#------------------------#

# Do a hard reset to remove any previous configuration that may have been
# present
pythonDaq.daqHardReset()

# Load the default DAQ configuration
config_file = daq_home + '/config/FrontEndBoardConfigC01.xml'
pythonDaq.daqLoadSettings(config_file)

# Set the number of events to take and the rate
pythonDaq.daqSetRunParameters("100Hz", 10000)

# Do a soft reset to sync all APVs
pythonDaq.daqSoftReset()

# Set the output file name
data_path = daq_data_home + "/feb_" +
dut = "feb" 

if feb_id < 10: 
	data_path += "0"
    dut += "0"
data_path += str(feb_id) + "/"
dut += str(feb_id) + "_run"

if run_number < 10: 
	data_path += "0"
    dut += "0"

data_path += str(run_number) + "/"
data_path = data_path + "/data"
os.system('mkdir -p ' + data_path)
dut += str(run_number)

#time = datetime.datatime.now().strftime("%y%m%d_%H:%M:%S)

print "Saving files to " + data_path

# Take both a baseline and calibration run
for iter in range(0, 9):

    # Close any existing open files
	pythonDaq.daqCloseData()

    file_name = data_path + "/" + dut
	
    # Configure the DAQ for the type of run that will be taken
	if ( iter == 8 ):
		pythonDaq.daqSetConfig("FrontEndTestFpga:FebCore:CalEn","False")
		pythonDaq.daqSetConfig("FrontEndTestFpga:FebCore:Hybrid:apv25:CalibInhibit","True")
		pythonDaq.daqSetConfig("FrontEndTestFpga:FebCore:Hybrid:apv25:CalGroup","0")
		file_name += "_baseline.bin"
		print "Running baseline" 
	else:
		pythonDaq.daqSetConfig("FrontEndTestFpga:FebCore:CalEn","True")
        # TODO: Set CalDelay
		#pythonDaq.daqSetConfig("FrontEndTestFpga:FebCore:CalDelay","")
		pythonDaq.daqSetConfig("FrontEndTestFpga:FebCore:Hybrid:apv25:CalibInhibit","False")
		pythonDaq.daqSetConfig("FrontEndTestFpga:FebCore:Hybrid:apv25:CalGroup", str(iter))
		file_name += '_cal' + str(iter) + ".bin"
		print("Running calibration group: " + str(iter))
	
    # Set the data file name and remove any old files that go by the same
	# name within the output directory
	os.system('rm -f ' + file_name)
	pythonDaq.daqSetConfig('DataFile',file_name)
	
    # Open the data file
	pythonDaq.daqSendCommand('OpenDataFile','')
	
    # Run the DAQ
	pythonDaq.daqSetRunState('Running')
	print "Running ..."
	while pythonDaq.daqGetRunState() == "Running":
        time.sleep(5)
		sys.stdout.write("\r%s" %  progress)
        sys.stdout.flush()
        progress += "."

	# Close any existing open files
	pythonDaq.daqCloseData()

print "Calibration run complete."
