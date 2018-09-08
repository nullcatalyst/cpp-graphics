#include "graphics/metal/context.hpp"

#include "util/debug.hpp" // ASSERT
#include "graphics/metal/convert.hpp"

namespace graphics::metal {
    Context::Context(MTKView * view) : _view(view), _frame(0) {
        _inFlightSemaphore  = dispatch_semaphore_create(BUFFER_COUNT);
        _device             = view.device;
        _commandQueue       = [_device newCommandQueue];
    }

    Context::~Context() {
        // dispatch_release(_inFlightSemaphore); // Not available in automatic reference counting (ARC) mode
        _inFlightSemaphore  = nil;
        _device             = nil;
        _commandQueue       = nil;
        _commandBuffer      = nil;
        _renderEncoder      = nil;
    }

    StaticBuffer * Context::createStaticBuffer(uint32_t size, uint32_t capacity, const void * data) {
        return new StaticBuffer(size, capacity, data, _device);
    }

    DynamicBuffer * Context::createDynamicBuffer(uint32_t size, uint32_t capacity) {
        return new DynamicBuffer(size, capacity, _device);
    }

    Texture * Context::createTexture(const TextureDesc & desc) {
        MTLTextureDescriptor * textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:convert(desc.format)
                                                                                                      width:desc.width
                                                                                                     height:desc.height
                                                                                                  mipmapped:desc.storage == StorageHint::MipMap];

        textureDescriptor.usage           = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        textureDescriptor.resourceOptions = convert(desc.storage);

