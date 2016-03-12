/* (c) Copyright 2014 Temasys Communication, Pte Ltd.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 *
 * VideoRenderer.cpp
 *
 *  Created on: Dec 2, 2013
 *      Author: Francois Temasys
 */

#include "VideoRenderer.h"
#include <QDebug>

VideoRenderer::VideoRenderer(const int nameReceiver,webrtc::VideoTrackInterface *track_to_render)
    :_nameReceiver(nameReceiver)
{
    _width          = 0;
    _height         = 0;
    _rendered_track = track_to_render;
    _updated        = true;
    _rendered_track -> AddRenderer(this);
}

VideoRenderer::~VideoRenderer()
{
    qDebug() << "Destroy Video Renderer";
    this->wait();
    _rendered_track->RemoveRenderer(this);
    _rendered_track = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// VideoRendererInterface implementation
//
void VideoRenderer::SetSize(int width,int height)
{
    qDebug() << "VideoRenderer::SetSize";
    qDebug() << width;
    qDebug() << height;
    qDebug() << "~VideoRenderer::SetSize";
    _width  = width;
    _height = height;
    _image.reset(new unsigned char[width * height * 4]);
}


void VideoRenderer::RenderFrame(const cricket::VideoFrame * frame)
{
    qDebug() << "VideoRenderer::RenderFrame";
    qDebug() << this->_width;
    qDebug() << this->_height;
    qDebug() << frame->GetWidth();
    qDebug() << frame->GetHeight();
    //TODO set size once, figure out why is not handled by default
    this->SetSize(frame->GetWidth(), frame->GetHeight());
    qDebug() << frame;
    qDebug() << "~VideoRenderer::RenderFrame";
    int size = _width * _height * 4;
    frame->ConvertToRgbBuffer(cricket::FOURCC_ARGB, _image.get(), size, _width * 4);
    this->start();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// QThread implementation
//
void VideoRenderer::run()
{
    //    qDebug() << "VideoRenderer::run";
    _mutex.lock();
    //TODO scale possibility

    uchar* data = reinterpret_cast<uchar *>(_image.get());
    // TODO move back
    if(_updated == true)
        //    if(_updated == false)
    {
        //        qDebug() << "===================VideoRenderer======================";
        //        qDebug() << data;
        //        qDebug() << "===================!~======================";
        _updated = false;
        _imageReceiver.reset(new QImage(data, _width, _height, QImage::Format_ARGB32));
        //        just an experiment
        //        qDebug() << "===================VideoRenderer======================";
        //        QString path = "test_out.png";
        //        Qthis->_imageReceiver.get()->save(path, "PNG");
        //        QImage img(data, _width, _height, QImage::Format_ARGB32);
        //        img.convertToFormat("").save(path);
        //~
        Q_EMIT imageToUpdate(_nameReceiver,_imageReceiver.get());
    }
    _mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
const unsigned char * VideoRenderer::getImage() const
{
    return _image.get();
}

int VideoRenderer::getWidth() const
{
    return _width;
}

int VideoRenderer::getHeight() const
{
    return _height;
}
