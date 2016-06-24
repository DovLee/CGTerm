/*
 * gravityball.h
 *
 *  Created on: Jun 6, 2016
 *      Author: Windows10
 */
#ifndef __gravityball_H__
#define __gravityball_H__

#include <app.h>
#include <Elementary.h>
#include <runtime_info.h>
#include <efl_extension.h>
#include <dlog.h>
#include <sensor.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "gravityball"

#if !defined(PACKAGE)
#define PACKAGE "org.tizen.gravityball"
#endif

#define KEY_END "XF86Stop"
#define ACCELEROMETER_INTERVAL_MS 20
#define BALL_DIAMETER 50
#define ELASTICITY 0.7
#define RESISTANCE 0.05
#define ACCURACY 10

#endif /* __gravityball_H__ */
