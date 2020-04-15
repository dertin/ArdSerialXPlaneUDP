import sys, getopt, signal, threading, serial
from time import sleep
from functools import partial
from lib import xpc

# get_params
param_port = 'COM3'
param_baud = 9600
stop_threads = False
client_xplane = None

def get_params():
    global param_port
    global param_baud
    strUsage = 'usage: ArdSerialUDP.py --port COM3 --baud 9600'
    arrValidArgsOpt = ["port=","baud="]
    arrValidArgsRequired  = ["--port","--baud"]
    if len(sys.argv)==1:
        print (strUsage)
        sys.exit(2)
    try:
      opts, args = getopt.getopt(sys.argv[1:],"h",arrValidArgsOpt)
    except getopt.GetoptError as err:
      print (err)
      print (strUsage)
      sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print (strUsage)
            sys.exit()
        elif opt in ("--port"):
            param_port = arg
        elif opt in ("--baud"):
            param_baud = arg

def exit_handler(thread, signal, frame):
    global stop_threads
    global client_xplane
    try:
        stop_threads = True
        thread.join()
        print ('Close socket UDP XPlane')
        client_xplane.close()
    except:
        print("Unexpected error:", sys.exc_info()[0])
        sys.exit(2)
    sys.exit(0)

def handle_xplane(input_data, serial_arduino):
    global client_xplane
    if client_xplane is None:
        print("WARNING: reset client_xplane")
        init_XPlane()
        return
    try:
        print("handle_xplane:", input_data)
        arrOperationXplane = input_data.split("#")
        if arrOperationXplane[0] == "sendDREF":
             dref = arrOperationXplane[1].strip()
             value = float(arrOperationXplane[2].strip())
             client_xplane.sendDREF(dref, value)
             print("sendDREF:", dref, value)
        elif arrOperationXplane[0] == "getDREF":
             dref = arrOperationXplane[1].strip()
             status_dref = client_xplane.getDREF(dref)
             print("getDREF:", dref, status_dref)
             toArduinoSerial = '<'+dref+'#'+str(status_dref[0])+'>'
             print (toArduinoSerial.encode())
             serial_arduino.write(toArduinoSerial.encode())
    except:
        print("ERROR: reset client_xplane")
        print("Unexpected error:", sys.exc_info()[0])
        init_XPlane()
        return

def read_from_port(serial_arduino):
    while True:
        global stop_threads
        if stop_threads:
            print ('Exiting thread...')
            if serial_arduino.is_open == True:
                print ('Close serial port')
                serial_arduino.close()
            break
        while serial_arduino.inWaiting() > 0:
            input_data = serial_arduino.readline().decode()
            if len(input_data) > 0:
                handle_xplane(input_data, serial_arduino)

def serial_arduino():
    try:
        serial_arduino = serial.Serial(param_port, param_baud, timeout=None)
    except serial.serialutil.SerialException:
        print ("Arduino not connected")
        sys.exit(2)
    thread = threading.Thread(target=read_from_port, args=(serial_arduino,))
    thread.start()
    signal.signal(signal.SIGINT, partial(exit_handler, thread))

def verify_udp_xplane():
    global client_xplane
    try:
        client_xplane.getDREF("sim/test/test_float")
    except:
        print ("Error connection to X-Plane.")
        init_XPlane() # loop - connection to X-Plane

def init_XPlane():
    global client_xplane
    try:
        client_xplane = xpc.XPlaneConnect(timeout = 1000)
    except:
        print ("Error establishing connection to X-Plane.")
    verify_udp_xplane()
    sleep(1)

if __name__ == "__main__":
    get_params()
    init_XPlane()
    serial_arduino()
    # Keep the main thread running, otherwise signals are ignored.
    while True:
        sleep(0.5)
