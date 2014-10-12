
import MySQLdb
import argparse
import sys

#------------#
#--- Main ---#
#------------#

# Parse the command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-u", "--user", help="Database user name")
parser.add_argument("-p", "--password", help="Password")
parser.add_argument("-c", "--host", help="Hostname")
args = parser.parse_args()

# If a hostname is not specified, notify the user and exit
if not args.host: 
	print "\nPlease specify the host name used to access the database."
	print "Use the -h flag for usage.\n"
	sys.exit(2)

# If a user name is not specified, notify the user and exit
if not args.user:
	print "\nPlease specify the user name used to access the database."
	print "Use the -h flag for usage.\n"
	sys.exit(2)

# If a password is not specified, notify the user and exit
if not args.password: 
	print "\nPlease specify the password used to access the database."
	print "Use the -h flag for usage.\n"
	sys.exit(2)

# Set some "constants"
TOTAL_SENSORS = 36
TOTAL_CHANNELS_PER_HYBRID = 640
TOTAL_SVT_CHANNEL_IDS = TOTAL_CHANNELS_PER_HYBRID*TOTAL_SENSORS
TOTAL_FEB = 10
MAX_HYBRIDS_PER_FEB = 4

# Connect to the database
print "Connecting to the database hps_conditions_dev ..."
database = MySQLdb.connect(host=args.host,
						   user=args.user, 
						   passwd=args.password,
						   db="hps_conditions_dev")
print "Connection to the database hps_conditions_dev established"

# Get a cursor 
cursor = database.cursor()

#--------------------------------#
#--- Load default SVT DAQ Map ---#
#--------------------------------#
daq_list = [ 
			(1,  0, 0, 0, 0,  'T', 1,  'A'),
			(2,  0, 0, 1, 1,  'T', 2,  'S'),
			(3,  0, 1, 0, 2,  'T', 3,  'A'),
			(4,  0, 1, 1, 3,  'T', 4,  'S'),
			(5,  0, 1, 2, 4,  'T', 5,  'A'),
			(6,  0, 1, 3, 5,  'T', 6,  'S'),
			(7,  0, 2, 0, 6,  'T', 7,  'A'),
			(8,  0, 2, 1, 7,  'T', 7,  'A'),
			(9,  0, 2, 2, 8,  'T', 8,  'S'),
			(10, 0, 2, 3, 9,  'T', 8,  'S'),
			(11, 0, 3, 0, 10, 'T', 9,  'A'),
			(12, 0, 3, 1, 11, 'T', 9,  'A'),
			(13, 0, 3, 2, 12, 'T', 10, 'S'),
			(14, 0, 3, 3, 13, 'T', 10, 'S'),
			(15, 0, 4, 0, 14, 'T', 11, 'A'),
			(16, 0, 4, 1, 15, 'T', 11, 'A'),
			(17, 0, 4, 2, 16, 'T', 12, 'S'),
			(18, 0, 4, 3, 17, 'T', 12, 'S'),
			(19, 0, 5, 0, 20, 'B', 1,  'S'),
			(20, 0, 5, 1, 21, 'B', 2,  'A'),
			(21, 0, 6, 1, 22, 'B', 3,  'S'),
			(22, 0, 6, 2, 23, 'B', 4,  'A'),
			(23, 0, 6, 3, 24, 'B', 5,  'S'),
			(24, 0, 6, 4, 25, 'B', 6,  'A'),
			(25, 0, 7, 0, 26, 'B', 7,  'S'),
			(26, 0, 7, 1, 27, 'B', 7,  'S'),
			(27, 0, 7, 2, 28, 'B', 8,  'A'),
			(28, 0, 7, 3, 29, 'B', 8,  'A'),
			(29, 0, 8, 0, 30, 'B', 9,  'S'),
			(30, 0, 8, 1, 31, 'B', 9,  'S'),
			(31, 0, 8, 2, 32, 'B', 10, 'A'),
			(32, 0, 8, 3, 33, 'B', 10, 'A'),
			(33, 0, 9, 0, 34, 'B', 11, 'S'),
			(34, 0, 9, 1, 35, 'B', 11, 'S'),
			(35, 0, 9, 2, 36, 'B', 12, 'A'),
			(36, 0, 9, 3, 37, 'B', 12, 'A')
			]


# If the first collection (collection_id = 0) exist, delete it and 
# load new defaults
sql_query = "SELECT * FROM svt_daq_map WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM svt_daq_map WHERE collection_id = '%i'" % (0)
		print "Deleting current SVT DAQ map defaults ..."
		cursor.execute(sql_query)
		database.commit()
	
	print "Loading default DAQ map ..."
	for daq_item in daq_list: 
		sql_query = "INSERT INTO svt_daq_map(id,\
					collection_id,\
					feb_id, feb_hybrid_id,\
					hybrid_id,\
					svt_half,\
					layer,\
					orientation)\
					VALUES('%i', '%i', '%i', '%i', '%i', '%s', '%i', '%s')" % daq_item 
		cursor.execute(sql_query)
	database.commit()
except: 
	print "[ERROR]: Unable to load SVT DAQ map."
	database.rollback()


#------------------------------------#
#--- Load the default channel map ---#
#------------------------------------#

