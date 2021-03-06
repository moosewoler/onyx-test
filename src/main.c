/*******************************************************************************
 *      onyx-test
 * Description :
 *  test onyx framebuffer operation.
 * History     :
 ******************************************************************************/
/*****  ORIGINAL INFORMATION **************************************************/
/*
 * Copyright (C) 2010-2012 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

/*
 * @file mxc_epdc_fb_test.c
 *
 * @brief Mxc EPDC framebuffer driver unit test application
 *
 */
/*****  ORIGINAL INFORMATION **************************************************/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/mxcfb.h>

#include "a2test.c"

#define TFAIL -1
#define TPASS 0

#define TRUE 1
#define FALSE 0

#define BUFFER_FB		0
#define BUFFER_OVERLAY		1

#define WAVEFORM_MODE_INIT	0x0	/* Screen goes to white (clears) */
#define WAVEFORM_MODE_DU	0x1	/* Grey->white/grey->black */
#define WAVEFORM_MODE_GC16	0x2	/* High fidelity (flashing) */
#define WAVEFORM_MODE_GC4	0x3	/* Lower fidelity */
#define WAVEFORM_MODE_A2	0x4	/* Fast black/white animation */

#define EPDC_STR_ID		"mxc_epdc_fb"

#define ALIGN_PIXEL_128(x)  ((x+ 127) & ~127)

#define CROSSHATCH_ALTERNATING  0
#define CROSSHATCH_COLUMNS_ROWS 1

#define ALLOW_COLLISIONS	0
#define NO_COLLISIONS		1

//----- Moose W. Oler ----------------------------------------------------------
int     fd_fb;                                      // fb的文件描述符
int     fd_fb_ioctl;                                // 本例中就是fb
struct  fb_var_screeninfo screen_info;              // 屏幕属性结构提 
int     g_fb_size;                                  // fb大小
unsigned short * fb;                                // fb的内存映射
__u32   scheme = UPDATE_SCHEME_SNAPSHOT;            // 更新策略
__u32   pwrdown_delay = 5000;                       // 休眠延迟，毫秒
__u32   marker_val = 1;
int     use_animation = 1;                          // 1: A2; 0:

const int font_height=15;
const int font_width=12;
const unsigned short demo[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF 
};
const unsigned short erase[] = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF 
};

static __u32 update_to_display(int left, int top, int width, int height, 
                               int wave_mode, 
                               int wait_for_complete, uint flags);
static void draw_rgb_color_squares(struct fb_var_screeninfo * var);
void draw_pic(int x, int y, int width, int height, int bpp, unsigned char* ptr);

int  SystemInit();
void test_data_type();
void test_draw_spot();
void test_draw_pic();
void test_animate();
void test_animation_mode(void);





/*******************************************************************************
 *          main()
 ******************************************************************************/
int main(int argc, char **argv)
{
	int retval = TPASS;
    retval = SystemInit();
    switch (retval)
    {
        case 1:
            goto err0;
        case 2:
            goto err1;
        case 3:
            goto err2;
    }

    // OK, 可以开始测试了
    printf("Test Begin\n");

    test_data_type();
    test_draw_spot();
    test_draw_pic();
    test_animate();

    printf("Test Done\n");

    // 释放资源
err2:
	munmap(fb, g_fb_size);
err1:
	screen_info.rotate = FB_ROTATE_CCW;
	screen_info.bits_per_pixel = 16;
	screen_info.grayscale = 0;
	retval = ioctl(fd_fb, FBIOPUT_VSCREENINFO, &screen_info);
	if (retval < 0)
	{
		printf("Back to normal failed\n");
	}
	close(fd_fb);
	if (fd_fb != fd_fb_ioctl)
    {
		close(fd_fb_ioctl);
    }
err0:
    printf("Clean up Done\n");
	return retval;
}

void draw_pic(int x, int y, int width, int height, int bpp, unsigned char* ptr)
{
	unsigned char* fbp = (unsigned char*)fb;
    int i,j,k;
    int index;

	for (i = y; i < y+height; i++)
    {
		for (j = x; j < x+width;j++)
        {
            index= i*screen_info.xres_virtual+j;
            for (k=0;k<bpp;k++)
            {
                fbp[index*bpp+k] = *ptr;
                ptr++;
            }
        }
    }
}

