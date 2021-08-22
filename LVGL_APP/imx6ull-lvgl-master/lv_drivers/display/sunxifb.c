/**
 * @file sunxifb.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "sunxifb.h"
#if USE_SUNXIFB

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#define USE_SUNXIFB_G2D
#ifdef USE_SUNXIFB_G2D
#include "g2d_driver_enh.h"
#define USE_SUNXIFB_G2D_ROTATE
#ifdef USE_SUNXIFB_G2D_ROTATE
#include <ion_mem_alloc.h>
#endif
#endif /* USE_SUNXIFB_G2D */

/*********************
 *      DEFINES
 *********************/
#ifndef SUNXIFB_PATH
#define SUNXIFB_PATH  "/dev/fb0"
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      STRUCTURES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static char *fbp = 0;
static long int screensize = 0;
static int fbfd = 0;
static uint32_t fbp_w;
static uint32_t fbp_h;
static uint32_t fbp_line_length;

#define USE_SUNXIFB_DOUBLE_BUFFER
#ifdef USE_SUNXIFB_DOUBLE_BUFFER
#define FBIO_CACHE_SYNC         0x4630
#define FBIO_ENABLE_CACHE       0x4631
#define FBIO_GET_IONFD          0x4632
#define FBIO_GET_PHY_ADDR       0x4633
#define FBIOGET_DMABUF          _IOR('F', 0x21, struct fb_dmabuf_export)

struct sunxifb_info {
    char *screenfbp[2];
    uint32_t fbnum;
    uint32_t fbindex;
    volatile bool dbuf_en;
#ifdef USE_SUNXIFB_G2D
    int g2dfd;
    g2d_fmt_enh format;
#ifdef USE_SUNXIFB_G2D_ROTATE
    struct SunxiMemOpsS *memops;
    g2d_blt_flags_h rotated;
    char *rotatefbp;
    uintptr_t rotatefbp_phy;
    uint32_t rotatefbp_w;
    uint32_t rotatefbp_h;
#endif /* USE_SUNXIFB_G2D_ROTATE */
#endif /* USE_SUNXIFB_G2D */
};

static struct sunxifb_info sinfo;

#ifdef USE_SUNXIFB_G2D
static int sunxifb_g2d_blit(int index);
#endif /* USE_SUNXIFB_G2D */
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void sunxifb_init(uint32_t rotated)
{
    // Open the file for reading and writing
    fbfd = open(SUNXIFB_PATH, O_RDWR);
    if(fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        return;
    }

    // Get fixed screen information
    if(ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        return;
    }

    // Get variable screen information
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        return;
    }

#if defined(USE_SUNXIFB_DOUBLE_BUFFER) && defined(USE_SUNXIFB_CACHE)
    // Speed up the speed of buf copy
    uintptr_t args[2] = { 1, 0 };
    if (ioctl(fbfd, FBIO_ENABLE_CACHE, args) < 0) {
        perror("Error: FBIO_ENABLE_CACHE fail");
    }
#endif /* USE_SUNXIFB_CACHE */

    // Figure out the size of the screen in bytes
    screensize =  finfo.smem_len; //finfo.line_length * vinfo.yres;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if((intptr_t)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        return;
    }

    printf("wh=%dx%d, vwh=%dx%d, bpp=%d, rotated=%d\n", vinfo.xres, vinfo.yres,
            vinfo.xres_virtual, vinfo.yres_virtual, vinfo.bits_per_pixel,
            rotated);

    fbp_w = vinfo.xres;
    fbp_h = vinfo.yres;
    fbp_line_length = finfo.line_length;

