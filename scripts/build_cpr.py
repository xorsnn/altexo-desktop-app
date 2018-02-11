import os
import subprocess
import shutil
from urllib.request import urlopen
import zipfile
import platform

def _download_cpr(SCRIPT_DIR, DOWNLOAD_DIR):

    CPR_DOWNLOAD_LINK = "https://github.com/whoshuu/cpr/archive/1.3.0.zip"
    CPR_ARCHIVE = os.path.realpath(os.path.join(DOWNLOAD_DIR, "1.3.0.zip"))

    print(" -> downloading cpr")
    response = urlopen(CPR_DOWNLOAD_LINK)
    CHUNK = 16 * 1024
    with open(CPR_ARCHIVE, 'wb') as f:
        while True:
            chunk = response.read(CHUNK)
            if not chunk:
                break
            f.write(chunk)

    print(" -> unzip cpr")
    zip_ref = zipfile.ZipFile( CPR_ARCHIVE, 'r' )
    zip_ref.extractall( DOWNLOAD_DIR )
    zip_ref.close()
    pass

def _build_cpr(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> building cpr")

    CPR_DIR = os.path.realpath(os.path.join(DOWNLOAD_DIR, "cpr-1.3.0"))
    CPR_BUILD_DIR = os.path.realpath(os.path.join(os.path.join(DOWNLOAD_DIR, "cpr-1.3.0"), "build"))
    CPR_INSTALL_DIR = os.path.realpath(os.path.join(os.path.join(DOWNLOAD_DIR, "cpr"), "build"))

    if not os.path.exists(CPR_BUILD_DIR):
        os.makedirs(CPR_BUILD_DIR)

    os.chdir(CPR_BUILD_DIR)

    if (platform.system() == "Linux"):
        subprocess.run("cmake .. -DUSE_SYSTEM_CURL=ON -DBUILD_CPR_TESTS=OFF -DUSE_SYSTEM_GTEST=OFF"
            , shell=True, check=True)
        # subprocess.run("cmake --build . --config Release --target install", shell=True, check=True)
        subprocess.run("make", shell=True, check=True)
        # subprocess.run("make DESTDIR=%s install" % CPR_INSTALL_DIR, shell=True, check=True)
    else:
        CURL_DIR = os.path.join(os.path.join(DOWNLOAD_DIR, "curl"))
        CURL_BUILD_DIR = os.path.realpath(os.path.join(CURL_DIR, "build"))
        # HACK! copy include files for curl
        src_dir = CURL_DIR + "/include/curl"
        dest_dir = CURL_BUILD_DIR + "/include/curl"
        src_files = os.listdir(src_dir)
        for file_name in src_files:
            full_file_name = os.path.join(src_dir, file_name)
            if (os.path.isfile(full_file_name)):
                shutil.copy(full_file_name, dest_dir)

        curl_include_dir2 = CURL_BUILD_DIR + "\include"

        subprocess.run(("cmake .. -DUSE_SYSTEM_CURL=ON -DBUILD_CPR_TESTS=OFF -DUSE_SYSTEM_GTEST=OFF -DCURL_LIBRARY=%s\lib\Release -DCURL_INCLUDE_DIR=%s" % (CURL_BUILD_DIR, curl_include_dir2))
            , shell=True, check=True)

        subprocess.run("MSBuild cpr.sln /p:Configuration=Release", shell=True, check=True)

    os.chdir(SCRIPT_DIR)


def build_cpr(SCRIPT_DIR, DOWNLOAD_DIR):
    _download_cpr(SCRIPT_DIR, DOWNLOAD_DIR)
    _build_cpr(SCRIPT_DIR, DOWNLOAD_DIR)
