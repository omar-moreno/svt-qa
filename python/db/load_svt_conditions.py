
#!/usr/bin/env python

import argparse
import MySQLdb
import yaml

def parse_config(config_file) :

    print('Loading configuration from %s ' % config_file)
    config = open(config_file, 'r')
    return yaml.load(config)

def main(): 

    # Parse all command line arguments
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-c', action='store', dest='config', 
                        help='Configuration file.')
    args = parser.parse_args()

    if not args.config :
        parser.error('A configuration file needs to be specified.')

    config = parse_config(args.config)

    host = config['host']
    user = config['user']
    pw   = config['password']

    # Connect to the database
    print 'Connecting to the database via host %s' % host
    db = MySQLdb.connect(host=host, user=user, 
                         passwd=pw, db='hps_conditions')

    # Check that a connection to the database has been established
    print 'Connection to the database hps_conditions has been established'

    # Get a cursor
    cursor = db.cursor()

    
if __name__ == "__main__": 
    main()
