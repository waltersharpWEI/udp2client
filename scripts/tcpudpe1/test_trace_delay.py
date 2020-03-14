import os
import sys
import subprocess
import time
from extractor import extract_last_line

def run_flow(flow_time=120, packet_interval = 100):
	NIC1ip = '192.168.6.139'

	cmd_start_remote_server = 'ssh ubuntu@' + NIC1ip + ' /home/ubuntu/workspace/udp2server_delay/tools/udp2server > run_log.txt'
	cmd_kill_remote_server = 'ssh ubuntu@' + NIC1ip + ' killall udp2server' 
        
	
	remote_server = subprocess.Popen(cmd_start_remote_server, shell=True, stdout=subprocess.PIPE)
	print(cmd_start_remote_server)
	cmd = "../../tools/udp2client" + " -p " + str(packet_interval) + " -k " + str(flow_time) + " -c " + str(-40) 
	local_client_runner = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        
	time_step = 0
	df_path="inter.csv"
	local_client_runner.wait()	
	print("local client finished running.")
	print(str(packet_interval) + ":" + "finshed.")
	print(cmd)
	#extract the data from run log
	dir_name = "run_log.txt"
	last_line=extract_last_line(dir_name)
	last_line.strip('\n')
        #print(last_line)
	if last_line[-1] == '\n':
		last_line = last_line[:-1]
        #print(last_line)

	with open(df_path,'a') as f1:
	    f1.write(last_line+'\n')

	remote_server_killer = subprocess.Popen(cmd_kill_remote_server, shell=True, stdout=subprocess.PIPE)
	remote_server_killer.wait()
        print("remote server killed.")
        

def run_exp(packet_interval = 100,df_path=""):
	run_flow(120, packet_interval)
        

if __name__ == "__main__":
        df_name="inter.csv"
        os.system("touch " + df_name)
	packet_interval = 769            
	run_exp(packet_interval,df_name)
	os.system("rm -rf *.txt")
