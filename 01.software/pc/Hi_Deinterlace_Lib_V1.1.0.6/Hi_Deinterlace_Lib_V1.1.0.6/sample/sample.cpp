/******************************************************************************

  Copyright (C), 2007-2017, Hisilicon Tech. Co., Ltd.
  ******************************************************************************
  File Name     : hi_deinterlace_sample.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia
  Created       : 2007/11/27
  Description   :
  History       :
  1.Date        : 2007/11/27
  Author        : y39262
  Modification  : Created file
******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DllDeinterlace.h"

#define FILED_WIDTH    720   //width of field picture
#define FIELD_HEIGHT   240   //height of field picture

//YUV420 to UYVY422 conversion.
void YUV420toUYVY422(unsigned char *pszSrcY, int iSrcPitchY, unsigned char *pszSrcU,
                     unsigned char *pszSrcV, int iSrcPitchUV, unsigned char *pszDst,
                     int iSrcWidth, int iSrcHeight, int iDstPitch)
{
    int iSrcStride = iSrcPitchY + iSrcPitchY - iSrcWidth;
    int iSrcStrideU = (iSrcPitchY - iSrcWidth) >> 1;
    int iDstStride =(iDstPitch - iSrcWidth) << 1;
    __asm
    {
        push ebx
        mov edi , [pszDst]
        mov esi , [pszSrcY]
        mov eax , [pszSrcU]
        mov ebx , [pszSrcV]
        mov ecx , [iSrcHeight]
        mov edx , [iSrcWidth]
cyc: 
        movq mm2 , qword ptr [eax] //u
        movq mm3 , qword ptr [ebx] //v
        movq mm0 , qword ptr [esi] //y1
        movq mm1 , qword ptr [esi+8] //y2
        movq mm4 , mm2
        punpcklbw mm2 , mm3 // uv1
        punpckhbw mm4 , mm3 // uv2
        movq mm6 , mm2
        movq mm7 , mm4
        movq mm3 , mm2
        movq mm5 , mm4
        punpcklbw mm2 , mm0 // uyvy1
        punpckhbw mm3 , mm0 // uyvy2
        punpcklbw mm4 , mm1 // uyvy3
        punpckhbw mm5 , mm1 // uyvy4
        movq qword ptr [edi] , mm2
        movq qword ptr [edi+8] , mm3
        movq qword ptr [edi+16] , mm4
        movq qword ptr [edi+24] , mm5
        add esi , [iSrcPitchY]
        add edi , [iDstPitch]
        movq mm0 , qword ptr [esi]   //y1
        movq mm1 , qword ptr [esi+8] //y2
        movq mm2 , mm6
        movq mm4 , mm7
        movq mm3 , mm2
        movq mm5 , mm4
        punpcklbw mm2 , mm0 // uyvy1
        punpckhbw mm3 , mm0 // uyvy2
        punpcklbw mm4 , mm1 // uyvy3
        punpckhbw mm5 , mm1 // uyvy4
        movq qword ptr [edi] , mm2
        movq qword ptr [edi+8] , mm3
        movq qword ptr [edi+16] , mm4
        movq qword ptr [edi+24] , mm5
        sub esi , [iSrcPitchY]
        sub edi , [iDstPitch]
        add eax , 8
        add ebx , 8
        add esi , 16
        add edi , 32
        sub edx , 16
        ja cyc
        mov edx,  [iSrcWidth]
        add esi , [iSrcStride]
        add eax , [iSrcStrideU]
        add ebx , [iSrcStrideU]
        add edi , [iDstStride]
        sub ecx,2
        ja cyc
        emms
        pop ebx
    }
}

//YUV420 to YUYV422 conversion
void YUV420toYUYV422(unsigned char *pszSrcY, int iSrcPitchY, unsigned char *pszSrcU,
                     unsigned char *pszSrcV, int iSrcPitchUV, unsigned char *pszDst,
                     int iSrcWidth, int iSrcHeight, int iDstPitch)
{
    int iSrcStride = iSrcPitchY + iSrcPitchY - iSrcWidth; 
    int iSrcStrideU = (iSrcPitchY - iSrcWidth) >> 1;
    int iDstStride =(iDstPitch - iSrcWidth) << 1;
    __asm
    {
        push ebx
        mov edi , [pszDst]
        mov esi , [pszSrcY]
        mov eax , [pszSrcU]
        mov ebx , [pszSrcV]
        mov ecx , [iSrcHeight]
        mov edx , [iSrcWidth]
cyc:
        movq mm2 , qword ptr [eax] //u
        movq mm3 , qword ptr [ebx] //v
        movq mm0 , qword ptr [esi] //y1
        movq mm1 , qword ptr [esi+8] //y2
        movq mm4 , mm2
        punpcklbw mm2 , mm3 // uv1
        punpckhbw mm4 , mm3 // uv2
        movq mm3 , mm0
        movq mm5 , mm1
        punpcklbw mm0 , mm2 // yuyv1
        punpckhbw mm3 , mm2 // yuyv2
        punpcklbw mm1 , mm4 // yuyv3
        punpckhbw mm5 , mm4 // yuyv4
        movq qword ptr [edi] , mm0
        movq qword ptr [edi+8] , mm3
        movq qword ptr [edi+16] , mm1
        movq qword ptr [edi+24] , mm5
        add esi , [iSrcPitchY]
        add edi , [iDstPitch]
        movq mm0 , qword ptr [esi]   //y1
        movq mm1 , qword ptr [esi+8] //y2
        movq mm3 , mm0
        movq mm5 , mm1
        punpcklbw mm0 , mm2 // yuyv1
        punpcklbw mm1 , mm4 // yuyv3
        punpckhbw mm3 , mm2 // yuyv2
        punpckhbw mm5 , mm4 // yuyv4
        movq qword ptr [edi] , mm0
        movq qword ptr [edi+8] , mm3
        movq qword ptr [edi+16] , mm1
        movq qword ptr [edi+24] , mm5
        sub esi , [iSrcPitchY]
        sub edi , [iDstPitch]
        add eax , 8
        add ebx , 8
        add esi , 16
        add edi , 32
        sub edx , 16   
        ja cyc
        mov edx , [iSrcWidth]
        add esi , [iSrcStride]
        add eax , [iSrcStrideU]
        add ebx , [iSrcStrideU]
        add edi , [iDstStride]
        sub ecx , 2
        ja cyc
        emms
        pop ebx
    }
}

/*
 * @brief      Allocates memory blocks on a 16 bytes boundary
 * @param [in] Bytes to allocate
 * @return     A pointer to the memory block that was allocated, or NULL if the operation failed
 * @note
 */
