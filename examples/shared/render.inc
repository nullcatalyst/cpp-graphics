_context->startFrame();

    // Render to the screen
    _context->setRenderTarget(nullptr);

    // Set the graphics state
    // This includes the shaders, depth and blend settings
    _context->setState(_state);

    // Set the buffers to use to render
    _context->setBuffer(0, _verticesBuffer);
    _context->setBuffer(1, _uniformsBuffer);

    // Finally draw the triangles
    _context->drawTriangleStrip(SIZEOF_ARRAY(VERTICES));

_context->commitFrame();
