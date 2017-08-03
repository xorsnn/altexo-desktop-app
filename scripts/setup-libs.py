from urllib.request import urlopen
import zipfile
import os
import shutil
from sha256_file import sha256_file
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
# print(SCRIPT_DIR)
DOWNLOAD_DIR = os.path.realpath(os.path.join(SCRIPT_DIR, "../side-libs"))
#
# print(os.getcwd())
# os.chdir( SCRIPT_DIR + "/" + DOWNLOAD_DIR )
# print()

BOOST_DIR = os.path.realpath(os.path.join(DOWNLOAD_DIR, "boost_1_64_0"))
BOOST_ARCHIVE = os.path.realpath(os.path.join(DOWNLOAD_DIR, "boost_1_64_0.zip"))
BOOST_DOWNLOAD_LINK = "https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.zip"
BOOST_ARCH_HASH = "49c6abfeb5b480f6a86119c0d57235966b4690ee6ff9e6401ee868244808d155"

SDL_DOWNLOAD_LINK = "https://www.libsdl.org/release/SDL2-2.0.5.zip"
SDL_ARCHIVE = os.path.realpath(os.path.join(DOWNLOAD_DIR, "SDL2-2.0.5.zip"))

def _download_boost():
    print(" -> downloading boost")
    response = urlopen(BOOST_DOWNLOAD_LINK)
    CHUNK = 16 * 1024
    with open(BOOST_ARCHIVE, 'wb') as f:
        while True:
            chunk = response.read(CHUNK)
            if not chunk:
                break
            f.write(chunk)

    print(" -> unzip boost")
    zip_ref = zipfile.ZipFile( BOOST_ARCHIVE, 'r' )
    zip_ref.extractall( DOWNLOAD_DIR )
    zip_ref.close()

def _clean():
    print(" -> clean download dir")
    if os.path.exists(DOWNLOAD_DIR):
        for the_file in os.listdir(DOWNLOAD_DIR):
            file_path = os.path.join(DOWNLOAD_DIR, the_file)
            try:
                if os.path.isfile(file_path):
                    os.unlink(file_path)
                elif os.path.isdir(file_path):
                    shutil.rmtree(file_path)
            except Exception as e:
                print(e)

def _build_boost():
    print(" -> building boost")
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "boost_1_64_0")))
    subprocess.run("bootstrap.bat", shell=True, check=True)
    subprocess.run("b2 toolset=msvc-14.0 --clean-all -n", shell=True, check=True)
    subprocess.run("b2 toolset=msvc-14.0 --build-type=complete --abbreviate-paths architecture=x86 address-model=32 install -j4", shell=True, check=True)
    os.chdir(SCRIPT_DIR)

def build_boost():
    _clean()
    _download_boost()
    _build_boost()

def _download_SDL():
    print(" -> downloading SDL")
    response = urlopen(SDL_DOWNLOAD_LINK)
    CHUNK = 16 * 1024
    with open(SDL_ARCHIVE, 'wb') as f:
        while True:
            chunk = response.read(CHUNK)
            if not chunk:
                break
            f.write(chunk)

    print(" -> unzip SDL")
    zip_ref = zipfile.ZipFile( SDL_ARCHIVE, 'r' )
    zip_ref.extractall( DOWNLOAD_DIR )
    zip_ref.close()

def _build_SDL():
    print(" -> building SDL")
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "SDL2-2.0.5")))

    build_dir = os.path.realpath(os.path.join(os.path.join(DOWNLOAD_DIR, "SDL2-2.0.5"), "build"))
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)
    subprocess.run("cmake ..", shell=True, check=True)
    subprocess.run("MSBuild SDL2.sln /p:Configuration=Release", shell=True, check=True)
    os.chdir(SCRIPT_DIR)

def build_SDL():
    # _download_SDL()
    _build_SDL()
    pass

def main():
    # build_boost()
    build_SDL()
    pass

if __name__ == "__main__":
    # execute only if run as a script
    main()
