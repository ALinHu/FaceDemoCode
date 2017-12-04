#ifndef _RGB2YUV_H_
#define _RGB2YUV_H_

#ifdef __cplusplus
extern "C" {
#endif

void init_lookup_table();

// 转换过程中会丢失部分信息
int rgb24_to_yuv420(int x_dim, int y_dim, unsigned char *bmp, unsigned char *yuv, int flip);

void rgb24_to_yuv420p(unsigned char *lum, unsigned char *cb, unsigned char *cr, unsigned char *src, int width, int height);

void rgb24_to_bgr24(unsigned char *rgb, unsigned char *bgr, int width, int height);

void rgb24_to_bgr24_2(unsigned char *buf, int width, int height);


#ifdef __cplusplus
}
#endif

#endif  /* _RGB2YUV_H_ */
