import os
import subprocess
from urllib.request import urlopen
import zipfile

SDL_DOWNLOAD_LINK = "https://www.libsdl.org/release/SDL2-2.0.5.zip"

def _download_SDL(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> downloading SDL")

    SDL_ARCHIVE = os.path.realpath(os.path.join(DOWNLOAD_DIR, "SDL2-2.0.5.zip"))

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

def _build_SDL(SCRIPT_DIR, DOWNLOAD_DIR):
    print(" -> building SDL")
    os.chdir(os.path.realpath(os.path.join(DOWNLOAD_DIR, "SDL2-2.0.5")))

    build_dir = os.path.realpath(os.path.join(os.path.join(DOWNLOAD_DIR, "SDL2-2.0.5"), "build"))
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)
    subprocess.run("cmake ..", shell=True, check=True)
    subprocess.run("MSBuild SDL2.sln /p:Configuration=Release", shell=True, check=True)
    os.chdir(SCRIPT_DIR)

def build_SDL(SCRIPT_DIR, DOWNLOAD_DIR):
    _download_SDL(SCRIPT_DIR, DOWNLOAD_DIR)
    _build_SDL(SCRIPT_DIR, DOWNLOAD_DIR)
    pass
