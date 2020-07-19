#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct PicWinRes {
    int iWidth;
    int iHeight;
    void *pPixAddr[3]; /* Y U V Planer. */
};

double LinerInterpolation(double iPrePos, double iPreValue, double iNextPos, double iNextValue, double iCurPos, double *CurValue)
{
    *CurValue = (iNextPos-iCurPos)*(iPreValue-iNextValue)/(iNextPos-iPrePos)+iNextValue;
    return *CurValue;
}

int TranslateOrigCoorToPixAddr(int iOrigOX, int iOrigOY, int iOrigCX, int iOrigCY, int iWidth)
{
    return (iOrigCX+iOrigOX)+(iOrigCY+iOrigOY)*iWidth;
}

/* Must be NV21 or Nv12 */
#if 0
void BilinerInterpolation(struct PicWinRes *pstSrc, struct PicWinRes *pstDst)
{
    if (abs(pstSrc->iWidth*pstDst->iHeight - pstSrc->iHeight*pstDst->iWidth) > 100) {
        printf("The proportions is too large. [%dx%d]->[%dx%d].\n",
            pstSrc->iWidth, pstSrc->iHeight, pstDst->iWidth, pstDst->iHeight);
    }

    double lfScaleFactorX = (double)pstSrc->iWidth/pstDst->iWidth;
    double lfScaleFactorY = (double)pstSrc->iHeight/pstDst->iHeight;

    int iLeftBorder, iRightBorder, iTopBorder, iBotBorder;
    double lfSrcPixTargetX, lfSrcPixTargetY;
    int iSrcOrigX = pstSrc->iWidth/2, iSrcOrigY = pstSrc->iHeight/2;

    int iDstPixIdx = 0;
    int iDstTransX, iDstTransY, iDstOrigX = pstDst->iWidth/2, iDstOrigY = pstDst->iHeight/2;

    double lfTargetTopValue, lfTargetBotValue, lfTargetValue;
    char *pSrcYAddr = pstSrc->pPixAddr[0], *pSrcUAddr = pstSrc->pPixAddr[1];
    char *pDstYAddr = pstDst->pPixAddr[0], *pDstUAddr = pstDst->pPixAddr[1];
    for (iDstPixIdx = 0; iDstPixIdx < pstDst->iWidth*pstDst->iHeight; iDstPixIdx++) {
        iDstTransX = iDstPixIdx%pstDst->iWidth - iDstOrigX;
        lfSrcPixTargetX  = (double)iDstTransX*lfScaleFactorX;

        iDstTransY = iDstPixIdx/pstDst->iWidth - iDstOrigY;
        lfSrcPixTargetY  = (double)iDstTransY*lfScaleFactorY;

        iLeftBorder  = (int)(lfSrcPixTargetX-0.5f);
//        iLeftBorder += iSrcOrigX;
        iRightBorder = (int)iLeftBorder+1;
//        iRightBorder+= iSrcOrigX;

        iTopBorder  = (int)(lfSrcPixTargetY-0.5f);
//        iTopBorder += iSrcOrigY;
        iBotBorder = (int)iTopBorder+1;
//        iBotBorder+= iSrcOrigY;
//        printf("%d %d %d %d", iLeftBorder, iRightBorder, iTopBorder, iBotBorder);

        LinerInterpolation(iLeftBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iLeftBorder, iTopBorder, pstSrc->iWidth)],
            iRightBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iRightBorder, iTopBorder, pstSrc->iWidth)],
            lfSrcPixTargetX, &lfTargetTopValue);
        LinerInterpolation(iLeftBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iLeftBorder, iBotBorder, pstSrc->iWidth)],
            iRightBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iRightBorder, iBotBorder, pstSrc->iWidth)],
            lfSrcPixTargetX, &lfTargetBotValue);
        LinerInterpolation(iTopBorder, lfTargetTopValue,
            iBotBorder, lfTargetBotValue,
            lfSrcPixTargetY, &lfTargetValue);