#ifdef USE_SUNXIFB_DOUBLE_BUFFER
    memset(&sinfo, 0, sizeof(struct sunxifb_info));
    sinfo.dbuf_en = true;
    // Do not clear fb and pointer to back fb
    sinfo.fbnum = (uint32_t)(screensize / (finfo.line_length * vinfo.yres));
    if (sinfo.fbnum > 1) {
        printf("Turn on double buffering.\n");

        sinfo.screenfbp[0] = fbp;
        sinfo.screenfbp[1] = fbp + finfo.line_length * vinfo.yres;

        if (vinfo.yoffset == 0) {
            sinfo.fbindex = !sinfo.fbindex;
            fbp = sinfo.screenfbp[sinfo.fbindex];
        }

#ifdef USE_SUNXIFB_G2D
        printf("Turn on 2d hardware acceleration.\n");

        if ((sinfo.g2dfd = open("/dev/g2d", O_RDWR)) < 0) {
            perror("Error: cannot open g2d device");
        }

        switch (vinfo.bits_per_pixel) {
        case 24:
            sinfo.format = G2D_FORMAT_RGB888;
            break;
        case 16:
            sinfo.format = G2D_FORMAT_RGB565;
            break;
        default:
            sinfo.format = G2D_FORMAT_ARGB8888;
            break;
        }

#ifdef USE_SUNXIFB_G2D_ROTATE
        printf("Turn on 2d hardware rotation acceleration.\n");

        sinfo.memops = GetMemAdapterOpsS();
        if (SunxiMemOpen(sinfo.memops) < 0) {
            perror("Error: cannot open ion device");
        }

        sinfo.rotatefbp = SunxiMemPalloc(sinfo.memops,
                finfo.line_length * vinfo.yres);
        if (sinfo.rotatefbp == 0) {
            perror("Error: cannot malloc rotate buffer");
            return;
        }

        fbp = sinfo.rotatefbp;
        sinfo.rotatefbp_phy = (uintptr_t) SunxiMemGetPhysicAddressCpu(
                sinfo.memops, sinfo.rotatefbp);

        switch (rotated) {
        case LV_DISP_ROT_90:
            sinfo.rotated = G2D_ROT_270;
            sinfo.rotatefbp_w = vinfo.yres;
            sinfo.rotatefbp_h = vinfo.xres;
            fbp_w = vinfo.yres;
            fbp_h = vinfo.xres;
            fbp_line_length = fbp_w * vinfo.bits_per_pixel / 8;
            break;
        case LV_DISP_ROT_180:
            sinfo.rotated = G2D_ROT_180;
            sinfo.rotatefbp_w = vinfo.xres;
            sinfo.rotatefbp_h = vinfo.yres;
            break;
        case LV_DISP_ROT_270:
            sinfo.rotated = G2D_ROT_90;
            sinfo.rotatefbp_w = vinfo.yres;
            sinfo.rotatefbp_h = vinfo.xres;
            fbp_w = vinfo.yres;
            fbp_h = vinfo.xres;
            fbp_line_length = fbp_w * vinfo.bits_per_pixel / 8;
            break;
        default:
            sinfo.rotated = 1024;
            sinfo.rotatefbp_w = vinfo.xres;
            sinfo.rotatefbp_h = vinfo.yres;
            break;
        }
#endif /* USE_SUNXIFB_G2D_ROTATE */
#endif /* USE_SUNXIFB_G2D */
    }
#else
    memset(fbp, 0, screensize);
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */
}

