#include "lvgl.h"
#include "lvgl_helpers.h"
#include "display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "rtc_sntp.h"
#include "table.h"
#include "util.h"

#define DISP_BUF_SIZE (LV_HOR_RES_MAX * 40)
#define LV_TICK_PERIOD_MS 	10

// Objects
static lv_obj_t * scene_header;
static lv_obj_t * scene_content;

// Styles
static lv_style_t style_header_bg;
static lv_style_t style_content_bg;
static lv_style_t style_content_bg_2;
static lv_style_t style_foot;
static lv_style_t style_title;
static lv_style_t style_txt;
static lv_style_t style_txt_bg_allgn;
static lv_style_t style_txt_select;
static lv_style_t style_txt_bg;
static lv_style_t style_border;
static lv_style_t style_border_custom;
static lv_style_t style_but;
static lv_style_t style_but_sel;
static lv_style_t style_box;

// Points (adjusted to 240x135)
static lv_point_t p_header[] = {{0, 10}, {240, 10}};
static lv_point_t p_content[] = {{0, 45}, {240, 45}};
static lv_point_t p_content_2[] = {{0, 45}, {240, 45}};
static lv_point_t p_border[] = {{2, 15}, {238, 15}, {238, 134}, {2, 134}, {2, 15}};
static lv_point_t p_border_Custom[5]; // Assign dynamically if needed

// Text styles
static lv_style_t style_txt_8;
static lv_style_t style_txt_8_bold1;
static lv_style_t style_txt_10;
static lv_style_t style_txt_10_bold1;
static lv_style_t style_txt_10_bold2;
static lv_style_t style_txt_12_bold1;
static lv_style_t style_txt_12_bold2;
static lv_style_t style_txt_14_bold0_wBG;
static lv_style_t style_txt_14_bold1;
static lv_style_t style_txt_14_bold2;
static lv_style_t style_txt_14_bold3_wBG;
static lv_style_t style_txt_18_wBG;
static lv_style_t style_txt_18_bold1;
static lv_style_t style_txt_20_bold1;
static lv_style_t style_txt_20_bold2;
static lv_style_t style_txt_20_bold3;
static lv_style_t style_txt_20_bold4;
static lv_style_t style_txt_22_bold1;
static lv_style_t style_txt_22_bold1_wbg;
static lv_style_t style_txt_22_bold2;
static lv_style_t style_txt_22_bold3_wBG;
static lv_style_t style_txt_28_bold1;
static lv_style_t style_txt_28_bold2;
static lv_style_t style_txt_28_bold3;
static lv_style_t style_txt_28_bold3_wbg;
static lv_style_t style_txt_28_bold4_wBG;
static lv_style_t style_txt_30;
static lv_style_t style_txt_40;
static lv_style_t style_txt_48_bold1;
static lv_style_t style_txt_48_bold2;
static lv_style_t style_txt_48_bold1_bg_allgn;

// Lines (adjusted)
static lv_point_t p_xline1[] = {{0, 15}, {240, 15}};
static lv_point_t p_xline2[] = {{0, 45}, {240, 45}};
static lv_point_t p_xline3[] = {{0, 70}, {240, 70}};
static lv_point_t p_xline5[] = {{0, 95}, {240, 95}};
static lv_point_t p_xline6[] = {{0, 110}, {240, 110}};
static lv_point_t p_xline7[] = {{0, 125}, {240, 125}};
static lv_point_t p_xline8[] = {{0, 85}, {240, 85}};
static lv_point_t p_xline9[] = {{0, 44}, {240, 44}};
static lv_point_t p_xline10[] = {{0, 65}, {240, 65}};

static lv_point_t p_yline1[] = {{120, 45}, {120, 95}};
static lv_point_t p_yline2[] = {{119, 65}, {119, 125}};
static lv_point_t p_yline3[] = {{130, 20}, {130, 70}};

static lv_point_t p_slopeR1[] = {{190, 70}, {200, 45}};
static lv_point_t p_slopeR2[] = {{180, 95}, {190, 70}};
static lv_point_t p_slopeL1[] = {{40, 45}, {50, 70}};
static lv_point_t p_slopeL2[] = {{50, 70}, {60, 95}};

// Line styles
static lv_style_t style_line1;
static lv_style_t style_line2;
static lv_style_t style_line3;


T8U init_display = 0;