//        printf("%d ", (char)lfTargetValue);
        pDstYAddr[iDstPixIdx] = (char)lfTargetValue;
#if 1
        /*
        int iYUVPlaneIdx = 0;
        for (iYUVPlaneIdx = 0; iYUVPlaneIdx < 2; iYUVPlaneIdx++) {
            LinerInterpolation(iLeftBorder,
                (double)pSrcUAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iLeftBorder, iTopBorder, pstSrc->iWidth)/2+iYUVPlaneIdx],
                iRightBorder,
                (double)pSrcUAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iRightBorder, iTopBorder, pstSrc->iWidth)/2+iYUVPlaneIdx],
                lfSrcPixTargetX, &lfTargetTopValue);
            LinerInterpolation(iLeftBorder,
                (double)pSrcUAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iLeftBorder, iBotBorder, pstSrc->iWidth)/2+iYUVPlaneIdx],
                iRightBorder,
                (double)pSrcUAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iRightBorder, iBotBorder, pstSrc->iWidth)/2+iYUVPlaneIdx],
                lfSrcPixTargetX, &lfTargetBotValue);
            LinerInterpolation(iTopBorder, lfTargetTopValue,
                iBotBorder, lfTargetBotValue,
                lfSrcPixTargetY, &lfTargetValue);

            pDstUAddr[iDstPixIdx/2+iYUVPlaneIdx] = (char)lfTargetValue;
        }
        */
#else
        iDstTransX = iDstPixIdx%pstDst->iWidth - iDstOrigX;
        lfSrcPixTargetX  = (double)iDstTransX*lfScaleFactorX;

        iDstTransY = iDstPixIdx/pstDst->iWidth - iDstOrigY;
        lfSrcPixTargetY  = (double)iDstTransY*lfScaleFactorY;

        iLeftBorder  = (int)(lfSrcPixTargetX-0.5f);
        iRightBorder = (int)iLeftBorder+1;

        iTopBorder  = (int)(lfSrcPixTargetY-0.5f);
        iBotBorder = (int)iTopBorder+1;

        LinerInterpolation(iLeftBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iLeftBorder, iTopBorder, pstSrc->iWidth)],
            iRightBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iRightBorder, iTopBorder, pstSrc->iWidth)],
            lfSrcPixTargetX, &lfTargetTopValue);
        LinerInterpolation(iLeftBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iLeftBorder, iBotBorder, pstSrc->iWidth)],
            iRightBorder,
            (double)pSrcYAddr[TranslateOrigCoorToPixAddr(iSrcOrigX, iSrcOrigY, iRightBorder, iBotBorder, pstSrc->iWidth)],
            lfSrcPixTargetX, &lfTargetBotValue);
        LinerInterpolation(iTopBorder, lfTargetTopValue,
            iBotBorder, lfTargetBotValue,
            lfSrcPixTargetY, &lfTargetValue);
//        printf("%d ", (char)lfTargetValue);
        pDstYAddr[iDstPixIdx] = (char)lfTargetValue;
#endif
    }
}
#else