void sunxifb_exit(void)
{
#ifdef USE_SUNXIFB_DOUBLE_BUFFER
#ifdef USE_SUNXIFB_CACHE
    uintptr_t args[2] = { 0, 0 };
    if (ioctl(fbfd, FBIO_ENABLE_CACHE, args) < 0) {
        perror("Error: FBIO_ENABLE_CACHE fail");
    }
#endif /* USE_SUNXIFB_CACHE */

#ifdef USE_SUNXIFB_G2D
    if (sinfo.g2dfd > 0)
        close(sinfo.g2dfd);
#endif /* USE_SUNXIFB_G2D */

#ifdef USE_SUNXIFB_G2D_ROTATE
    if (sinfo.rotatefbp) {
        SunxiMemPfree(sinfo.memops, sinfo.rotatefbp);
        sinfo.rotatefbp = NULL;
    }
    if (sinfo.memops) {
        SunxiMemClose(sinfo.memops);
        sinfo.memops = NULL;
    }
#endif /* USE_SUNXIFB_G2D */
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */
    close(fbfd);
}

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void sunxifb_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(fbp == NULL ||
            area->x2 < 0 ||
            area->y2 < 0 ||
            area->x1 > (int32_t)fbp_w - 1 ||
            area->y1 > (int32_t)fbp_h - 1) {
        lv_disp_flush_ready(drv);
        return;
    }

    /*Truncate the area to the screen*/
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > (int32_t)fbp_w - 1 ? (int32_t)fbp_w - 1 : area->x2;
    int32_t act_y2 = area->y2 > (int32_t)fbp_h - 1 ? (int32_t)fbp_h - 1 : area->y2;

    lv_coord_t w = (act_x2 - act_x1 + 1);
    long int location = 0;
    long int byte_location = 0;
    unsigned char bit_location = 0;

    /*32 or 24 bit per pixel*/
    if(vinfo.bits_per_pixel == 32 || vinfo.bits_per_pixel == 24) {
        uint32_t * fbp32 = (uint32_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
#ifdef USE_SUNXIFB_DOUBLE_BUFFER
            if (sinfo.fbnum > 1)
                location = act_x1 + y  * fbp_line_length / 4;
            else
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */
            location = (act_x1 + vinfo.xoffset) + (y + vinfo.yoffset) * finfo.line_length / 4;
            memcpy(&fbp32[location], (uint32_t *)color_p, (act_x2 - act_x1 + 1) * 4);
            color_p += w;
        }
    }
    /*16 bit per pixel*/
    else if(vinfo.bits_per_pixel == 16) {
        uint16_t * fbp16 = (uint16_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
#ifdef USE_SUNXIFB_DOUBLE_BUFFER
            if (sinfo.fbnum > 1)
                location = act_x1 + y * fbp_line_length / 2;
            else
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */
            location = (act_x1 + vinfo.xoffset) + (y + vinfo.yoffset) * finfo.line_length / 2;
            memcpy(&fbp16[location], (uint32_t *)color_p, (act_x2 - act_x1 + 1) * 2);
            color_p += w;
        }
    }
    /*8 bit per pixel*/
    else if(vinfo.bits_per_pixel == 8) {
        uint8_t * fbp8 = (uint8_t *)fbp;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
#ifdef USE_SUNXIFB_DOUBLE_BUFFER
            if (sinfo.fbnum > 1)
                location = act_x1 + y * fbp_line_length;
            else
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */
            location = (act_x1 + vinfo.xoffset) + (y + vinfo.yoffset) * finfo.line_length;
            memcpy(&fbp8[location], (uint32_t *)color_p, (act_x2 - act_x1 + 1));
            color_p += w;
        }
    }
    /*1 bit per pixel*/
    else if(vinfo.bits_per_pixel == 1) {
        uint8_t * fbp8 = (uint8_t *)fbp;
        int32_t x;
        int32_t y;
        for(y = act_y1; y <= act_y2; y++) {
            for(x = act_x1; x <= act_x2; x++) {
#ifdef USE_SUNXIFB_DOUBLE_BUFFER
                if (sinfo.fbnum > 1)
                    location = x + y * fbp_w;
                else
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */
                location = (x + vinfo.xoffset) + (y + vinfo.yoffset) * vinfo.xres;
                byte_location = location / 8; /* find the byte we need to change */
                bit_location = location % 8; /* inside the byte found, find the bit we need to change */
                fbp8[byte_location] &= ~(((uint8_t)(1)) << bit_location);
                fbp8[byte_location] |= ((uint8_t)(color_p->full)) << bit_location;
                color_p++;
            }

            color_p += area->x2 - act_x2;
        }
    } else {
        /*Not supported bit per pixel*/
    }

    //May be some direct update command is required
    //ret = ioctl(state->fd, FBIO_UPDATE, (unsigned long)((uintptr_t)rect));

