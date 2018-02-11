install
- cmake
- perl
- http://www.nasm.us
- ppm install dmake


to build from cmd
MSBuild rootproject.sln /p:Configuration=Release
cmake --build . --config Release

#change cmd locale
chcp 437
