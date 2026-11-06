#include "Main.h"

int runmode;

SDL_Surface *screen;
int state;
Main mainstruct;
int loading_max = 108;
int loading_progress = 0;

void
testModesInFormat(SDL_PixelFormat * format)
{
  SDL_Rect **modes;
  int i;

  printf("Available hardware accelerated, fullscreen modes in %d bpp:\n",
         format->BitsPerPixel);

  /* Get available fullscreen/hardware modes */
  modes =
    SDL_ListModes(format, SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF);

  /* Check is there are any modes available */
  if(modes == (SDL_Rect **) 0) {
    printf("\tNo modes available!\n");
    return;
  }

  /* Check if our resolution is restricted */
  if(modes == (SDL_Rect **) - 1) {
    printf("\tAll resolutions available.\n");
  } else {
    /* Print valid modes */
    for(i = 0; modes[i]; ++i)
      printf("\t%d x %d\n", modes[i]->w, modes[i]->h);
  }

  free(modes);
}

void
testModes()
{
  SDL_PixelFormat format;

  format.BitsPerPixel = 16;
  testModesInFormat(&format);
  format.BitsPerPixel = 24;
  testModesInFormat(&format);
  format.BitsPerPixel = 32;
  testModesInFormat(&format);
}

void
showLoadingProgress()
{
  SDL_Rect rect;
  int w = 200, h = 10;

  rect.x = screen->w / 2 - w / 2;
  rect.y = screen->h / 2 - h / 2;
  rect.w = w;
  rect.h = h;
  SDL_FillRect(screen, &rect,
               SDL_MapRGBA(screen->format, 0x40, 0x40, 0x00, 0x00));

  rect.x = screen->w / 2 - w / 2;
  rect.y = screen->h / 2 - h / 2;
  rect.w =
    (int) (((double) w / (double) loading_max) *
           (double) (loading_progress++));
  rect.h = h;
  SDL_FillRect(screen, &rect,
               SDL_MapRGBA(screen->format, 0x80, 0x80, 0x00, 0x00));

  SDL_Flip(screen);
  if(loading_progress >= loading_max)
    loading_max += 20;
  //SDL_Delay(50);
}