#ifdef USE_SUNXIFB_DOUBLE_BUFFER
    if (sinfo.fbnum > 1 && sinfo.dbuf_en && lv_disp_flush_is_last(drv)) {
#ifdef USE_SUNXIFB_CACHE
        uintptr_t args[2];
        args[0] = (uintptr_t) sinfo.screenfbp[sinfo.fbindex];
        args[1] = finfo.line_length * vinfo.yres;
        if (ioctl(fbfd, FBIO_CACHE_SYNC, args) < 0) {
            perror("Error: FBIO_CACHE_SYNC fail");
        }
#endif /* USE_SUNXIFB_CACHE */

#ifdef USE_SUNXIFB_G2D_ROTATE
#if 0 /* TEST */
        static int outfp, ret = 0;
        if (ret == 0) {
            if ((outfp = fopen("/mnt/UDISK/out", "wb+")) == NULL) {
                printf("open file fail. \n");
            } else {
                printf("open file ok. \n");
            }
            printf("==out_size=%d, addr=%p==\n", finfo.line_length * vinfo.yres,
                    sinfo.rotatefbp);
            ret = fwrite(sinfo.rotatefbp, finfo.line_length * vinfo.yres, 1,
                    outfp);
            printf("fwrite,ret=%d\n", ret);
            fclose(outfp);
        }
#endif
        SunxiMemFlushCache(sinfo.memops, sinfo.rotatefbp,
                finfo.line_length * vinfo.yres);
        sunxifb_g2d_blit(!sinfo.fbindex);
#endif /* USE_SUNXIFB_G2D_ROTATE */

        vinfo.yoffset = sinfo.fbindex * vinfo.yres;
        if (ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo) < 0) {
            perror("Error: FBIOPAN_DISPLAY fail");
        }

#if defined(USE_SUNXIFB_G2D) && !defined(USE_SUNXIFB_G2D_ROTATE)
        sunxifb_g2d_blit(sinfo.fbindex);
#elif !defined(USE_SUNXIFB_G2D_ROTATE)
        memcpy(sinfo.screenfbp[!sinfo.fbindex], sinfo.screenfbp[sinfo.fbindex],
                finfo.line_length * vinfo.yres);
#endif /* USE_SUNXIFB_G2D && !USE_SUNXIFB_G2D_ROTATE */

        sinfo.fbindex = !sinfo.fbindex;
#ifndef USE_SUNXIFB_G2D_ROTATE
        fbp = sinfo.screenfbp[sinfo.fbindex];
#endif /* USE_SUNXIFB_G2D_ROTATE */
    }
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */

    lv_disp_flush_ready(drv);
}

void sunxifb_get_sizes(uint32_t *width, uint32_t *height) {
    if (width)
        *width = vinfo.xres;

    if (height)
        *height = vinfo.yres;
}

#ifdef USE_SUNXIFB_DOUBLE_BUFFER
bool sunxifb_get_dbuf_en() {
    return sinfo.dbuf_en;
}

int sunxifb_set_dbuf_en(lv_disp_drv_t * drv, bool dbuf_en) {
    if (sinfo.dbuf_en == dbuf_en)
        return 0;

    // if (drv->draw_buf->flushing)
    //     return -2;

#ifdef USE_SUNXIFB_CACHE
    uintptr_t args[2] = { dbuf_en, 0 };
    if (ioctl(fbfd, FBIO_ENABLE_CACHE, args) < 0) {
        perror("Error: FBIO_ENABLE_CACHE fail");
        return -1;
    }

    munmap(sinfo.screenfbp[0], finfo.line_length * vinfo.yres);

    // Re mmap the device to memory, settings will be effective
    fbp = (char*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd,
            0);
    if ((intptr_t) fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        return -1;
    }

    sinfo.screenfbp[0] = fbp;
    sinfo.screenfbp[1] = fbp + finfo.line_length * vinfo.yres;
#endif /* USE_SUNXIFB_CACHE */

    if (dbuf_en) {
        vinfo.yoffset = sinfo.fbindex * vinfo.yres;
        if (ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo) < 0) {
            perror("Error: FBIOPAN_DISPLAY fail");
        }

#ifdef USE_SUNXIFB_G2D
        sunxifb_g2d_blit(sinfo.fbindex);
#else
        memcpy(sinfo.screenfbp[!sinfo.fbindex], sinfo.screenfbp[sinfo.fbindex],
                finfo.line_length * vinfo.yres);
#endif /* USE_SUNXIFB_G2D */
    }

    sinfo.fbindex = !sinfo.fbindex;
#ifdef USE_SUNXIFB_G2D_ROTATE
    fbp = sinfo.rotatefbp;
#else
    fbp = sinfo.screenfbp[sinfo.fbindex];
#endif /* USE_SUNXIFB_G2D_ROTATE */

    sinfo.dbuf_en = dbuf_en;
    return 0;
}