static void* MallocAlign16(size_t size)
{
    unsigned char *rawblock = (unsigned char *)calloc(size + 0x10,1);
    if (rawblock == NULL)
    {
        return NULL;
    }
    else if ((int)rawblock & 0xF)
    {
        unsigned char *p = (unsigned char *)(((int)rawblock + 0xF) & (~0xF));
        p[-1] = p - rawblock;
        return p;
    }
    else
    {
        unsigned char *p = rawblock + 0x10;
        p[-1] = p - rawblock;
        return p;
    }
}

/*
 * @brief       Frees a memory block by function MallocAlign16
 * @param [in]  Pointer to the memory block to be freed
 * @return      None
 * @note
 */
static void FreeAlign16(void *memblock)
{
    if (memblock != NULL)
    {
        free((unsigned char *)memblock - *((unsigned char *)memblock - 1));
    }
}

int main(int argc, char *argv[])
{
    void*          pHandle          = NULL;               //deinterlace handle
    unsigned char* pszYUV422        = NULL;               
    FILE*          fInFilehYUV420   = NULL;               
    FILE*          fOutFrameYUV422  = NULL;               
    bool           bInitFlag        = false;              //Deinterlace initialized flag
    PIC_TYPE_E     tPicFlag         = PIC_INTERLACED_ODD; //field flag
    unsigned int   s32NumDecFrame_S = 0;                  //frame counter
    DEINTERLACE_FRAME_S struDstFrame;                     //output frame buffer
    DEINTERLACE_PARA_S  struPara;                         //Deinterlace parameters
    unsigned char* pszSrc = (unsigned char*)MallocAlign16(FILED_WIDTH * FIELD_HEIGHT * 3 / 2);//allocate input field buffer
    struDstFrame.pszY = NULL;
    struDstFrame.pszU = NULL;
    struDstFrame.pszV = NULL;

    if (argc < 2)
    {
        //argv[1] is the input YUV420 file name
        //argv[2] is the output YUV422 file name.
        fprintf(stderr, "No YUV420 stream found!\n");
        goto exitmain;
    }

    fInFilehYUV420 = fopen(argv[1], "rb"); // open input YUV420 file
    if (NULL == fInFilehYUV420)
    {
        fprintf(stderr, "Unable to open fInFilehYUV420 file %s.\n", argv[1]);
        goto exitmain;
    }

    if (argc > 2)
    {
        fOutFrameYUV422 = fopen(argv[2], "wb"); // open output file
        if (NULL == fOutFrameYUV422)
        {
            fprintf(stderr, "Unable to open fOutFrameYUV422 file %s.\n", argv[2]);
            goto exitmain;
        }
    }

    /* deinterlace process */
    while (1 == fread(pszSrc, FILED_WIDTH * FIELD_HEIGHT * 3 / 2, 1, fInFilehYUV420))
    {
        if (false == bInitFlag)  // if deinterlace handle haven't been initialized
        {
            bInitFlag = true;

            //allocate output YUV422 frame buffer
            pszYUV422 = (unsigned char*)MallocAlign16(FILED_WIDTH * FIELD_HEIGHT * 4);

            //allocate YUV420 frame buffer
            struDstFrame.pszY = (unsigned char*)MallocAlign16(FILED_WIDTH * FIELD_HEIGHT * 2);
            struDstFrame.pszU = (unsigned char*)MallocAlign16(FILED_WIDTH * FIELD_HEIGHT / 2);
            struDstFrame.pszV = (unsigned char*)MallocAlign16(FILED_WIDTH * FIELD_HEIGHT / 2);

            //initialize deinterlace parameters.
            struPara.iFieldWidth  = FILED_WIDTH;
            struPara.iFieldHeight = FIELD_HEIGHT;
            struPara.iSrcYPitch   = FILED_WIDTH;
            struPara.iSrcUVPitch  = FILED_WIDTH / 2;
            struPara.iDstYPitch   = FILED_WIDTH;     //destination stride for Y.
            struPara.iDstUVPitch  = FILED_WIDTH / 2; //destination stride for UV.

            HI_InitDeinterlace(&pHandle, struPara);  // init Deinterlace handle & malloc.

            if(NULL == pHandle || NULL == pszYUV422 || NULL == struDstFrame.pszY || NULL == struDstFrame.pszU || NULL == struDstFrame.pszV)
            {
                fprintf(stderr, "Unable to creat deinterlace handle.\n");
                goto exitmain;
            }
 
            //There are two samples below, 
            //    the first OSD area is  (x=0,y=0,w=50,h=20).
            //    the second OSD area is (x=515£¬y=0£¬w=185, h=20).
            DEINTERLACE_OSDRECT_S rcOsd[2];
            rcOsd[0].x = 0; rcOsd[0].y = 0; rcOsd[0].w = 50;    rcOsd[0].h = 20;
            rcOsd[1].x = 515; rcOsd[1].y = 0; rcOsd[1].w = 185;    rcOsd[1].h = 20;
            HI_SetOsd(pHandle, rcOsd, 2);
        } //end of initialization

        //deinterlace process. two fields combined to one frame
        HI_Deinterlace(pHandle,                                     //deinteralce handle
                       struDstFrame,                                //output YUV420 frame
                       pszSrc,                                      //Y of input field 
                       pszSrc + FILED_WIDTH * FIELD_HEIGHT,         //U of input field
                       pszSrc + FILED_WIDTH * FIELD_HEIGHT * 5 / 4, //V of input field
                       tPicFlag);                                   //field flag

        /* save output frame */
        if (PIC_INTERLACED_EVEN == tPicFlag)
        {            
            // YUV420 to UYVY422 conversion
            YUV420toUYVY422(struDstFrame.pszY,  //pointer to Y
                            FILED_WIDTH,        //Y stride
                            struDstFrame.pszU,  //pointer to U
                            struDstFrame.pszV,  //pointer to V
                            FILED_WIDTH / 2,    //UV stride
                            pszYUV422,          //output frame buffer
                            FILED_WIDTH,        //width of output frame
                            FIELD_HEIGHT * 2,   //height of output frame
                            FILED_WIDTH * 2);   //stride of output frame

            fwrite(pszYUV422, 1, FILED_WIDTH * FIELD_HEIGHT * 4, fOutFrameYUV422);
            printf("Success Frame: %d\n", s32NumDecFrame_S++);
        }

        /* update field flag */
        if (PIC_INTERLACED_ODD == tPicFlag)
        {
            tPicFlag = PIC_INTERLACED_EVEN;
        }
        else
        {
            tPicFlag = PIC_INTERLACED_ODD;
        }
    }

exitmain:
    if (NULL != pHandle)
    {
        HI_ReleaseDeinterlace(pHandle);//release deinterlace handle
        pHandle = NULL;               
    }
    if (NULL != pszYUV422)
    {
        FreeAlign16(pszYUV422);
        pszYUV422 = NULL;
    }
    if (NULL != struDstFrame.pszY)
    {
        FreeAlign16(struDstFrame.pszY);
        struDstFrame.pszY = NULL;
    }
    if (NULL != struDstFrame.pszU)
    {
        FreeAlign16(struDstFrame.pszU);
        struDstFrame.pszU = NULL;
    }
    if (NULL != struDstFrame.pszV)
    {
        FreeAlign16(struDstFrame.pszV);
        struDstFrame.pszV = NULL;
    }
    if (NULL != pszSrc)
    {
        FreeAlign16(pszSrc);
        pszSrc = NULL;
    }
    if (NULL != fInFilehYUV420)
    {
        fclose(fInFilehYUV420);
        fInFilehYUV420 = NULL;
    }
    if (NULL != fOutFrameYUV422)
    {
        fclose(fOutFrameYUV422);
        fOutFrameYUV422 = NULL;
    }
    
    return 0;
}
