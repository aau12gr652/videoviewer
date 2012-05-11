/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "videoplayer.h"

#include "videowidget.h"

#include <QtMultimedia>

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , surface(0)
    , playButton(0)
    , joinButton(0)
    , openButton(0)
{

//    connect(&movie, SIGNAL(stateChanged(QMovie::MovieState)),
//            this, SLOT(movieStateChanged(QMovie::MovieState)));

    qRegisterMetaType<QImage>("QImage");
    connect(this, SIGNAL(new_image_ready(QImage)),this,SLOT(frameChanged(QImage)));

    vid_source = 0;
    vid_sink = 0;
    m_blockbuster = 0;

    VideoWidget *videoWidget = new VideoWidget;
    surface = videoWidget->videoSurface();

    openButton = new QPushButton(tr("Open..."));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));

    joinButton = new QPushButton(tr("Join"));
    connect(joinButton, SIGNAL(clicked()), this, SLOT(joinStream()));


    playButton = new QPushButton;
    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(play()));

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(joinButton);
    controlLayout->addWidget(playButton);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);

    setLayout(layout);
}

void VideoPlayer::testsig(int format, int width, int height, AVFrame* f)
{
    qDebug("Received AVFrame with following ints: %d, %d, %d", format, width, height);
    QImage image = QImage((uchar*)f->data[0], width, height, f->linesize[0], QImage::Format_RGB888);
    if(presentImage(image)) qDebug("sucessfully presented Qimage");
}

VideoPlayer::~VideoPlayer()
{
    if (vid_sink)
    {
        delete vid_sink;
        vid_sink = 0;
    }
    if (vid_source)
    {
        delete vid_source;
        vid_source = 0;
    }

}

void VideoPlayer::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),
            QDir::homePath());

    if (!fileName.isEmpty()) {
        qDebug() << "videoplayer.cpp opened file" << fileName;
        surface->stop();

        if(m_blockbuster)
            delete m_blockbuster;
        m_blockbuster = new blockbuster(false); // false = outbound

        if (vid_source)
        {
            delete vid_source;
            vid_source = 0;
        }
        int errorcode;
        vid_source = new hollywood_source;
        errorcode = vid_source->set_file(fileName.toAscii());
//                qDebug("errorcode is %d",errorcode);
        if (errorcode<0)
        {
            qWarning() << "videoplayer.cpp FAILED to initialize interface";
        }

        if(vid_sink) // Create a local sink
        {
            delete vid_sink;
            vid_sink = 0;
        }
        vid_sink = new hollywood_sink(vid_source->video_codec_id, vid_source->audio_codec_id);

        // Connect local sink to source
        vid_source->signal_video_packet.connect( boost::bind( &hollywood_sink::handle_video_packet, vid_sink, _1 ) );

        // Connect blockbuster to source
        vid_source->signal_video_packet.connect(
                    boost::bind( &blockbuster::prepare_for_kodo_encoder, m_blockbuster, _1));


        //vid_source->serial.signal_new_buffer.connect( boost::bind( &hollywood_source::test_deserializer_signal, vid_source, _1,_2) );

        // Connect sink to Qt frame drawing mechanism
        vid_sink->signal_bitmap_ready.connect( boost::bind( &VideoPlayer::convert_to_qimage_and_signal, this, _1,_2,_3,_4) );

        qDebug() << "videoplayer.cpp hollywood interface created";

        joinButton->setEnabled(false); // disable join button
        playButton->setEnabled(true);
        playing = false;
//        qDebug() << "videoplayer.cpp jump to frame";
    }
}

//void VideoPlayer::test_deserialize(uint8_t* buffer_pointer, uint32_t size)
//{
//    AVPacket packet;
//    av_init_packet(&packet);
//    packet.data = buffer_pointer;
//    packet.size = size;

//    vid_sink->handle_video_packet(&packet);
//}

void VideoPlayer::joinStream()
{
    qDebug("Joining stream..");

    // Setup code for receiving from network and decoding network coding.
    // bind signal about new avpacket to decode function

    openButton->setEnabled(false);

    qDebug("seg1?\n");
    if (m_blockbuster)
    {
        delete m_blockbuster;
    }
    qDebug("seg2?\n");
    m_blockbuster = new blockbuster(true); // true for inbound
    qDebug("seg3?\n");
    vid_sink = new hollywood_sink((CodecID)13);
    qDebug("seg4?\n");
    m_blockbuster->signal_new_avpacket.connect( boost::bind( &hollywood_sink::handle_video_packet, vid_sink, _1 ) );
    qDebug("seg5?\n");

    vid_sink->signal_bitmap_ready.connect( boost::bind( &VideoPlayer::convert_to_qimage_and_signal, this, _1,_2,_3,_4) );

    m_blockbuster->connect_to_stream();
    vid_sink->play();
}

void VideoPlayer::play()
{
    if(!playing)
    {
        if(!vid_source->start()) qDebug("started video");
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        vid_sink->play();
        qDebug("sink is running in decode thread");
        playing = true;
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        vid_source->pause();
        vid_sink->pause();
        playing = false;
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void VideoPlayer::movieStateChanged(QMovie::MovieState state)
{
    qDebug() << "videoplayer.cpp movie changed state to:" << state;
    switch(state) {
    case QMovie::NotRunning: // state = 0
    case QMovie::Paused:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    case QMovie::Running: // state = 2
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    }
}

void VideoPlayer::frameChanged(const QImage &image)
{
    // Present QImage in widget:
    presentImage(image);

}

void VideoPlayer::setPosition(int frame)
{
    qDebug("this function not implemented");
//    movie.jumpToFrame(frame);
}

bool VideoPlayer::presentImage(const QImage &image)
{

    QVideoFrame frame(image);

    if (!frame.isValid())
        return false;

//    qDebug() << "videoplayer.cpp presenting new image with size" << frame.size();

    QVideoSurfaceFormat currentFormat = surface->surfaceFormat();

    if (frame.pixelFormat() != currentFormat.pixelFormat()
            || frame.size() != currentFormat.frameSize()) {
        QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());

        if (!surface->start(format))
            return false;
    }
    if (!surface->present(frame)) {
        surface->stop();

        return false;
    } else {
        return true;
    }
}

void VideoPlayer::convert_to_qimage_and_signal(int ffmpeg_pix_format, int width, int height, AVFrame* data_frame)
{
//    qDebug("converting to qimage");
//    Convert to Qimage. FFmpeg pixel format is discarded -> we know that RGB24 is used, and choose the QT-version (RGB888) of this format manually.
    QImage image = QImage((uchar*)data_frame->data[0], width, height, data_frame->linesize[0], QImage::Format_RGB888);
    emit new_image_ready(image);
//    emit signal with Qimage as arg. Remember to do qRegisterType<Qimage> before making signal connection.
}
