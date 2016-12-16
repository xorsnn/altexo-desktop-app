
import os

DIRECTORY = 'out'
WEBRTC_DIR = '/home/xors/workspace/lib/webrtc-checkout/src/out/Default'

def main():
    libs = []
    lines = []
    if not os.path.exists(DIRECTORY):
        os.makedirs(DIRECTORY)


    for root, dirs, files in os.walk(WEBRTC_DIR):
        for file in files:
            if file.endswith(".a"):
                # print(os.path.join(root, file))
                # print(file)
                filePath = os.path.join(root, file)
                lines.append('${WEBRTCBUILD}' + filePath.replace(WEBRTC_DIR, ''))

    fOut = open(DIRECTORY + '/out.cmake', 'w')
    # f = open(DIRECTORY + '/in.txt', 'r')
    #
    for line in lines:
        pathArray = line.strip().split('/')
        libName = pathArray[len(pathArray)-1].split('.')[0].upper()

        resLine = ''

        resLine += 'ADD_LIBRARY('
        resLine += libName
        resLine += ' STATIC IMPORTED)\n'

        resLine += 'SET_TARGET_PROPERTIES('
        resLine += libName
        resLine += ' PROPERTIES IMPORTED_LOCATION ' + line.strip() + ')\n'

        # todo: debug
        # resLine = line.strip() + '\n'

        fOut.write(resLine)

        libs.append(libName)
    #
    # f.close()
    #
    # for libName in libs:
    fOut.write('SET (WEBRTC_LIBS\n')
    # fOut.write("\n  PRIVATE ".join(libs))
    # fOut.write("\n  PRIVATE ".join(libs))
    fOut.write("\n  PUBLIC ".join(libs))
    fOut.write(')\n')
    fOut.close()

if __name__ == '__main__':
    main()
