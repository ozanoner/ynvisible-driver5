#pragma once

#ifdef __has_include
#if __has_include("lvgl.h")
#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#define LV_LVGL_H_INCLUDE_SIMPLE
#endif
#endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

extern const lv_image_dsc_t disp434v1pv1;
extern const lv_image_dsc_t disp437v2pv1;
extern const lv_image_dsc_t disp438v2pv1;
extern const lv_image_dsc_t disp431v2pv1;
extern const lv_image_dsc_t disp443v2pv1;
extern const lv_image_dsc_t disp433v1pv1;
extern const lv_image_dsc_t disp442v2pv1;
extern const lv_image_dsc_t disp444v1pv2;
extern const lv_image_dsc_t disp440v2pv1;