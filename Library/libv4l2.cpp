#include "libv4l2.h"

v4l2Cam::v4l2Cam(uint videoNum){
#ifdef DEBUGMODE
    cout << "Turning on video" << videoNum << endl;
#endif
    sprintf(cameraaddr,"/dev/video%d",videoNum);
    if ((fd = open(cameraaddr, O_RDWR)) == -1){
#ifdef DEBUGMODE
        cout << "Opening video device " << cameraaddr << " error!" << endl;
#endif
        return;
    }
    queryam();
}

int v4l2Cam::queryam(){
    struct v4l2_capability cap;
    memset(&cap, 0, sizeof(struct v4l2_capability));
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1){
#ifdef DEBUGMODE
        cout << "ERROR:unable Querying Capabilities" << endl;
#endif
        return -1;
    }
    devName = string ((const char*)cap.card);

    struct v4l2_fmtdesc fmtdesc;
    struct v4l2_frmsizeenum frmsize;
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) != -1){
        formatType type;
        type.index = fmtdesc.index++;
        type.description = string((const char*)fmtdesc.description);
        frmsize.pixel_format = fmtdesc.pixelformat;
        frmsize.index = 0;
        while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0){
            stringstream ss;
            if(frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE){
                ss << frmsize.discrete.width << "x" << frmsize.discrete.height;
                type.framesize.push_back(ss.str());
            }else if(frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE){
                ss << frmsize.discrete.width << "x" << frmsize.discrete.height;
                type.framesize.push_back(ss.str());
            }
            frmsize.index++;
        }
        FormatType.push_back(type);
    }
#ifdef DEBUGMODE
    cout << "driver:    " << cap.driver << endl;
    cout << "card:      " << cap.card << endl;
    cout << "bus_info:  " << cap.bus_info << endl;
    cout << "version:   " << cap.version << endl;
    if((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE)
        cout << "Camera support capture." << endl;
    else if((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
        cout << "Camera support streaming." << endl;
#endif
    return 0;
}

int v4l2Cam::setPixelformat(__u32 width, __u32 height, __u32 format, __u32 field){
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2img.imgwidth = fmt.fmt.pix.width = width;
    v4l2img.imgheight = fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = format;
    fmt.fmt.pix.field = field;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1){
#ifdef DEBUGMODE
        cout << "Setting Pixel Format failed" << endl;
#endif
        return -1;
    }
    if (ioctl(fd, VIDIOC_G_FMT, &fmt) == -1){
#ifdef DEBUGMODE
        cout << "Setting Pixel Format failed" << endl;
#endif
        return -1;
    }
    imgWidth = Width = fmt.fmt.pix.width;
    imgHeight = Height = fmt.fmt.pix.height;
#ifdef DEBUGMODE
    cout << "Setted Pixel Format: " << width << " x " << height << endl;
#endif
    return 0;
}

int v4l2Cam::setFps(__u32 fps, bool HDmode){
    struct v4l2_streamparm str;
    memset(&str, 0, sizeof(struct v4l2_streamparm));
    str.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    str.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
    if(HDmode)
        str.parm.capture.capturemode = V4L2_MODE_HIGHQUALITY;
    str.parm.capture.timeperframe.numerator = 1;
    str.parm.capture.timeperframe.denominator = fps;
    if (ioctl(fd, VIDIOC_S_PARM, &str) == -1){
#ifdef DEBUGMODE
        cout << "Setting FPS failed" << endl;
#endif
        return -1;
    }
    if (ioctl(fd, VIDIOC_G_PARM, &str) == -1){
#ifdef DEBUGMODE
        cout << "Setting FPS failed" << endl;
#endif
        return -1;
    }
    Fps = str.parm.capture.timeperframe.denominator;
#ifdef DEBUGMODE
    cout << "Setted FPS: " << fps << endl;
#endif
    return 0;
}

int v4l2Cam::memsetCam(){
    struct v4l2_requestbuffers req;
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1){
#ifdef DEBUGMODE
        cout << "Requesting Buffer error" << endl;
#endif
        return -1;
    }

    v4l2img.addr = (uchar*)malloc(req.count * sizeof(*v4l2img.addr));
    if(!v4l2img.addr){
#ifdef DEBUGMODE
        cout << "Out of memory" << endl;
#endif
        return -1;
    }
    for(__u32 n_buffers = 0;n_buffers < req.count; n_buffers++){
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        if(ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1){
#ifdef DEBUGMODE
            cout << "Querying Buffer error" << endl;
#endif
            return -1;
        }
        v4l2img.addr = (uchar*)mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if(v4l2img.addr == MAP_FAILED){
#ifdef DEBUGMODE
            cout << "Buffer map error" << endl;
#endif
            return -1;
        }
    }
    for(__u32 n_buffers = 0;n_buffers < req.count; n_buffers++){
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        if(ioctl(fd, VIDIOC_QBUF, &buf) == -1){
#ifdef DEBUGMODE
            cout << "Querying Buffer error" << endl;
#endif
            return -1;
        }
    }
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd,VIDIOC_STREAMON,&type) == -1){
#ifdef DEBUGMODE
        cout << "Stream on error" << endl;
#endif
        return -1;
    }

    Camera_Read();
    return 0;
}

int v4l2Cam::getBrightness(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_BRIGHTNESS;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getContrast(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_CONTRAST;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getHUE(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_HUE;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getSaturation(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_SATURATION;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getGAMMA(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_GAMMA;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getWhiteBalance(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_WHITENESS;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getAutoWhiteBalance(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_AUTO_WHITE_BALANCE;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return (bool)ctrl.value;
}

int v4l2Cam::getGain(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_GAIN;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getAutoGain(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_AUTOGAIN;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return (bool)ctrl.value;
}

int v4l2Cam::getExposure(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_EXPOSURE;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    return ctrl.value;
}

int v4l2Cam::getAutoExposure(){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_EXPOSURE_AUTO;
    ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
    if(ret != 0) return -255;
    if(ctrl.value == 1)
        return false;
    else
        return true;
}


int v4l2Cam::setBrightness(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_BRIGHTNESS;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setContrast(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_CONTRAST;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setHUE(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_HUE;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setSaturation(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_SATURATION;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setGAMMA(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_GAMMA;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setWhiteBalance(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_WHITENESS;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setAutoWhiteBalance(bool value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_AUTO_WHITE_BALANCE;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setGain(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_GAIN;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setAutoGain(bool value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_AUTOGAIN;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setExposure(int value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_EXPOSURE;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

int v4l2Cam::setAutoExposure(bool value){
    struct v4l2_control ctrl;
    int ret;
    ctrl.id = V4L2_CID_EXPOSURE_AUTO_PRIORITY;
    ctrl.value = value;
    ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
    if(ret != 0) return -1;
    return 0;
}

v4l2pic* v4l2Cam::Camera_Read(){
    ioctl(fd,VIDIOC_DQBUF,&buf);
    buf.index = 0;
    memcpy(&pic2send,&v4l2img,sizeof(v4l2img));
    ioctl(fd,VIDIOC_QBUF,&buf);
    return &pic2send;
}

int v4l2Cam::releaseMen(){
    if(buf.length > 1){
        munmap(v4l2img.addr,buf.length);
        close(fd);
        return 0;
    }
    else
        return -1;
}
