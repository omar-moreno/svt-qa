
#!/usr/bin/env python

import argparse
import MySQLdb

def main(): 

    # Parse all command line arguments
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-c', action='store', dest='channel_col_id', 
                        help='Channel collection ID.')
    parser.add_argument('-t', action='store', dest='t0_col_id', 
                        help='t0 shifts collection ID.')
    parser.add_argument('-b', action='store', dest='cal_col_id', 
                        help='Calibrations IDs')
    parser.add_argument('-f', action='store', dest='fit_col_id', 
                        help='Shape fit IDs')
    parser.add_argument('-g', action='store', dest='gain_col_id', 
                        help='gain IDs')
    args = parser.parse_args()

    host = 'hpsdb.jlab.org'
    user = 'hpscalibrations'
    pw   = 'W42R6xqP'

    # Connect to the database
    print 'Connecting to the database via host %s' % host
    db = MySQLdb.connect(host=host, user=user, 
                         passwd=pw, db='hps_conditions')

    # Check that a connection to the database has been established
    print 'Connection to the database hps_conditions has been established'

    # Get a cursor
    cursor = db.cursor()

    # Set some "constants"
    TOTAL_SENSORS = 40
    TOTAL_CHANNELS_PER_NOM_HYBRID = 640
    TOTAL_CHANNELS_PER_L0_HYBRID = 512
    #TOTAL_SVT_CHANNEL_IDS = TOTAL_CHANNELS_PER_HYBRID*TOTAL_SENSORS
    TOTAL_FEB = 10
    MAX_HYBRIDS_PER_FEB = 4


    if args.channel_col_id: 
        # Check if the collection id exist.  If so, warn the user and exit.
        col_id = int(args.channel_col_id.strip()) 
        sql_query = "SELECT * FROM svt_channels WHERE collection_id = '%i'" % col_id 
    
        try: 
            cursor.execute(sql_query)
            results = cursor.fetchall()
            
            if len(results) != 0: 
                print 'Collection exists!'
                return 
        
            print 'Loading SVT channel map ...'
            channel_id = 0
            for feb_id in range(0, TOTAL_FEB):
                total_channels = TOTAL_CHANNELS_PER_NOM_HYBRID
                for feb_hybrid_id in range(0, MAX_HYBRIDS_PER_FEB):
                    if (feb_id == 2 or feb_id == 9): total_channels = TOTAL_CHANNELS_PER_L0_HYBRID
                    for channel in range(0, total_channels):
                        sql_query = "INSERT INTO svt_channels(collection_id, channel_id, feb_id, feb_hybrid_id, channel) VALUES('%i', '%i', '%i', '%i', '%i')" % (col_id, channel_id, feb_id, feb_hybrid_id, channel)
                        #print sql_query
                        print cursor.execute(sql_query)
                        channel_id += 1
                     
            db.commit()
            print 'Total channel IDs added to DB: %s' % channel_id
            
        except:   
            print "[ERROR]: Unable to load SVT channel ID map."
            db.rollback()

    if args.t0_col_id: 
        
        # Check if the collection id exist.  If so, warn the user and exit.
        col_id = int(args.t0_col_id.strip()) 
        sql_query = "SELECT * FROM svt_t0_shifts WHERE collection_id = '%i'" % col_id
 
        try: 
            cursor.execute(sql_query)
            results = cursor.fetchall()
            
            if len(results) != 0: 
                print 'Collection exists!'
                return 

            print "Loading default t0 shift values ..."
            key = 1;
            t0_shift = 0;
            for feb_id in range(0, TOTAL_FEB):
                for feb_hybrid_id in range(0, MAX_HYBRIDS_PER_FEB):
                    sql_query = "INSERT INTO svt_t0_shifts(collection_id,\
                            feb_id, feb_hybrid_id, t0_shift)\
                            VALUES('%i', '%i', '%i', '%f')"\
                            % (col_id, feb_id, feb_hybrid_id, t0_shift)
                    cursor.execute(sql_query)
                    key += 1
                     
            db.commit()
            print 'Total shifts added DB: %s' % key
        
        except:   
            print "[ERROR]: Unable to load shifts"
            db.rollback()

    if args.cal_col_id: 
        # Check if the collection id exist.  If so, warn the user and exit.
        col_id = int(args.cal_col_id.strip()) 
        sql_query = "SELECT * FROM svt_calibrations WHERE collection_id = '%i'" % col_id 
    
        try: 
            cursor.execute(sql_query)
            results = cursor.fetchall()
            
            if len(results) != 0: 
                print 'Collection exists!'
                return 

            print "Loading default values of pedestal and noise for all SVT channels ..."
            pedestal = 6000.
            noise = 65
            for svt_channel_id in range(0, 24576):
                sql_query = "INSERT INTO svt_calibrations(collection_id, svt_channel_id,\
                    pedestal_0, pedestal_1, pedestal_2, pedestal_3, pedestal_4, pedestal_5,\
                    noise_0, noise_1, noise_2, noise_3, noise_4, noise_5)\
                    VALUES('%i', '%i', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f')"\
                    % (col_id, svt_channel_id,\
                    pedestal, pedestal, pedestal, pedestal, pedestal, pedestal,\
                    noise, noise, noise, noise, noise, noise)

                cursor.execute(sql_query)
            db.commit()
        except:
            print "[ERROR]: Unable to load pedestal and noise values."
            db.rollback()

    if args.fit_col_id: 
        # Check if the collection id exist.  If so, warn the user and exit.
        col_id = int(args.fit_col_id.strip()) 
        sql_query = "SELECT * FROM svt_shape_fit_parameters WHERE collection_id = '%i'" % col_id 
    
        try: 
            cursor.execute(sql_query)
            results = cursor.fetchall()
            
            if len(results) != 0: 
                print 'Collection exists!'
                return 

            print "Loading default shape fit parameters for all SVT channels ..."
            amplitude = 2500
            t0 = -10
            tp = 35
            tp2 = 10
            for svt_channel_id in range(0, 24576):
                sql_query = "INSERT INTO svt_shape_fit_parameters(collection_id, svt_channel_id,\
                    amplitude, t0, tp, tp2)\
                    VALUES('%i', '%i', '%f', '%f', '%f', '%f')"\
                    % (col_id, svt_channel_id, amplitude, t0, tp, tp2)

                cursor.execute(sql_query)
            db.commit()
        except:
            print "[ERROR]: Unable to load pedestal and noise values."
            db.rollback()

    if args.gain_col_id: 
        # Check if the collection id exist.  If so, warn the user and exit.
        col_id = int(args.gain_col_id.strip()) 
        sql_query = "SELECT * FROM svt_gains WHERE collection_id = '%i'" % col_id 
    
        try: 
            cursor.execute(sql_query)
            results = cursor.fetchall()
            
            if len(results) != 0: 
                print 'Collection exists!'
                return 


            print "Loading default values of gain and offset for all SVT channels ..."
            gain = 0.09
            offset = 100
            for svt_channel_id in range(0, 24576):
                sql_query = "INSERT INTO svt_gains(collection_id, svt_channel_id,\
                    gain, offset)\
                    VALUES('%i', '%i', '%f', '%f')"\
                    % (col_id, svt_channel_id, gain, offset)

                cursor.execute(sql_query)
            db.commit()
        except:
            print "[ERROR]: Unable to load pedestal and noise values."
            db.rollback()


if __name__ == "__main__": 
    main()
