/**
 Photon Splatting Using a View-Sample Cluster Hierarchy
 Brno University of Technology (BUT)
 Faculty of Information Technologies (FIT)
 
 @file    GLDraw.cpp
 @author  Marcel Kiss, xkissm01@stud.fit.vutbr.cz
 @date    27/11/18
 Copyright © 2019 Marcel Kiss
 */

#include "GLDraw.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

std::string glErrorToString(GLenum e) {
    switch (e) {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        default: return "<Unknown>";
    }
}

#if INCLUDE_OPENCL
std::string clErrorToString(cl_int e) {
    switch (e) {
        case CL_SUCCESS: return "CL_SUCCESS";
        case CL_DEVICE_NOT_FOUND: return "CL_DEVICE_NOT_FOUND";
        case CL_DEVICE_NOT_AVAILABLE: return "CL_DEVICE_NOT_AVAILABLE";
        case CL_COMPILER_NOT_AVAILABLE: return "CL_COMPILER_NOT_AVAILABLE";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case CL_OUT_OF_RESOURCES: return "CL_OUT_OF_RESOURCES";
        case CL_OUT_OF_HOST_MEMORY: return "CL_OUT_OF_HOST_MEMORY";
        case CL_PROFILING_INFO_NOT_AVAILABLE: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case CL_MEM_COPY_OVERLAP: return "CL_MEM_COPY_OVERLAP";
        case CL_IMAGE_FORMAT_MISMATCH: return "CL_IMAGE_FORMAT_MISMATCH";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case CL_BUILD_PROGRAM_FAILURE: return "CL_BUILD_PROGRAM_FAILURE";
        case CL_MAP_FAILURE: return "CL_MAP_FAILURE";
        case CL_MISALIGNED_SUB_BUFFER_OFFSET: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case CL_COMPILE_PROGRAM_FAILURE: return "CL_COMPILE_PROGRAM_FAILURE";
        case CL_LINKER_NOT_AVAILABLE: return "CL_LINKER_NOT_AVAILABLE";
        case CL_LINK_PROGRAM_FAILURE: return "CL_LINK_PROGRAM_FAILURE";
        case CL_DEVICE_PARTITION_FAILED: return "CL_DEVICE_PARTITION_FAILED";
        case CL_KERNEL_ARG_INFO_NOT_AVAILABLE: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
        case CL_INVALID_VALUE: return "CL_INVALID_VALUE";
        case CL_INVALID_DEVICE_TYPE: return "CL_INVALID_DEVICE_TYPE";
        case CL_INVALID_PLATFORM: return "CL_INVALID_PLATFORM";
        case CL_INVALID_DEVICE: return "CL_INVALID_DEVICE";
        case CL_INVALID_CONTEXT: return "CL_INVALID_CONTEXT";
        case CL_INVALID_QUEUE_PROPERTIES: return "CL_INVALID_QUEUE_PROPERTIES";
        case CL_INVALID_COMMAND_QUEUE: return "CL_INVALID_COMMAND_QUEUE";
        case CL_INVALID_HOST_PTR: return "CL_INVALID_HOST_PTR";
        case CL_INVALID_MEM_OBJECT: return "CL_INVALID_MEM_OBJECT";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case CL_INVALID_IMAGE_SIZE: return "CL_INVALID_IMAGE_SIZE";
        case CL_INVALID_SAMPLER: return "CL_INVALID_SAMPLER";
        case CL_INVALID_BINARY: return "CL_INVALID_BINARY";
        case CL_INVALID_BUILD_OPTIONS: return "CL_INVALID_BUILD_OPTIONS";
        case CL_INVALID_PROGRAM: return "CL_INVALID_PROGRAM";
        case CL_INVALID_PROGRAM_EXECUTABLE: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case CL_INVALID_KERNEL_NAME: return "CL_INVALID_KERNEL_NAME";
        case CL_INVALID_KERNEL_DEFINITION: return "CL_INVALID_KERNEL_DEFINITION";
        case CL_INVALID_KERNEL: return "CL_INVALID_KERNEL";
        case CL_INVALID_ARG_INDEX: return "CL_INVALID_ARG_INDEX";
        case CL_INVALID_ARG_VALUE: return "CL_INVALID_ARG_VALUE";
        case CL_INVALID_ARG_SIZE: return "CL_INVALID_ARG_SIZE";
        case CL_INVALID_KERNEL_ARGS: return "CL_INVALID_KERNEL_ARGS";
        case CL_INVALID_WORK_DIMENSION: return "CL_INVALID_WORK_DIMENSION";
        case CL_INVALID_WORK_GROUP_SIZE: return "CL_INVALID_WORK_GROUP_SIZE";
        case CL_INVALID_WORK_ITEM_SIZE: return "CL_INVALID_WORK_ITEM_SIZE";
        case CL_INVALID_GLOBAL_OFFSET: return "CL_INVALID_GLOBAL_OFFSET";
        case CL_INVALID_EVENT_WAIT_LIST: return "CL_INVALID_EVENT_WAIT_LIST";
        case CL_INVALID_EVENT: return "CL_INVALID_EVENT";
        case CL_INVALID_OPERATION: return "CL_INVALID_OPERATION";
        case CL_INVALID_GL_OBJECT: return "CL_INVALID_GL_OBJECT";
        case CL_INVALID_BUFFER_SIZE: return "CL_INVALID_BUFFER_SIZE";
        case CL_INVALID_MIP_LEVEL: return "CL_INVALID_MIP_LEVEL";
        case CL_INVALID_GLOBAL_WORK_SIZE: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case CL_INVALID_PROPERTY: return "CL_INVALID_PROPERTY";
        case CL_INVALID_IMAGE_DESCRIPTOR: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case CL_INVALID_COMPILER_OPTIONS: return "CL_INVALID_COMPILER_OPTIONS";
        case CL_INVALID_LINKER_OPTIONS: return "CL_INVALID_LINKER_OPTIONS";
        case CL_INVALID_DEVICE_PARTITION_COUNT: return "CL_INVALID_DEVICE_PARTITION_COUNT";
        default: return "<Unknown>";
    }
}



