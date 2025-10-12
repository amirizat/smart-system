#include "lvgl.h"
#include "lvgl_helpers.h"
#include "target.h"

void init();
#define BUTTON_NOBORDER		0
#define BUTTON_BORDER1		1
#define BUTTON_BORDER2		2
#define BUTTON_BORDERALL	0xFF

//static void lv_tick_task(void *arg);
void display_init();
void set_ui_timer();
void photo_display(int index);
void clear_display(void);
void clear_header_display(void);
void clear_content_display(void);
void content_set_bg(lv_color_t bgcolor);
void content_set_bg_2(lv_color_t bgcolor);
void set_foot_bg(lv_color_t bgcolor);
void img_show_gpro(void);
void img_show_server(void);
void get_server(const char *txt,T8U x);
void verifying(T8U seconds);
void title_display(const char *txt,int x,int y);
void text_display(const char *txt,int x,int y);
void text_select_display(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_display_with_bg(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_display_with_bg_alignX(const char *txt,int y, lv_color_t textcolor, lv_color_t bgcolor, lv_align_t align);
void num_display(int num,int x,int y);
void num_select_display(int num,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void border_display(void);
void border_custom_display(int x, int xlen, int y, int ylen);
void two_button_display(const char *txt1, const char *txt2, int x1, int y1, int x2, int y2, int border);
void header_display(void);
void draw_contentLines(void);
void draw_headFootLines(void);
void text_8(const char *txt,int x,int y, lv_color_t textcolor);
void text_8_bold1(const char *txt,int x,int y, lv_color_t textcolor);
void text_8_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_10(const char *txt,int x,int y, lv_color_t textcolor);
void text_10_bold1(const char *txt,int x,int y, lv_color_t textcolor);
void text_10_bold2(const char *txt,int x,int y, lv_color_t textcolor);
void text_12_bold1(const char *txt,int x,int y, lv_color_t textcolor);
void text_12_bold2(const char *txt,int x,int y, lv_color_t textcolor);
void text_12_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_14_bold1(const char *txt,int x,int y, lv_color_t textcolor);
void text_14_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_14_bold2(const char *txt,int x,int y, lv_color_t textcolor);
void text_14_bold3_select(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_18_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_18_bold1(const char *txt,int x,int y, lv_color_t textcolor);
void text_20_bold1(const char *txt,int x,int y, lv_color_t textcolor);
void text_20_bold1_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_20_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_20_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_20_bold3_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_20_bold3_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_20_bold4_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_22_bold1(const char *txt,int x,int y, lv_color_t textcolor);
void text_22_bold1_2(const char *txt,int x,int y, lv_color_t textcolor,lv_color_t bgcolor);
void text_22_bold2(const char *txt,int x,int y, lv_color_t textcolor);
void text_22_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_22_bold3_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_22_bold3_midFix_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_28_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_28_bold1_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_28_bold2_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_28_bold2_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_28_bold3_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_28_bold3_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_28_bold3_midFix2(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_28_bold4_midFix_wBG(const char *txt,int x,int y, lv_color_t textcolor, lv_color_t bgcolor);
void text_48_bold1_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_48_bold1_endFix(const char *txt,int x,int y, lv_color_t textcolor);
void text_48_bold2_midFix(const char *txt,int x,int y, lv_color_t textcolor);
void img_show_trendDown(void);
void img_show_trendUp(void);
void img_show_eyeIcon(void);
void img_show_lineProd(void);
void img_show_gpro2(void);
void img_show_success(void);
void img_show_warning(void);
void img_show_scanBundle(void);
void img_show_scanEmployee(void);
void img_show_scanLogin(void);
void text_48_bold1_bg_alignX(const char *txt,int y, lv_color_t textcolor, lv_color_t bgcolor, lv_align_t align);
void draw_box(T16U x1, T16U x2, T16U y1, T16U y2, T16U width, lv_color_t color);
void draw_line_y2(void);
void draw_contentline_split(void);
char keyboard(T32S btnCount);
void display_tbox_kybrd(char tempstring[20],char tempstring2[20], T8U cursor, T32S btnCount);
extern T8U flag_kybd;
extern T8U flag_txtbox;
