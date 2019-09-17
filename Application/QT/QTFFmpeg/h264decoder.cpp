#include "h264decoder.h"
#include <iostream>
H264Decoder::H264Decoder()
{
   pCodec =  nullptr;
   pCodecCtx = nullptr;
   pVideoFrame = nullptr;
   img_conver_ctx  = nullptr;
   pictureWidth = 0;
   setRecordREsolveState = 0;

   //找到对应的解码器
   pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
   if(!pCodec)
   {
       std::cout << "codec not found" << std::endl;
   }

   //通过AvCodec得到AvCodecContext;
   pCodecCtx = avcodec_alloc_context3(pCodec);
   if(!pCodecCtx)
   {
       std::cout << "alloc codec ctx failed " << std::endl;
   }

   //打开解码器
   avcodec_open2(pCodecCtx, pCodec, nullptr);
   pVideoFrame = av_frame_alloc(); //解码之后的数据

   avpicture_alloc(&outPicture, AV_PIX_FMT_RGB24, 640, 360);

}

H264Decoder::~H264Decoder()
{
    if(!pCodecCtx)
    {
        avcodec_close(pCodecCtx);
        pCodecCtx = nullptr;
    }
    if(!pVideoFrame)
    {
        av_frame_free(&pVideoFrame);
        pVideoFrame = nullptr;
    }

    avpicture_free(&outPicture);

    if(!img_conver_ctx)
    {
        sws_freeContext(img_conver_ctx);
        img_conver_ctx = nullptr;
    }

}

int H264Decoder::DecodeH264Frames(unsigned char *inputBuffer, int aLength, RGBData_Define *outputRGBData)
{
    int gotPicPtr = 0;
    int result = 0;

    av_init_packet(&pAvPacket);
    pAvPacket.data = (unsigned char *)inputBuffer;
    pAvPacket.size = aLength;
    result = avcodec_decode_video2(pCodecCtx, pVideoFrame, &gotPicPtr, &pAvPacket);
    if(gotPicPtr)
    {
        if(img_conver_ctx == NULL)
        {
            img_conver_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 640, 360, AV_PIX_FMT_RGB24
                                            , SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
        }
        //转RGB
        sws_scale(img_conver_ctx, (const uint8_t **)pVideoFrame->data, pVideoFrame->linesize,
                  0 , pVideoFrame->height, outPicture.data,outPicture.linesize);

        outputRGBData->width = pCodecCtx->width;
        outputRGBData->height = pCodecCtx->height;
        outputRGBData->bufferSize = outPicture.linesize[0];
        outputRGBData->dataBuffer = outPicture.data[0];
    }

    av_free_packet(&pAvPacket);

    return 0;
}
