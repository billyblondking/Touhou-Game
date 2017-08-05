﻿#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#ifndef STYLE_CONFIG_H
#define STYLE_CONFIG_H

#include <string>

using namespace std;

enum class StyleType
{
    SCATTER,
    ODDEVEN,
};

struct BulletConfig
{
    string name;
    int harm;
};

struct StyleConfig
{

    /* Style类型 */
    StyleType style;

    /* 公共参数 */
    float duration;  //持续时间
    float frequency; //频率
    int count;       //子弹连发数
    int number;      //条数

    /* 子弹参数 */
    BulletConfig bc;

    /* Scatter参数 */
    int startAngle; //起始角度
    int endAngle;   //终止角度
};

#endif