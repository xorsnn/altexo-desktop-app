
def main():
    libs = []
    fOut = open('out.txt', 'w')
    f = open('in.txt', 'r')
    for line in f.readlines():
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

    f.close()

    # for libName in libs:
    fOut.write('TARGET_LINK_LIBRARIES(${PROJECT_NAME}\n')
    # fOut.write("\n  PRIVATE ".join(libs))
    fOut.write("\n  PRIVATE ".join(libs))
    fOut.write(')\n')
    fOut.close()

if __name__ == '__main__':
    main()
