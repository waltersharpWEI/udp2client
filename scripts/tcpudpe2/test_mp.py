import os
import sys
import subprocess
import time

def run_flow(flow_time=30, loss_x = 0, delay_x = 0, offset=40):
	NIC1ip = '192.168.6.139'
	NIC1 = 'ens33'
	target_NIC = 'ens33'
	throughput_log_file = 'log.txt'
        packet_interval = 769
	
	cmd_start_remote_server = 'ssh ubuntu@' + NIC1ip + ' /home/ubuntu/workspace/tcp2server_delay/tools/tcp2server' 
	cmd_kill_remote_server = 'ssh ubuntu@' + NIC1ip + ' killall tcp2server'
	cmd_start_remote = 'ssh ubuntu@'+ NIC1ip + ' ifstat -b -n -i '+  target_NIC + ' -t 0.1 ' + '>' + throughput_log_file 
	
	remote_monitor = subprocess.Popen(cmd_start_remote, shell=True, stdout=subprocess.PIPE)
	print(cmd_start_remote_server)
	print(cmd_start_remote)
	
	delay = delay_x
	loss = loss_x
	os.system("sudo tc qdisc change dev " + target_NIC + " root netem loss 0% delay 0ms")

	remote_server = subprocess.Popen(cmd_start_remote_server, shell=True, stdout=subprocess.PIPE)
	#remote_server.wait()
	os.system("sudo tc qdisc change dev " +  target_NIC + " root netem delay " + str(delay[i]) + "ms loss "+str(loss[j])+"%")
	cmd = "../tools/udp2client " + " -c " + str(offset) + " -p " + str(packet_interval) + " -k " + str(flow_time)  
	os.system(cmd+'>'+'loss'+str(loss[j])+'delay'+str(delay[i])+'.txt')
	#jammer.kill()
	#time.sleep(5)
	remote_server_killer = subprocess.Popen(cmd_kill_remote_server, shell=True, stdout=subprocess.PIPE)
	remote_server_killer.wait()
	print("killcmd sent")
	#time.sleep(5)
	print(str(delay[i]) + ':' + str(loss[j]))
	print(cmd)

	os.system("sudo tc qdisc change dev " + target_NIC + " root netem loss 0% delay 0ms")
	
	remote_monitor.kill()


def run_exp(delay_x=0,loss_x=0):
	flow_time = 30
	run_flow(flow_time,loss_x,delay_x)
	
if __name__ == "__main__":
	for loss in [1,2,3,4,5,6,7,8,9,10]:
		for delay in [50]:
			run_exp(loss,delay)
