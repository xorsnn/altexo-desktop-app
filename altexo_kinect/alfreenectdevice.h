#ifndef ALFREENECTDEVICE_H
#define ALFREENECTDEVICE_H

#include "libfreenect.hpp"
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include <QDebug>
#include <QSettings>
#include "alcolor.h"

#define FLT_EPSILON 1.19209290E-07F

class Mutex {
public:
    Mutex() {
        pthread_mutex_init( &m_mutex, NULL );
    }
    void lock() {
        pthread_mutex_lock( &m_mutex );
    }
    void unlock() {
        pthread_mutex_unlock( &m_mutex );
    }

    class ScopedLock
    {
        Mutex & _mutex;
    public:
        ScopedLock(Mutex & mutex)
            : _mutex(mutex)
        {
            _mutex.lock();
        }
        ~ScopedLock()
        {
            _mutex.unlock();
        }
    };
private:
    pthread_mutex_t m_mutex;
};

class ALFreenectDevice : public Freenect::FreenectDevice
{
public:
    ALFreenectDevice(freenect_context *_ctx, int _index);
    //~ALFreenectDevice(){}

    // Do not call directly even in child
    void VideoCallback(void* _rgb, uint32_t timestamp);

    // Do not call directly even in child
    void DepthCallback(void* _depth, uint32_t timestamp);

    bool getRGB(std::vector<uint8_t> &buffer);

    bool getDepth(std::vector<uint8_t> &buffer);

    void createRainbowPallet();

    int getMaxDepth();
    void setMaxDepth(int newDepth);

    ALColor huePixelForDepth(uint16_t pix);
    float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp);

    void updateSettings();
private:
    std::vector<uint8_t> m_buffer_depth;
    std::vector<uint8_t> m_buffer_depth_new;
    std::vector<uint8_t> m_buffer_video;


    uint16_t* m_depth;

    Mutex m_rgb_mutex;
    Mutex m_depth_mutex;
public:
    bool m_newRgbFrame;
    bool m_newDepthFrame;

//    int minDepth = 200;
    int minDepth = 555;
//    int maxDepth = 900;
    int maxDepth = 1005;


//    int max_depth;
};

#endif // ALFREENECTDEVICE_H
