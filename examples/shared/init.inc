_state = _context->createState(stateDesc);

_uniformsBuffer = _context->createDynamicBuffer<Uniforms>(1);
_verticesBuffer = _context->createStaticBuffer(SIZEOF_ARRAY(VERTICES), VERTICES);
