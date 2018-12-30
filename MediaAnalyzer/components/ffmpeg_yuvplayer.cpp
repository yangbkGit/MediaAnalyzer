#include "ffmpeg_yuvplayer.h"

#define DISP_DEBUG
#ifdef DISP_DEBUG
#define PLAYER_DBUG(fmt, arg...) UMFDBG("[FFMPEG_YUVPlayer][%s:%d]" fmt , __FUNCTION__, __LINE__, ##arg)
#else
#define PLAYER_DBUG(fmt, arg...)
#endif


//event message
#define REFRESH_EVENT  (SDL_USEREVENT + 1)
#define QUIT_EVENT  (SDL_USEREVENT + 2)

static int thread_exit=0;



int FFMPEG_YUVPlayer::refresh_video_timer(void *udata){
    thread_exit=0;
    while (!thread_exit) {
        SDL_Event event;
        event.type = REFRESH_EVENT;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }

    thread_exit=0;

    //push quit event
    SDL_Event event;
    event.type = QUIT_EVENT;
    SDL_PushEvent(&event);

    return 0;
}

/**
 * @brief FFMPEG_YUVPlayer::player
 * @param videoPath
 * @param w_width
 * @param w_height
 * @param video_width
 * @param video_height
 * @param pixFormat
 * @return 0 is success.
 *
 * FFMPEG_YUVPlayer::player("F:/MediaAnalyzer/SupportedFiles/Forrest_Gump_IMAX.yuv",
                         650, 362, 640, 352, SDL_PIXELFORMAT_IYUV);
 */
int FFMPEG_YUVPlayer::player(const char *videoPath,
                             int w_width, int w_height,
                             const int video_width, const int video_height,
                             Uint32 pixFormat)
{
    FILE *pVideo = NULL;

    SDL_Event event;
    SDL_Rect rect;

    SDL_Window *pWin = NULL;
    SDL_Renderer *pRenderer = NULL;
    SDL_Texture *pTexture = NULL;

    SDL_Thread *pTimer_thread = NULL;
    size_t video_buff_len = 0;

    const unsigned int yuv_frame_len = video_width * video_height * 12 / 8;
    unsigned int tmp_yuv_frame_len = yuv_frame_len;

    if(yuv_frame_len & 0xF){
        tmp_yuv_frame_len = (yuv_frame_len & 0xFFF0) + 0x10;
    }
    Uint8 *video_buf = (Uint8 *)malloc(tmp_yuv_frame_len);

    MPEG_FUNC_CALL(SDL_Init(SDL_INIT_VIDEO));
    //creat window from SDL
    MEM_FUNC_CALL(pWin = SDL_CreateWindow("YUV Player",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           w_width, w_height,
                                           SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE));

    MEM_FUNC_CALL(pRenderer = SDL_CreateRenderer(pWin, -1, 0));

    //create texture for render
    MEM_FUNC_CALL(pTexture = SDL_CreateTexture(pRenderer, pixFormat,
                                                SDL_TEXTUREACCESS_STREAMING,
                                                video_width, video_height));

    //open yuv file
    MEM_FUNC_CALL(pVideo = fopen(videoPath, "r"));

    //read block data
    if((video_buff_len = fread(video_buf, 1, yuv_frame_len, pVideo)) <= 0){
        PLAYER_DBUG("Failed to read data from yuv file!\n");
        goto __FAIL;
    }

    pTimer_thread = SDL_CreateThread(refresh_video_timer,
                                    NULL,
                                    NULL);

    do {
        //Wait
        SDL_WaitEvent(&event);

        if(event.type == REFRESH_EVENT){
            SDL_UpdateTexture( pTexture, NULL, video_buf, video_width);

            //FIX: If window is resize
            rect.x = 0;
            rect.y = 0;
            rect.w = w_width;
            rect.h = w_height;

            SDL_RenderClear( pRenderer );
            SDL_RenderCopy( pRenderer, pTexture, NULL, &rect);
            SDL_RenderPresent( pRenderer );

            //read data from yuv file to buffer
            if((video_buff_len = fread(video_buf, 1, yuv_frame_len, pVideo)) <= 0){
                PLAYER_DBUG("eof, ready exit thread! video_buff_len = %d\n", video_buff_len);
                //thread_exit = 1;
                continue;// to wait event for exiting
            }

        }else if(event.type==SDL_WINDOWEVENT){
            //If Resize
            SDL_GetWindowSize(pWin, &w_width, &w_height);
        }else if(event.type==SDL_QUIT){
            thread_exit=1;
        }else if(event.type==QUIT_EVENT){
            break;
        }
    }while ( 1 );

__FAIL:
    if(video_buf){
        free(video_buf);
        video_buf = NULL;
    }

    //close file
    if(pVideo){
        fclose(pVideo);
        pVideo = NULL;
    }

    if(pTexture){
        SDL_DestroyTexture(pTexture);
        pTexture = NULL;
    }
    if(pRenderer){
        SDL_DestroyRenderer(pRenderer);
        pRenderer = NULL;
    }
    if(pWin){
        SDL_DestroyWindow(pWin);
        pWin = NULL;
    }

    SDL_Quit();

    return 0;
}





































FFMPEG_YUVPlayer::FFMPEG_YUVPlayer()
{

}

FFMPEG_YUVPlayer::~FFMPEG_YUVPlayer()
{

}

