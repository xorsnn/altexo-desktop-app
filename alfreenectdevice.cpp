#include "alfreenectdevice.h"

ALFreenectDevice::ALFreenectDevice(freenect_context *_ctx, int _index) :
    Freenect::FreenectDevice(_ctx, _index),
    m_buffer_depth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2),
//    m_buffer_depth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT).bytes),
    m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
    m_buffer_depth_new(480*640*3),
//    m_gamma(2048),
    m_new_rgb_frame(false),
    m_new_depth_frame(false)
{
    setDepthFormat(FREENECT_DEPTH_REGISTERED);
//    for( unsigned int i = 0 ; i < 2048 ; i++) {
//        float v = i/2048.0;
//        v = std::pow(v, 3)* 6;
//        m_gamma[i] = v*6*256;
//    }
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
//    std::rotate(m_buffer_video.begin(),m_buffer_video.begin()+this->moveX*3,m_buffer_video.end());

//    int move_x = this->moveX;
    //xors
//    int xx = 100;
//    for(int i = 0 ; i < 640*480 ; i++) {
////        if (i > this->moveX) {
////            if ((m_buffer_depth[3*(i-this->moveX + (this->moveY*640)) + 0] == 0) &&
//////                    (m_buffer_depth[3*(i-this->moveX + (this->moveY*640)) + 1] == 0) &&
//////                    (m_buffer_depth[3*(i-this->moveX + (this->moveY*640)) + 2] == 0)) {
//            if ((m_buffer_depth[3*i + 0] == 0) &&
//                    (m_buffer_depth[3*i + 1] == 0) &&
//                    (m_buffer_depth[3*i + 2] == 0)) {

//                m_buffer_video[3*i+0] = 0;
//                m_buffer_video[3*i+1] = 0;
//                m_buffer_video[3*i+2] = 0;
//            }
////        } else {
////            m_buffer_video[3*i+0] = 0;
////            m_buffer_video[3*i+1] = 0;
////            m_buffer_video[3*i+2] = 0;
////        }
//    }
    //~xors
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
//    unsigned int HSLtoRGB(const unsigned int& h, const unsigned int& s, const unsigned int& l);
    float alignedPix = ALFreenectDevice::ofMap(pix, this->minDepth, this->maxDepth, 0, 255, true);

    color.setHsb(alignedPix, 255, 255, 255);
    return color;
}


void ALFreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
    Mutex::ScopedLock lock(m_depth_mutex);
    uint16_t* depth = static_cast<uint16_t*>(_depth);
    std::copy(depth, depth+getDepthBufferSize()/2, m_buffer_depth.begin());
//    qDebug() << m_buffer_depth.size();
    for( unsigned int i = 0 ; i < 640*480 ; i++) {
//                RGB_t color = this->huePixelForDepth(depth[i]);

                ALColor color = this->huePixelForDepth(depth[i]);
//                int k = 0;
        //        k+=1;
                m_buffer_depth_new[3*i+0] = (int) floorf(color.r);
                m_buffer_depth_new[3*i+1] = (int) floorf(color.g);
                m_buffer_depth_new[3*i+2] = (int) floorf(color.b);
    }
        //        int pval = m_gamma[depth[i]];
        //        int lb = pval & 0xff;
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

        //        int lookup = depth[i] / (max_depth / 256);
        //        //int lookup = ofMap( depthPixels.getPixels()[i], 0, max_depth, 0, 255, true);
        //        m_buffer_depth[(i*3)+0] = LUTR[lookup];
        //        m_buffer_depth[(i*3)+1] = LUTG[lookup];
        //        m_buffer_depth[(i*3)+2] = LUTB[lookup];
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
//    }
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

int ALFreenectDevice::getMoveX() {
    return this->moveX;
}

void ALFreenectDevice::setMoveX(int newMoveX) {
    Mutex::ScopedLock lock(m_rgb_mutex);
    this->moveX = newMoveX;
}

int ALFreenectDevice::getMoveY() {
    return this->moveY;
}

void ALFreenectDevice::setMoveY(int newMoveY) {
    this->moveY = newMoveY;
}
