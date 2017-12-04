#ifndef JPEG_FILE_H
#define JPEG_FILE_H

#include <stdio.h>
#include <stdlib.h>


enum ColorSpace
{
    cs_RGB,
    cs_BGR,
    cs_YUV
};

#ifdef __cplusplus
extern "C" {
#endif
  
int write_JPEG_file(const char * filename, unsigned char * image_buffer, int image_width, int image_height, int quality);
int decode_JPEG_file(const char *filename, ColorSpace cs, int *image_width, int *image_height, int *pixsize, unsigned char * image_buffer);
int decode_JPEG(unsigned char *jpegbuf, int jpegsize, int *image_width, int *image_height, int *pixsize, unsigned char * image_buffer);

#ifdef __cplusplus
}		/* extern "C" */
#endif	/* __cplusplus */


#endif /* PNG_FILE */