int SystemInit()
{
    int retval=TPASS;
	char fb_filename[] = "/dev/fb0";

    // 打开设备文件
    fd_fb = open(fb_filename, O_RDWR, 0);
    if (fd_fb < 0) 
    {
        printf("Unable to open %s\n", fb_filename);
        return 1;
    }

    {
        // 判断该framebuffer是否为EPDC 
        struct  fb_fix_screeninfo screen_info_fix;
        retval = ioctl(fd_fb, FBIOGET_FSCREENINFO, &screen_info_fix);
        if (retval < 0)
        {
            printf("Unable to read fixed screeninfo for %s\n", fb_filename);
            return 2;
        }
        else
        {
            if (!strcmp(EPDC_STR_ID, screen_info_fix.id)) 
            {
                printf("Opened EPDC fb device %s\n", fb_filename);
            }
        }
    }
    
    {
        // 如果有内核测试驱动，用之
        fd_fb_ioctl = open("/dev/epdc_test", O_RDWR, 0);
        if (fd_fb_ioctl >= 0)
        {
            printf("\n****Using EPDC kernel module test driver!****\n\n");
        }
        else
        {
            fd_fb_ioctl = fd_fb;
        }
    }

    {
        // 获取并修改设备信息
        retval = ioctl(fd_fb, FBIOGET_VSCREENINFO, &screen_info);
        if (retval < 0)
        {
            return 2;
        }
        printf("Set the background to 16-bpp\n");
        screen_info.bits_per_pixel = 16;
        screen_info.xres_virtual = 768;
        screen_info.yres_virtual =1024;
        screen_info.grayscale = 0;
        screen_info.yoffset = 0;
        screen_info.rotate = FB_ROTATE_CW;      // set rotation to 180
        screen_info.activate = FB_ACTIVATE_FORCE;
        retval = ioctl(fd_fb, FBIOPUT_VSCREENINFO, &screen_info);
        if (retval < 0)
        {
            return 2;
        }

    }

    {
        // 将fb映射到地址空间上
        g_fb_size = screen_info.xres_virtual * screen_info.yres_virtual * screen_info.bits_per_pixel / 8;

        printf("screen_info.xres_virtual = %d\nscreen_info.yres_virtual = %d\nscreen_info.bits_per_pixel = %d\n",
                screen_info.xres_virtual, screen_info.yres_virtual, screen_info.bits_per_pixel);

        printf("Mem-Mapping FB\n");

        fb = (unsigned short *)mmap(0, g_fb_size,PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
        if ((int)fb <= 0)
        {
            printf("\nError: failed to map framebuffer device 0 to memory.\n");
            return 2;
        }
    }

    {
        // 设置自动更新的模式
        int auto_update_mode;
        printf("Set to region update mode\n");
        auto_update_mode = AUTO_UPDATE_MODE_REGION_MODE;
        retval = ioctl(fd_fb_ioctl, MXCFB_SET_AUTO_UPDATE_MODE, &auto_update_mode);
        if (retval < 0)
        {
            printf("\nError: failed to set update mode.\n");
            return 3;
        }
    }

    {
        // 设置波形模式
        struct  mxcfb_waveform_modes wv_modes;
        printf("Set waveform modes\n");
        wv_modes.mode_init = WAVEFORM_MODE_INIT;
        wv_modes.mode_du = WAVEFORM_MODE_DU;
        wv_modes.mode_gc4 = WAVEFORM_MODE_GC4;
        wv_modes.mode_gc8 = WAVEFORM_MODE_GC16;
        wv_modes.mode_gc16 = WAVEFORM_MODE_GC16;
        wv_modes.mode_gc32 = WAVEFORM_MODE_GC16;
        retval = ioctl(fd_fb_ioctl, MXCFB_SET_WAVEFORM_MODES, &wv_modes);
        if (retval < 0)
        {
            printf("\nError: failed to set waveform mode.\n");
            return 3;
        }
    }

    {
        // 设置更新策略
        printf("Set update scheme - %d\n", scheme);
        retval = ioctl(fd_fb_ioctl, MXCFB_SET_UPDATE_SCHEME, &scheme);
        if (retval < 0)
        {
            printf("\nError: failed to set update scheme.\n");
            return 3;
        }
    }

    {
        // 设置休眠延迟
        printf("Set pwrdown_delay - %d\n", pwrdown_delay);
        retval = ioctl(fd_fb_ioctl, MXCFB_SET_PWRDOWN_DELAY, &pwrdown_delay);
        if (retval < 0)
        {
            printf("\nError: failed to set power down delay.\n");
            return 3;
        }
    }
}

void test_data_type()
{
    printf("int = %d\nshort=%d\nchar=%d\nlong=%d\nlong long=%d\n", 
            sizeof(int), sizeof(short), sizeof(char), sizeof(long), sizeof(long long));
}

void test_draw_spot()
{
	int retval = TPASS;
	int wave_mode = WAVEFORM_MODE_A2;
    int x,y;

    printf("Blank whole screen\n");
    memset(fb, 0xFF, g_fb_size);
    update_to_display(0, 0, screen_info.xres, screen_info.yres, WAVEFORM_MODE_AUTO, TRUE, 0);
    
	printf("test draw_spot\n");
  
    /* 旋转为FB_ROTATE_CW 
     *      ->x
     *   +--------+
     *   |        |
     * | |        |
     * v |        |
     * y |        |
     *   |        |
     *   +--------+
     */
    for (y=0;y<screen_info.yres;y+=20)
    {
        for (x=0;x<screen_info.xres;x+=2)
        {
            int index;
            index = y*screen_info.xres_virtual + x;
            fb[index] = 0x00;   // 直接操作fb的话，操作对象是单个像素。如果转化为byte流指针的话，需要使用bpp信息
        }
        update_to_display(0, y, screen_info.xres, 1, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
    }
    sleep(2);
}

void test_draw_pic()
{
	int retval = TPASS;
	int wave_mode = WAVEFORM_MODE_A2;

    printf("Blank whole screen\n");
    memset(fb, 0xFF, g_fb_size);
    update_to_display(0, 0, screen_info.xres, screen_info.yres, WAVEFORM_MODE_AUTO, TRUE, 0);
    
	printf("test draw pic\n");
    {
        int x=100;
        int y=200;
        draw_pic(x,y, gimp_image_001.width, gimp_image_001.height, 2, (unsigned char*)(gimp_image_001.pixel_data));
        update_to_display(x, y, gimp_image_001.width, gimp_image_001.height, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);

    }
    sleep(2);
}

void test_animate()
{
	int retval = TPASS;
	int wave_mode = WAVEFORM_MODE_A2;

    printf("Blank whole screen\n");
    memset(fb, 0xFF, g_fb_size);
    update_to_display(0, 0, screen_info.xres, screen_info.yres, WAVEFORM_MODE_AUTO, TRUE, 0);
    
	printf("test animate\n");
    {
        int i;
        int x=100;
        int y=100;
        int frame=0;
        static const TImage* images[] = {
            &gimp_image_001,&gimp_image_002,&gimp_image_003,&gimp_image_004,
            &gimp_image_005,&gimp_image_006,&gimp_image_007,&gimp_image_008,
            &gimp_image_009,&gimp_image_010,&gimp_image_011,&gimp_image_012,
            &gimp_image_013,&gimp_image_014,&gimp_image_015,&gimp_image_016
        };

        for (i=0;i<20;i++)
        {
            for (frame=0;frame<16;frame++)
            {
                draw_pic(x,y, images[frame]->width, images[frame]->height, 2, (unsigned char*)(images[frame]->pixel_data));
                update_to_display(x, y, images[frame]->width, images[frame]->height, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
            }
        }
    }
    sleep(2);
}

static __u32 update_to_display(int left, int top, int width, int height, int wave_mode, int wait_for_complete, uint flags)
{
	struct mxcfb_update_data upd_data;
    __u32 upd_marker_data;
	int retval;
	int wait = wait_for_complete | flags;
	int max_retry = 10;

	upd_data.update_mode = UPDATE_MODE_PARTIAL;
	upd_data.waveform_mode = wave_mode;
	upd_data.update_region.left = left;
	upd_data.update_region.width = width;
	upd_data.update_region.top = top;
	upd_data.update_region.height = height;
	upd_data.temp = TEMP_USE_AMBIENT;
	upd_data.flags = flags;

	if (wait)
		/* Get unique marker value */
		upd_data.update_marker = marker_val++;
	else
		upd_data.update_marker = 0;

	retval = ioctl(fd_fb_ioctl, MXCFB_SEND_UPDATE, &upd_data);
	while (retval < 0) 
    {
		/* We have limited memory available for updates, so wait and
		 * then try again after some updates have completed */
		sleep(1);
		retval = ioctl(fd_fb_ioctl, MXCFB_SEND_UPDATE, &upd_data);
		if (--max_retry <= 0) 
        {
			printf("Max retries exceeded\n");
			wait = 0;
			flags = 0;
			break;
		}
	}

	if (wait) 
    {
		upd_marker_data = upd_data.update_marker;

		/* Wait for update to complete */
		retval = ioctl(fd_fb_ioctl, MXCFB_WAIT_FOR_UPDATE_COMPLETE, &upd_marker_data);
		if (retval < 0) 
        {
			printf("Wait for update complete failed.  Error = 0x%x", retval);
			flags = 0;
		}
	}

    return upd_data.waveform_mode;
}

static void draw_rgb_color_squares(struct fb_var_screeninfo * var)
{
	int i, j;
	int *fbp = (int *)fb;

	/* Draw red square */
	for (i = 50; i < 250; i++)
		for (j = 50; j < 250; j += 2)
			fbp[(i*var->xres_virtual+j)*2/4] = 0xF800FFFF;

	/* Draw green square */
	for (i = 50; i < 250; i++)
		for (j = 350; j < 550; j += 2)
			fbp[(i*var->xres_virtual+j)*2/4] = 0x07E0FFFF;

	/* Draw blue square */
	for (i = 350; i < 550; i++)
		for (j = 50; j < 250; j += 2)
			fbp[(i*var->xres_virtual+j)*2/4] = 0x001FFFFF;

	/* Draw black square */
	for (i = 350; i < 550; i++)
		for (j = 350; j < 550; j += 2)
			fbp[(i*var->xres_virtual+j)*2/4] = 0x0000FFFF;
}

void test_animation_mode(void)
{
	int retval = TPASS;
	int iter;
	int wave_mode = use_animation ? WAVEFORM_MODE_A2 : WAVEFORM_MODE_AUTO;

	int i;

	printf("Blank screen\n");
	//memset(fb, 0xFF, 800*600*2);
    memset(fb, 0xFF, g_fb_size);
	update_to_display(0, 0, screen_info.xres, screen_info.yres, WAVEFORM_MODE_AUTO, TRUE, 0);

	sleep(2);

	/* Swap quickly back-and-forth between black and white screen */
	for (i = 0; i < 10; i++) {
		/* black */
        memset(fb, 0x00, g_fb_size);
		update_to_display(0, 0, screen_info.xres, screen_info.yres, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
		/* white */
        memset(fb, 0xFF, g_fb_size);
		update_to_display(0, 0, screen_info.xres, screen_info.yres, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
	}

    
	printf("Squares update normal\n");
	draw_rgb_color_squares(&screen_info);
	update_to_display(50, 50, 200, 200, WAVEFORM_MODE_AUTO, TRUE, 0);
	update_to_display(350, 50, 200, 200, WAVEFORM_MODE_AUTO, TRUE, 0);
	update_to_display(50, 350, 200, 200, WAVEFORM_MODE_AUTO, TRUE, 0);
	update_to_display(350, 350, 200, 200, WAVEFORM_MODE_AUTO, TRUE, 0);
	sleep(2);

	/*
	 * Rule for animiation mode is that you must enter and exit all white or all black
	 */
	printf("Squares update A2\n");
	if (use_animation) {
		printf("Blank whole screen\n");
        memset(fb, 0xFF, g_fb_size);
		update_to_display(0, 0, screen_info.xres, screen_info.yres, WAVEFORM_MODE_AUTO, TRUE, 0);
	}
    draw_rgb_color_squares(&screen_info);
	update_to_display(50, 50, 200, 200, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
	update_to_display(350, 50, 200, 200, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
	update_to_display(50, 350, 200, 200, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
	update_to_display(350, 350, 200, 200, wave_mode, TRUE, EPDC_FLAG_FORCE_MONOCHROME);
	sleep(2);


	printf("Change back to non-inverted RGB565\n");
	screen_info.rotate = FB_ROTATE_CW;
	screen_info.bits_per_pixel = 16;
	screen_info.grayscale = 0;
	retval = ioctl(fd_fb, FBIOPUT_VSCREENINFO, &screen_info);
	if (retval < 0)
	{
		printf("Back to normal failed\n");
	}
}
