#define GL_SILENCE_DEPRECATION 1
#import <Cocoa/Cocoa.h>

@interface GLView : NSOpenGLView

- (void) startRendering;
- (void) stopRendering;
- (void) drawFrame;

@end
