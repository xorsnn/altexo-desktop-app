import os
import subprocess

CURL_REV = "cf93a7b364a70b56150cf6ea77492b799ec02a45" # 7.49.1

def _download_libcurl(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> downloading curl")
    os.chdir(DOWNLOAD_DIR)
    subprocess.run(
        "git clone https://github.com/curl/curl.git curl",
        shell=True,
        check=True
    )
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "curl")))
    subprocess.run("git checkout %s" % CURL_REV, shell=True, check=True)
    os.chdir(SCRIPT_DIR)

def _build_libcurl(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> building curl")
    
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "curl")))

    build_dir = os.path.realpath(os.path.join(os.path.join(DOWNLOAD_DIR, "curl"), "build"))
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)
    subprocess.run("cmake ..\ -DBUILD_TESTING=OFF -DBUILD_CURL_EXE=OFF -DENABLE_MANUAL=OFF", shell=True, check=True)
    subprocess.run("MSBuild CURL.sln /p:Configuration=Release", shell=True, check=True)

    os.chdir(SCRIPT_DIR)

def build_curl(SCRIPT_DIR, DOWNLOAD_DIR):
    _download_libcurl(SCRIPT_DIR, DOWNLOAD_DIR)
    _build_libcurl(SCRIPT_DIR, DOWNLOAD_DIR)
