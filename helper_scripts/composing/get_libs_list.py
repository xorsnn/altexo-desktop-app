def parse_ninja():
    f = open('out/peerconnection_client.ninja', 'r')
    for line in f.readlines():
        lines = line.split(" ")
        for l in lines:
            print(l)
    f.close()

def create_list_of_libs():
    f = open('out/client_libs.txt', 'r')
    for line in f.readlines():
        segments = line.strip().split('/')
        print('PUBLIC ' + segments[len(segments)-1].split('.')[0].upper())
    f.close()


def main():
    create_list_of_libs()

if __name__ == '__main__':
    main()
