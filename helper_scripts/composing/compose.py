
import os

DIRECTORY = 'out'
WEBRTC_DIR = '/home/xors/workspace/lib/webrtc-checkout/src/out/Default'

def composeLink(line):
    ms = line.split(' ')
    ms = filter(lambda x: x <> '', map(lambda x: x.strip() if (x.strip()[-2:] == '.o') or (x.strip()[-2:] == '.a') else '', ms))
    return ms

def composeStaticLibsSimple(staticLibs):
    f = open('out/out_simple.cmake', 'w')

    libs = []
    for line in staticLibs:

        libs.append(line.strip().replace('obj/', WEBRTC_DIR + '/obj/'))


    f.write('SET (WEBRTC_LIBS_SIMPLE\n')

    f.write("\n".join(map(lambda x: "\"" + x + "\"", libs)))

    f.write(')\n')

    f.close()

def composeStaticLibs(staticLibs):
    f = open('out/out.cmake', 'w')

    libs = []
    for line in staticLibs:
        pathArray = line.strip().split('/')
        libName = pathArray[len(pathArray)-1].split('.')[0].upper()

        resLine = ''

        resLine += 'ADD_LIBRARY('
        resLine += libName
        resLine += ' STATIC IMPORTED)\n'

        resLine += 'SET_TARGET_PROPERTIES('
        resLine += libName
        resLine += ' PROPERTIES IMPORTED_LOCATION ' + line.strip().replace('obj/', '${WEBRTCBUILD}/obj/') + ')\n'

        f.write(resLine)

        libs.append(libName)


    f.write('SET (WEBRTC_LIBS\n')

    f.write("\n".join(map(lambda x: " PUBLIC " + x, libs)))

    f.write(')\n')

    f.close()

def composeObjectFiles(objectFiles):

    f = open('out/outObj.cmake', 'w')
    f.write("SET (OBJ_DEPS \n")
    f.write("\n".join(map(lambda x: x.strip().replace('obj/', '${WEBRTCBUILD}/obj/'), objectFiles)))
    f.write(')\n')
    f.close()

def main():
    f= open("out/peerconnection_client.ninja")
    for line in f.readlines():
        if "build ./peerconnection_client: " in line:

            libs = composeLink(line)

            oFiles = filter(lambda x: x[-2:] == ".o" and not "examples" in x, libs)
            composeObjectFiles(oFiles)

            aFiles = filter(lambda x: x[-2:] == ".a", libs)
            composeStaticLibs(aFiles)
            composeStaticLibsSimple(aFiles)

    f.close()

if __name__ == '__main__':
    main()