# If the first collection (collection_id = 0) exist, delete it and 
# load new defaults
sql_query = "SELECT * FROM svt_channels WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM svt_channels WHERE collection_id = '%i'" % (0)
		print "Deleting current default SVT channel map ..."
		cursor.execute(sql_query)
		database.commit()
	
	print "Loading default SVT channel map ..."
	channel_id = 0
	for feb_id in range(0, TOTAL_FEB):
		for feb_hybrid_id in range(0, MAX_HYBRIDS_PER_FEB):
			if (feb_id == 0 or feb_id == 1 or feb_id == 5 or feb_id == 6) and (feb_hybrid_id == 3): continue
			for channel in range(0, TOTAL_CHANNELS_PER_HYBRID):
				sql_query = "INSERT INTO svt_channels(id, collection_id, channel_id,\
						feb_id,\
						feb_hybrid_id,\
						channel)\
						VALUES('%i', '%i', '%i', '%i', '%i', '%i')" % (channel_id+1, 0, channel_id, feb_id, feb_hybrid_id, channel)
				cursor.execute(sql_query)
				channel_id += 1
	database.commit()
except:
	print "[ERROR]: Unable to load SVT channel ID map."
	database.rollback()

#----------------------------------------------------------------------#
#--- Load default values of pedestal and noise for all SVT channels ---#
#----------------------------------------------------------------------#

# If the first collection (collection_id = 0) exist, delete it and load new 
# defaults
sql_query = "SELECT * FROM svt_calibrations WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM svt_calibrations WHERE collection_id = '%i'" % (0)
		print "Deleting existing default values of pedestal and noise for all all SVT channels ..."
		cursor.execute(sql_query)
		database.commit()
	
	print "Loading default values of pedestal and noise for all SVT channels ..."
	pedestal = 4300.
	noise = 84.69
	for svt_channel_id in range(0, TOTAL_SVT_CHANNEL_IDS):
		sql_query = "INSERT INTO svt_calibrations(id, collection_id, svt_channel_id,\
				pedestal_0, pedestal_1, pedestal_2, pedestal_3, pedestal_4, pedestal_5,\
				noise_0, noise_1, noise_2, noise_3, noise_4, noise_5)\
				VALUES('%i', '%i', '%i', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f')"\
				% (svt_channel_id+1, 0, svt_channel_id,\
				pedestal, pedestal, pedestal, pedestal, pedestal, pedestal,\
				noise, noise, noise, noise, noise, noise)

		cursor.execute(sql_query)
	database.commit()
except:
	print "[ERROR]: Unable to load pedestal and noise values."
	database.rollback()

#-------------------------------------------------------------------#
#--- Load default values of gain and offset for all SVT channels ---#
#-------------------------------------------------------------------#

# If the first collection (collection_id = 0) exist, delete it and load new 
# defaults
sql_query = "SELECT * FROM svt_gains WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM svt_gains WHERE collection_id = '%i'" % (0)
		print "Deleting existing default values of gain and offset for all all SVT channels ..."
		cursor.execute(sql_query)
		database.commit()
	
	print "Loading default values of gain and offset for all SVT channels ..."
	gain = 0.09
	offset = 100
	for svt_channel_id in range(0, TOTAL_SVT_CHANNEL_IDS):
		sql_query = "INSERT INTO svt_gains(id, collection_id, svt_channel_id,\
				gain, offset)\
				VALUES('%i', '%i', '%i', '%f', '%f')"\
				% (svt_channel_id+1, 0, svt_channel_id, gain, offset)
		cursor.execute(sql_query)
	database.commit()
except:
	print "[ERROR]: Unable to load gain and offset values."
	database.rollback()

#------------------------------------------------------------------------#
#--- Load default values of shape fit parameters for all SVT channels ---#
#------------------------------------------------------------------------#

# If the first collection (collection_id = 0) exist, delete it and load new 
# defaults
sql_query = "SELECT * FROM svt_shape_fit_parameters WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM svt_shape_fit_parameters WHERE collection_id = '%i'" % (0)
		print "Deleting existing default shape fit parameters for all all SVT channels ..."
		cursor.execute(sql_query)
		database.commit()
	
	print "Loading default shape fit parameters for all SVT channels ..."
	amplitude = 2500
	t0 = 20
	tp = 35
	for svt_channel_id in range(0, TOTAL_SVT_CHANNEL_IDS):
		sql_query = "INSERT INTO svt_shape_fit_parameters(id, collection_id, svt_channel_id,\
				amplitude, t0, tp)\
				VALUES('%i', '%i', '%i', '%f', '%f', '%f')"\
				% (svt_channel_id+1, 0, svt_channel_id, amplitude, t0, tp)
		cursor.execute(sql_query)
	database.commit()
except:
	print "[ERROR]: Unable to load shape fit parameters."
	database.rollback()

#----------------------------------------#
#--- Load default values of t0 shifts ---#
#----------------------------------------#

# If the first collection (collection_id = 0) exist, delete it and load new 
# defaults
sql_query = "SELECT * FROM svt_t0_shifts WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM svt_t0_shifts WHERE collection_id = '%i'" % (0)
		print "Deleting existing default t0 shift values ..."
		cursor.execute(sql_query)
		database.commit()
	
	print "Loading default t0 shift values ..."
	key = 1;
	t0_shift = 0;
	for feb_id in range(0, TOTAL_FEB):
		for feb_hybrid_id in range(0, MAX_HYBRIDS_PER_FEB):
			if (feb_id == 0 or feb_id == 1 or feb_id == 5 or feb_id == 6) and (feb_hybrid_id == 3): continue
			sql_query = "INSERT INTO svt_t0_shifts(id, collection_id,\
					feb_id, feb_hybrid_id, t0_shift)\
					VALUES('%i', '%i', '%i', '%i', '%f')"\
					% (key, 0, feb_id, feb_hybrid_id, t0_shift)
			key += 1
			cursor.execute(sql_query)
	database.commit()
except:
	print "[ERROR]: Unable to load t0 values."
	database.rollback()

database.close()