using namespace std;

cl_platform_id CLPlatformGet() {
    cl_platform_id platform;
    cl_int err = clGetPlatformIDs(1, &platform, NULL);
    clCheckError(err);
    return platform;
}

cl_device_id CLDeviceGet(cl_platform_id platform, cl_device_type device_type) {
    cl_device_id device_id;
    cl_int err = clGetDeviceIDs(platform, device_type, 1, &device_id, NULL);
    clCheckError(err);
    return device_id;
}

#if defined(_WIN32)

#include <Windows.h>
#include <GL/glcorearb.h>
#include <KHR/khrplatform.h>
#include <CL/cl_gl.h>

#pragma OPENCL EXTENSION CL_KHR_gl_sharing : enable

// TODO: Test windows
cl_context CLCreateContext(cl_device_id device, cl_platform_id platform) {
    
    cl_context_properties clContextProperties[] = {
        CL_GL_CONTEXT_KHR,   (cl_context_properties)wglGetCurrentContext(),
        CL_WGL_HDC_KHR,      (cl_context_properties)wglGetCurrentDC(),
        CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
        0
    };
    cl_int err = 0;
    cl_context context = clCreateContext(clContextProperties, 1, &device, NULL, NULL, &err);
    clCheckError(err);
    return context;
}

#elif defined(__APPLE__)

#include <OpenGL/OpenGL.h>
#include <OpenCL/opencl.h>

/* This is not neccesary, just for backup */
//#pragma OPENCL EXTENSION CL_APPLE_gl_sharing : enable

cl_context CLCreateContext(cl_device_id device, cl_platform_id platform) {
    CGLContextObj cglContext = CGLGetCurrentContext();
    CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
    cl_context_properties clContextProperties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties) cglShareGroup,
        0
    };
    cl_int err = 0;
    cl_context context = clCreateContext(clContextProperties, 1, &device, NULL, NULL, &err);
    clCheckError(err);
    return context;
}

#else

#pragma OPENCL EXTENSION CL_KHR_gl_sharing : enable

// TODO: Test linux
cl_context CLCreateContext(cl_device_id device, cl_platform_id platform) {
    cl_context_properties clContextProperties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
        0
    };
    cl_int err = 0;
    cl_context context = clCreateContext(clContextProperties, 1, &device, NULL, NULL, &err);
    clCheckError(err);
    return context;
}