        Texture * texture = new Texture();
        texture->_texture = [_device newTextureWithDescriptor:textureDescriptor];
        return texture;
    }

    Texture * Context::loadTexture(const char * textureName) {
        /// TODO: Implement
        return nullptr;
    }

    RenderTarget * Context::createRenderTarget(const RenderTargetDesc & targetDesc) {
        MTLRenderPassDescriptor * renderPassDescriptor = [[MTLRenderPassDescriptor alloc] init];

        for (int i = 0; i < targetDesc.colorAttachmentCount; ++i) {
            const Attachment & attachment = targetDesc.colorAttachments[i];
            renderPassDescriptor.colorAttachments[attachment.id].texture        = ((Texture *) attachment.texture)->_texture;
            renderPassDescriptor.colorAttachments[attachment.id].loadAction     = attachment.clear ? MTLLoadActionClear : MTLLoadActionDontCare;

#if OS(ios)
            renderPassDescriptor.colorAttachments[attachment.id].storeAction    = attachment.texture->_texture.storageMode == MTLStorageModeMemoryless ? MTLStoreActionDontCare : MTLStoreActionStore;
#else
            renderPassDescriptor.colorAttachments[attachment.id].storeAction    = MTLStoreActionStore;
#endif
        }

        if (targetDesc.depthAttachment.texture != nullptr) {
            renderPassDescriptor.depthAttachment.texture        = ((Texture *) targetDesc.depthAttachment.texture)->_texture;
            renderPassDescriptor.depthAttachment.loadAction     = targetDesc.depthAttachment.clear ? MTLLoadActionClear : MTLLoadActionDontCare;
#if OS(ios)
            renderPassDescriptor.depthAttachment.storeAction    = targetDesc.depthAttachment.texture->_texture.storageMode == MTLStorageModeMemoryless ? MTLStoreActionDontCare : MTLStoreActionStore;
#else
            renderPassDescriptor.depthAttachment.storeAction    = MTLStoreActionStore;
#endif
            renderPassDescriptor.depthAttachment.clearDepth     = 1.0;
        }/* else {
            renderPassDescriptor.depthAttachment.texture        = _view.depthStencilTexture;
        }*/

        return new RenderTarget(renderPassDescriptor);
    }

    void Context::changeColorAttachment(RenderTarget * target, uint32_t id, Texture * texture) {
        target->_renderPassDescriptor.colorAttachments[id].texture = texture->_texture;
    }

    void Context::changeDepthAttachment(RenderTarget * target, Texture * texture) {
        target->_renderPassDescriptor.depthAttachment.texture = texture->_texture;
    }

    State * Context::createState(const StateDesc & desc) {
        id<MTLLibrary>  defaultLibrary      = [_device newDefaultLibrary];
        id<MTLFunction> vertexFunction      = desc.vertexShader != nullptr ? [defaultLibrary newFunctionWithName:[NSString stringWithUTF8String:desc.vertexShader]] : nullptr;
        id<MTLFunction> fragmentFunction    = desc.fragmentShader != nullptr ? [defaultLibrary newFunctionWithName:[NSString stringWithUTF8String:desc.fragmentShader]] : nullptr;

        MTLVertexDescriptor * vertexDescriptor = nil;
        if (desc.vertexDesc != nullptr) {
            vertexDescriptor = [[MTLVertexDescriptor alloc] init];

            for (int i = 0; i < desc.vertexDesc->attributeCount; ++i) {
                const VertexAttribute & attribute = desc.vertexDesc->attributes[i];
                vertexDescriptor.attributes[attribute.id].format            = convert(attribute.format);
                vertexDescriptor.attributes[attribute.id].offset            = attribute.offset;
                vertexDescriptor.attributes[attribute.id].bufferIndex       = attribute.bufferIndex;
            }

            for (int i = 0; i < desc.vertexDesc->layoutCount; ++i) {
                const BufferLayout & layout = desc.vertexDesc->layouts[i];
                vertexDescriptor.layouts[layout.bufferIndex].stride         = layout.stride;
                vertexDescriptor.layouts[layout.bufferIndex].stepRate       = 1;
                vertexDescriptor.layouts[layout.bufferIndex].stepFunction   = convert(layout.stepFunction);
            }
        }

        MTLRenderPipelineDescriptor * pipelineStateDescriptor   = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.vertexFunction                  = vertexFunction;
        pipelineStateDescriptor.fragmentFunction                = fragmentFunction;
        pipelineStateDescriptor.vertexDescriptor                = vertexDescriptor;

        if (desc.targetDesc != nullptr) {
            pipelineStateDescriptor.sampleCount                 = desc.targetDesc->sampleCount;

            for (int i = 0; i < desc.targetDesc->colorAttachmentCount; ++i) {
                const Attachment & attachment = desc.targetDesc->colorAttachments[i];
                pipelineStateDescriptor.colorAttachments[attachment.id].pixelFormat = ((Texture *) attachment.texture)->_texture.pixelFormat;
            }

            if (desc.targetDesc->depthAttachment.texture != nullptr) {
                pipelineStateDescriptor.depthAttachmentPixelFormat      = ((Texture *) desc.targetDesc->depthAttachment.texture)->_texture.pixelFormat;
                pipelineStateDescriptor.stencilAttachmentPixelFormat    = MTLPixelFormatInvalid;//_view.depthStencilPixelFormat;
            } else {
                pipelineStateDescriptor.depthAttachmentPixelFormat      = MTLPixelFormatInvalid;//_view.depthStencilPixelFormat;
                pipelineStateDescriptor.stencilAttachmentPixelFormat    = MTLPixelFormatInvalid;
            }
        } else {
            pipelineStateDescriptor.sampleCount                     = _view.sampleCount;
            pipelineStateDescriptor.colorAttachments[0].pixelFormat = _view.colorPixelFormat;
            pipelineStateDescriptor.depthAttachmentPixelFormat      = _view.depthStencilPixelFormat;
            pipelineStateDescriptor.stencilAttachmentPixelFormat    = MTLPixelFormatInvalid;//_view.depthStencilPixelFormat;
        }

        NSError * error = nil;
        id<MTLRenderPipelineState> renderPipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        if (error != nil) {
            NSLog(@"ERROR: %@", error);
        }
        ASSERT(renderPipelineState != nil, "");

        MTLDepthStencilDescriptor * depthStateDescriptor    = [[MTLDepthStencilDescriptor alloc] init];
        depthStateDescriptor.depthCompareFunction           = convert(desc.depthCompareFunction);
        depthStateDescriptor.depthWriteEnabled              = desc.depthWriteEnabled;
        id<MTLDepthStencilState> depthStencilState          = [_device newDepthStencilStateWithDescriptor:depthStateDescriptor];

        State * state = new State();
        state->_renderPipelineState     = renderPipelineState;
        state->_depthStencilState       = depthStencilState;
        state->_cullMode                = desc.cullMode;
        state->_winding                 = desc.winding;
        state->_depthBias               = desc.depthBias;
        state->_depthSlopeScale         = desc.depthSlopeScale;
        state->_depthBiasClamp          = desc.depthBiasClamp;
        return state;
    }

    void Context::setRenderTarget(RenderTarget * target) {
        if (_renderEncoder != nil) {
            [_renderEncoder endEncoding];
            _renderEncoder = nil;
        }

        if (target == nullptr) {
            _renderEncoder = [_commandBuffer renderCommandEncoderWithDescriptor:_view.currentRenderPassDescriptor];
        } else {
            _renderEncoder = [_commandBuffer renderCommandEncoderWithDescriptor:target->_renderPassDescriptor];
        }
    }

    void Context::setState(const State * state) {
        ASSERT(state != nullptr, "");
        ASSERT(_renderEncoder != nil, "");

        [_renderEncoder setRenderPipelineState:state->_renderPipelineState];
        [_renderEncoder setDepthStencilState:state->_depthStencilState];
        [_renderEncoder setFrontFacingWinding:convert(state->_winding)];
        [_renderEncoder setCullMode:convert(state->_cullMode)];

//        if (metalState->_depthBias != 0.0f || metalState->_depthSlopeScale != 0.0f || metalState->_depthBiasClamp != 0) {
            [_renderEncoder setDepthBias:state->_depthBias slopeScale:state->_depthSlopeScale clamp:state->_depthBiasClamp];
//        }
    }

    void Context::startFrame() {
        dispatch_semaphore_wait(_inFlightSemaphore, DISPATCH_TIME_FOREVER);

        _frame = getNextFrame();
        _commandBuffer = [_commandQueue commandBuffer];

        __block dispatch_semaphore_t block_sema = _inFlightSemaphore;
        [_commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
            dispatch_semaphore_signal(block_sema);
        }];
    }

    void Context::commitFrame() {
        [_renderEncoder endEncoding];
        [_commandBuffer presentDrawable:_view.currentDrawable];
        [_commandBuffer commit];

        _renderEncoder = nil;
        _commandBuffer = nil;
    }

    void Context::setBuffer(uint32_t id, StaticBuffer * buffer, uint32_t offset, bool forFragmentShader) {
        ASSERT(buffer != nullptr, "");

        if (forFragmentShader) {
            [_renderEncoder setFragmentBuffer:buffer->_buffer offset:0 atIndex:id];
        } else {
            [_renderEncoder setVertexBuffer:buffer->_buffer offset:0 atIndex:id];
        }
    }

    void Context::setBuffer(uint32_t id, DynamicBuffer * buffer, uint32_t offset, bool forFragmentShader) {
        ASSERT(buffer != nullptr, "");

        if (forFragmentShader) {
            [_renderEncoder setFragmentBuffer:buffer->_buffer offset:buffer->getOffset(_frame) + offset atIndex:id];
        } else {
            [_renderEncoder setVertexBuffer:buffer->_buffer offset:buffer->getOffset(_frame) + offset atIndex:id];
        }
    }

    void Context::setTexture(uint32_t id, Texture * texture, bool forFragmentShader) {
#if OS(ios)
        if (texture->_texture.storageMode != MTLStorageModeMemoryless) {
            [_renderEncoder setFragmentTexture:texture->_texture atIndex:id];
        }
#else
        [_renderEncoder setFragmentTexture:texture->_texture atIndex:id];
#endif
    }

    void Context::drawTriangles(uint32_t count) {
        [_renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                           vertexStart:0
                           vertexCount:count];
    }

    void Context::drawTriangleStrip(uint32_t count) {
        [_renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                           vertexStart:0
                           vertexCount:count];
    }

    void Context::drawIndexedTriangles(uint32_t count, StaticBuffer * indexBuffer) {
        ASSERT(indexBuffer != nullptr, "");

        [_renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                   indexCount:count
                                    indexType:MTLIndexTypeUInt16
                                  indexBuffer:indexBuffer->_buffer
                            indexBufferOffset:0];
    }

    void Context::drawIndexedTriangleStrip(uint32_t count, StaticBuffer * indexBuffer) {
        ASSERT(indexBuffer != nullptr, "");

        [_renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangleStrip
                                   indexCount:count
                                    indexType:MTLIndexTypeUInt16
                                  indexBuffer:indexBuffer->_buffer
                            indexBufferOffset:0];
    }

    void Context::drawInstancedTriangles(uint32_t instanceCount, uint32_t vertexCount) {
        [_renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                           vertexStart:0
                           vertexCount:vertexCount
                         instanceCount:instanceCount];
    }

    void Context::drawInstancedTriangleStrip(uint32_t instanceCount, uint32_t vertexCount) {
        [_renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                           vertexStart:0
                           vertexCount:vertexCount
                         instanceCount:instanceCount];
    }

    void Context::drawInstancedIndexedTriangles(uint32_t instanceCount, uint32_t indexCount, StaticBuffer * indexBuffer) {
        ASSERT(indexBuffer != nullptr, "");

        [_renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                   indexCount:indexCount
                                    indexType:MTLIndexTypeUInt16
                                  indexBuffer:indexBuffer->_buffer
                            indexBufferOffset:0
                                instanceCount:instanceCount];
    }

    void Context::drawInstancedIndexedTriangleStrip(uint32_t instanceCount, uint32_t indexCount, StaticBuffer * indexBuffer) {
        ASSERT(indexBuffer != nullptr, "");

        [_renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangleStrip
                                   indexCount:indexCount
                                    indexType:MTLIndexTypeUInt16
                                  indexBuffer:indexBuffer->_buffer
                            indexBufferOffset:0
                                instanceCount:instanceCount];
    }
}
