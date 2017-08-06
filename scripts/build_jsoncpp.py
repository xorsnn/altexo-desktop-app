import os
import subprocess

JSONCPP_REV = "a679dde58d675e75eaf0fe31f62e833868e2f22d" # 1.8.1

def _download_jsoncpp(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> downloading jsoncpp")
    os.chdir(DOWNLOAD_DIR)
    subprocess.run(
        "git clone https://github.com/open-source-parsers/jsoncpp.git jsoncpp",
        shell=True,
        check=True
    )
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "jsoncpp")))
    subprocess.run("git checkout %s" % JSONCPP_REV, shell=True, check=True)
    os.chdir(SCRIPT_DIR)

def _build_jsoncpp(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> building jsoncpp")

    INSTALL_DIR = os.path.realpath(os.path.join(DOWNLOAD_DIR, "jsoncpp_install"))

    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "jsoncpp")))

    build_dir = os.path.realpath(os.path.join(os.path.join(DOWNLOAD_DIR, "jsoncpp"), "build"))
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)
    subprocess.run("cmake ..\  -DCMAKE_INSTALL_PREFIX=%s" % INSTALL_DIR, shell=True, check=True)
    subprocess.run("cmake --build . --config Release --target install", shell=True, check=True)

    os.chdir(SCRIPT_DIR)

def build_jsoncpp(SCRIPT_DIR, DOWNLOAD_DIR):
    _download_jsoncpp(SCRIPT_DIR, DOWNLOAD_DIR)
    _build_jsoncpp(SCRIPT_DIR, DOWNLOAD_DIR)