#ifdef USE_SUNXIFB_G2D
static int sunxifb_g2d_blit(int index) {
    g2d_blt_h info;
    memset(&info, 0, sizeof(g2d_blt_h));

#ifdef USE_SUNXIFB_G2D_ROTATE
    info.flag_h = sinfo.rotated;
    info.src_image_h.width = sinfo.rotatefbp_w;
    info.src_image_h.height = sinfo.rotatefbp_h;
    info.src_image_h.laddr[0] = sinfo.rotatefbp_phy;

    info.src_image_h.clip_rect.x = 0;
    info.src_image_h.clip_rect.y = 0;
    info.src_image_h.clip_rect.w = sinfo.rotatefbp_w;
    info.src_image_h.clip_rect.h = sinfo.rotatefbp_h;
#else
    info.flag_h = 1024;
    info.src_image_h.width = vinfo.xres_virtual;
    info.src_image_h.height = vinfo.yres_virtual;
    info.src_image_h.laddr[0] = finfo.smem_start;

    info.src_image_h.clip_rect.x = 0;
    info.src_image_h.clip_rect.y = index * vinfo.yres;
    info.src_image_h.clip_rect.w = vinfo.xres;
    info.src_image_h.clip_rect.h = vinfo.yres;
#endif /* USE_SUNXIFB_G2D_ROTATE */

    info.src_image_h.format = sinfo.format;

    info.src_image_h.mode = G2D_GLOBAL_ALPHA;
    info.src_image_h.alpha = 255;
    info.src_image_h.color = 0xee8899;
    info.src_image_h.align[0] = 0;
    info.src_image_h.align[1] = info.src_image_h.align[0];
    info.src_image_h.align[2] = info.src_image_h.align[0];
    info.src_image_h.laddr[1] = (uintptr_t) 0;
    info.src_image_h.laddr[2] = (uintptr_t) 0;
    info.src_image_h.use_phy_addr = 1;

    info.dst_image_h.format = sinfo.format;

    info.dst_image_h.width = vinfo.xres_virtual;
    info.dst_image_h.height = vinfo.yres_virtual;

    info.dst_image_h.clip_rect.x = 0;
    info.dst_image_h.clip_rect.y = !index * vinfo.yres;
    info.dst_image_h.clip_rect.w = vinfo.xres;
    info.dst_image_h.clip_rect.h = vinfo.yres;

    info.dst_image_h.mode = G2D_GLOBAL_ALPHA;
    info.dst_image_h.alpha = 255;
    info.dst_image_h.color = 0xee8899;
    info.dst_image_h.align[0] = 0;
    info.dst_image_h.align[1] = info.dst_image_h.align[0];
    info.dst_image_h.align[2] = info.dst_image_h.align[0];
    info.dst_image_h.laddr[0] = finfo.smem_start;
    info.dst_image_h.laddr[1] = (uintptr_t) 0;
    info.dst_image_h.laddr[2] = (uintptr_t) 0;
    info.dst_image_h.use_phy_addr = 1;

    if (ioctl(sinfo.g2dfd, G2D_CMD_BITBLT_H, (uintptr_t)(&info)) < 0) {
        perror("Error: G2D_CMD_BITBLT_H failed");
        printf(
                "Error: src_addr=%u src.format=%d src_wh=[%d %d] src_clip=[%d %d %d %d] "
                        "dst_addr=%u dst.format=%d dst_wh=[%d %d] dst_clip=[%d %d %d %d]\n",
                info.src_image_h.laddr[0], info.src_image_h.format,
                info.src_image_h.width, info.src_image_h.height,
                info.src_image_h.clip_rect.x, info.src_image_h.clip_rect.y,
                info.src_image_h.clip_rect.w, info.src_image_h.clip_rect.h,
                info.dst_image_h.laddr[0], info.dst_image_h.format,
                info.dst_image_h.width, info.dst_image_h.height,
                info.dst_image_h.clip_rect.x, info.dst_image_h.clip_rect.y,
                info.dst_image_h.clip_rect.w, info.dst_image_h.clip_rect.h);
        return -1;
    }
    return 0;
}
#endif /* USE_SUNXIFB_G2D */
#endif /* USE_SUNXIFB_DOUBLE_BUFFER */

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif
