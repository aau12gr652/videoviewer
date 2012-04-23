videoviewer
===========

Modified Qt example code, used for displaying video. Utilizes the FFmpeg provided libraries to decode video, instead of QMovie.

Initialize libhollywood submodule with:

* `git submodule init`

* `git submodule update`

Build libhollywood with:

* `cd hollywood`

* `./waf configure build`

A static library should after a succesful build be located in the build folder.

# Include dependency libraries to .pro file using Qt-creator:
Please follow these instructions to include dependency libraries using Qt-creator:

* Open .pro file in Qt-creater.
* Right-click in text editor -> Add Library.. -> External Library
* Navigate to both library file and include directory.
* Remove tick marks for platforms not supported by the respective library.

Do this for all dependency libraries.

All dependency libraries are listed below:

* `boost` - An easy-to-use version can be found at `git://github.com/steinwurf/external-boost.git`
* `libavformat`, `libavcodec`, `libavutil` and `libswscale` - An easy-to-use version can be found at `git://github.com/steinwurf/external-ffmpeg.git`
* `libhollywood` - This is included as a submodule. Alternatively, it can be cloned from the repository at `git://github.com/jepkri/hollywood.git`
