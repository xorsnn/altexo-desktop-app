
import os
import sys
# walk_dir = sys.argv[1]
walk_dir = "D:\\WORKSPACE\\webrtc\\src\\out\\Release"

def ifExt(fileName):
    nameLen = len(fileName)
    ext = (fileName[(nameLen-4):(nameLen)])
    # print(ext)
    if (ext.lower() == ".lib"):
        return True
    else:
        return False

def composeDetails():
    print('walk_dir = ' + walk_dir)
    libsList = []
    fOut = open("res.cmake", "w")

    # If your current working directory may change during script execution, it's recommended to
    # immediately convert program arguments to an absolute path. Then the variable root below will
    # be an absolute path as well. Example:
    # walk_dir = os.path.abspath(walk_dir)
    print('walk_dir (absolute) = ' + os.path.abspath(walk_dir))
    for root, subdirs, files in os.walk(walk_dir):
        for filename in files:
            file_path = os.path.join(root, filename)
            if ifExt(filename):
                print('\t- file %s (full path: %s)' % (filename, file_path))
                libName = filename.split(".")[0].upper()
                fOut.write("ADD_LIBRARY(" + libName + " STATIC IMPORTED)\n")
                basePath = "D:\\WORKSPACE\\webrtc\\src\\out\\Release"
                resPath = file_path.replace(basePath, "${WEBRTCBUILD}")
                fOut.write("SET_TARGET_PROPERTIES(" + libName + " PROPERTIES IMPORTED_LOCATION " + resPath.replace("\\", "\\\\") + ")\n")
                libsList.append(libName)
    fOut.write("SET(WEBRTC_LIBS\n")
    for lib in libsList:
        fOut.write("  PUBLIC " + lib + "\n")

    fOut.write(")\n")
    fOut.close()

def composeBrief():
    print('walk_dir = ' + walk_dir)
    libsList = []
    fOut = open("res.cmake", "w")

    # If your current working directory may change during script execution, it's recommended to
    # immediately convert program arguments to an absolute path. Then the variable root below will
    # be an absolute path as well. Example:
    # walk_dir = os.path.abspath(walk_dir)
    print('walk_dir (absolute) = ' + os.path.abspath(walk_dir))
    for root, subdirs, files in os.walk(walk_dir):
        for filename in files:
            file_path = os.path.join(root, filename)
            if ifExt(filename):
                print('\t- file %s (full path: %s)' % (filename, file_path))
                libName = filename.split(".")[0].upper()
                # fOut.write("ADD_LIBRARY(" + libName + " STATIC IMPORTED)\n")
                basePath = "D:\\WORKSPACE\\webrtc\\src\\out\\Release"
                resPath = file_path.replace(basePath, "${WEBRTCBUILD}")
                # fOut.write("SET_TARGET_PROPERTIES(" + libName + " PROPERTIES IMPORTED_LOCATION " + resPath.replace("\\", "\\\\") + ")\n")
                libsList.append(resPath.replace("\\", "\\\\") + "\n")
    fOut.write("SET(WEBRTC_LIBS\n")
    for lib in libsList:
        fOut.write("  " + lib)

    fOut.write(")\n")
    fOut.close()


def composeComment():
    print('walk_dir = ' + walk_dir)
    libsList = []
    fOut = open("res.cmake", "w")

    # If your current working directory may change during script execution, it's recommended to
    # immediately convert program arguments to an absolute path. Then the variable root below will
    # be an absolute path as well. Example:
    # walk_dir = os.path.abspath(walk_dir)
    print('walk_dir (absolute) = ' + os.path.abspath(walk_dir))
    for root, subdirs, files in os.walk(walk_dir):
        for filename in files:
            file_path = os.path.join(root, filename)
            if ifExt(filename):
                print('\t- file %s (full path: %s)' % (filename, file_path))
                libName = filename.split(".")[0].upper()
                # fOut.write("ADD_LIBRARY(" + libName + " STATIC IMPORTED)\n")
                # basePath = "D:\\WORKSPACE\\webrtc\\src\\out\\Release"
                # resPath = file_path.replace(basePath, "${WEBRTCBUILD}")
                # fOut.write("SET_TARGET_PROPERTIES(" + libName + " PROPERTIES IMPORTED_LOCATION " + resPath.replace("\\", "\\\\") + ")\n")
                libsList.append(file_path.replace("\\", "\\\\"))

    for lib in libsList:
        fOut.write("#pragma comment (lib, \" " + lib + "\")\n")
    fOut.close()
    #pragma comment (lib, "d3d9.lib")

def main():
    # composeDetails()
    # composeBrief()
    # composeComment()

if __name__ == '__main__':
    main()