#endif

cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t global) {
    cl_event e;
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global, NULL, 0, NULL, &e);
    clCheckError(err);
    return e;
}

cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t global, cl_event &event) {
    cl_event e;
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global, NULL, 1, &event, &e);
    clCheckError(err);
    return e;
}

cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t g, size_t l) {
    cl_event e;
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &g, &l, 0, NULL, &e);
    clCheckError(err);
    return e;
}

cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t g, size_t l, cl_event &event) {
    cl_event e;
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &g, &l, 1, &event, &e);
    clCheckError(err);
    return e;
}

cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy) {
    cl_event e;
    const size_t gsize[] = { gx, gy };
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, gsize, NULL, 0, NULL, &e);
    clCheckError(err);
    return e;
}

cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy, cl_event &event) {
    cl_event e;
    const size_t gsize[] = { gx, gy };
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, gsize, NULL, 1, &event, &e);
    clCheckError(err);
    return e;
}

cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy, size_t lx, size_t ly) {
    cl_event e;
    const size_t gsize[] = { gx, gy };
    const size_t lsize[] = { lx, ly };
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, gsize, lsize, 0, NULL, &e);
    clCheckError(err);
    return e;
}

cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy, size_t lx, size_t ly, cl_event &event) {
    cl_event e;
    const size_t gsize[] = { gx, gy };
    const size_t lsize[] = { lx, ly };
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, gsize, lsize, 1, &event, &e);
    clCheckError(err);
    return e;
}

void CLEnqueueAcquireGLObjects(cl_command_queue queue, vector<cl_mem> objs) {
    cl_int err = clEnqueueAcquireGLObjects(queue, objs.size(), objs.data(), 0, NULL, NULL);
    clCheckError(err);
}

void CLEnqueueReleaseGLObjects(cl_command_queue queue, vector<cl_mem> objs) {
    cl_int err = clEnqueueReleaseGLObjects(queue, objs.size(), objs.data(), 0, NULL, NULL);
    clCheckError(err);
}

cl_ulong CLGetEventNanoTime(cl_event event) {
    cl_ulong time_start;
    cl_ulong time_end;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    return time_end - time_start;
}

/** This function dynamicaly allocates memory for build
 log string (malloc). User should dealloc it after usage. */
char* CLGetBuildInfo(cl_program program, cl_device_id device) {
    size_t len = 0;
    cl_int ret = CL_SUCCESS;
    ret = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
    clCheckError(ret);
    char *buffer = (char *)malloc(len);
    ret = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
    clCheckError(ret);
    return buffer;
}

cl_program CLCreateAndBuildProgramWithSource(cl_context context, cl_device_id device, const char *source, size_t length) {
    cl_int err = 0;
    cl_program program = clCreateProgramWithSource(context, 1, &source, &length, &err);
    clCheckError(err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    char *buildInfo = NULL;
    buildInfo = CLGetBuildInfo(program, device);
    if (buildInfo) std::cerr << buildInfo << std::endl;
    clCheckError(err);
    if (buildInfo) delete buildInfo;
    return program;
}

cl_program CLCreateAndBuildProgramWithFile(cl_context context, cl_device_id device, const char *path) {
    std::string source;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        source = stream.str();
        return CLCreateAndBuildProgramWithSource(context, device, source.c_str(), source.length());
    } catch(std::ifstream::failure e) {
        std::cerr << "Failed to load file '" << path << "'" << std::endl;
    }
    return NULL;
}

cl_kernel CLCreateKernel(cl_program program, const char * name) {
    cl_int err = 0;
    cl_kernel kernel = clCreateKernel(program, name, &err);
    clCheckError(err);
    return kernel;
}

const char* clChannelOrderToString(cl_channel_order channel_order) {
    switch (channel_order) {
        case CL_R: return "CL_R";
        case CL_A: return "CL_A";
        case CL_RG: return "CL_RG";
        case CL_RA: return "CL_RA";
        case CL_RGB: return "CL_RGB";
        case CL_RGBA: return "CL_RGBA";
        case CL_BGRA: return "CL_BGRA";
        case CL_ARGB: return "CL_ARGB";
        case CL_INTENSITY: return "CL_INTENSITY";
        case CL_LUMINANCE: return "CL_LUMINANCE";
        case CL_Rx: return "CL_Rx";
        case CL_RGx: return "CL_RGx";
        case CL_RGBx: return "CL_RGBx";
        case CL_DEPTH: return "CL_DEPTH";
        case CL_DEPTH_STENCIL: return "CL_DEPTH_STENCIL";
        default: return "<unknown>";
    }
}

