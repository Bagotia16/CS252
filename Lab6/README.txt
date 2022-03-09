Put Sender.c and Receiver.c and script.py in the same folder.

Now run script.py [python3 script.py]
    (   individual file could be rune like 
        gcc Sender.c -o Sender
        gcc Receiver.c -o Receiver
        ./Sender <PortNo> <protocol>
        ./Receiver <PortNo>
    )
It will run 20 iteration of every combination of Delay and Loss for both 'Reno' and 'Cubic'. 

It will create a send.txt, a recv.txt and a throughput.txt that will store the result of all 360 process in the format {'protocol' 'Loss' 'Delay' 'Throughput'}
