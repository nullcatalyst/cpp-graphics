#import "./GLView.h"

#import <CoreVideo/CoreVideo.h> // CVDisplayLinkRef
#include <simd/simd.h>
#include <graphics/graphics.hpp>

#define SIZEOF_ARRAY(array) (sizeof(array) / sizeof(*array))

// Declare the graphics data used for initialization
#include "../shared/decl.inc"

namespace {
    CVReturn displayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp * now, const CVTimeStamp * outputTime, CVOptionFlags flagsIn, CVOptionFlags * flagsOut, void * displayLinkContext) {
        @autoreleasepool {
            // go back to Obj-C for easy access to instance variables
            [((__bridge GLView *) displayLinkContext) drawFrame];
            return kCVReturnSuccess;
        }
    }
}

@implementation GLView {
    CVDisplayLinkRef            _displayLink;
    graphics::Context *         _context;
    graphics::State *           _state;
    graphics::DynamicBuffer *   _uniformsBuffer;
    graphics::StaticBuffer *    _verticesBuffer;
}

- (nonnull instancetype) initWithFrame:(NSRect)frameRect {
    NSOpenGLPixelFormatAttribute pixelFormatAttribs[] = {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFADepthSize, 32,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        0,
    };

    NSOpenGLPixelFormat * pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttribs];
    if (pixelFormat == nil) {
        fprintf(stderr, "Unable to create windowed pixel format.\n");
        abort();
    }

    if ((self = [super initWithFrame:frameRect pixelFormat:pixelFormat])) {
        NSOpenGLContext * currentContext = self.openGLContext;
        [currentContext makeCurrentContext];

        // Remember to lock the context before we touch it since display link is threaded
        CGLContextObj cglContext = currentContext.CGLContextObj;
        CGLLockContext(cglContext);

        // Enable vsync to eliminate tearing
        GLint vsync = 1;
        [[self openGLContext] setValues:&vsync forParameter:NSOpenGLContextParameterSwapInterval];

        // Create the graphics context
        _context = new graphics::Context();

        // Initialize the graphics objects
        #include "../shared/init.inc"

        CGLUnlockContext(cglContext);

        // Set up the display link
        CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
        CVDisplayLinkSetOutputCallback(_displayLink, displayLinkCallback, (__bridge void *) self);
        CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj) [[self pixelFormat] CGLPixelFormatObj];
        CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(_displayLink, cglContext, cglPixelFormat);
    }

    return self;
}

- (void) dealloc {
    CVDisplayLinkStop(_displayLink);
}

- (void) startRendering {
    CVDisplayLinkStart(_displayLink);
}

- (void) stopRendering {
    CVDisplayLinkStop(_displayLink);
}

- (void) drawFrame {
    NSOpenGLContext * currentContext = self.openGLContext;

    // Must lock GL context because display link is threaded
    CGLLockContext(currentContext.CGLContextObj);
    [currentContext makeCurrentContext];

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the frame
//    #include "../shared/update.inc"

    // Render the frame
    #include "../shared/render.inc"

    [currentContext flushBuffer];
    CGLUnlockContext(currentContext.CGLContextObj);
}

- (void) drawRect:(NSRect)rect {
    [self drawFrame];
}

@end
