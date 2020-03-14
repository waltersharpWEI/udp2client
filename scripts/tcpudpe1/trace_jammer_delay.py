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


def sampleLoss(NICname='ens33',path_log='sl.csv'):
    jam1 = Jammer(NICname)
    jam1.initJam()
    time_step = 0
    cmd = "python test_trace_delay.py"
    loss_list = [1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,10]
    for loss in loss_list:
        jam1.changeLoss(loss)
        test_trace = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        test_trace.wait()
        print("c_loss:"+str(jam1.loss))
        with open(path_log,'a') as f1:
		f1.write(str(jam1.delay)+"\n")
        jam1.changeLoss(0)


if __name__=="__main__":
    os.system("rm inter.csv sl.csv; touch inter.csv; touch sl.csv")
    sampleLoss('ens33','sl.csv')
