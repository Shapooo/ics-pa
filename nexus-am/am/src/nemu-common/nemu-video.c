#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define WIDTH 400
#define HEIGHT 300

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
  case _DEVREG_VIDEO_INFO: {
    _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
    info->width = WIDTH;
    info->height = HEIGHT;
    return sizeof(_DEV_VIDEO_INFO_t);
  }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
  case _DEVREG_VIDEO_FBCTL: {
    _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;

    int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
    uint32_t *pixels = ctl->pixels;
    int cp_size = (w < WIDTH - x ? w : WIDTH - x);
    for (int j = 0; j < h && y + j < HEIGHT; j++) {
      for (int i = 0; i < cp_size; ++i) {
        fb[(y + j) * WIDTH + x + i] = pixels[i];
      }
      pixels += w;
    }

    if (ctl->sync) {
      outl(SYNC_ADDR, 0);
    }
    return size;
  }
  }
  return 0;
}

void __am_vga_init() {
}