void display_init()
{
	init_display = 1;
    lv_init();
    lvgl_driver_init();

    static lv_color_t buf1[DISP_BUF_SIZE];
    static lv_color_t buf2[DISP_BUF_SIZE];
    static lv_disp_buf_t disp_buf;

    lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    lv_obj_t * scr = lv_scr_act();
    lv_obj_reset_style_list(scr, LV_OBJ_PART_MAIN);
    lv_obj_set_style_local_bg_opa(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);

    scene_header = lv_obj_create(scr, NULL);
    lv_obj_reset_style_list(scene_header, LV_OBJ_PART_MAIN);
    lv_obj_set_size(scene_header, 320, 30);
    lv_obj_align(scene_header, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    scene_content = lv_obj_create(scr, NULL);
    lv_obj_reset_style_list(scene_content, LV_OBJ_PART_MAIN);
    lv_obj_set_size(scene_content, 320, 240);
    lv_obj_align(scene_content, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	//header bg
	lv_style_init(&style_header_bg);
	lv_style_set_line_color(&style_header_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&style_header_bg, LV_STATE_DEFAULT, 20);

    //content bg
	lv_style_init(&style_content_bg);
	lv_style_set_line_color(&style_content_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&style_content_bg, LV_STATE_DEFAULT, 220);

    //content bg 2
	lv_style_init(&style_content_bg_2);
	lv_style_set_line_color(&style_content_bg_2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&style_content_bg_2, LV_STATE_DEFAULT, 195);

	//foot
	lv_style_init(&style_foot);
    lv_style_set_bg_opa(&style_foot, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_foot, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_pad_top(&style_foot, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_bottom(&style_foot, LV_STATE_DEFAULT, 3);
    lv_style_set_pad_left(&style_foot, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_right(&style_foot, LV_STATE_DEFAULT, 320);

    //title
	lv_style_init(&style_title);
    lv_style_set_bg_opa(&style_title, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_title, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_pad_top(&style_title, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_bottom(&style_title, LV_STATE_DEFAULT, 3);
//	lv_style_set_pad_bottom(&style_txt, LV_STATE_DEFAULT, 3);
    lv_style_set_pad_left(&style_title, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_right(&style_title, LV_STATE_DEFAULT, 315);
    lv_style_set_text_color(&style_title, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_title, LV_STATE_DEFAULT,  LV_THEME_DEFAULT_FONT_TITLE);

    //text
	lv_style_init(&style_txt);
	lv_style_set_bg_opa(&style_txt, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_pad_top(&style_txt, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_bottom(&style_txt, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_left(&style_txt, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_right(&style_txt, LV_STATE_DEFAULT, 320);
	lv_style_set_text_color(&style_txt, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	//text select
	lv_style_init(&style_txt_select);
    lv_style_set_bg_opa(&style_txt_select, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_txt_select, LV_STATE_DEFAULT, LV_COLOR_TEAL);
    lv_style_set_pad_top(&style_txt_select, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_bottom(&style_txt_select, LV_STATE_DEFAULT, 3);
    lv_style_set_pad_left(&style_txt_select, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_right(&style_txt_select, LV_STATE_DEFAULT, 320);
    lv_style_set_text_color(&style_txt_select, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //text with bg
	lv_style_init(&style_txt_bg);
	lv_style_set_bg_opa(&style_txt_bg, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_bg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_pad_top(&style_txt_bg, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_bottom(&style_txt_bg, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_left(&style_txt_bg, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_right(&style_txt_bg, LV_STATE_DEFAULT, 3);
	lv_style_set_text_color(&style_txt_bg, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	//text with bg & allign
	lv_style_init(&style_txt_bg_allgn);
	lv_style_set_bg_opa(&style_txt_bg_allgn, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_bg_allgn, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_pad_top(&style_txt_bg_allgn, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_bottom(&style_txt_bg_allgn, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_left(&style_txt_bg_allgn, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_right(&style_txt_bg_allgn, LV_STATE_DEFAULT, 3);
	lv_style_set_text_color(&style_txt_bg_allgn, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	//border
	lv_style_init(&style_border);
	lv_style_set_line_color(&style_border, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_line_width(&style_border, LV_STATE_DEFAULT, 3);

	//border custom
	lv_style_init(&style_border_custom);
	lv_style_set_line_color(&style_border_custom, LV_STATE_DEFAULT, LV_COLOR_TEAL);
	lv_style_set_line_width(&style_border_custom, LV_STATE_DEFAULT, 2);

	//box
	lv_style_init(&style_box);
	lv_style_set_line_color(&style_box, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
	lv_style_set_line_width(&style_box, LV_STATE_DEFAULT, 50);

	//button
	lv_style_init(&style_but);
	lv_style_set_radius(&style_but, LV_STATE_DEFAULT, 30);
	lv_style_set_bg_opa(&style_but, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_but, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_pad_top(&style_but, LV_STATE_DEFAULT, 10);
	lv_style_set_pad_bottom(&style_but, LV_STATE_DEFAULT, 10);
	lv_style_set_pad_left(&style_but, LV_STATE_DEFAULT, 20);
	lv_style_set_pad_right(&style_but, LV_STATE_DEFAULT, 20);
	lv_style_set_text_color(&style_but, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	//button select
	lv_style_init(&style_but_sel);
	lv_style_set_radius(&style_but_sel, LV_STATE_DEFAULT, 30);
	lv_style_set_bg_opa(&style_but_sel, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_but_sel, LV_STATE_DEFAULT, LV_COLOR_SILVER);
	lv_style_set_pad_top(&style_but_sel, LV_STATE_DEFAULT, 10);
	lv_style_set_pad_bottom(&style_but_sel, LV_STATE_DEFAULT, 10);
	lv_style_set_pad_left(&style_but_sel, LV_STATE_DEFAULT, 20);
	lv_style_set_pad_right(&style_but_sel, LV_STATE_DEFAULT, 20);
	lv_style_set_text_color(&style_but_sel, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_border_width(&style_but_sel, LV_STATE_DEFAULT, 3);
	lv_style_set_border_color(&style_but_sel, LV_STATE_DEFAULT, LV_COLOR_BLUE);

	////////////////////test new ui///////////////////////////////////////////////////////////
	LV_FONT_DECLARE(lv_font_montserrat_8)
	lv_style_init(&style_txt_8);
	lv_style_set_bg_opa(&style_txt_8, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_8, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_8, LV_STATE_DEFAULT,  &lv_font_montserrat_8);

	LV_FONT_DECLARE(lv_font_montserrat_bold_8)
	lv_style_init(&style_txt_8_bold1);
	lv_style_set_bg_opa(&style_txt_8_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_8_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_8_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_8);

	LV_FONT_DECLARE(lv_font_montserrat_10)
	lv_style_init(&style_txt_10);
	lv_style_set_bg_opa(&style_txt_10, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_10, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_10, LV_STATE_DEFAULT,  &lv_font_montserrat_10);

	LV_FONT_DECLARE(lv_font_montserrat_bold_10)
	lv_style_init(&style_txt_10_bold1);
	lv_style_set_bg_opa(&style_txt_10_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_10_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_10_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_10);

	lv_style_init(&style_txt_10_bold2);
	lv_style_set_bg_opa(&style_txt_10_bold2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_10_bold2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_10_bold2, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_10);

	LV_FONT_DECLARE(lv_font_montserrat_bold_12)
	lv_style_init(&style_txt_12_bold1);
	lv_style_set_bg_opa(&style_txt_12_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_12_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_12_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_12);

	lv_style_init(&style_txt_12_bold2);
	lv_style_set_bg_opa(&style_txt_12_bold2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_12_bold2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_12_bold2, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_12);

	LV_FONT_DECLARE(lv_font_montserrat_bold_14)
	lv_style_init(&style_txt_14_bold0_wBG);
	lv_style_set_bg_opa(&style_txt_14_bold0_wBG, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_14_bold0_wBG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_14_bold0_wBG, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_14);

	lv_style_init(&style_txt_14_bold1);
	lv_style_set_bg_opa(&style_txt_14_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_14_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_14_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_14);

	lv_style_init(&style_txt_14_bold2);
	lv_style_set_bg_opa(&style_txt_14_bold2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_14_bold2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_14_bold2, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_14);

	lv_style_init(&style_txt_14_bold3_wBG);
	lv_style_set_bg_opa(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_pad_top(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_bottom(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, 3);
    lv_style_set_pad_left(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_right(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, 320);
    lv_style_set_text_color(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_14);

	lv_style_init(&style_txt_18_wBG);
	lv_style_set_bg_opa(&style_txt_18_wBG, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_18_wBG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_18_wBG, LV_STATE_DEFAULT,  &lv_font_montserrat_18);

	LV_FONT_DECLARE(lv_font_montserrat_bold_18)
	lv_style_init(&style_txt_18_bold1);
	lv_style_set_bg_opa(&style_txt_18_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_18_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_18_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_18);

	LV_FONT_DECLARE(lv_font_montserrat_bold_20)
	lv_style_init(&style_txt_20_bold1);
	lv_style_set_bg_opa(&style_txt_20_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_20_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_20_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_20);

	lv_style_init(&style_txt_20_bold2);
	lv_style_set_bg_opa(&style_txt_20_bold2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_20_bold2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_20_bold2, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_20);

	lv_style_init(&style_txt_20_bold3);
	lv_style_set_bg_opa(&style_txt_20_bold3, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_20_bold3, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_20_bold3, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_20);

	lv_style_init(&style_txt_20_bold4);
	lv_style_set_bg_opa(&style_txt_20_bold4, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_20_bold4, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_20_bold4, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_20);

	LV_FONT_DECLARE(lv_font_montserrat_bold_22)
	lv_style_init(&style_txt_22_bold1);
	lv_style_set_bg_opa(&style_txt_22_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_22_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_22_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_22);

	LV_FONT_DECLARE(lv_font_montserrat_bold_22)
	lv_style_init(&style_txt_22_bold1_wbg);
	lv_style_set_bg_opa(&style_txt_22_bold1_wbg, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_22_bold1_wbg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_22_bold1_wbg, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_22);

	lv_style_init(&style_txt_22_bold2);
	lv_style_set_bg_opa(&style_txt_22_bold2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_22_bold2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_22_bold2, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_22);

	lv_style_init(&style_txt_22_bold3_wBG);
	lv_style_set_bg_opa(&style_txt_22_bold3_wBG, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_22_bold3_wBG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_22_bold3_wBG, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_22);

	LV_FONT_DECLARE(lv_font_montserrat_bold_28)
	lv_style_init(&style_txt_28_bold1);
	lv_style_set_bg_opa(&style_txt_28_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_28_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_28_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_28);

	lv_style_init(&style_txt_28_bold2);
	lv_style_set_bg_opa(&style_txt_28_bold2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_28_bold2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_28_bold2, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_28);

	lv_style_init(&style_txt_28_bold3);
	lv_style_set_bg_opa(&style_txt_28_bold3, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_28_bold3, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_28_bold3, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_28);

	lv_style_init(&style_txt_28_bold3_wbg);
	lv_style_set_bg_opa(&style_txt_28_bold3_wbg, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_28_bold3_wbg, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_28_bold3_wbg, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_28);

	lv_style_init(&style_txt_28_bold4_wBG);
	lv_style_set_bg_opa(&style_txt_28_bold4_wBG, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_28_bold4_wBG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_28_bold4_wBG, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_28);

	LV_FONT_DECLARE(lv_font_montserrat_30)
	lv_style_init(&style_txt_30);
	lv_style_set_bg_opa(&style_txt_30, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_30, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_30, LV_STATE_DEFAULT,  &lv_font_montserrat_30);

	LV_FONT_DECLARE(lv_font_montserrat_40)
	lv_style_init(&style_txt_40);
	lv_style_set_bg_opa(&style_txt_40, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_40, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_40, LV_STATE_DEFAULT,  &lv_font_montserrat_40);

	LV_FONT_DECLARE(lv_font_montserrat_bold_48)
	lv_style_init(&style_txt_48_bold1);
	lv_style_set_bg_opa(&style_txt_48_bold1, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_48_bold1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_48_bold1, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_48);

	LV_FONT_DECLARE(lv_font_montserrat_bold_48)
	lv_style_init(&style_txt_48_bold2);
	lv_style_set_bg_opa(&style_txt_48_bold2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_48_bold2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_txt_48_bold2, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_48);

	lv_style_init(&style_txt_48_bold1_bg_allgn);
	lv_style_set_bg_opa(&style_txt_48_bold1_bg_allgn, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_txt_48_bold1_bg_allgn, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_color(&style_txt_48_bold1_bg_allgn, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_text_font(&style_txt_48_bold1_bg_allgn, LV_STATE_DEFAULT,  &lv_font_montserrat_bold_48);

	lv_style_init(&style_line1);
	lv_style_set_line_color(&style_line1, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_line_width(&style_line1, LV_STATE_DEFAULT, 1);

	lv_style_init(&style_line2);
	lv_style_set_line_color(&style_line2, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_line_width(&style_line2, LV_STATE_DEFAULT, 2);

	lv_style_init(&style_line3);
}

static void lv_tick_task(void *arg)
{
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}

void set_ui_timer()
{
	if(!init_display)return;
	const esp_timer_create_args_t periodic_timer_args = {
				.callback = &lv_tick_task,
				.name = "periodic_gui"
			};
	esp_timer_handle_t periodic_timer;
	ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
	ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
}

void clear_display(void)
{
	if(!init_display)return;
	clear_header_display();
	clear_content_display();
}

void clear_header_display(void)
{
	if(!init_display)return;
	lv_obj_clean(scene_header);
}

void clear_content_display(void)
{
	if(!init_display)return;
	lv_obj_clean(scene_content);
}

void header_set_bg(lv_color_t bgcolor)
{
	if(!init_display)return;

	lv_style_set_line_color(&style_header_bg, LV_STATE_DEFAULT, bgcolor);

	/*Create an object with the new style*/
	lv_obj_t * obj = lv_line_create(scene_header, NULL);
	lv_obj_add_style(obj, LV_LINE_PART_MAIN, &style_header_bg);

	lv_line_set_points(obj, p_header, 2);
	lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void content_set_bg(lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_line_color(&style_content_bg, LV_STATE_DEFAULT, bgcolor);
	/*Create an object with the new style*/
	lv_obj_t * obj = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj, LV_LINE_PART_MAIN, &style_content_bg);

	lv_line_set_points(obj, p_content, 2);
	lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void content_set_bg_2(lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_line_color(&style_content_bg_2, LV_STATE_DEFAULT, bgcolor);
	/*Create an object with the new style*/
	lv_obj_t * obj = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj, LV_LINE_PART_MAIN, &style_content_bg_2);

	lv_line_set_points(obj, p_content_2, 2);
	lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void set_foot_bg(lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_foot, LV_STATE_DEFAULT, bgcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_foot);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, " ");
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, 2, 210);
}

void img_show_gpro(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(GPROlogo);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 140, 70);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 35);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &GPROlogo);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_server(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(server);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 200, 140);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 60, 30);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &server);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

//void progress_bar(const char *txt, T8U x)
//{
//	clear_content_display();
//
//	img_show_server();
//
//    lv_obj_t *curved_box = lv_obj_create(scene_content, NULL);
//    lv_obj_set_size(curved_box, 270, 20);
//    lv_obj_set_style_local_radius(curved_box, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);  // Set radius for all corners
//    lv_obj_set_style_local_border_width(curved_box, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);  // Set border width
//    lv_obj_set_style_local_border_color(curved_box, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));  // Set border color
//    lv_obj_align(curved_box, NULL, LV_ALIGN_CENTER, 0, 80);
//
//    lv_obj_t *curved_box2 = lv_obj_create(scene_content, NULL);
//    lv_obj_set_size(curved_box2, x+50, 20);
//    lv_obj_set_style_local_radius(curved_box2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);  // Set radius for all corners
//    lv_obj_set_style_local_border_width(curved_box2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);  // Set border width
//    lv_obj_set_style_local_border_color(curved_box2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));  // Set border color
//    lv_obj_set_style_local_bg_color(curved_box2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(100, 150, 200));
//    lv_obj_align(curved_box2, NULL, LV_ALIGN_IN_LEFT_MID, 20, 80);
//
//    text_display(txt,20,130);
//
//}

void get_server(const char *txt, T8U percentage)
{
    clear_content_display();

    img_show_server();

    // Calculate the number of dots based on the percentage
    T8U num_dots = (percentage * 20) / 100;

    // Calculate the total width of the dots
    T8U total_width = 20 * 12; // Assuming each dot is separated by 12 units

    // Calculate the starting position to center the dots
    T8U start_position = (LV_HOR_RES - total_width) / 2;

    // Create dots
    for (T8U i = 0; i < 20; ++i)
    {
        lv_obj_t *dot = lv_obj_create(scene_content, NULL);
        lv_obj_set_size(dot, 10, 10);
        lv_obj_set_style_local_radius(dot, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);

        // Set dot color based on whether it represents progress or not
        if (i < num_dots) {
            lv_obj_set_style_local_bg_color(dot, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(100, 150, 200));
        } else {
            lv_obj_set_style_local_bg_color(dot, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(255, 255, 255));
        }

        lv_obj_align(dot, NULL, LV_ALIGN_IN_LEFT_MID, start_position + i * 12, 90);
    }

    // Create label
    lv_obj_t *label = lv_label_create(scene_content, NULL);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &style_txt_18_bold1);
    lv_label_set_text(label, txt);
    if(GetLineCount((T8U*)txt)==2)
    {
    	lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 160);  // Adjust the vertical alignment
    }
    else if(GetLineCount((T8U*)txt)==1)
    {
    	lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 175);  // Adjust the vertical alignment
    }
}

void verifying(T8U seconds)
{
	if(!init_display)return;

	clear_content_display();

	lv_obj_t *outer_box = lv_obj_create(scene_content, NULL);
    lv_obj_set_size(outer_box, 320, 240);
    lv_obj_set_style_local_bg_color(outer_box, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
    lv_obj_set_style_local_border_color(outer_box, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(255, 255, 255));
    lv_obj_set_style_local_border_width(outer_box, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 2);

    lv_obj_t *inner_box = lv_obj_create(scene_content, NULL);
    lv_obj_set_size(inner_box, 290, 210);
    lv_obj_align(inner_box, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(inner_box, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_40);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, "SCAN DONE");
	lv_obj_align(text, NULL, LV_ALIGN_CENTER, 0, -60);

	lv_style_set_text_color(&style_txt_30, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_obj_t * text2 = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text2, LV_LABEL_PART_MAIN, &style_txt_30);
	lv_theme_apply(text2, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text2, "Remove Tag");
	lv_obj_align(text2, NULL, LV_ALIGN_CENTER, 0, -15);

	lv_style_set_text_color(&style_txt_22_bold2, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
	lv_obj_t * text3 = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text3, LV_LABEL_PART_MAIN, &style_txt_22_bold2);
	lv_theme_apply(text3, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text3, "Verifying Logic...");
	lv_obj_align(text3, NULL, LV_ALIGN_CENTER, 0, 40);

    //seconds = (seconds > 5) ? 5 : seconds;
	T8U load = seconds % 5;

    // Create dots
    for (T8U i = 0; i < 5; ++i)
    {
        lv_obj_t *dot = lv_obj_create(scene_content, NULL);  // Use inner_box as the parent
        lv_obj_set_size(dot, 20, 20);
        lv_obj_set_style_local_radius(dot, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);

        // Set dot color based on whether it represents progress or not
        if ((i < load) || ((load == 0) && (seconds != 0)))//seconds)
        {
            lv_obj_set_style_local_bg_color(dot, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(100, 150, 200));
        }
        else
        {
            lv_obj_set_style_local_bg_color(dot, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(255, 255, 255));
        }

//        lv_obj_align(dot, NULL, LV_ALIGN_IN_LEFT_MID, 100 + i * 27, 75);
        lv_obj_align(dot, NULL, LV_ALIGN_IN_LEFT_MID, 80 + i * 35, 75);
    }

    // Create percentage label
    char seconds_str[5];
    lv_snprintf(seconds_str, sizeof(seconds_str), "%ds", seconds);

    lv_obj_t *seconds_label = lv_label_create(scene_content, NULL);
    lv_label_set_text(seconds_label, seconds_str);
    lv_obj_align(seconds_label, NULL, LV_ALIGN_OUT_LEFT_TOP, 300, 182);  // Adjust the vertical alignment

}

void title_display(const char *txt,int x,int y)
{
	if(!init_display)return;
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_title);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_display(const char *txt,int x,int y)
{
	if(!init_display)return;
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x,y);
}

void text_select_display(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
    lv_style_set_bg_color(&style_txt_select, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_select, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_select);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_display_with_bg(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_txt_bg, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_bg, LV_STATE_DEFAULT, textcolor);

    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_bg);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_display_with_bg_alignX(const char *txt,int y, lv_color_t textcolor, lv_color_t bgcolor, lv_align_t align)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_txt_bg_allgn, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_bg_allgn, LV_STATE_DEFAULT, textcolor);

    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_bg_allgn);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, align, 0, y);
}

void num_display(int num,int x,int y)
{
	if(!init_display)return;
	char no[3] = {};
	sprintf(no, "%u", num);

	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, no);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x,y);
}

void num_select_display(int num,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
	char no[3] = {};
	sprintf(no, "%u", num);

	lv_style_set_bg_color(&style_txt_select, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_select, LV_STATE_DEFAULT, textcolor);

    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_select);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, no);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x,y);
}

void border_display(void)
{
	if(!init_display)return;

	/*Create an object with the new style*/
	lv_obj_t * obj = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj, LV_LINE_PART_MAIN, &style_border);
	lv_line_set_points(obj, p_border, 5);
	lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
}

void border_custom_display(int x, int xlen, int y, int ylen)
{
	if(!init_display)return;

	/*Create an object with the new style*/
	lv_obj_t * obj = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj, LV_LINE_PART_MAIN, &style_border_custom);

	p_border_Custom[0].x = x;
	p_border_Custom[0].y = y;

	p_border_Custom[1].x = x + xlen;
	p_border_Custom[1].y = y;

	p_border_Custom[2].x = x + xlen;
	p_border_Custom[2].y = y + ylen;

	p_border_Custom[3].x = x;
	p_border_Custom[3].y = y + ylen;

	p_border_Custom[4].x = x;
	p_border_Custom[4].y = y;

	lv_line_set_points(obj, p_border_Custom, 5);

	lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
}

void two_button_display(const char *txt1, const char *txt2, int x1, int y1, int x2, int y2, int border)
{
	if(!init_display)return;
    /*Create an object with the new style*/
    lv_obj_t * obj1 = lv_label_create(scene_content, NULL);
    if((border == BUTTON_BORDER1) || (border == BUTTON_BORDERALL))
    {
    	lv_obj_add_style(obj1, LV_LABEL_PART_MAIN, &style_but_sel);
    }
    else
    {
    	lv_obj_add_style(obj1, LV_LABEL_PART_MAIN, &style_but);
    }
    lv_label_set_text(obj1, txt1);
    lv_obj_align(obj1, NULL, LV_ALIGN_IN_TOP_LEFT, x1, y1);

    /*Create an object with the new style*/
    lv_obj_t * obj2 = lv_label_create(scene_content, NULL);
    if((border == BUTTON_BORDER2) || (border == BUTTON_BORDERALL))
    {
    	lv_obj_add_style(obj2, LV_LABEL_PART_MAIN, &style_but_sel);
    }
    else
    {
    	lv_obj_add_style(obj2, LV_LABEL_PART_MAIN, &style_but);
    }
    lv_label_set_text(obj2, txt2);
    lv_obj_align(obj2, NULL, LV_ALIGN_IN_TOP_LEFT, x2, y2);
}

/////////////////////////test new ui/////////////////////////////////////////////////////////////////
void img_show_person_icon(void)
{
	if(!init_display)return;
	LV_IMG_DECLARE(personIcon);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 20, 20);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &personIcon);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 9, 10);
}

void employee_display(void)
{
	if(!init_display)return;

	if(CHK_EMPIN() == OFF)return;

	char tempstring[20];

	sprintf(tempstring, "%.10s", tbl_emp.empcode);

    lv_style_set_text_color(&style_txt_14_bold1, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_header, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_14_bold0_wBG);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, tempstring);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, 17, 3);
}

void clock_display(void)
{
	if(!init_display)return;

	char clock[30];
	char tempstring[5];
	T8U temp = 0;

	if(currenttime.month == 1)
	{
		sprintf(tempstring, "Jan");
	}
	else if(currenttime.month == 2)
	{
		sprintf(tempstring, "Feb");
	}
	else if(currenttime.month == 3)
	{
		sprintf(tempstring, "Mar");
	}
	else if(currenttime.month == 4)
	{
		sprintf(tempstring, "Apr");
	}
	else if(currenttime.month == 5)
	{
		sprintf(tempstring, "May");
	}
	else if(currenttime.month == 6)
	{
		sprintf(tempstring, "Jun");
	}
	else if(currenttime.month == 7)
	{
		sprintf(tempstring, "Jul");
	}
	else if(currenttime.month == 8)
	{
		sprintf(tempstring, "Aug");
	}
	else if(currenttime.month == 9)
	{
		sprintf(tempstring, "Sep");
	}
	else if(currenttime.month == 10)
	{
		sprintf(tempstring, "Oct");
	}
	else if(currenttime.month == 11)
	{
		sprintf(tempstring, "Nov");
	}
	else if(currenttime.month == 12)
	{
		sprintf(tempstring, "Dec");
	}

	if(currenttime.hour >= 12)
	{
		if(currenttime.hour > 12)
		{
			temp = currenttime.hour - 12;
		}
		else
		{
			temp = currenttime.hour;
		}
		sprintf(clock, "%02u %s %02u:%02u PM", currenttime.day, tempstring, temp, currenttime.minute);
	}
	else
	{
		sprintf(clock, "%02u %s %02u:%02u AM", currenttime.day, tempstring, currenttime.hour, currenttime.minute);
	}

	lv_obj_t * text = lv_label_create(scene_header, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_14_bold0_wBG);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, clock);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, 261, 3);//LV_ALIGN_IN_TOP_LEFT, 148, 3);
}

void img_show_send_icon(void)
{
	if(!init_display)return;
	LV_IMG_DECLARE(send);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 320, 20);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &send);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 271, 10);
}

void img_show_online_icon(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(online_arrow);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 320, 20);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &online_arrow);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 289, 10);
}

void img_show_wifi_icon(void)
{
	if(!init_display)return;
	LV_IMG_DECLARE(wifi);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 320, 20);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &wifi);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 308, 10);
}

void img_show_wifi_excellent_icon(void)
{
	if(!init_display)return;
	LV_IMG_DECLARE(wifi_excellent);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 320, 21);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &wifi_excellent);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 308, 10);
}