const char* clChannelTypeToString(cl_channel_type channel_type) {
    switch (channel_type) {
        case CL_SNORM_INT8: return "CL_SNORM_INT8";
        case CL_SNORM_INT16: return "CL_SNORM_INT16";
        case CL_UNORM_INT8: return "CL_UNORM_INT8";
        case CL_UNORM_INT16: return "CL_UNORM_INT16";
        case CL_UNORM_SHORT_565: return "CL_UNORM_SHORT_565";
        case CL_UNORM_SHORT_555: return "CL_UNORM_SHORT_555";
        case CL_UNORM_INT_101010: return "CL_UNORM_INT_101010";
        case CL_SIGNED_INT8: return "CL_SIGNED_INT8";
        case CL_SIGNED_INT16: return "CL_SIGNED_INT16";
        case CL_SIGNED_INT32: return "CL_SIGNED_INT32";
        case CL_UNSIGNED_INT8: return "CL_UNSIGNED_INT8";
        case CL_UNSIGNED_INT16: return "CL_UNSIGNED_INT16";
        case CL_UNSIGNED_INT32: return "CL_UNSIGNED_INT32";
        case CL_HALF_FLOAT: return "CL_HALF_FLOAT";
        case CL_FLOAT: return "CL_FLOAT";
        case CL_UNORM_INT24: return "CL_UNORM_INT24";
        default: return "<unknown>";
    }
}

cl_mem CLCreateBuffer(cl_context ctx, size_t size) {
    cl_int err;
    cl_mem buffer = clCreateBuffer(ctx, CL_MEM_READ_WRITE, size, NULL, &err);
    clCheckError(err);
    return buffer;
}

void CLReleaseMemory(cl_mem mem) {
    if (mem) {
        cl_int err = clReleaseMemObject(mem);
        clCheckError(err);
    }
}

cl_image_format clGetImageFormat(cl_mem image) {
    cl_image_format image_format;
    size_t size;
    cl_int error = clGetImageInfo(image, CL_IMAGE_FORMAT, sizeof(image_format), &image_format, &size);
    clCheckError(error);
    return image_format;
}



#endif










GLTexture2D::GLTexture2D(): _id(0) {}

bool GLTexture2D::init() {
    glCheckError();
    glGenTextures(1, &_id);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();
    bool success = (_id != 0);
    if (!success) cerr << "[GLTexture2D] Unable to create texture" << endl;
    return success;
}

void GLTexture2D::destroy() {
    glDeleteTextures(1, &_id);
    _id = 0;
}

void GLTexture2D::data(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *data) const {
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glCheckError();
    unbind();
}

void GLTexture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, _id);
}

void GLTexture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint GLTexture2D::operator()() const {
    return _id;
}




#if INCLUDE_OPENCL
void GLTexture2D::clMake(cl_context ctx) {
    cl_int clError = 0;
    _cl_read = clCreateFromGLTexture(ctx, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, _id, &clError);
    clCheckError(clError);
    _cl_write = clCreateFromGLTexture(ctx, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, _id, &clError);
    clCheckError(clError);
}

cl_mem GLTexture2D::clRead() const {
    return _cl_read;
}

cl_mem GLTexture2D::clWrite() const {
    return _cl_write;
}
#endif


inline string GLShaderTypeToString(GLShaderType shaderType) {
    switch (shaderType) {
        case GLShader_Vertex: return "Vertex";
        case GLShader_TessControl: return "TessControl";
        case GLShader_TessEval: return "TessEval";
        case GLShader_Geometry: return "Geometry";
        case GLShader_Fragment: return "Fragment";
        default: return "<Unknown>";
    }
}

