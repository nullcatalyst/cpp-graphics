#include "graphics/opengl/context.hpp"

#include <cstdlib> // malloc
#include <cstdio> // fprintf, stderr
#include <cstring> // memcpy

#include "util/debug.hpp" // ASSERT
#include "graphics/opengl/convert.hpp"

namespace graphics::opengl {
    namespace {
        const char * loadShader(const char * fileName) {
            const size_t fileNameLength = strlen(fileName);
            const size_t extLength      = strlen(".glsl");

            char buffer[fileNameLength + extLength + 1];
            strcpy(buffer, fileName);
            strcpy(buffer + fileNameLength, ".glsl");

            FILE * file = fopen(buffer, "r");
            if (file == nullptr) {
                return nullptr;
            }

            fseek(file, 0, SEEK_END);
            size_t fileLength = ftell(file);
            fseek(file, 0, SEEK_SET);

            char * fileBytes = (char *) malloc(fileLength);
            fread(fileBytes, sizeof(char), fileLength, file);
            fclose(file);

            return fileBytes;
        }

        GLuint compileShader(GLenum type, const GLchar * source) {
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            GLint logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            if (logLength > 0) {
                GLchar * log = (GLchar *) malloc(logLength);
                glGetShaderInfoLog(shader, logLength, &logLength, log);
                fprintf(stderr, "[OpenGL] Shader compile log: %s\n", log);
                free(log);
            }

            GLint status;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (status == 0) {
                fprintf(stderr, "[OpenGL] Failed to compile shader\n");
                return 0;
            }

            return shader;
        }

        GLuint compileProgram(const char * vertSource, const char * fragSource) {
            GLuint vertShader = 0;
            GLuint fragShader = 0;
            GLuint program = glCreateProgram();

            vertShader = compileShader(GL_VERTEX_SHADER, vertSource);
            fragShader = compileShader(GL_FRAGMENT_SHADER, fragSource);

            // Attach the shaders to our program
            glAttachShader(program, vertShader);
            glAttachShader(program, fragShader);

            // Delete the shaders since they are now attached to the program, which will retain a reference to them
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);

            glLinkProgram(program);

            { // Assert that the program was successfully linked
                GLint logLength;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
                if (logLength > 0) {
                    GLchar * log = (GLchar *) malloc(logLength);
                    glGetProgramInfoLog(program, logLength, &logLength, log);
                    fprintf(stderr, "[OpenGL] Program link log:\n%s\n", log);
                    free(log);
                }

                GLint status;
                glGetProgramiv(program, GL_LINK_STATUS, &status);
                if (status == 0) {
                    fprintf(stderr, "[OpenGL] Failed to link shader\n");
                    glDeleteProgram(program);
                    return 0;
                }
            }

            return program;
        }

