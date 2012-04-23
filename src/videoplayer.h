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

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QtGui/QMovie>
#include <QtGui/QWidget>

#include <hollywood_source.hpp>
#include <hollywood_sink.hpp>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractVideoSurface;
class QSlider;
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    void testsig(int i1, int i2, int i3, AVFrame* f);
    void convert_to_qimage_and_signal(int ffmpeg_pix_format, int width, int height, AVFrame* data_frame);

public slots:
    void openFile();
    void play();

private slots:
    void movieStateChanged(QMovie::MovieState state);
//    void frameChanged(int frame);
    void frameChanged(const QImage &image);
    void setPosition(int frame);

signals:
    void new_image_ready(const QImage &image);

private:
    bool presentImage(const QImage &image);
    bool playing;

    hollywood_source *vid_source;
    hollywood_sink *vid_sink;

    QAbstractVideoSurface *surface;
    QAbstractButton *playButton;
    QSlider *positionSlider;
};

#endif