GLuint GLShaderCompileFile(GLShaderType shaderType, const string path) {
    string source;
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        file.open(path);
        stringstream stream;
        stream << file.rdbuf();
        file.close();
        source = stream.str();
        return GLShaderCompileSource(shaderType, source);
    } catch(ifstream::failure e) {
        cerr << "[GLShader][" << GLShaderTypeToString(shaderType) << "]: Failed to load file '" << path << "'" << endl;
    }
    return 0;
}

GLuint GLShaderCompileSource(GLShaderType shaderType, const string &source) {
    GLuint id = 0;
    const char *src = source.c_str();
    id = glCreateShader(shaderType);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        cerr << "[GLShader][" << GLShaderTypeToString(shaderType) << "] Compilation failed:" << endl;
        cerr << infoLog << endl;
    }
    return id;
}

GLProgram::GLProgram() {}
GLProgram::~GLProgram() { glDelete(); }

void GLProgram::glInit(GLuint s1) { const vector<GLuint> s = { s1 }; glInit(s); }
void GLProgram::glInit(GLuint s1, GLuint s2) { const vector<GLuint> s = { s1, s2 }; glInit(s); }
void GLProgram::glInit(GLuint s1, GLuint s2, GLuint s3) { const vector<GLuint> s = { s1, s2, s3 }; glInit(s); }
void GLProgram::glInit(const vector<GLuint> &shaders) {
    _id = glCreateProgram();
    if (!_id) cerr << "[GLProgram] Unable to create program" << endl;
    for (auto shader : shaders) {
        glAttachShader(_id, shader);
    }
    glLinkProgram(_id);
    int success;
    char infoLog[512];
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        cerr << "[GLProgram] Linking failed:" << endl;
        cerr << infoLog << endl;
    }
}

void GLProgram::glDelete() { glDeleteProgram(_id); _id = 0; }

GLint GLProgram::location(const string &name) {
    if (_locations.count(name) > 0) {
        return _locations[name];
    } else {
        int loc = glGetUniformLocation(_id, name.c_str());
        _locations[name] = loc;
        if (loc < 0) cerr << "[GLProgram] Unknown location for '" + name + "' (" << loc << ")" << endl;
        return loc;
    }
}


void GLProgram::set(const string &name, bool value) { glProgramUniform1i(_id, location(name), (int)value); }
void GLProgram::set(const string &name, int value) { glProgramUniform1i(_id, location(name), value); }
void GLProgram::set(const string &name, unsigned int value) { glProgramUniform1i(_id, location(name), value); }
void GLProgram::set(const string &name, float value) { glProgramUniform1f(_id, location(name), value); }

void GLProgram::set4f(const std::string &name, float v0, float v1, float v2, float v3) { glProgramUniform4f(_id, location(name), v0, v1, v2, v3); }
void GLProgram::set4x4f(const std::string &name, const float *v) { glProgramUniformMatrix4fv(_id, location(name), 1, GL_FALSE, v); }

void GLProgram::set(const string &name, const glm::ivec2 v) { glProgramUniform2iv(_id, location(name), 1, &v[0]); }
void GLProgram::set(const string &name, const glm::ivec3 v) { glProgramUniform3iv(_id, location(name), 1, &v[0]); }
void GLProgram::set(const string &name, const glm::ivec4 v) { glProgramUniform4iv(_id, location(name), 1, &v[0]); }

void GLProgram::set(const string &name, const glm::vec2 v) { glProgramUniform2fv(_id, location(name), 1, &v[0]); }
void GLProgram::set(const string &name, const glm::vec3 v) { glProgramUniform3fv(_id, location(name), 1, &v[0]); }
void GLProgram::set(const string &name, const glm::vec4 v) { glProgramUniform4fv(_id, location(name), 1, &v[0]); }

void GLProgram::set(const string &name, const glm::mat4 m) { glProgramUniformMatrix4fv(_id, location(name), 1, GL_FALSE, &m[0][0]); }

GLuint GLProgram::operator ()() const { return _id; }

void GLProgram::use() const { glUseProgram(_id); }











static GLuint _glDrawSharedVAO = 0;
static GLuint _glDrawSharedVertexVBO = 0;
static GLuint _glDrawSharedTexCoordVBO = 0;

