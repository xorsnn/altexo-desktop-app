from urllib.request import urlopen
import zipfile
import os
import shutil
import subprocess
from build_sdl import build_SDL
from build_boost import build_boost
from build_curl import build_curl
from build_cpr import build_cpr
from build_glm import build_glm
from build_jsoncpp import build_jsoncpp
from build_openssl import build_openssl
from build_libwebsockets import build_libwebsockets

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
DOWNLOAD_DIR = os.path.realpath(os.path.join(SCRIPT_DIR, "../side-libs"))

def main():
    # build_boost(SCRIPT_DIR, DOWNLOAD_DIR)
    # build_SDL(SCRIPT_DIR, DOWNLOAD_DIR)
    # build_glm(SCRIPT_DIR, DOWNLOAD_DIR)
    # build_curl(SCRIPT_DIR, DOWNLOAD_DIR)
    # build_cpr(SCRIPT_DIR, DOWNLOAD_DIR)
    # build_jsoncpp(SCRIPT_DIR, DOWNLOAD_DIR)
    # build_openssl(SCRIPT_DIR, DOWNLOAD_DIR)
    build_libwebsockets(SCRIPT_DIR, DOWNLOAD_DIR)
    pass

if __name__ == "__main__":
    # execute only if run as a script
    main()
