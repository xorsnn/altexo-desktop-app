#include "alfreenectdevice.h"

ALFreenectDevice::ALFreenectDevice(freenect_context *_ctx, int _index) :
    Freenect::FreenectDevice(_ctx, _index),
    m_buffer_depth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2),
    m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
    m_buffer_depth_new(480*640*3),
    m_new_rgb_frame(false),
    m_new_depth_frame(false)
{
    setDepthFormat(FREENECT_DEPTH_REGISTERED);
    QSettings settings;
    this->minDepth = settings.value("altexo/minDepth", 555).toInt();
    this->maxDepth = settings.value("altexo/maxDepth", 1005).toInt();
}

int ALFreenectDevice::getMaxDepth() {
    return this->maxDepth;
}

void ALFreenectDevice::setMaxDepth(int newDepth) {
    this->maxDepth = newDepth;
}


void ALFreenectDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
    Mutex::ScopedLock lock(m_rgb_mutex);
    uint8_t* rgb = static_cast<uint8_t*>(_rgb);
    std::copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
    m_new_rgb_frame = true;
}

float ALFreenectDevice::ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

    if (fabs(inputMin - inputMax) < FLT_EPSILON){
        qDebug() << "ofMap(): avoiding possible divide by zero, check inputMin and inputMax: " << inputMin << " " << inputMax;
        return outputMin;
    } else {
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }

}

ALColor ALFreenectDevice::huePixelForDepth(uint16_t pix) {
    ALColor color;
    color.set(0,255);
    if(pix == 0 || pix < this->minDepth || pix > this->maxDepth ) {
        return color;
    }
    float alignedPix = ALFreenectDevice::ofMap(pix, this->minDepth, this->maxDepth, 0, 255, true);

    color.setHsb(alignedPix, 255, 255, 255);
    return color;
}


void ALFreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
    Mutex::ScopedLock lock(m_depth_mutex);
    uint16_t* depth = static_cast<uint16_t*>(_depth);
    std::copy(depth, depth+getDepthBufferSize()/2, m_buffer_depth.begin());
    for( unsigned int i = 0 ; i < 640*480 ; i++) {
        ALColor color = this->huePixelForDepth(depth[i]);
        m_buffer_depth_new[3*i+0] = (int) floorf(color.r);
        m_buffer_depth_new[3*i+1] = (int) floorf(color.g);
        m_buffer_depth_new[3*i+2] = (int) floorf(color.b);
    }
    m_new_depth_frame = true;
}

bool ALFreenectDevice::getRGB(std::vector<uint8_t> &buffer) {
    Mutex::ScopedLock lock(m_rgb_mutex);
    if (!m_new_rgb_frame)
        return false;
    buffer.swap(m_buffer_video);
    m_new_rgb_frame = false;
    return true;
}

bool ALFreenectDevice::getDepth(std::vector<uint8_t> &buffer) {
    Mutex::ScopedLock lock(m_depth_mutex);
    if (!m_new_depth_frame)
        return false;
    buffer.swap(m_buffer_depth_new);
    m_new_depth_frame = false;
    return true;
}

void ALFreenectDevice::updateSettings() {
    QSettings settings;
    this->minDepth = settings.value("altexo/minDepth", 555).toInt();
    this->maxDepth = settings.value("altexo/maxDepth", 1005).toInt();
}