const string _glDraw2_Vertex = R"glsl(
#version 400 core
layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_uv;
uniform mat4 transform;
out vec2 uv;
void main() { gl_Position = transform * vec4(in_pos, 0.0f, 1.0f); uv = in_uv; }
)glsl";

const string _glDraw3_Vertex = R"glsl(
#version 400 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;
uniform mat4 transform;
out vec2 uv;
void main() { gl_Position = transform * vec4(in_pos, 1.0f); uv = in_uv; }
)glsl";


const string _glDraw_Fragment = R"glsl(
#version 400 core
in vec2 uv;
out vec4 frag;
uniform vec4 color;
uniform sampler2D tex;
uniform usampler2D utex;
uniform float near;
uniform float far;

subroutine vec4 getFragmentColor();
subroutine(getFragmentColor) vec4 clearColor() { return color; }
subroutine(getFragmentColor) vec4 textureColor() { return texture(tex, uv) * color; }
subroutine(getFragmentColor) vec4 textureUColor() { uvec4 c = texture(utex, uv); return vec4(c.r / 255.0, c.g / 255.0, c.b / 255.0, c.a / 255.0); }
subroutine(getFragmentColor) vec4 textureDepth() { return vec4(vec3(texture(tex, uv).x), 1.0f); }
subroutine(getFragmentColor) vec4 textureLinearDepth() {
    float d = (2 * near) / (far + near - texture(tex, uv).x * (far - near));
    return vec4(vec3(d), 1.0f);
}

subroutine uniform getFragmentColor fragmentColorEval;
void main() { frag = fragmentColorEval(); }
)glsl";


static GLuint _glDraw2Subroutines[GLDrawMode_Count];
static GLuint _glDraw3Subroutines[GLDrawMode_Count];
static GLProgram _glDraw2_Program;
static GLProgram _glDraw3_Program;

