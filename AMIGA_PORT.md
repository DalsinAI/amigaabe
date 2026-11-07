Abe's Amazing Adventure 1.1 for AmigaOS 3.x
===========================================

Abe's Amazing Adventure (Gabor Torok, Pedro Izecksohn and Alex Clarck),
the flip-screen platform game after Montezuma's Revenge: guide Abe through
a vast pyramid of traps, ladders, doors and monsters, collecting keys and
balloons, with its own Ogg Vorbis music. For AmigaOS 3.x on a 68040 or
68060 with an FPU and an RTG screen, on OpenGPU's SDL 1.2 (sdl12-compat).

Base: abe_1.1+dfsg.orig.tar.gz from Debian (sha256
7da9e237bd16448b6d9f79193a3898aaef52dd9d2653e30a67b817f6b4d47d48; upstream's
tarball without its Windows SDL DLLs), with Debian's patches (1.1+dfsg-6.1)
as the second commit. Branch team2/abe-amiga.

Status: plays in the lab (Instance-45 on GFOpenUp 0.8.19, 6 November
2026): the pictures unpacked from images.tar, the title with its scroller,
Start Game, Abe walking and jumping through the first rooms (the arrow trap,
the score, a life lost), Escape to the title, and Exit back to the Shell,
the intro and game music decoded all the while. The lab is muted, so the
music and sounds were not heard there. Screenshots from that run are in
amiga/screenshots: the title (abe-title.png), the first room
(abe-first-room.png) and the arrow trap (abe-arrow-trap.png).


What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.x, an RTG card (or the
  AmigaChrome lab).
- GFOpenUp 0.8.18 or later (LIBS:OpenGPU/SDL2.module). Everything else is
  in the program.


How to play
-----------

Open the Abe icon, or run Abe from a Shell. It opens a 640x480 window
(Abe --nosound for no sound). On the title the cursor keys and Return
choose. In the game the cursor keys move and climb, Space jumps, Return
rides a balloon, Escape goes back to the title. ReadMe-upstream in the
drawer is upstream's README. The pictures, maps, sounds and music are in
the drawer's images, maps and sounds; the saved games are kept in user/.
The game takes a while to start: every picture is unpacked from
images.tar first.


Building
--------

    make -f amiga/Makefile.amigaos -j2 dist

with GFOpenAmigaGCC 16.2.0-open4, the OpenNDK headers and the OpenGPU kit
(see the Makefile's head). The drawer is dist/Abe (and dist/Abe.info).


What was changed
----------------

- src/Main.c: on AmigaOS the game opens a window (upstream starts full
  screen). src/Directories.c: the saved games and the pictures' scratch
  file are in the drawer's user/ (no getpwuid(); amiga/include/pwd.h only
  for the header).
- amiga/Makefile.amigaos builds src/Makefile.am's sources with the data in
  the drawer (BASE_DIR=PROGDIR:).
- amiga/amiga_paths.c (-Wl,--wrap=fopen, open, opendir, stat, mkdir and
  chdir, from the IceBreaker port): the game joins "PROGDIR:" and a name
  with "/", and to AmigaDOS ":/" is the parent drawer; it is folded.
- amiga/amiga_rwops.c (-Wl,--wrap=SDL_RWFromFP and SDL_RWFromFile): the
  maps and saved games are read through RWops, and sdl12-compat's never
  returned from the first map; here they are plain stdio.
- No SDL_mixer 1.2 for OpenGPU's SDL 1.2: the Team's amiga/sdl_mixer_lite.c
  (WAV), from the Circus Linux! port, now with Ogg Vorbis music through
  stb_vorbis (amiga/stb_vorbis.c, Sean Barrett, public domain), decoded a
  buffer at a time in the audio callback and taken down to 22050 Hz;
  libmikmod comes with it (amiga/libmikmod).
- amiga/stack.c: 512 KB of stack.


Licences
--------

Abe's Amazing Adventure, its pictures, maps, sounds and music: GPL 2 or
later (COPYING.txt). stb_vorbis: public domain (or MIT). libmikmod: LGPL 2
or later (COPYING-libmikmod.txt). The Amiga files (amiga/): the AmiCradle
Team, GPL 2 or later, except the mixer, which comes from the Team's Wizznic
port and is GPL 3 or later, so the program as built here is GPL 3 or later;
the icon is Abe himself, tom4.bmp from upstream's images.tar.
