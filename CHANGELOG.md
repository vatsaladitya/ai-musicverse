## v2.0.0
Full migration to SDL2!
- FreeGLUT replaced with SDL2 for window management
- OpenGL replaced with SDL2 renderer
- IrrKlang replaced with SDL2_mixer for audio playback
- libdrawtext replaced with SDL2_ttf for rendering text
- SOIL replaced with SDL2_image for image loading
- Removed external resource image dependency and added resource images to resource.res
- Rewritten internal app structure from scratch
- Added drag-and-drop feature
- Minor bug fixes 