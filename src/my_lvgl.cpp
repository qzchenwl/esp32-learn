#include <lvgl.h>
#include "my_lvgl.h"
#include "my_screen.h"

static lv_disp_drv_t disp_drv;
static int32_t x1_flush;
static int32_t y1_flush;
static int32_t x2_flush;
static int32_t y2_fill;
static int32_t y_fill_act;
static const lv_color_t *buf_to_flush;

static void tft_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
  /*Return if the area is out the screen*/
  if (area->x2 < 0)
    return;
  if (area->y2 < 0)
    return;
  if (area->x1 > TFT_HEIGHT - 1)
    return;
  if (area->y1 > TFT_WIDTH - 1)
    return;

  /*Truncate the area to the screen*/
  int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
  int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
  int32_t act_x2 = area->x2 > TFT_HEIGHT - 1 ? TFT_HEIGHT - 1 : area->x2;
  int32_t act_y2 = area->y2 > TFT_WIDTH - 1 ? TFT_WIDTH - 1 : area->y2;

  uint32_t w = (act_x2 - act_x1 + 1);
  uint32_t h = (act_y2 - act_y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(act_x1, act_y1, w, h);
  tft.pushColors((uint16_t *)color_p, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(&disp_drv);
}

/**
 * Initialize your display here
 */
void setup_my_lvgl()
{
  lv_init();

  static lv_color_t disp_buf1[TFT_HEIGHT * 40];
  static lv_disp_draw_buf_t buf;
  lv_disp_draw_buf_init(&buf, disp_buf1, NULL, TFT_HEIGHT * 40);

  lv_disp_drv_init(&disp_drv);

  disp_drv.draw_buf = &buf;
  disp_drv.flush_cb = tft_flush;
  disp_drv.hor_res = TFT_HEIGHT;
  disp_drv.ver_res = TFT_WIDTH;
  lv_disp_drv_register(&disp_drv);
}

void my_lvgl_demo() {
  // 创建一个简单的标签
  lv_obj_t *label = lv_label_create(lv_scr_act()); // 创建标签并附加到活动屏幕
  lv_label_set_text(label, "Hello, LVGL!"); // 设置标签文本
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 10); // 居中对齐标签

  // 创建一个按钮
  lv_obj_t *btn = lv_btn_create(lv_scr_act()); // 创建按钮
  lv_obj_set_pos(btn, 40, 40); // 设置按钮位置
  lv_obj_set_size(btn, 50, 30); // 设置按钮大小

  // 为按钮添加标签
  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click");
}