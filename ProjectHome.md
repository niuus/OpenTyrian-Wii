This is a new and fresh port of the wonderful game OpenTyrian to the Wii system.  This is separate from Nuvalo's port, which uses wii-linux.  This port instead is a direct port using the SDL Wii port cleaned up and put together by Tantric.

Everything works.  Music is now correctly synthesized at 32khz, and the sound effects are running a little slower than that (but not by much).

In order to compile, you'll need <a href='http://code.google.com/p/sdl-wii'>SDL Wii</a> by Tantric (use the directions located <a href='http://www.wiibrew.org/wiki/SDL_Wii'>on Wiibrew</a> to incorporate into libogc), the libogc compiled libraries located on Tantric's SNES9x GX <a href='http://code.google.com/p/snes9x-gx/downloads/list'>downloads list</a> (though I would strongly suggest you get the latest SVNs of both of them AND of libfat and build them yourselves.  They hold many fixes important to OpenTyrianWii), and you'll need <a href='http://www.wiibrew.org/wiki/Libwiikeyboard'>libwiikeyboard</a>.  Again, follow the directions there pertaining to installation.

Everything needed to run the program is included in the downloads (including the Tyrian files).

Installation is as easy as copying the contents of the archives to the root of your SD card (and don't worry about any warnings windows might give you.  It won't overwrite anything you've got on there).  You don't have to copy the included readme.  For those who have versions older than 1.1.0, please move your tyrian.cfg and tyrian.sav files into the directory tyrian/userdata.  Otherwise you'll have to start over.