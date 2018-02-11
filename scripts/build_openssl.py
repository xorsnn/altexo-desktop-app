import os
import subprocess
import tarfile
from urllib.request import urlopen

OPENSSL_REV = "577d7010c6ffa98e8dc61d8826693db9d8f57bd5" # OpenSSL_1_1_0f
OPENSSL_LINK = "https://www.openssl.org/source/openssl-1.0.2l.tar.gz"

def _download_openssl(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> downloading openssl")
    OPENSSL_ARCHIVE = os.path.realpath(os.path.join(DOWNLOAD_DIR, "openssl.tar.gz"))

    response = urlopen(OPENSSL_LINK)
    CHUNK = 16 * 1024
    with open(OPENSSL_ARCHIVE, 'wb') as f:
        while True:
            chunk = response.read(CHUNK)
            if not chunk:
                break
            f.write(chunk)

    print(" -> unzip openssl")
    tar = tarfile.open( OPENSSL_ARCHIVE )
    tar.extractall( DOWNLOAD_DIR )
    tar.close()

def _build_openssl(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> building openssl")

    OPENSSL_INSTALL_DIR = os.path.realpath(os.path.join(DOWNLOAD_DIR, "openssl"))

    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "openssl-1.0.2l")))

    build_dir = os.path.realpath(os.path.join(DOWNLOAD_DIR, "openssl-1.0.2l"))
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)
    subprocess.run("perl Configure VC-WIN32 no-asm --prefix=%s" % OPENSSL_INSTALL_DIR, shell=True, check=True)
    subprocess.run("ms\do_ms", shell=True, check=True)
    subprocess.run("nmake -f ms\\ntdll.mak", shell=True, check=True)
    subprocess.run("nmake -f ms\\ntdll.mak install", shell=True, check=True)

    os.chdir(SCRIPT_DIR)

def build_openssl(SCRIPT_DIR, DOWNLOAD_DIR):
    _download_openssl(SCRIPT_DIR, DOWNLOAD_DIR)
    _build_openssl(SCRIPT_DIR, DOWNLOAD_DIR)