static GLProgram *_glDraw_ActiveProgram = NULL;
static GLDrawMode _glDraw_ActiveColorMode = GLDrawMode_Color;
static float _glDraw_UniformColor[4] = { 1, 1, 1, 1 };
static const float _glDraw_TransformIdentity[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
static float _glDraw_UniformTransform[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
static GLuint _glDraw_UniformTexture = 0;
static float _glDraw_UniformNear = 1;
static float _glDraw_UniformFar = 1000;

static bool _glDraw_EnableDefaultPipeline = true;
void glDrawEnableDefaultPipeline(bool enableDefaultPipeline) { _glDraw_EnableDefaultPipeline = enableDefaultPipeline; }
bool glDrawIsEnabledDefaultPipeline() { return _glDraw_EnableDefaultPipeline; }

static const GLint _glDraw_DefaultVertexAttribIndex = 0;
static GLint _glDraw_VertexAttribIndex = _glDraw_DefaultVertexAttribIndex;
void glDrawSetDefaultVertexAtribIndex() { _glDraw_VertexAttribIndex = _glDraw_DefaultVertexAttribIndex; }
void glDrawSetVertexAttribIndex(GLint vertexAttrib) { _glDraw_VertexAttribIndex = vertexAttrib; }
GLint glDrawGetVertexAttribIndex() { return _glDraw_VertexAttribIndex; }

static const GLint _glDraw_DefaultTexCoordAttribIndex = 1;
static GLint _glDraw_TexCoordAttribIndex = _glDraw_DefaultTexCoordAttribIndex;
void glDrawSetDefaultTexCoordAttribIndex() { _glDraw_TexCoordAttribIndex = _glDraw_DefaultTexCoordAttribIndex; }
void glDrawSetTexCoordAttribIndex(GLint texCoordAttrib) { _glDraw_TexCoordAttribIndex = texCoordAttrib; }
GLint glDrawGetTexCoordAttribIndex() { return _glDraw_TexCoordAttribIndex; }


bool _glDrawInit() {
    if (!_glDrawSharedVAO || !_glDrawSharedVertexVBO || !_glDrawSharedTexCoordVBO) {
        glGenVertexArrays(1, &_glDrawSharedVAO);
        glGenBuffers(1, &_glDrawSharedVertexVBO);
        glGenBuffers(1, &_glDrawSharedTexCoordVBO);
        
        GLuint glDrawVertex2 = GLShaderCompileSource(GLShader_Vertex, _glDraw2_Vertex);
        GLuint glDrawVertex3 = GLShaderCompileSource(GLShader_Vertex, _glDraw3_Vertex);
        GLuint glDrawFragment = GLShaderCompileSource(GLShader_Fragment, _glDraw_Fragment);
        
        _glDraw2_Program.glInit(glDrawVertex2, glDrawFragment);
        _glDraw3_Program.glInit(glDrawVertex3, glDrawFragment);
        
        glDeleteShader(glDrawVertex2);
        glDeleteShader(glDrawVertex3);
        glDeleteShader(glDrawFragment);
        
        const char *subroutines[GLDrawMode_Count] = {
            "clearColor", "textureColor", "textureUColor", "textureDepth", "textureLinearDepth"
        };
        for (int colorModeIndex = 0; colorModeIndex < GLDrawMode_Count; colorModeIndex++) {
            _glDraw2Subroutines[colorModeIndex] = glGetSubroutineIndex(_glDraw2_Program(), GL_FRAGMENT_SHADER, subroutines[colorModeIndex]);
            _glDraw3Subroutines[colorModeIndex] = glGetSubroutineIndex(_glDraw3_Program(), GL_FRAGMENT_SHADER, subroutines[colorModeIndex]);
        }
    }
    
    static bool _glDrawErrorThrown = false;
    if ((!_glDrawSharedVAO || !_glDrawSharedVertexVBO || !_glDrawSharedTexCoordVBO) && !_glDrawErrorThrown) {
        cerr << "[OpenGL][glDraw] Error with initialization glDraw!" << endl;
        _glDrawErrorThrown = true;
        return false;
    }
    return true;
}

void _glDrawUniforms() {
    if (_glDraw_ActiveColorMode == GLDrawMode_TextureUColor) {
        _glDraw_ActiveProgram->set("tex", _glDraw_UniformTexture + 1);
        _glDraw_ActiveProgram->set("utex", _glDraw_UniformTexture);
        glCheckError();
    } else {
        _glDraw_ActiveProgram->set("utex", _glDraw_UniformTexture + 1);
        _glDraw_ActiveProgram->set("tex", _glDraw_UniformTexture);
        glCheckError();
    }
    _glDraw_ActiveProgram->set4f("color", _glDraw_UniformColor[0], _glDraw_UniformColor[1], _glDraw_UniformColor[2], _glDraw_UniformColor[3]);
    _glDraw_ActiveProgram->set4x4f("transform", _glDraw_UniformTransform);
    _glDraw_ActiveProgram->set("near", _glDraw_UniformNear);
    _glDraw_ActiveProgram->set("far", _glDraw_UniformFar);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &(_glDraw2Subroutines[_glDraw_ActiveColorMode]));
}

void _glDrawUseProgram2() {
    _glDraw_ActiveProgram = &_glDraw2_Program;
    _glDraw_ActiveProgram->use();
    _glDrawUniforms();
}

void _glDrawUseProgram3() {
    _glDraw_ActiveProgram = &_glDraw3_Program;
    _glDraw_ActiveProgram->use();
    _glDrawUniforms();
}

void _glDraw(const vector<float> &v, const vector<float> &uv, int dimension, GLenum mode) {
    glBindVertexArray(_glDrawSharedVAO);
    glEnableVertexAttribArray(_glDraw_VertexAttribIndex);
    glBindBuffer(GL_ARRAY_BUFFER, _glDrawSharedVertexVBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(_glDraw_VertexAttribIndex, dimension, GL_FLOAT, GL_FALSE, 0, 0);
    if (uv.size()) {
        glEnableVertexAttribArray(_glDraw_TexCoordAttribIndex);
        glBindBuffer(GL_ARRAY_BUFFER, _glDrawSharedTexCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(float), uv.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(_glDraw_TexCoordAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    } else {
        glDisableVertexAttribArray(_glDraw_TexCoordAttribIndex);
    }
    glDrawArrays(mode, 0, (int)v.size() / dimension);
}

void glDrawSetTexture(GLuint texture) {
    _glDraw_UniformTexture = texture;
}

void glDrawSetColor(float v0, float v1, float v2, float v3) {
    _glDraw_UniformColor[0] = v0;
    _glDraw_UniformColor[1] = v1;
    _glDraw_UniformColor[2] = v2;
    _glDraw_UniformColor[3] = v3;
}

void glDrawSetLinearDepthRange(float near, float far) {
    _glDraw_UniformNear = near;
    _glDraw_UniformFar = far;
}

void glDrawSetMode(GLDrawMode colorMode) {
    _glDraw_ActiveColorMode = colorMode;
}

void glDrawLoadTransform(const float *v) {
    memcpy(_glDraw_UniformTransform, v, 16 * sizeof(float));
}

void glDrawLoadTransformIdentity() {
    memcpy(_glDraw_UniformTransform, _glDraw_TransformIdentity, 16 * sizeof(float));
}

void glDraw2(GLenum mode, const vector<float> &vertices, const vector<float> &uv) {
    _glDrawInit();
    if (_glDraw_EnableDefaultPipeline)
        _glDrawUseProgram2();
    _glDraw(vertices, uv, 2, mode);
}

void glDraw3(GLenum mode, const vector<float> &vertices, const vector<float> &uv) {
    _glDrawInit();
    if (_glDraw_EnableDefaultPipeline)
        _glDrawUseProgram3();
    _glDraw(vertices, uv, 3, mode);
}

void glDraw(GLenum mode, const vector<glm::vec2> &vertices, const vector<glm::vec2> &uv) { glDraw2(mode, (const vector<float> &)vertices, (const vector<float> &)uv); }
void glDraw(GLenum mode, const vector<glm::vec3> &vertices, const vector<glm::vec2> &uv) { glDraw3(mode, (const vector<float> &)vertices, (const vector<float> &)uv); }

void glDrawLines(const std::vector<glm::vec2> &vertices) { glDraw(GL_LINES, vertices); }
void glDrawLines(const std::vector<glm::vec3> &vertices) { glDraw(GL_LINES, vertices); }

void glDrawLine(glm::vec2 a, glm::vec2 b) { 
	const vector<glm::vec2> v = { a, b };
	glDrawLines(v); }
void glDrawLine(glm::vec3 a, glm::vec3 b) { 
	const vector<glm::vec3> v = { a, b };
	glDrawLines(v); 
}

void glDrawLineStrip(const std::vector<glm::vec2> &vertices) { glDraw(GL_LINE_STRIP, vertices); }
void glDrawLineStrip(const std::vector<glm::vec3> &vertices) { glDraw(GL_LINE_STRIP, vertices); }

void glDrawLineLoop(const std::vector<glm::vec2> &vertices) { glDraw(GL_LINE_LOOP, vertices); }
void glDrawLineLoop(const std::vector<glm::vec3> &vertices) { glDraw(GL_LINE_LOOP, vertices); }

void glDrawRect2(float x, float y, float w, float h) {
    vector<float> v = {
        x, y, x + w, y, x + w, y + h, x + w, y + h, x, y + h, x, y
    };
    static const vector<float> _glDraw_UV2 = {
        0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0
    };
    glDraw2(GL_TRIANGLES, v, _glDraw_UV2);
}

void glDrawRect3(float x, float y, float z, float w, float h, float d) {
    float s = x + w, t = y + h, r = z + d;
    vector<float> v = {
        x,y,z, s,t,z, s,y,z, s,t,z, x,y,z, x,t,z,
        x,y,r, s,y,r, s,t,r, s,t,r, x,t,r, x,y,r,
        x,t,z, s,t,r, s,t,z, s,t,r, x,t,z, x,t,r,
        x,y,z, s,y,z, s,y,r, s,y,r, x,y,r, x,y,z,
        x,y,z, x,t,r, x,t,z, x,t,r, x,y,z, x,y,r,
        s,y,z, s,t,z, s,t,r, s,t,r, s,y,r, s,y,z,
    };
    static const vector<float> _glDraw_UV3 = {
        0,0, 1,1, 1,0, 1,1, 0,0, 0,1,
        0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
        0,0, 1,1, 1,0, 1,1, 0,0, 0,1,
        0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
        0,0, 1,1, 1,0, 1,1, 0,0, 0,1,
        0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
    };
    glDraw3(GL_TRIANGLES, v, _glDraw_UV3);
}


