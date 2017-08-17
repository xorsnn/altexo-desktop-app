import os
import subprocess
from urllib.request import urlopen
import zipfile
import platform

# BOOST_DIR = os.path.realpath(os.path.join(DOWNLOAD_DIR, "boost_1_64_0"))
BOOST_DOWNLOAD_LINK = "https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.zip"
# BOOST_ARCH_HASH = "49c6abfeb5b480f6a86119c0d57235966b4690ee6ff9e6401ee868244808d155"

def _download_boost(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> downloading boost")

    BOOST_ARCHIVE = os.path.realpath(os.path.join(DOWNLOAD_DIR, "boost_1_64_0.zip"))

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

def _clean(SCRIPT_DIR, DOWNLOAD_DIR):
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

def _build_boost(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> building boost")
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "boost_1_64_0")))

    INSTALL_DIR = os.path.realpath(os.path.join(DOWNLOAD_DIR, "boost"))

    # TODO: provide prefix var
    subprocess.run("bootstrap.bat", shell=True, check=True)
    subprocess.run("b2 toolset=msvc-14.0 --clean-all -n", shell=True, check=True)
    subprocess.run("b2 toolset=msvc-14.0 --build-type=complete --abbreviate-paths architecture=x86 address-model=32 install -j4 --prefix=%s" % INSTALL_DIR, shell=True, check=True)

    os.chdir(SCRIPT_DIR)

def build_boost(SCRIPT_DIR, DOWNLOAD_DIR):
    _clean(SCRIPT_DIR, DOWNLOAD_DIR)
    _download_boost(SCRIPT_DIR, DOWNLOAD_DIR)
    _build_boost(SCRIPT_DIR, DOWNLOAD_DIR)
