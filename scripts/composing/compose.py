
import os

DIRECTORY = 'out'
SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
WEBRTC_DIR = '/home/xors/workspace/lib/webrtc/webrtc-checkout/src/out/Default'
# AL_LIB_DIR = '/home/xors/workspace/lib/webrtc-checkout/src/webrtc/examples/peerconnection_2'

def composeLink(line):
    ms = line.split(' ')
    ms = filter(lambda x: x != '', map(lambda x: x.strip() if (x.strip()[-2:] == '.o') or (x.strip()[-2:] == '.a') else '', ms))
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

def composeDefines(defs):
    f = open('out/al-out-defines.cmake', 'w')
    defLines = map(lambda x: "add_definitions(" + x + ")", defs)
    f.write("\n".join(defLines))
    f.close()

def composeIncludeDirs(dirs):
    f = open('out/al-includes.cmake', 'w')
    defLines = map(lambda x: "INCLUDE_DIRECTORIES(" + x.replace("-I", "") + ")", dirs)
    f.write("\n".join(defLines))
    f.close()

def composeLibFiles():
    # for root, dirs, files in os.walk(AL_LIB_DIR + "/src"):
    #     for file in files:
    #         # if file.endswith(".a"):
    #             # print(os.path.join(root, file))
    #             # print(file)
    #         filePath = os.path.join(root, file)
    #         print("\"" + filePath.replace(AL_LIB_DIR, "peerconnection_2") + "\",")
    #             # lines.append('${WEBRTCBUILD}' + filePath.replace(WEBRTC_DIR, ''))
    pass

def main():
    if not os.path.exists(DIRECTORY):
        os.makedirs(DIRECTORY)

    f = open(os.path.join(WEBRTC_DIR, "obj/webrtc/examples/peerconnection_client.ninja"))
    for line in f.readlines():
        if "build ./peerconnection_client: " in line:

            libs = composeLink(line)

            oFiles = filter(lambda x: x[-2:] == ".o" and not "examples" in x, libs)
            composeObjectFiles(oFiles)

            aFiles = filter(lambda x: x[-2:] == ".a", libs)
            composeStaticLibs(aFiles)
            composeStaticLibsSimple(aFiles)

        if "defines = " in line:
            defs = line.replace("defines = ", "").split(" ")
            composeDefines(map(lambda x: x.strip(), defs))

        if "include_dirs = " in line:
            composeIncludeDirs(line.replace("include_dirs = ", "").split(" "))

    f.close()

    composeLibFiles()

if __name__ == '__main__':
    main()
