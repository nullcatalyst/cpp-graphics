#import "./Renderer.h"

#include <simd/simd.h>
#include <graphics/graphics.hpp>

#define SIZEOF_ARRAY(array) (sizeof(array) / sizeof(*array))

// Declare the graphics data used for initialization
#include "../shared/decl.inc"

@implementation Renderer {
    graphics::Context *         _context;
    graphics::State *           _state;
    graphics::DynamicBuffer *   _uniformsBuffer;
    graphics::StaticBuffer *    _verticesBuffer;
}

- (nonnull instancetype) initWithMetalKitView:(nonnull MTKView *)view {
    if ((self = [super init])) {
        _context = new graphics::Context(view);

        // Initialize the graphics objects
        #include "../shared/init.inc"
    }

    return self;
}

- (void) drawInMTKView:(nonnull MTKView *)view {
    // Render the frame
    #include "../shared/update.inc"

    // Render the frame
    #include "../shared/render.inc"
}

- (void) mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    // Respond to drawable size or orientation changes here
}

@end
