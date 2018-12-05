
import argparse
import subprocess
import yaml

def parse_config(config_file): 
    
    print 'Loading configuration from %s' % config_file
    config = open(config_file, 'r')
    return yaml.load(config)

def main(): 

    # Parse all command line arguments using the argparse module
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-c', action='store', dest='config', 
                        help='Configuration file.')
    args = parser.parse_args()

    if not args.config: 
        parser.error('A configuration file needs to be specified.')
   
    config = parse_config(args.config)
    
    # Get the MYA variables to sample
    variables = config['Variables']
    
    # Get the start time. Format is 'YYYY-MM-DD HH:MM:SS'.
    stime = config['Start'][0]

    # Get the end time. Format is 'YYYY-MM-DD HH:MM:SS'.   
    etime = config['End'][0]
 
    # Get the name of the file to dump the data to
    output = config['Output File'][0]

    var_list = ""
    for variable in variables: 
        var_list += " %s" % variable
    
    command = 'myData -m history -b "%s" -e "%s" %s >> %s' % (stime, etime, var_list, output)
    print command
    subprocess.Popen(command, shell=True).wait()
 
if __name__ == '__main__': 
    main()
