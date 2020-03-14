import os
import sys
import subprocess
import time
from extractor import extract_last_line

def run_flow(flow_time=120, packet_interval = 100):
	NIC1ip = '192.168.6.139'

	cmd_start_remote_server = 'ssh ubuntu@' + NIC1ip + ' /home/ubuntu/workspace/udp2server/tools/udp2server > run_log.txt'
        cmd_start_remote_buffer_monitor = 'ssh ubuntu@' + NIC1ip + ' python /home/ubuntu/workspace/buffer_logger.py'
        cmd_kill_remote_buffer_monitor = 'ssh ubuntu@' + NIC1ip + ' killall python'
        cmd_kill_remote_server = 'ssh ubuntu@' + NIC1ip + ' killall udp2server' 
        
	
	print(cmd_start_remote_server)
	remote_server = subprocess.Popen(cmd_start_remote_server, shell=True, stdout=subprocess.PIPE)
        remote_buffer_monitor = subprocess.Popen(cmd_start_remote_buffer_monitor, shell=True, stdout=subprocess.PIPE)
	cmd = "../tools/udp2client " + " -p " + str(packet_interval) + " -k " + str(flow_time) 
	local_client_runner = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        
	time_step = 0
        df_path="inter.csv"
	while time_step <= 150:
		dir_name = "run_log.txt"
		last_line=extract_last_line(dir_name)
		last_line.strip('\n')
                if last_line[-1] == '\n':
                	last_line = last_line[:-1]
 
		with open(df_path,'a') as f1:
		    f1.write(str(time_step)+','+last_line+'\n')
		time_step += 1
		time.sleep(1)	

	local_client_runner.wait()	
	print(str(packet_interval) + ":" + "finshed.")
	print(cmd)

	remote_server_killer = subprocess.Popen(cmd_kill_remote_server, shell=True, stdout=subprocess.PIPE)
	remote_server_killer.wait()
        remote_buffer_monitor_killer = subprocess.Popen(cmd_kill_remote_buffer_monitor, shell=True, stdout=subprocess.PIPE)
        remote_buffer_monitor_killer.wait()


def run_exp(packet_interval = 100,df_path=""):
	run_flow(120, packet_interval)
	#dir_name = "inter"+str(packet_interval)+'.txt'
	#cmd1 = "mv run_log.txt " + dir_name
	#os.system(cmd1)
        
        

if __name__ == "__main__":
        df_name="inter.csv"
        os.system("touch " + df_name)
	packet_interval = 200            
	run_exp(packet_interval,df_name)
	os.system("rm -rf *.txt")
