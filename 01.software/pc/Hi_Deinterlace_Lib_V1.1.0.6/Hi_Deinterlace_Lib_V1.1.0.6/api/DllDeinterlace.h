/*****************************************************************

  Copyright (C), 2007-2017, Hisilicon Tech. Co., Ltd.
 ****************************************************************
  File Name     : DllDeinterlace.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia
  Created       : 2007/11/28
  Description   :
  History       :
  1.Date        : 2007/11/28
  Author        : f65132
  Modification  : Created file
*****************************************************************/
#ifndef _DLLDEINTERLACE_H_
#define _DLLDEINTERLACE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
    
#if defined(WIN32)
#if !defined(USELIB)
#define HI_API __declspec( dllexport ) 
#define HI_APICALL __stdcall
#else
#define HI_API
#define HI_APICALL
#endif
#elif defined(LINUX)
#define HI_API
#define HI_APICALL
#else
#error "Please specify a design-in platform!"
#endif


#define HI_SUCCESS_DEINTERLACE  0   // success
#define HI_ERR_MALLOC           -1  // memory allocation failed
#define HI_ERR_PITCH            -2  // input parameter error
#define HI_ERR_POINT_NULL       -3  // input pointer is NULL
#define HI_ERR_FIELD_FLAG       -4  // input picture is not field
#define HI_ERR_OSDNUM           -5  // OSD number out of range

typedef enum hiPIC_TYPE_E
{
    PIC_PROGRESSIVE = 0,    // frame
    PIC_INTERLACED_ODD,     // odd field
    PIC_INTERLACED_EVEN     // even field
} PIC_TYPE_E;

/***************************************************************** 
*   deinterlace parameters
*****************************************************************/
typedef struct hiDEINTERLACE_PARA_S
{
    int iFieldWidth;    
    int iFieldHeight;   
    int iSrcYPitch;     //Y  stride of input picture
    int iSrcUVPitch;    //UV stride of input picture
    int iDstYPitch;     //Y  stride of output picture
    int iDstUVPitch;    //UV stride of output picture
}DEINTERLACE_PARA_S;

/***************************************************************** 
*   output frame
*****************************************************************/
typedef struct hiDEINTERLACE_FRAME_S
{
    unsigned char *pszY;  // Y
    unsigned char *pszU;  // U
    unsigned char *pszV;  // V
}DEINTERLACE_FRAME_S;

/***************************************************************** 
*   OSD parameters
*****************************************************************/
typedef struct hiDEINTERLACE_OSDRECT_S
{
    int x;    //left offset int frame coordinate of OSD
    int y;    //top offset int frame coordinate of OSD
    int w;    //width of OSD
    int h;    //height of OSD
}DEINTERLACE_OSDRECT_S;

/***************************************************************** 
* Function Name  : HI_InitDeinterlace
* Description    : init deinterlace handle & allocate memory
* Parameters     : 
                   **pHandle (output): 
				               deinterlace handle (if success)
				               NULL (if failed).
                   struPara (intput): 
				               deinterlace parameters
* Return Type    : 
                   HI_SUCCESS_DEINTERLACE:  success
                   HI_ERR_MALLOC         :  memory allocated failed
                   HI_ERR_PITCH          :  parameters error 
* Last Modified  : 2007-11-28
*****************************************************************/
HI_API int HI_InitDeinterlace(void **pHandle, DEINTERLACE_PARA_S struPara);

/***************************************************************** 
* Function Name  : HI_ReleaseDeinterlace
* Description    : release deinterlace handle & free memory
* Parameters     : pHandle:  deinterlace handle created by HI_InitDeinterlace
* Return Type    : none
* Last Modified  : 2007-11-27
*****************************************************************/
HI_API int HI_ReleaseDeinterlace(void *pHandle);


/***************************************************************** 
* Function Name  : HI_Deinterlace
* Description    : deinterlace process 
* Parameters     : pHandle      : deinterlace handle created by HI_InitDeinterlace
*                : struDstFrame : output picture
*                : pszSrcY      : Y of input field
*                : pszSrcU      : U of input field
*                : pszSrcV      : V of input field
*                : tFieldFlag   : input field flag
* Return Type    : 
                   HI_SUCCESS_DEINTERLACE: success
                   HI_ERR_POINT_NULL     : input parameter error
                   HI_ERR_FIELD_FLAG     : input picture is not field
* Last Modified  : 2007-11-27
*****************************************************************/
HI_API int HI_Deinterlace(void *pHandle,
                          DEINTERLACE_FRAME_S struDstFrame,
                          unsigned char *pszSrcY,
                          unsigned char *pszSrcU,
                          unsigned char *pszSrcV,
                          PIC_TYPE_E tFieldFlag);

/***************************************************************** 
* Function Name  : HI_SetOsd
* Description    : set OSD rectangle area
* Parameters     : pHandle:  deinterlace handle created by HI_InitDeinterlace
                   rcOsd  :  OSD parameters
				   iOsdNum:  number of OSD
* Return Type    : 
                   HI_SUCCESS_DEINTERLACE: success
                   HI_ERR_POINT_NULL     : OSD parameter error
                   HI_ERR_OSDNUM         : OSD number out of range
* Last Modified  : 2007-11-27
*****************************************************************/
HI_API int HI_SetOsd(void *pHandle,
					 DEINTERLACE_OSDRECT_S rcOsd[],
                     int iOsdNum);

/***************************************************************** 
* Function Name : HI_GetVersion
* Description   : get version number
* Parameters    : pszVersion : string version number
* Return Type   : 
                  HI_SUCCESS_DEINTERLACE (if success); 
*                 HI_ERR_POINT_NULL (if failed).
* Last Modified : 2007-11-27
*****************************************************************/
HI_API int HI_GetVersion(char **pszVersion);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif   //_DLLDEINTERLACE_H_
