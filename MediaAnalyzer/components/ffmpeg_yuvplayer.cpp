#include "ffmpeg_yuvplayer.h"


#define BLOCK_SIZE 4096000

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


int FFMPEG_YUVPlayer::player()
{
    FILE *pVideo = NULL;

    SDL_Event event;
    SDL_Rect rect;

    Uint32 pixformat = 0;

    SDL_Window *pWin = NULL;
    SDL_Renderer *pRenderer = NULL;
    SDL_Texture *pTexture = NULL;

    SDL_Thread *pTimer_thread = NULL;

    int w_width = 640, w_height = 480;
    const int video_width = 608, video_height = 368;

    Uint8 *pVideo_pos = NULL;
    Uint8 *pVideo_end = NULL;

    unsigned int remain_len = 0;
    unsigned int video_buff_len = 0;
    unsigned int blank_space_len = 0;
    Uint8 video_buf[BLOCK_SIZE];

    const char *pPath = "/Users/lichao/Documents/out.yuv";

    const unsigned int yuv_frame_len = video_width * video_height * 12 / 8;

    //initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO)) {
        fprintf( stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    //creat window from SDL
    pWin = SDL_CreateWindow("YUV Player",
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           w_width, w_height,
                           SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if(!pWin) {
        fprintf(stderr, "Failed to create window, %s\n",SDL_GetError());
        goto __FAIL;
    }

    pRenderer = SDL_CreateRenderer(pWin, -1, 0);

    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    pixformat= SDL_PIXELFORMAT_IYUV;

    //create texture for render
    pTexture = SDL_CreateTexture(pRenderer,
                                pixformat,
                                SDL_TEXTUREACCESS_STREAMING,
                                video_width,
                                video_height);

    //open yuv file
    pVideo = fopen(pPath, "r");
    if( !pVideo ){
        fprintf(stderr, "Failed to open yuv file\n");
        goto __FAIL;
    }

    //read block data
    if((video_buff_len = fread(video_buf, 1, BLOCK_SIZE, pVideo)) <= 0){
        fprintf(stderr, "Failed to read data from yuv file!\n");
        goto __FAIL;
    }

    //set video positon
    pVideo_pos = video_buf;
    pVideo_end = video_buf + video_buff_len;
    blank_space_len = BLOCK_SIZE - video_buff_len;

    pTimer_thread = SDL_CreateThread(refresh_video_timer,
                                    NULL,
                                    NULL);

    do {
        //Wait
        SDL_WaitEvent(&event);
        if(event.type==REFRESH_EVENT){
            //not enought data to render
            if((pVideo_pos + yuv_frame_len) > pVideo_end){

                //have remain data, but there isn't space
                remain_len = pVideo_end - pVideo_pos;
                if(remain_len && !blank_space_len) {
                    //copy data to header of buffer
                    memcpy(video_buf, pVideo_pos, remain_len);

                    blank_space_len = BLOCK_SIZE - remain_len;
                    pVideo_pos = video_buf;
                    pVideo_end = video_buf + remain_len;
                }

                //at the end of buffer, so rotate to header of buffer
                if(pVideo_end == (video_buf + BLOCK_SIZE)){
                    pVideo_pos = video_buf;
                    pVideo_end = video_buf;
                    blank_space_len = BLOCK_SIZE;
                }

                //read data from yuv file to buffer
                if((video_buff_len = fread(pVideo_end, 1, blank_space_len, pVideo)) <= 0){
                    fprintf(stderr, "eof, exit thread!");
                    thread_exit = 1;
                    continue;// to wait event for exiting
                }

                //reset video_end
                pVideo_end += video_buff_len;
                blank_space_len -= video_buff_len;
                printf("not enought data: pos:%p, pVideo_end:%p, blank_space_len:%d\n", pVideo_pos, pVideo_end, blank_space_len);
            }

            SDL_UpdateTexture( pTexture, NULL, pVideo_pos, video_width);

            //FIX: If window is resize
            rect.x = 0;
            rect.y = 0;
            rect.w = w_width;
            rect.h = w_height;

            SDL_RenderClear( pRenderer );
            SDL_RenderCopy( pRenderer, pTexture, NULL, &rect);
            SDL_RenderPresent( pRenderer );

            printf("not enought data: pos:%p, pVideo_end:%p, blank_space_len:%d\n", pVideo_pos, pVideo_end, blank_space_len);
            pVideo_pos += yuv_frame_len;

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

    //close file
    if(pVideo){
        fclose(pVideo);
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