int
main(int argc, char *argv[])
{
  Uint32 flags = SDL_DOUBLEBUF;
  int i, j;
  static int available_resolution_width [] = { 320, 320, 640, 640, 800, 1024, 1280, 1600, 1280, 1280, 1440, 1400, 0 };
  static int available_resolution_height[] = { 200, 240, 400, 480, 600,  768, 1024, 1200,  768,  800,  900, 1050, 0 };
  int w, h, bpp, n;
  int hw_mem = 1;
  int intro = 0;
  char *mapname;
  int mapwidth, mapheight;

  mainstruct.drawBackground = 1;
  mainstruct.alphaBlend = 1;
  mainstruct.effects_enabled = 1;
  runmode = RUNMODE_SPLASH;
  mainstruct.full_screen = 1;
  
  // my laptop can't handle fullscreen for some reason
#if defined(__APPLE__) || defined(__MACH_O__)
  mainstruct.full_screen = 0;
#endif

  w = 640;
  h = 480;
  bpp = 16;

  if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }

  atexit(SDL_Quit);

  SDL_VideoInfo *vid_info;
  vid_info = (SDL_VideoInfo*) SDL_GetVideoInfo ();
  if( vid_info )
    if( vid_info->current_w == 1440 && vid_info->current_h ==900
    ||  vid_info->current_w == 1400 && vid_info->current_h ==1050 )
    {
      w = vid_info->current_w;
      h = vid_info->current_h;
    }

  for(i = 0; i < argc; i++) {
    if(!strcmp(argv[i], "--window")) {
      mainstruct.full_screen = 0;
    } else if(!strcmp(argv[i], "--system") || !strcmp(argv[i], "-s")) {
      hw_mem = 0;
    } else if(!strcmp(argv[i], "--editor") || !strcmp(argv[i], "-e")) {
      runmode = RUNMODE_EDITOR;
    } else if(!strcmp(argv[i], "--nosound")) {
      sound_enabled = 0;
    } else if(!strcmp(argv[i], "--intro") || !strcmp(argv[i], "-i")) {
      runmode = RUNMODE_EDITOR;
      intro = 1;
    } else if(!strcmp(argv[i], "--game") || !strcmp(argv[i], "-g")) {
      runmode = RUNMODE_GAME;
    } else if(!strcmp(argv[i], "--size") && i < argc - 1) {
      n = atoi(argv[i + 1]);
      for( j = 0 ; available_resolution_width[j] != 0 ; j++ )
        if( j == n )
        {
          w = available_resolution_width[j];
          h = available_resolution_height[j];
          break;
        }
    } else if((!strcmp(argv[i], "--bpp") || !strcmp(argv[i], "-b"))
              && i < argc - 1) {
      n = atoi(argv[i + 1]);
      if(n == 15 || n == 16 || n == 24 || n == 32)
        bpp = n;
    } else if(!strcmp(argv[i], "--test") || !strcmp(argv[i], "-t")) {
      testModes();
      exit(0);
    } else if(!strcmp(argv[i], "--convert")) {
      convertMap(argv[i + 1], argv[i + 2]);
      exit(0);
    } else if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-?")
              || !strcmp(argv[i], "-h")) {
      printf("Abe!! Happy Birthday, 2002\n\n");
      printf("--window           Run in windowed mode.\n");
      printf
        ("-e --editor        Skip the splash screen and run the editor.\n");
      printf("-i --intro         Edit intro map.\n");
      printf("-g --game          Skip the splash screen and run the game.\n");
      printf("-t --test          Test video modes only.\n");
      printf
        ("-s --system        Use system memory instead of video(default) memory.\n");
      printf
        ("--size #           Use this width/height for the video mode.\n");
      printf
        ("\tModes:\n");
      for( j = 0 ; available_resolution_width[j] != 0 ; j++ )
        printf( "\t  %2d: %4d/%4d (%1.2f)\n", j,
          available_resolution_width[j], available_resolution_height[j],
          (float)available_resolution_width[j] / (float)available_resolution_height[j] );
      printf("-b --bpp #         Use this bpp for the video mode.\n");
      printf("--nosound          Don't use sound.\n");
      printf("-? -h --help       Show this help message.\n");
      exit(0);
    }
  }

  // the default map
  if(runmode != RUNMODE_SPLASH && !intro) {
    mapname = strdup("default");
    mapwidth = 1000;
    mapheight = 1000;
  } else {
    mapname = strdup("intro");
    mapwidth = 640 / TILE_W;
    mapheight = 480 / TILE_H;
  }

  if(hw_mem) {
    flags |= SDL_HWSURFACE;
  } else {
    flags |= SDL_SWSURFACE;
  }
  if(mainstruct.full_screen) {
    flags |= SDL_FULLSCREEN;
  }

  fprintf(stderr, "Attempting to set %dx%dx%d video mode.\n", w, h, bpp);
  fflush(stderr);
  screen = SDL_SetVideoMode(w, h, bpp, flags);
  if(screen == NULL) {
    fprintf(stderr, "Unable to set %dx%dx%d video: %s\n", w, h, bpp,
            SDL_GetError());
    exit(1);
  }
  fprintf(stderr, "Success:\n");
  fprintf(stderr, "\tSDL_HWSURFACE =%s\n",
          (screen->flags & SDL_HWSURFACE ? "true" : "false"));
  fprintf(stderr, "\tSDL_FULLSCREEN=%s\n",
          (screen->flags & SDL_FULLSCREEN ? "true" : "false"));
  fprintf(stderr, "\tSDL_DOUBLEBUF =%s\n",
          (screen->flags & SDL_DOUBLEBUF ? "true" : "false"));
  fprintf(stderr, "\tw=%d h=%d bpp=%d pitch=%d\n", screen->w, screen->h,
          screen->format->BitsPerPixel, screen->pitch);
  fflush(stderr);

  SDL_WM_SetCaption("Abe's Amazing Adventure!!", (const char *) NULL);

  SDL_ShowCursor(0);
  showLoadingProgress();

  initAudio();
  showLoadingProgress();

  initMonsters();
  showLoadingProgress();

  loadImages();
  showLoadingProgress();

  initEditor();
  showLoadingProgress();

  initGame();
  showLoadingProgress();

  if(intro) {
    initMap("intro", 640 / TILE_W, 480 / TILE_H);
    editMap();
  } else {
    showIntro();
  }

  return 0;
}
