
import os

DIRECTORY = 'out'
WEBRTC_DIR = '/home/xors/workspace/lib/webrtc-checkout/src/out/Default'

def main():
    f= open("out/peerconnection_client.ninja.include")
    for line in f.readlines():
        includes = line.strip().split('-I')
        includes = map(lambda x: x.replace('../../', 'include_directories(${WEBRTCTRUNK}/') + ')', includes)
        for include in includes:
            print include
    f.close()
if __name__ == '__main__':
    main()
