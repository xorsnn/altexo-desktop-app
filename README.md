# Altexo application (very much work in progress) #

## Altexo desktop application ui (proof of concept) ##

Holographic chat application. While video conferencing is somewhat mainstream, the next major jump is to add a new dimension to make collaboration more authentic and fun. Using Altexo, you can communicate with holograms.

## Examples ##
Here is an example of WebVR client. Volumetric data is transmitted over the internet using WebRTC. The video is recorded using Samsung galaxy 7.
![WebVR altexto example](https://github.com/xorsnn/altexo-chat-web/blob/master/static/WebVR.gif)

See more demos and examples on the [youtube channel](https://youtu.be/hpWKITMRGRw)

## Build ##
### Building core app ###
```
mkdir build
cd build
cmake ..
make
```
### Building webrtc backend ###
*   Build [webrtc](https://webrtc.org/native-code/development/)
*   See in "altexosdk" folder patch file 

## Works on ##

-   Mac
-   Linux

## Doesn not work on (yet) ##

-   Windowns