        void bindBuffer(uint32_t bufferIndex, GLuint vbo, const VertexDesc & vertexDesc) {
            const BufferLayout *  layout      = nullptr;

            for (int i = 0; i < vertexDesc.layoutCount; ++i) {
                if (vertexDesc.layouts[i].bufferIndex == bufferIndex) {
                    layout = &vertexDesc.layouts[i];
                    break;
                }
            }

            if (layout == nullptr) {
                return;
            }

            if (layout->stepFunction == StepFunction::Constant) {
                glBindBuffer(GL_UNIFORM_BUFFER, vbo);
                return;
            } else {
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
            }

            for (int i = 0; i < vertexDesc.attributeCount; ++i) {
                const graphics::VertexAttribute & attribute = vertexDesc.attributes[i];
                if (attribute.bufferIndex == bufferIndex) {
                    switch (attribute.format) {
                        case VertexFormat::Float:           glVertexAttribPointer(attribute.id, 1, GL_FLOAT, false, layout->stride, (const void *) (intptr_t) attribute.offset); break;
                        case VertexFormat::Float2:          glVertexAttribPointer(attribute.id, 2, GL_FLOAT, false, layout->stride, (const void *) (intptr_t) attribute.offset); break;
                        case VertexFormat::Float3:          glVertexAttribPointer(attribute.id, 3, GL_FLOAT, false, layout->stride, (const void *) (intptr_t) attribute.offset); break;
                        case VertexFormat::Float4:          glVertexAttribPointer(attribute.id, 4, GL_FLOAT, false, layout->stride, (const void *) (intptr_t) attribute.offset); break;
                        default: break;
                    }

                    switch (layout->stepFunction) {
                        case StepFunction::PerVertex:       glVertexAttribDivisor(attribute.id, 0); break;
                        case StepFunction::PerInstance:     glVertexAttribDivisor(attribute.id, 1); break;
                        default: break;
                    }
                }
            }
        }
    }

    Context::Context() : _activeState(nullptr), _loadShader(loadShader) {}

    Context::Context(LoadShaderFn loadShader) : _activeState(nullptr), _loadShader(loadShader) {}

    Context::~Context() {
        
    }

    StaticBuffer * Context::createStaticBuffer(uint32_t size, uint32_t count, const void * data) {
        return new StaticBuffer(size, count, data);
    }

    DynamicBuffer * Context::createDynamicBuffer(uint32_t size, uint32_t capacity) {
        return new DynamicBuffer(size, capacity);
    }

    Texture * Context::createTexture(const TextureDesc & desc) {
        GLuint tex;
        glGenTextures(1, &tex);
        if (tex == 0) {
            return nullptr;
        }

        glBindTexture(GL_TEXTURE_2D, tex);

        switch (desc.format) {
            case PixelFormat::RGB24u:   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,               desc.width, desc.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr); break;
            case PixelFormat::RGB24s:   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8_SNORM,         desc.width, desc.height, 0, GL_RGB, GL_BYTE, nullptr); break;
            case PixelFormat::RGBA32u:  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,              desc.width, desc.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); break;
            case PixelFormat::RGBA32s:  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8_SNORM,        desc.width, desc.height, 0, GL_RGBA, GL_BYTE, nullptr); break;
            case PixelFormat::BGRA32u:  glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA_INTEGER,       desc.width, desc.height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, nullptr); break;
            case PixelFormat::Depth32f: glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, desc.width, desc.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); break;
            default:                    UNREACHABLE();
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convert(desc.minFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convert(desc.magFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     convert(desc.wrapX));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     convert(desc.wrapY));

        Texture * texture       = new Texture();
        texture->_textureType   = GL_TEXTURE_2D;
        texture->_texture       = tex;
        return texture;
    }

    Texture * Context::loadTexture(const char * textureName) {
        /// TODO: Implement
        return nullptr;
    }

    RenderTarget * Context::createRenderTarget(const RenderTargetDesc & targetDesc) {
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        if (fbo == 0) {
            return nullptr;
        }

        for (int i = 0; i < targetDesc.colorAttachmentCount; ++i) {
            const graphics::Attachment & attachment = targetDesc.colorAttachments[i];
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment.id, ((opengl::Texture *) attachment.texture)->_texture, 0);
        }

        if (targetDesc.depthAttachment.texture != nullptr) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ((opengl::Texture *) targetDesc.depthAttachment.texture)->_texture, 0);
        }

        RenderTarget * target   = new RenderTarget();
        target->_fbo            = fbo;
        return target;
    }

    void Context::setRenderTarget(const RenderTarget * target) {
        if (target == nullptr) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    State * Context::createState(const StateDesc & desc) {
        GLuint shaderProgram = 0;
        if (_loadShader != nullptr) {
            const char * vertexContents   = _loadShader(desc.vertexShader);
            const char * fragmentContents = _loadShader(desc.fragmentShader);
            shaderProgram = compileProgram(vertexContents, fragmentContents);
            free((void *) vertexContents);
            free((void *) fragmentContents);
        } else {
            shaderProgram = compileProgram(desc.vertexShader, desc.fragmentShader);
        }

        if (shaderProgram == 0) {
            return nullptr;
        }

        State * state                   = new State();
        state->_shaderProgram           = shaderProgram;
        state->_cullMode                = convert(desc.cullMode);
        state->_winding                 = convert(desc.winding);
        state->_depthWriteEnabled       = desc.depthWriteEnabled;
        state->_depthCompareFunction    = convert(desc.depthCompareFunction);
        state->_depthBias               = desc.depthBias;
        state->_depthSlopeScale         = desc.depthSlopeScale;

        if (desc.vertexDesc != nullptr) {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            state->_vao = vao;

            const uint32_t      attributeCount      = desc.vertexDesc->attributeCount;
            const uint32_t      attributeLength     = sizeof(VertexAttribute) * attributeCount;
            const uint32_t      layoutCount         = desc.vertexDesc->layoutCount;
            const uint32_t      layoutLength        = sizeof(BufferLayout) * layoutCount;
            void *              vertexDescBuffer    = malloc(attributeLength + layoutLength);
            VertexAttribute *   attributes          = (VertexAttribute *) vertexDescBuffer;
            BufferLayout *      layouts             = (BufferLayout *) &attributes[attributeCount];

            memcpy(attributes, desc.vertexDesc->attributes, attributeLength);
            memcpy(layouts, desc.vertexDesc->layouts, layoutLength);

            state->_vertexDesc.attributeCount       = attributeCount;
            state->_vertexDesc.layoutCount          = layoutCount;
            state->_vertexDesc.attributes           = attributes;
            state->_vertexDesc.layouts              = layouts;

            for (int i = 0; i < attributeCount; ++i) {
                glEnableVertexAttribArray(desc.vertexDesc->attributes[i].id);
            }
        }

        return state;
    }

    void Context::setState(const State * state) {
        _activeState = state;

        glUseProgram(state->_shaderProgram);
        glBindVertexArray(state->_vao);

        if (state->_cullMode != GL_NONE) {
            glEnable(GL_CULL_FACE);
            glCullFace(state->_cullMode);
        } else {
            glDisable(GL_CULL_FACE);
        }

        glDepthMask(state->_depthWriteEnabled);
        glDepthFunc(state->_depthCompareFunction);
        glPolygonOffset(state->_depthSlopeScale, state->_depthBias);
    }

    void Context::startFrame() {
        _frame = getNextFrame();
        _activeState = nullptr;
    }

    void Context::commitFrame() {
        _activeState = nullptr;
        glFlush();
    }

    void Context::setBuffer(uint32_t id, StaticBuffer * buffer, bool forFragmentShader) {
        ASSERT(_activeState != nullptr && _activeState->_vao != 0, "");

        bindBuffer(id, buffer->_vbo, _activeState->_vertexDesc);
    }

    void Context::setBuffer(uint32_t id, DynamicBuffer * buffer, bool forFragmentShader) {
        ASSERT(_activeState != nullptr && _activeState->_vao != 0, "");

        bindBuffer(id, buffer->_vbos[_frame], _activeState->_vertexDesc);
    }

    void Context::setTexture(uint32_t id, Texture * texture, bool forFragmentShader) {
        glUniform1i(/* uniform */ 0, id);
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(texture->_textureType, texture->_texture);
    }

    void Context::drawTriangles(uint32_t vertexCount) {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void Context::drawTriangleStrip(uint32_t vertexCount) {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
    }

    void Context::drawIndexedTriangles(uint32_t indexCount, StaticBuffer * indexBuffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->_vbo);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, nullptr);
    }

    void Context::drawIndexedTriangleStrip(uint32_t indexCount, StaticBuffer * indexBuffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->_vbo);

        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_SHORT, nullptr);
    }

    void Context::drawInstancedTriangles(uint32_t instanceCount, uint32_t vertexCount) {
        glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
    }

    void Context::drawInstancedTriangleStrip(uint32_t instanceCount, uint32_t vertexCount) {
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, vertexCount, instanceCount);
    }

    void Context::drawInstancedIndexedTriangles(uint32_t instanceCount, uint32_t indexCount, StaticBuffer * indexBuffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->_vbo);

        glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, nullptr, instanceCount);
    }

    void Context::drawInstancedIndexedTriangleStrip(uint32_t instanceCount, uint32_t indexCount, StaticBuffer * indexBuffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->_vbo);

        glDrawElementsInstanced(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_SHORT, nullptr, instanceCount);
    }
}
