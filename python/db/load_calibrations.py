
#!/usr/bin/env python

import argparse
import MySQLdb

def main(): 

    # Parse all command line arguments
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-f', action='store', dest='cfile', 
                        help='Calibrations file to load into the database.')
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

    #
    cfile = open(args.cfile)
    
    for line in cfile: 
        print line

    '''
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
    '''

if __name__ == "__main__": 
    main()
