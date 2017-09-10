#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>
#include <malloc.h>

using namespace std;

typedef unsigned char uchar;

struct v4l2Set{
    __s32 exposureType;
    __s32 exposure;
    __s32 gain;
    __s32 whiteBalance;
    __s32 brightness;
    __s32 saturation;
    __s32 contrast;
};

struct formatType{
    int index;
    string description;
    vector<string> framesize;
};

struct v4l2pic{
    int imgwidth;
    int imgheight;
    uchar* addr;
};

class v4l2Cam
{
public:
    v4l2Cam(uint videoNum);
    v4l2pic *Camera_Read();
    v4l2pic pic2send;
    string devName;
    void Camera_Set(v4l2Set set);
    __u32 Width, Height, Fps;
    vector<formatType> FormatType;
    int setPixelformat(__u32 width = 640, __u32 height = 480, __u32 format = V4L2_PIX_FMT_MJPEG, __u32 field = V4L2_FIELD_NONE);
    int setFps(__u32 fps = 60, bool HDmode = false);
    int memsetCam();
    int queryam();
    int releaseMen();

    int setBrightness(int value);
    int setContrast(int value);
    int setHUE(int value);
    int setSaturation(int value);
    int setGAMMA(int value);
    int setWhiteBalance(int value);
    int setAutoWhiteBalance(bool value);
    int setGain(int value);
    int setAutoGain(bool value);
    int setExposure(int value);
    int setAutoExposure(bool value);

    int getBrightness();
    int getContrast();
    int getHUE();
    int getSaturation();
    int getGAMMA();
    int getWhiteBalance();
    int getAutoWhiteBalance();
    int getGain();
    int getAutoGain();
    int getExposure();
    int getAutoExposure();

private:
    char cameraaddr[15];
    int fd;
    struct v4l2_buffer buf;
    v4l2pic v4l2img;
    __u32 imgWidth, imgHeight;
};

#endif // CAMERA_H