/* Must be NV21 or Nv12 */
void BilinerInterpolation(struct PicWinRes *pstSrc, struct PicWinRes *pstDst)
{
    if (abs(pstSrc->iWidth*pstDst->iHeight - pstSrc->iHeight*pstDst->iWidth) > 100) {
        printf("The proportions is too large. [%dx%d]->[%dx%d].\n",
            pstSrc->iWidth, pstSrc->iHeight, pstDst->iWidth, pstDst->iHeight);
    }

    double lfScaleFactorX = (double)pstSrc->iWidth/pstDst->iWidth;
    double lfScaleFactorY = (double)pstSrc->iHeight/pstDst->iHeight;

    int iLeftBorder, iRightBorder, iTopBorder, iBotBorder;
    double lfSrcPixTargetX, lfSrcPixTargetY;
    int iSrcOrigX = pstSrc->iWidth/2, iSrcOrigY = pstSrc->iHeight/2;

    int iDstPixIdx = 0;
    int iDstTransX, iDstTransY, iDstOrigX = pstDst->iWidth/2, iDstOrigY = pstDst->iHeight/2;

    double lfTargetTopValue, lfTargetBotValue, lfTargetValue;
    char *pSrcYAddr = pstSrc->pPixAddr[0], *pSrcUAddr = pstSrc->pPixAddr[1];
    char *pDstYAddr = pstDst->pPixAddr[0], *pDstUAddr = pstDst->pPixAddr[1];
    for (iDstPixIdx = 0; iDstPixIdx < pstDst->iWidth*pstDst->iHeight; iDstPixIdx++) {
        lfSrcPixTargetX = (iDstPixIdx%pstDst->iWidth + iDstOrigX)*lfScaleFactorX - iSrcOrigX;
        lfSrcPixTargetY = (iDstPixIdx/pstDst->iWidth + iDstOrigY)*lfScaleFactorY - iSrcOrigY;

        iLeftBorder  = (int)(lfSrcPixTargetX);
        iRightBorder = iLeftBorder+1;
        iTopBorder  = (int)(lfSrcPixTargetY);
        iBotBorder = iTopBorder+1;
//        printf("%d %d %d %d", iLeftBorder, iRightBorder, iTopBorder, iBotBorder);

        LinerInterpolation(iLeftBorder,
            (double)pSrcYAddr[iLeftBorder+iTopBorder*pstSrc->iWidth],
            iRightBorder,
            (double)pSrcYAddr[iRightBorder+iTopBorder*pstSrc->iWidth],
            lfSrcPixTargetX, &lfTargetTopValue);
        LinerInterpolation(iLeftBorder,
            (double)pSrcYAddr[iLeftBorder+iBotBorder*pstSrc->iWidth],
            iRightBorder,
            (double)pSrcYAddr[iRightBorder+iBotBorder*pstSrc->iWidth],
            lfSrcPixTargetX, &lfTargetBotValue);
        LinerInterpolation(iTopBorder, lfTargetTopValue,
            iBotBorder, lfTargetBotValue,
            lfSrcPixTargetY, &lfTargetValue);
//        printf("%d ", (char)lfTargetValue);
        pDstYAddr[iDstPixIdx] = (char)lfTargetValue;
    }

    int iUVSrcWidth = pstSrc->iWidth/2, iUVSrcHeight = pstSrc->iHeight/2;
    int iUVDstWidth = pstDst->iWidth/2, iUVDstHeight = pstDst->iHeight/2;
    for (iDstPixIdx = 0; iDstPixIdx < iUVDstWidth*iUVDstHeight; iDstPixIdx++) {
        lfSrcPixTargetX = (iDstPixIdx%iUVDstWidth + iDstOrigX/2)*lfScaleFactorX - iSrcOrigX/2;
        lfSrcPixTargetY = (iDstPixIdx/iUVDstWidth + iDstOrigY/2)*lfScaleFactorY - iSrcOrigY/2;

        iLeftBorder  = (int)(lfSrcPixTargetX);
        iRightBorder = iLeftBorder+1;
        iTopBorder  = (int)(lfSrcPixTargetY);
        iBotBorder = iTopBorder+1;
//        printf("%d %d %d %d", iLeftBorder, iRightBorder, iTopBorder, iBotBorder);

        LinerInterpolation(iLeftBorder,
            (double)pSrcUAddr[(iLeftBorder+iTopBorder*iUVSrcWidth)*2],
            iRightBorder,
            (double)pSrcUAddr[(iRightBorder+iTopBorder*iUVSrcWidth)*2],
            lfSrcPixTargetX, &lfTargetTopValue);
        LinerInterpolation(iLeftBorder,
            (double)pSrcUAddr[(iLeftBorder+iBotBorder*iUVSrcWidth)*2],
            iRightBorder,
            (double)pSrcUAddr[(iRightBorder+iBotBorder*iUVSrcWidth)*2],
            lfSrcPixTargetX, &lfTargetBotValue);
        LinerInterpolation(iTopBorder, lfTargetTopValue,
            iBotBorder, lfTargetBotValue,
            lfSrcPixTargetY, &lfTargetValue);
//        printf("%d ", (char)lfTargetValue);
        pDstUAddr[iDstPixIdx*2] = (char)lfTargetValue;
#if 1
        LinerInterpolation(iLeftBorder,
            (double)pSrcUAddr[(iLeftBorder+iTopBorder*iUVSrcWidth)*2+1],
            iRightBorder,
            (double)pSrcUAddr[(iRightBorder+iTopBorder*iUVSrcWidth)*2+1],
            lfSrcPixTargetX, &lfTargetTopValue);
        LinerInterpolation(iLeftBorder,
            (double)pSrcUAddr[(iLeftBorder+iBotBorder*iUVSrcWidth)*2+1],
            iRightBorder,
            (double)pSrcUAddr[(iRightBorder+iBotBorder*iUVSrcWidth)*2+1],
            lfSrcPixTargetX, &lfTargetBotValue);
        LinerInterpolation(iTopBorder, lfTargetTopValue,
            iBotBorder, lfTargetBotValue,
            lfSrcPixTargetY, &lfTargetValue);
//        printf("%d ", (char)lfTargetValue);
        pDstUAddr[iDstPixIdx*2+1] = (char)lfTargetValue;
#endif
    }
}

