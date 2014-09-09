
import MySQLdb
import argparse
import sys

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

# Connect to the database
print "Connecting to the database hps_conditions_dev ..."
database = MySQLdb.connect(host=args.host,
						   user=args.user, 
						   passwd=args.password,
						   db="hps_conditions_dev")
print "Connection to the database hps_conditions_dev established"

# Get a cursor 
cursor = database.cursor()

#--- Load default SVT DAQ Map ---#
#--------------------------------#
daq_list = [ 
			(1,  0, 0, 0, 0,  'T', 1,  'S'),
			(2,  0, 0, 1, 1,  'T', 2,  'A'),
			(3,  0, 0, 2, 2,  'T', 3,  'S'),
			(4,  0, 1, 0, 3,  'T', 4,  'A'),
			(5,  0, 1, 1, 4,  'T', 5,  'S'),
			(6,  0, 1, 2, 5,  'T', 6,  'A'),
			(7,  0, 2, 0, 6,  'T', 7,  'S'),
			(8,  0, 2, 2, 7,  'T', 7,  'S'),
			(9,  0, 2, 1, 8,  'T', 8,  'A'),
			(10, 0, 2, 3, 9,  'T', 8,  'A'),
			(11, 0, 3, 0, 10, 'T', 9,  'S'),
			(12, 0, 3, 2, 11, 'T', 9,  'S'),
			(13, 0, 3, 1, 12, 'T', 10, 'A'),
			(14, 0, 3, 3, 13, 'T', 10, 'A'),
			(15, 0, 4, 0, 14, 'T', 11, 'S'),
			(16, 0, 4, 2, 15, 'T', 11, 'S'),
			(17, 0, 4, 1, 16, 'T', 12, 'A'),
			(18, 0, 4, 3, 17, 'T', 12, 'A'),
			(19, 0, 5, 0, 20, 'B', 1,  'A'),
			(20, 0, 5, 1, 21, 'B', 2,  'S'),
			(21, 0, 5, 2, 22, 'B', 3,  'A'),
			(22, 0, 6, 0, 23, 'B', 4,  'S'),
			(23, 0, 6, 1, 24, 'B', 5,  'A'),
			(24, 0, 6, 2, 25, 'B', 6,  'S'),
			(25, 0, 7, 0, 26, 'B', 7,  'A'),
			(26, 0, 7, 2, 27, 'B', 7,  'A'),
			(27, 0, 7, 1, 28, 'B', 8,  'S'),
			(28, 0, 7, 3, 29, 'B', 8,  'S'),
			(29, 0, 8, 0, 30, 'B', 9,  'A'),
			(30, 0, 8, 2, 31, 'B', 9,  'A'),
			(31, 0, 8, 1, 32, 'B', 10, 'S'),
			(32, 0, 8, 3, 33, 'B', 10, 'S'),
			(33, 0, 9, 0, 34, 'B', 11, 'A'),
			(34, 0, 9, 2, 35, 'B', 11, 'A'),
			(35, 0, 9, 1, 36, 'B', 12, 'S'),
			(36, 0, 9, 3, 37, 'B', 12, 'S')
			]


# If the first collection (collection_id = 0) exist, delete it and 
# load new defaults
sql_query = "SELECT * FROM eng_run_svt_daq_map WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM eng_run_svt_daq_map WHERE collection_id = '%i'" % (0)
		print "Deleting current SVT DAQ map defaults ..."
		cursor.execute(sql_query)
	
	print "Loading default DAQ map ..."
	for daq_item in daq_list: 
		sql_query = "INSERT INTO eng_run_svt_daq_map(id,\
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
	database.rollback()

#--------------------------------#

#--- Load the default channel map ---#
#------------------------------------#

# If the first collection (collection_id = 0) exist, delete it and 
# load new defaults
sql_query = "SELECT * FROM eng_run_svt_channels WHERE collection_id = '%i'" % (0) 

try:
	cursor.execute(sql_query)
	results = cursor.fetchall()

	if len(results) != 0:
		sql_query = "DELETE FROM eng_run_svt_channels WHERE collection_id = '%i'" % (0)
		print "Deleting current default SVT channel map ..."
		cursor.execute(sql_query)
	
	print "Loading default SVT channel map ..."
	channel_id = 0
	for feb_id in range(0, 10):
		for feb_hybrid_id in range(0, 4):
			if (feb_id == 0 or feb_id == 1 or feb_id == 5 or feb_id == 6) and (feb_hybrid_id == 3): continue
			for channel in range(0, 640):
				channel_id += 1
				#print "Channel ID: " + str(channel_id) + " FEB ID: " + str(feb_id) + " FEB Hybrid ID: " + str(feb_hybrid_id) + " Channel: " + str(channel)
				sql_query = "INSERT INTO eng_run_svt_channels(id, collection_id,\
						feb_id,\
						feb_hybrid_id,\
						channel)\
						VALUES('%i', '%i', '%i', '%i', '%i')" % (channel_id, 0, feb_id, feb_hybrid_id, channel)
						
				cursor.execute(sql_query)
				database.commit()
except: 
	database.rollback()

database.close()



