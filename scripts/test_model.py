import os
import sys
import subprocess
import time
from extractor import extract_last_line

def run_flow(flow_time=120, packet_interval = 100):
	NIC1ip = '192.168.6.139'

	cmd_start_remote_server = 'ssh ubuntu@' + NIC1ip + ' /home/ubuntu/workspace/udp2server/tools/udp2server > run_log.txt'
	cmd_kill_remote_server = 'ssh ubuntu@' + NIC1ip + ' killall udp2server' 
	
	print(cmd_start_remote_server)
	remote_server = subprocess.Popen(cmd_start_remote_server, shell=True, stdout=subprocess.PIPE)
	cmd = "../tools/udp2client " + " -p " + str(packet_interval) + " -k " + str(flow_time) 
	local_client_runner = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        local_client_runner.wait()
	
	print(str(packet_interval) + ":" + "finshed.")
	print(cmd)

	
	remote_server_killer = subprocess.Popen(cmd_kill_remote_server, shell=True, stdout=subprocess.PIPE)
	remote_server_killer.wait()


def run_exp(packet_interval = 100,df_path=""):
	run_flow(120, packet_interval)
	dir_name = "inter"+str(packet_interval)+'.txt'
	cmd1 = "mv run_log.txt " + dir_name
	os.system(cmd1)
        last_line=extract_last_line(dir_name)
        with open(df_path,'a') as f1:
            f1.write(str(packet_interval)+','+last_line+'\n')
        

if __name__ == "__main__":
        df_name="inter.csv"
        os.system("touch " + df_name)
	for packet_interval in [50,100,150,200,250,300,400,450,500,600,700,800,900,1000,1100,1200]:
		run_exp(packet_interval,df_name)
	#file_name = "interval"+".zip"
	#os.system("zip " + file_name + " inter*")
	os.system("rm -rf *.txt")