void img_show_wifi_good_icon(void)
{
	if(!init_display)return;
	LV_IMG_DECLARE(wifi_good);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 320, 21);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &wifi_good);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 308, 10);
}

void img_show_wifi_fair_icon(void)
{
	if(!init_display)return;
	LV_IMG_DECLARE(wifi_fair);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 320, 21);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &wifi_fair);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 308, 10);
}

void img_show_wifi_weak_icon(void)
{
	if(!init_display)return;
	LV_IMG_DECLARE(wifi_weak);

	lv_obj_t * icon;
	lv_obj_t * cont = lv_cont_create(scene_header, NULL);
	lv_obj_set_size(cont, 320, 21);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	icon = lv_img_create(cont, NULL);
	lv_img_set_src(icon, &wifi_weak);
	lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 308, 10);
}

void header_display(void)
{
	clear_header_display();

	if(FLAG_STATE & F_NEWFIRMDONE)
	{
		header_set_bg(LV_COLOR_LIME);
		lv_style_set_bg_color(&style_txt_14_bold0_wBG, LV_STATE_DEFAULT, LV_COLOR_LIME);
	}
	else if(FLAG_STATE & F_OTASTART)
	{
		header_set_bg(LV_COLOR_YELLOW);
		lv_style_set_bg_color(&style_txt_14_bold0_wBG, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
	}
	else
	{
		lv_style_set_bg_color(&style_txt_14_bold0_wBG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	}

	img_show_person_icon();
	employee_display();
//	if(FLAG_STATE & F_TSYNCSERVER)
//	{
		clock_display();
//	}
	if(FLAG_STATE & F_CONNECTSERVER)
	{
		img_show_online_icon();
	}
	if(FLAG_STATE & F_SENDINGDATA)
	{
		img_show_send_icon();
	}
	if(FLAG_STATE & F_WIFICONNECT)
	{
//		img_show_wifi_icon();
		if(wifiRssi > -50)
		{
			ESP_LOGI("display", "wifi excellent %d", wifiRssi);
			img_show_wifi_excellent_icon();
		}
		else if((wifiRssi <= -50) && (wifiRssi > -60))
		{
			ESP_LOGI("display", "wifi good %d", wifiRssi);
			img_show_wifi_good_icon();
		}
		else if((wifiRssi <= -60) && (wifiRssi > -70))
		{
			ESP_LOGI("display", "wifi fair %d", wifiRssi);
			img_show_wifi_fair_icon();
		}
		if(wifiRssi <= -70)
		{
			img_show_wifi_weak_icon();
			ESP_LOGI("display", "wifi weak %d", wifiRssi);
		}
	}

}

void draw_contentLines(void)
{
	if(!init_display)return;

	lv_obj_t * obj1 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj1, LV_LINE_PART_MAIN, &style_line2);
	lv_line_set_points(obj1, p_xline1, 2);
	lv_obj_align(obj1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj2 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj2, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj2, p_xline2, 2);
	lv_obj_align(obj2, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj3 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj3, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj3, p_xline3, 2);
	lv_obj_align(obj3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj5 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj5, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj5, p_xline5, 2);
	lv_obj_align(obj5, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj6 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj6, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj6, p_xline6, 2);
	lv_obj_align(obj6, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj7 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj7, LV_LINE_PART_MAIN, &style_line2);
	lv_line_set_points(obj7, p_xline7, 2);
	lv_obj_align(obj7, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj8 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj8, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj8, p_yline1, 2);
	lv_obj_align(obj8, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj9 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj9, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj9, p_slopeR1, 2);
	lv_obj_align(obj9, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj10 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj10, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj10, p_slopeR2, 2);
	lv_obj_align(obj10, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj11 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj11, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj11, p_slopeL1, 2);
	lv_obj_align(obj11, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj12 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj12, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj12, p_slopeL2, 2);
	lv_obj_align(obj12, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void draw_headFootLines(void)
{
	if(!init_display)return;

	lv_obj_t * obj1 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj1, LV_LINE_PART_MAIN, &style_line2);
	lv_line_set_points(obj1, p_xline1, 2);
	lv_obj_align(obj1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj7 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj7, LV_LINE_PART_MAIN, &style_line2);
	lv_line_set_points(obj7, p_xline7, 2);
	lv_obj_align(obj7, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void text_8(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_8, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_8);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_8_bold1(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_8_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_8_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_8_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_8_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_8_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);
}

void text_10(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_10, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_10);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_10_bold1(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_10_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_10_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_10_bold2(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_10_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_10_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_12_bold1(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_12_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_12_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_12_bold2(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_12_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_12_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_12_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_12_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_12_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);
}

void text_14_bold1(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_14_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_14_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_14_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_14_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_14_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);
}

void text_14_bold2(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_14_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_14_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_14_bold3_select(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
    lv_style_set_bg_color(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_14_bold3_wBG, LV_STATE_DEFAULT, textcolor);

    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_14_bold3_wBG);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_18_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_txt_18_wBG, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_18_wBG, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_18_wBG);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_18_bold1(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_18_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_18_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_20_bold1(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_20_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_20_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_20_bold1_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_20_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_20_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_20_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_20_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_20_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP , x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);LV_ALIGN_OUT_LEFT_TOP
}

void text_20_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_20_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_20_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_20_bold3_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_20_bold3, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_20_bold3);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_20_bold3_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_20_bold3, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_20_bold3);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_20_bold4_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_20_bold4, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_20_bold4);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_22_bold1(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_22_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_22_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_22_bold1_2(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_txt_22_bold1_wbg, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_22_bold1_wbg, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_22_bold1_wbg);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_22_bold2(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_22_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_22_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_22_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_22_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_22_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_22_bold3_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_txt_22_bold3_wBG, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_22_bold3_wBG, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_22_bold3_wBG);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_22_bold3_midFix_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_txt_22_bold3_wBG, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_22_bold3_wBG, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_22_bold3_wBG);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_28_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_28_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_28_bold1_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_28_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_28_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_28_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_28_bold2_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_28_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_28_bold3_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_28_bold3, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold3);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_28_bold3_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_28_bold3, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold3);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_28_bold3_midFix2(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
    lv_style_set_bg_color(&style_txt_28_bold3_wbg, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_28_bold3_wbg, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold3_wbg);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_28_bold4_midFix_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor)
{
	if(!init_display)return;
	lv_style_set_bg_color(&style_txt_28_bold4_wBG, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_28_bold4_wBG, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_28_bold4_wBG);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT  , x, y);
}

