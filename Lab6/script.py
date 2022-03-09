import subprocess
import filecmp
import time

Delay = [10, 50, 1000]
loss = [0.1, 0.5, 1]

# Create a 5 MB file to transfer
subprocess.Popen("yes Data to send | head -c 5000000 > send.txt", shell=True)

s = subprocess.Popen("gcc Sender.c -o sender", shell=True )
s.wait()
s_ = subprocess.Popen("gcc Receiver.c -o receiver", shell=True )
s_.wait()

for k in range(0, 20):
    for i in loss:
        for j in Delay:
            print('Delay %d and Loss %d', j,i)
            print("Cubic")

            # run for CUBIC

            # Set delay and loss
            s0 = subprocess.Popen("sudo tc qdisc add dev wlo1 root netem delay "+str(i)+"ms loss "+str(j)+"%", shell=True)
            s0.wait()

            #run both program
            s1 = subprocess.Popen("./sender "+str(8080+j)+" cubic &", shell=True )
            time.sleep(5)
            s2 = subprocess.Popen("./receiver "+str(8080+j)+" "+str(j)+" "+str(i)+" cubic &", shell=True )
            time.sleep(10)

            #Comparing 2 files
            if(filecmp.cmp('send.txt', 'recv.txt')):
                print("Send.txt and recv.txt are same")
            else:
                print("ERROR! Send.txt and recv.txt are not same")

            #return to initial state
            s3 = subprocess.Popen("sudo tc qdisc delete dev wlo1 root netem", shell=True)
            s3.wait()

            # run for Reno
            print("Reno")

            #Set delay and loss
            s4 = subprocess.Popen("sudo tc qdisc add dev wlo1 root netem delay "+str(i)+"ms loss "+str(j)+"%", shell=True)
            s4.wait()

            #run both program
            s5 = subprocess.Popen("./sender "+str(6000+j)+" reno &", shell=True )
            time.sleep(5)
            s6 = subprocess.Popen("./receiver "+str(6000+j)+" "+str(j)+" "+str(i)+" reno &", shell=True )
            time.sleep(10)

            # Comparing 2 files
            if(filecmp.cmp('send.txt', 'recv.txt')):
                print("Send.txt and recv.txt are same")
            else:
                print("ERROR! Send.txt and recv.txt are not same")

            #return to initial state
            s7 = subprocess.Popen("sudo tc qdisc delete dev wlo1 root netem", shell=True)
            s7.wait()

