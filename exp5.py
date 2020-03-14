import os
import sys
import subprocess
import time

#the path to the udp client executable
client_exe_path = ' tools/udp2client '

#the function to run an instance of experiment
#flow_time is the time of flow lasts in seconds, delay_x is the physical delay we whant,
#packet_interval is the interval between the CBR stream
def run_flow(flow_time=10,delay_x=0,packet_interval=0,lite=False,clock_offset=0):
    #the ip of server NIC
    NICip = '192.168.243.145'
    #the network NIC device id of correspoding subnet
    NIC = 'ens33'
    #the network NIC that will be influence by netem
    target_NIC = 'ens33'
    #the log trace of the expriment retrieved from server using scp
    trace_log_file = 'log.txt'

    #use netem to change the network situation add delay of delay_x in qdisc root
    os.system("sudo tc qdisc change dev " + target_NIC + " root netem delay " + str(delay_x) + "ms")
    #the command to be ran during the test
    cmd = client_exe_path + " -k " + str(flow_time) + " -p " + str(packet_interval) + " -c " + str(clock_offset)
    #wheter to use udp-lite
    if lite:
        cmd += " -o "
    #run the command and output the log
    print(cmd) 
    os.system(cmd+' > '+'delay'+str(delay_x)+'interval'+str(packet_interval)+'.txt')
    print('delay ' + str(delay_x) + ' interval ' + str(packet_interval) + ' finished.')
#    time.sleep(flow_time)
#    os.system('killall udp2client')
    print('retriving the trace log from server '+NICip)
    os.system('scp ' + 'ubuntu@' + NICip + ':workspace/udp2server/run_log25.txt' + ' . ')
    os.system('mv run_log25.txt' + ' delay'+str(delay_x)+'interval'+str(packet_interval)+'s.txt')
    print('retrieved the trace log from server')
    print(cmd+' has finished ruuning')


def run_exp():
    delays = [100,200,300,400,500]
    intervals = [769,1538,3076,6152]
    for delay_x in delays:
        for interval in intervals:
            run_flow(60,delay_x,interval,True,1)


if __name__ == "__main__":
    run_exp()