void text_48_bold1_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_48_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_48_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_48_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_48_bold1, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_48_bold1);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, LV_ALIGN_OUT_LEFT_TOP, x, y);//LV_ALIGN_IN_TOP_LEFT, x, y);
}

void text_48_bold2_midFix(const char *txt,int x,int y, lv_color_t textcolor)
{
	if(!init_display)return;
    lv_style_set_text_color(&style_txt_48_bold2, LV_STATE_DEFAULT, textcolor);
    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_48_bold2);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align_origo(text, NULL, LV_ALIGN_IN_TOP_LEFT, x, y);
}

void img_show_trendDown(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(trendDown);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 60, 30);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 113);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &trendDown);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_trendUp(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(trendUp);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 60, 30);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 113);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &trendUp);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_eyeIcon(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(eyeIcon);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 25, 20);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 175);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &eyeIcon);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_lineProd(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(lineProd);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 20, 20);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 200);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &lineProd);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_gpro2(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(gproLogo15);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 65, 20);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 255, 219);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &gproLogo15);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_success(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(successWhite);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 128, 128);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 100);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &successWhite);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_warning(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(warning);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 100, 82);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 120);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &warning);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_scanBundle(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(scanBundle);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 260, 120);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 75);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &scanBundle);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_scanEmployee(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(scanEmp);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 260, 135);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 75);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &scanEmp);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void img_show_scanLogin(void)
{
	if(!init_display)return;

	LV_IMG_DECLARE(scanLogin);

	lv_obj_t * img1;

	lv_obj_t * cont = lv_cont_create(scene_content, NULL);
	lv_obj_set_size(cont, 260, 180);
	lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
	lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 36);

	img1 = lv_img_create(cont, NULL);
	lv_img_set_src(img1, &scanLogin);
	lv_obj_align_origo(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void text_48_bold1_bg_alignX(const char *txt,int y, lv_color_t textcolor, lv_color_t bgcolor, lv_align_t align)
{
	if(!init_display)return;

    lv_style_set_bg_color(&style_txt_48_bold1_bg_allgn, LV_STATE_DEFAULT, bgcolor);
    lv_style_set_text_color(&style_txt_48_bold1_bg_allgn, LV_STATE_DEFAULT, textcolor);

    /*Create an object with the new style*/
	lv_obj_t * text = lv_label_create(scene_content, NULL);
	lv_obj_add_style(text, LV_LABEL_PART_MAIN, &style_txt_48_bold1_bg_allgn);
	lv_theme_apply(text, _LV_THEME_BUILTIN_LAST);
	lv_label_set_text(text, txt);
	lv_obj_align(text, NULL, align, 0, y);
}

void draw_box(T16U x1, T16U x2, T16U y1, T16U y2, T16U width, lv_color_t color)
{
	if(!init_display)return;

	p_xline8[0].x = x1;
	p_xline8[0].y = y1;
	p_xline8[1].x = x2;
	p_xline8[1].y = y2;

	lv_style_set_line_color(&style_line3, LV_STATE_DEFAULT, color);
	lv_style_set_line_width(&style_line3, LV_STATE_DEFAULT, width);

	lv_obj_t * obj = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj, LV_LINE_PART_MAIN, &style_line3);
	lv_line_set_points(obj, p_xline8, 2);
	lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void draw_line_y2(void)
{
	if(!init_display)return;
	lv_obj_t * obj1 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj1, LV_LINE_PART_MAIN, &style_line2);
	lv_line_set_points(obj1, p_yline2, 2);
	lv_obj_align(obj1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

void draw_contentline_split(void)
{
	if(!init_display)return;
	lv_obj_t * obj1 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj1, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj1, p_xline9, 2);
	lv_obj_align(obj1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj2 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj2, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj2, p_xline10, 2);
	lv_obj_align(obj2, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * obj3 = lv_line_create(scene_content, NULL);
	lv_obj_add_style(obj3, LV_LINE_PART_MAIN, &style_line1);
	lv_line_set_points(obj3, p_yline3, 2);
	lv_obj_align(obj3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

T8U flag_kybd = 0;
T8U flag_txtbox = 0;

static const char * btnm_map[] = {"1", "2", "3","4", "5", "6","7","8","9","0","\n",
								  "q", "w","e", "r", "t", "y","u","i","o","p", "\n",
								  "a", "s","d", "f", "g", "h","j","k","l",";","\n",
								  "z", "x","c", "v", "b", "n","m",",",".","/","\n",
								  "<<", ">>", "A/a","[__]", "ENT",""};

static const char * btnm_map2[] = {"!", "@", "#", "$", "%", "&","*","(",")","_","\n",
								  "Q", "W","E", "R", "T", "Y","U","I","O","P", "\n",
								  "A", "S","D", "F", "G", "H","J","K","L", ":","\n",
								  "Z", "X","C", "V", "B", "N","M","[","]","?","\n",
								  "<<", ">>", "A/a","[__]","ENT",""};

void display_tbox_kybrd(char tempstring[20],char tempstring2[20], T8U cursor, T32S btnCount){
    lv_obj_t * label1 = lv_label_create(scene_content, NULL);
    lv_obj_t * ta2 = lv_textarea_create(scene_content,NULL);
    lv_label_set_text(label1, "SSID:");
    lv_obj_set_width(label1, 5);
    lv_obj_align(label1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 12);

    lv_obj_t * ta = lv_textarea_create(scene_content,NULL);
    lv_textarea_set_one_line(ta, true);
    lv_obj_align(ta,NULL, LV_ALIGN_IN_TOP_MID, 40, 5);

    if(flag_txtbox==0){

        lv_textarea_cursor_right(ta);
        lv_textarea_set_cursor_pos(ta, cursor);
        lv_textarea_set_cursor_hidden(ta2, YES);
     }
    lv_textarea_set_text(ta, tempstring);

    lv_obj_t * label2 = lv_label_create(scene_content, NULL);
    lv_label_set_text(label2, "PASS:");
    lv_obj_set_width(label2, 5);
    lv_obj_align(label2, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 54);

    lv_textarea_set_one_line(ta2,true);
    lv_obj_align(ta2,NULL, LV_ALIGN_IN_TOP_MID, 40, 47);

    if(flag_txtbox==1){

        lv_textarea_cursor_right(ta2);
        lv_textarea_set_cursor_pos(ta2, cursor);
        lv_textarea_set_cursor_hidden(ta, YES);
     }
    lv_textarea_set_text(ta2, tempstring2);


    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_pad_inner(&style, LV_STATE_DEFAULT, 2);

    lv_obj_t * btnm1 = lv_btnmatrix_create(scene_content, NULL);
    if(flag_kybd==0){
    	lv_btnmatrix_set_map(btnm1, btnm_map);
    }else if(flag_kybd==1){
    	lv_btnmatrix_set_map(btnm1, btnm_map2);
    }

    lv_obj_set_height(btnm1,145);
    lv_obj_set_width(btnm1,310);
    lv_btnmatrix_set_focused_btn(btnm1, btnCount);
    lv_obj_align(btnm1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    lv_obj_add_style(btnm1, LV_LABEL_PART_MAIN, &style);
}

char keyboard(T32S btnCount) {
    char val = '\0';

    if (flag_kybd == 0 && btnCount >= 0 && btnCount < sizeof(btnm_map) / sizeof(btnm_map[0])) {
        val = *btnm_map[btnCount];
    } else if (flag_kybd == 1 && btnCount >= 0 && btnCount < sizeof(btnm_map2) / sizeof(btnm_map2[0])) {
        val = *btnm_map2[btnCount];
    }

    return val;
}
