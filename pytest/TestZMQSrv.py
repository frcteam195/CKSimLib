import zmq
import random
import sys
import time

port = "10502"
if len(sys.argv) > 1:
    port = sys.argv[1]
    int(port)

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://*:%s" % port)

while True:
    topic = 10000
    messagedata = "test msg" + str(random.randrange(1,215) - 80)
    print("%d %s" % (topic, messagedata))
    socket.send(("%d %s" % (topic, messagedata)).encode('ascii'), 0)
    time.sleep(1)