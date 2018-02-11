import os
import subprocess
from urllib.request import urlopen
import zipfile

GLM_DOWNLOAD_LINK = "https://github.com/g-truc/glm/archive/0.9.8.4.zip"

def _download_glm(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> downloading glm")

    GLM_ARCHIVE = os.path.realpath(os.path.join(DOWNLOAD_DIR, "0.9.8.4.zip"))

    response = urlopen(GLM_DOWNLOAD_LINK)
    CHUNK = 16 * 1024
    with open(GLM_ARCHIVE, 'wb') as f:
        while True:
            chunk = response.read(CHUNK)
            if not chunk:
                break
            f.write(chunk)

    print(" -> unzip glm")
    zip_ref = zipfile.ZipFile( GLM_ARCHIVE, 'r' )
    zip_ref.extractall( DOWNLOAD_DIR )
    zip_ref.close()
    pass

def _build_glm(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> building glm")
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "glm-0.9.8.4")))

    build_dir = os.path.realpath(os.path.join(os.path.join(DOWNLOAD_DIR, "glm-0.9.8.4"), "build"))
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)
    subprocess.run("cmake ..", shell=True, check=True)
    subprocess.run("MSBuild glm.sln /p:Configuration=Release", shell=True, check=True)
    os.chdir(SCRIPT_DIR)

def build_glm(SCRIPT_DIR, DOWNLOAD_DIR):
    _download_glm(SCRIPT_DIR, DOWNLOAD_DIR)
    _build_glm(SCRIPT_DIR, DOWNLOAD_DIR)
    pass
