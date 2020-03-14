#written by lai wei
#1 Feb 2020, 12:00PM
#This script meant to build a jammer that simulate a real network
#transmission process with varing loss and delay
import os
import random
import time
import subprocess


#Jammer class
class Jammer:
    def __init__(self, NICname='ens33'):
        self.loss = 0
        self.delay = 0
        self.NICname = NICname	

    def initJam(self):
        cmd = "sudo tc qdisc add dev " + self.NICname + " root netem loss 0"
        os.system(cmd)

    def applyTc(self):
        cmd = "sudo tc qdisc change dev " + self.NICname + " root netem loss " + str(self.loss) + " delay " + str(self.delay)
        os.system(cmd)

    def changeLoss(self,loss=0):
        self.loss = loss
        self.applyTc()

    def changeDelay(self,delay=0):
        self.delay = delay
        self.applyTc()


def stochasticLoss(NICname='ens33',path_log='sl_log.txt'):
    jam1 = Jammer(NICname)
    jam1.initJam()
    time_step = 0
    cmd = "python test_trace_delay.py"
    test_trace = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    while time_step < 170:
        vari = random.randint(0,30) - 4 
        #print(vari)
        if jam1.delay+vari >= 0:
            if jam1.delay+vari > 200:
                jam1.delay = 0
                vari = 0
            jam1.delay = 300
            vari = 0
            jam1.changeDelay(jam1.delay+vari)
        print("timestep:"+str(time_step)+",c_delay:"+str(jam1.delay))
        with open(path_log,'a') as f1:
		f1.write(str(time_step)+","+str(jam1.delay)+"\n")
        time.sleep(1)
        time_step += 1


if __name__=="__main__":
    os.system("rm inter.csv sl_log.txt; touch inter.csv; touch sl_log.txt")
    stochasticLoss('ens33','sl_log.txt')
