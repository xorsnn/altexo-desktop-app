#include "alfreenectdevice.h"

ALFreenectDevice::ALFreenectDevice(freenect_context *_ctx, int _index) :
    Freenect::FreenectDevice(_ctx, _index),
    m_buffer_depth(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
    m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
    m_gamma(2048),
    m_new_rgb_frame(false),
    m_new_depth_frame(false)
{
    for( unsigned int i = 0 ; i < 2048 ; i++) {
        float v = i/2048.0;
        v = std::pow(v, 3)* 6;
        m_gamma[i] = v*6*256;
    }

    this->createRainbowPallet();
    this->max_depth = 2100;
}

int ALFreenectDevice::getMaxDepth() {
    return this->max_depth;
}

void ALFreenectDevice::setMaxDepth(int newDepth) {
    this->max_depth = newDepth;
}

void ALFreenectDevice::createRainbowPallet() {

    unsigned char r, g, b;
    memset(LUTR, 0, 256);
    memset(LUTG, 0, 256);
    memset(LUTB, 0, 256);

    for (int i=1; i<255; i++) {
        if (i<=29) {
            r = (unsigned char)(129.36-i*4.36);
            g = 0;
            b = (unsigned char)255;
        }
        else if (i<=86) {
            r = 0;
            g = (unsigned char)(-133.54+i*4.52);
            b = (unsigned char)255;
        }
        else if (i<=141) {
            r = 0;
            g = (unsigned char)255;
            b = (unsigned char)(665.83-i*4.72);
        }
        else if (i<=199) {
            r = (unsigned char)(-635.26+i*4.47);
            g = (unsigned char)255;
            b = 0;
        }
        else {
            r = (unsigned char)255;
            g = (unsigned char)(1166.81-i*4.57);
            b = 0;
        }
        LUTR[i] = r;
        LUTG[i] = g;
        LUTB[i] = b;
    }
}


void ALFreenectDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
    Mutex::ScopedLock lock(m_rgb_mutex);
    uint8_t* rgb = static_cast<uint8_t*>(_rgb);
    std::copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
    //xors
//    for( unsigned int i = 0 ; i < 640*480 ; i++) {
//        if ((m_buffer_depth[3*i+0] == 0) && (m_buffer_depth[3*i+1] == 0) && (m_buffer_depth[3*i+2] == 0)) {
//            m_buffer_video[3*i+0] = 0;
//            m_buffer_video[3*i+1] = 0;
//            m_buffer_video[3*i+2] = 0;
//        }
//    }
    //~xors
    m_new_rgb_frame = true;
}

void ALFreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
    Mutex::ScopedLock lock(m_depth_mutex);
    uint16_t* depth = static_cast<uint16_t*>(_depth);
//    std::copy(depth, depth+getDepthBufferSize()/2, m_buffer_depth.begin());
    for( unsigned int i = 0 ; i < 640*480 ; i++) {
//        int pval = m_gamma[depth[i]];
        int lb = pval & 0xff;
        //xors experiment
        //default color
//        switch (pval>>8) {
//            case 0:
//                m_buffer_depth[3*i+0] = 255;
//                m_buffer_depth[3*i+1] = 255-lb;
//                m_buffer_depth[3*i+2] = 255-lb;
//                break;
//            case 1:
//                m_buffer_depth[3*i+0] = 255;
//                m_buffer_depth[3*i+1] = lb;
//                m_buffer_depth[3*i+2] = 0;
//                break;
//            case 2:
//                m_buffer_depth[3*i+0] = 255-lb;
//                m_buffer_depth[3*i+1] = 255;
//                m_buffer_depth[3*i+2] = 0;
//                break;
//            default:
//                m_buffer_depth[3*i+0] = 0;
//                m_buffer_depth[3*i+1] = 0;
//                m_buffer_depth[3*i+2] = 0;
//                break;
//        }

        //~default color
        //color from mr. doob
//        switch (pval>>8) {
//            case 0:
//                m_buffer_depth[3*i+0] = 255-lb;
//                m_buffer_depth[3*i+1] = 255;
//                m_buffer_depth[3*i+2] = 255;
//                break;
//            case 1:
//                m_buffer_depth[3*i+0] = 0;
//                m_buffer_depth[3*i+1] = 255-lb;
//                m_buffer_depth[3*i+2] = 255;
//                break;
//            case 2:
//                m_buffer_depth[3*i+0] = 0;
//                m_buffer_depth[3*i+1] = 0;
//                m_buffer_depth[3*i+2] = 255-lb;
//                break;
//            default:
//                m_buffer_depth[3*i+0] = 0;
//                m_buffer_depth[3*i+1] = 0;
//                m_buffer_depth[3*i+2] = 0;
//                break;
//        }
        //from depth kit

        int lookup = depth[i] / (max_depth / 256);
        //int lookup = ofMap( depthPixels.getPixels()[i], 0, max_depth, 0, 255, true);
        m_buffer_depth[(i*3)+0] = LUTR[lookup];
        m_buffer_depth[(i*3)+1] = LUTG[lookup];
        m_buffer_depth[(i*3)+2] = LUTB[lookup];
        //~from depth kit
        //~xors experiment
//        switch (pval>>8) {
//        case 0:
//            m_buffer_depth[3*i+0] = 255;
//            m_buffer_depth[3*i+1] = 255-lb;
//            m_buffer_depth[3*i+2] = 255-lb;
//            break;
//        case 1:
//            m_buffer_depth[3*i+0] = 255;
//            m_buffer_depth[3*i+1] = lb;
//            m_buffer_depth[3*i+2] = 0;
//            break;
//        case 2:
//            m_buffer_depth[3*i+0] = 255-lb;
//            m_buffer_depth[3*i+1] = 255;
//            m_buffer_depth[3*i+2] = 0;
//            break;
//        case 3:
//            m_buffer_depth[3*i+0] = 0;
//            m_buffer_depth[3*i+1] = 255;
//            m_buffer_depth[3*i+2] = lb;
//            break;
//        case 4:
//            m_buffer_depth[3*i+0] = 0;
//            m_buffer_depth[3*i+1] = 255-lb;
//            m_buffer_depth[3*i+2] = 255;
//            break;
//        case 5:
//            m_buffer_depth[3*i+0] = 0;
//            m_buffer_depth[3*i+1] = 0;
//            m_buffer_depth[3*i+2] = 255-lb;
//            break;
//        default:
//            m_buffer_depth[3*i+0] = 0;
//            m_buffer_depth[3*i+1] = 0;
//            m_buffer_depth[3*i+2] = 0;
//            break;
//        }
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
    buffer.swap(m_buffer_depth);
    m_new_depth_frame = false;
    return true;
}
