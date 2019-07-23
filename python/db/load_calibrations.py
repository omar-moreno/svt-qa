#!/usr/bin/env python

import argparse
import MySQLdb

def main(): 

    # Parse all command line arguments
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-f', action='store', dest='cfile', 
                        help='Calibrations file to load into the database.')
    parser.add_argument('-r', action='store', dest='run', 
                        help='Run number.')
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

    sql_query = "SELECT MAX(collection_id) FROM svt_calibrations"
    try:
        cursor.execute(sql_query)
        results = cursor.fetchall()
    except:
        print "[ERROR]: Unable to load pedestal and noise values."
        db.rollback()

    max_col_id = results[0][0]
    col_id = max_col_id + 1

    print 'Loading SVT calibrations with collection ID %s' % col_id

    cfile = open(args.cfile)
    for line in cfile: 
        values = [float(x) for x in line.split(" ")]
        try:
            sql_query = "INSERT INTO svt_calibrations(collection_id, svt_channel_id,\
                pedestal_0, pedestal_1, pedestal_2, pedestal_3, pedestal_4, pedestal_5,\
                noise_0, noise_1, noise_2, noise_3, noise_4, noise_5)\
                VALUES('%i', '%i', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f')"\
                % (col_id, values[0],\
                    values[1], values[2], values[3], values[4], values[5], values[6],\
                    values[7], values[8], values[9], values[10], values[11], values[12])
            #print sql_query    
            cursor.execute(sql_query)
        except:
            print "[ERROR]: Unable to load pedestal and noise values."
            db.rollback()
    db.commit()
    
    print 'Adding conditions record.'  
 
    sql_query = "INSERT INTO conditions(run_start, run_end, updated, created, \
        tag, created_by, notes, name, table_name, collection_id) \
        value ('%i', 999999, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, 'phys2019', 'omoreno', \
        'Pedestals and noise extracted from run %s.', 'svt_calibrations', 'svt_calibrations', '%i')"\
        % (int(args.run.strip()), int(args.run.strip()), col_id)

    try: 
        cursor.exectute(sql_query)
    except: 
        print "[ ERROR ]: Unable to load conditions record."
        db.rollback()
    db.commit()
 
if __name__ == "__main__": 
    main()