#endif

int main(int argc, char *argv[])
{
    int iRet = -1;
    printf("You must confirm the YUV file which should be NV12/NV21.\n");

    FILE *pSrcYUVFp = NULL;
    FILE *pDstYUVFp = NULL;

    pSrcYUVFp = fopen(argv[1], "rb");
    if (NULL == pSrcYUVFp) {
        printf("Can't open file[%s], check if it is exist.\n", argv[1]);
        goto EOSrcFile;
    }

    pDstYUVFp = fopen("/mnt/extsd/SavedYUVBilinerInterFile.yuv", "w+");
    if (NULL == pDstYUVFp) {
        printf("Can't open file[/mnt/extsd/SavedYUVBilinerInterFile.yuv], check if it is exist.\n");
        goto EODstFile;
    }

    int iSrcWidth = 1920, iSrcHeight = 1080;
    int iDstWidth = 2400, iDstHeight = 1344;
    char *pSrcPixPkt = NULL;
    char *pDstPixPkt = NULL;

    pSrcPixPkt = malloc(iSrcWidth*iSrcHeight*3/2);
    if (NULL == pSrcPixPkt) {
        printf("Alloc memory for source picture failed. errno[%d]\n", errno);
        goto EAllocSrc;
    }
    fread(pSrcPixPkt, iSrcWidth*iSrcHeight*3/2, 1, pSrcYUVFp);

    pDstPixPkt = malloc(iDstWidth*iDstHeight*3/2);
    if (NULL == pSrcPixPkt) {
        printf("Alloc memory for destination picture failed. errno[%d]\n", errno);
        goto EAllocDst;
    }

    struct PicWinRes stSrc, stDst;
    stSrc.pPixAddr[0] = pSrcPixPkt;
    stSrc.pPixAddr[1] = pSrcPixPkt+iSrcWidth*iSrcHeight;
    stSrc.iWidth   = iSrcWidth;
    stSrc.iHeight  = iSrcHeight;
    stDst.pPixAddr[0] = pDstPixPkt;
    stDst.pPixAddr[1] = pDstPixPkt+iDstWidth*iDstHeight;
    stDst.iWidth   = iDstWidth;
    stDst.iHeight  = iDstHeight;
    BilinerInterpolation(&stSrc, &stDst);

    fwrite(pDstPixPkt, iDstWidth*iDstHeight*3/2, 1, pDstYUVFp);

    iRet = 0;
EAllocDst:
EAllocSrc:
    fclose(pSrcYUVFp);
    fclose(pDstYUVFp);

EODstFile:
EOSrcFile:
    return iRet;
}

