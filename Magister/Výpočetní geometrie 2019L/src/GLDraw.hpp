/**
 Photon Splatting Using a View-Sample Cluster Hierarchy
 Brno University of Technology (BUT)
 Faculty of Information Technologies (FIT)
 
 @file    GLDraw.cpp
 @author  Marcel Kiss, xkissm01@stud.fit.vutbr.cz
 @date    27/11/18
 Copyright © 2019 Marcel Kiss
 */

#ifndef GLDraw_hpp
#define GLDraw_hpp

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __APPLE__
    #ifndef GL_SILENCE_DEPRECATION
        /* Silence deprecation warning */
        #define GL_SILENCE_DEPRECATION  // OpenCL is deprecated from macOS 10.14
    #endif
    #include <OpenGL/gl3.h>
    #include <OpenGL/OpenGL.h>
#else
    /* Windows needs to load function for OpenGL 3.X externaly, GLFW library does it for us. */
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#define INCLUDE_OPENCL  0

#if INCLUDE_OPENCL
#ifdef __APPLE__
    #ifndef CL_SILENCE_DEPRECATION
        /* Silence deprecation warning */
        #define CL_SILENCE_DEPRECATION  // OpenCL is deprecated from macOS 10.14
    #endif
    #include <OpenCL/cl.h>
    #include <OpenCL/OpenCL.h>
#else
    #include <CL/cl.h>
#endif
#endif



/** Return string for OpenGL error code */
std::string glErrorToString(GLenum e);

#if INCLUDE_OPENCL
/** Return string for OpenCL error code */
std::string clErrorToString(cl_int e);
#endif

/* Return preprocessor file name without path */
#define __FILENAME__    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/* Comment this line to make OpenGL error checks non-fatal  */
#define glCheckFatal()  exit(EXIT_FAILURE)
/* Comment this line to make OpenCL error checks non-fatal  */
#define clCheckFatal()  exit(EXIT_FAILURE)

/* Check for OpenGL errors and exit if it's enabled */
#define glCheckError()  do {GLenum e=glGetError();if(e){std::cerr<<"[OpenGL] ["<<__FILENAME__<<":"<<__LINE__<<" in "<<__FUNCTION__<<"] Error code: "<<glErrorToString(e)<<" ("<<e<<")"<<std::endl;glCheckFatal();}}while(0)

#if INCLUDE_OPENCL
/* Check for OpenCL errors and exit if it's enabled */
#define clCheckError(e) do {if(e){std::cerr<<"[OpenCL] ["<<__FILENAME__<<":"<<__LINE__<<" in "<<__FUNCTION__<<"] Error code: "<<clErrorToString(e)<<" ("<<e<<")"<<std::endl;clCheckFatal();}}while(0)
#endif


#if INCLUDE_OPENCL

cl_platform_id CLPlatformGet();
cl_device_id CLDeviceGet(cl_platform_id platform, cl_device_type device_type);

cl_context CLCreateContext(cl_device_id device, cl_platform_id platform);

cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t global);
cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t global, cl_event &event);
cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t g, size_t l);
cl_event CLEnqueueNDRangeKernel1D(cl_command_queue queue, cl_kernel kernel, size_t g, size_t l, cl_event &event);

cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy);
cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy, cl_event &event);
cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy, size_t lx, size_t ly);
cl_event CLEnqueueNDRangeKernel2D(cl_command_queue queue, cl_kernel kernel, size_t gx, size_t gy, size_t lx, size_t ly, cl_event &event);

void CLEnqueueAcquireGLObjects(cl_command_queue queue, std::vector<cl_mem> objs);
void CLEnqueueReleaseGLObjects(cl_command_queue queue, std::vector<cl_mem> objs);

cl_ulong CLGetEventNanoTime(cl_event event);

/** This function dynamicaly allocates memory for build
 log string (malloc). User should dealloc it after usage. */
char* CLGetBuildInfo(cl_program program, cl_device_id device);

cl_program CLCreateAndBuildProgramWithSource(cl_context context, cl_device_id device, const char *source, size_t length);
cl_program CLCreateAndBuildProgramWithFile(cl_context context, cl_device_id device, const char *path);
cl_kernel CLCreateKernel(cl_program program, const char * name);

const char* clChannelOrderToString(cl_channel_order channel_order);
const char* clChannelTypeToString(cl_channel_type channel_type);

cl_mem CLCreateBuffer(cl_context ctx, size_t size);

void CLReleaseMemory(cl_mem mem);

class CLProgram {
    cl_program _program;
    std::string _source;
    std::map<std::string, cl_kernel> _kernels;
public:
    CLProgram(const CLProgram &p): _program(p._program), _source(p._source), _kernels(p._kernels) {}
    CLProgram(): _program(0) {}
    CLProgram(std::string path): _program(0) { source(path); }
    CLProgram(cl_context context, cl_device_id device, std::string path): _program(0) { source(path); init(context, device); }
    CLProgram(cl_program program): _program(program) {}
    
    void source(std::string path) {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            _source = stream.str();
        } catch(std::ifstream::failure e) {
            std::cerr << "[CLProgram] Failed to load file '" << path << "'" << std::endl;
        }
    }
    
    void init(cl_context context, cl_device_id device) {
        _program = CLCreateAndBuildProgramWithSource(context, device, _source.c_str(), _source.length());
    }
    
    void destroy() {
        for (auto pair : _kernels)
            clReleaseKernel(pair.second);
        clReleaseProgram(_program);
    }
    
    cl_kernel kernel(std::string name) {
        if (!_kernels.count(name))
            _kernels[name] = CLCreateKernel(_program, name.c_str());
        return _kernels.at(name);
    }
    
    cl_kernel operator[](std::string name) { return kernel(name); }
};

class CLKernel {
    cl_kernel _kernel;
    int _argi;
    template<class T> void _push(T t, int i = 1) { clSetKernelArg(_kernel, _argi++, sizeof(t), &t); }
public:
    CLKernel(const CLKernel &k): _kernel(k._kernel), _argi(k._argi) {}
    CLKernel(cl_program program, std::string name): _argi(0) {
        _kernel = CLCreateKernel(program, name.c_str());
    }
    
    CLKernel(cl_kernel kernel): _argi(0) {
        _kernel = kernel;
    }
    
    void push(cl_int a) { _push(a); }
    void push(cl_uint a) { _push(a); }
    void push(cl_int2 a) { _push(a); }
    void push(cl_mem a) { _push(a); }
    void push(cl_float a) { _push(a); }
    void push(cl_float2 a) { _push(a); }
    void push(cl_float4 a) { _push(a); }
    void push(glm::mat4 m) { clSetKernelArg(_kernel, _argi++, sizeof(cl_float16), &m[0][0]); }
    void restart() { _argi = 0; }
    
    void pushLocalMemory(size_t size) { clSetKernelArg(_kernel, _argi++, size, NULL); }
    template<class T> void pushLocalMemoryObjects(size_t count) { pushLocalMemory(count * sizeof(T)); }
    
    template<class T> void set(int i, T t) { clSetKernelArg(_kernel, i, sizeof(t), &t); }
    cl_kernel operator()() const { return _kernel; }
};

#endif






class GLTexture2D {
    GLuint _id;
public:
    GLTexture2D();
    bool init();
    void destroy();
    void data(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *data = NULL) const;
    void bind() const;
    static void unbind();
    GLuint operator()() const;
    
#if INCLUDE_OPENCL
private:
    cl_mem _cl_read, _cl_write;
public:
    void clMake(cl_context ctx);
    cl_mem clRead() const;
    cl_mem clWrite() const;
#endif
};

/** Shader type */
enum GLShaderType {
    GLShader_Vertex         = GL_VERTEX_SHADER,
    GLShader_TessControl    = GL_TESS_CONTROL_SHADER,
    GLShader_TessEval       = GL_TESS_EVALUATION_SHADER,
    GLShader_Geometry       = GL_GEOMETRY_SHADER,
    GLShader_Fragment       = GL_FRAGMENT_SHADER,
};

/** Compile shader from file content at path */
GLuint GLShaderCompileFile(GLShaderType shaderType, const std::string path);

/** Compile shader from source string */
GLuint GLShaderCompileSource(GLShaderType type, const std::string &source);

/* Wrapper for OpenGL program */
class GLProgram {
    GLuint _id;
    std::map<std::string, GLint> _locations;
public:
    GLProgram();
    ~GLProgram();
    
    void glInit(GLuint s1);
    void glInit(GLuint s1, GLuint s2);
    void glInit(GLuint s1, GLuint s2, GLuint s3);
    void glInit(const std::vector<GLuint> &shaders);
    void glDelete();
    
    GLint location(const std::string &name);
    
    void set(const std::string &name, bool value);
    void set(const std::string &name, int value);
    void set(const std::string &name, unsigned int value);
    void set(const std::string &name, float value);
    
    void set4f(const std::string &name, float v0, float v1, float v2, float v3);
    void set4x4f(const std::string &name, const float *v);
    
    void set(const std::string &name, const glm::ivec2 v);
    void set(const std::string &name, const glm::ivec3 v);
    void set(const std::string &name, const glm::ivec4 v);
    
    void set(const std::string &name, const glm::vec2 v);
    void set(const std::string &name, const glm::vec3 v);
    void set(const std::string &name, const glm::vec4 v);
    
    void set(const std::string &name, const glm::mat4 m);
    
    void use() const;
    
    GLuint operator ()() const;
};

/** Shared reference of GLProgram */
typedef std::shared_ptr<GLProgram> GLProgramRef;


/// glDraw

enum GLDrawMode {
    GLDrawMode_Color = 0,
    GLDrawMode_TextureColor,
    GLDrawMode_TextureUColor,
    GLDrawMode_TextureDepth,
    GLDrawMode_TextureLinearDepth,
    
    GLDrawMode_Count,
};

void glDrawEnableDefaultPipeline(bool enableDefaultPipeline);
bool glDrawIsEnabledDefaultPipeline();

void glDrawSetDefaultVertexAtribIndex();
void glDrawSetVertexAttribIndex(GLint vertexAttrib);
GLint glDrawGetVertexAttribIndex();

void glDrawSetDefaultTexCoordAttribIndex();
void glDrawSetTexCoordAttribIndex(GLint texCoordAttrib);
GLint glDrawGetTexCoordAttribIndex();

void glDrawSetTexture(GLuint texture);
void glDrawSetColor(float v0, float v1, float v2, float v3);
void glDrawSetLinearDepthRange(float near, float far);
void glDrawSetMode(GLDrawMode colorMode);


void glDrawLoadTransform(const float *v);
void glDrawLoadTransformIdentity();

void glDraw2(GLenum mode, const std::vector<float> &vertices, const std::vector<float> &uv = std::vector<float>());
void glDraw3(GLenum mode, const std::vector<float> &vertices, const std::vector<float> &uv = std::vector<float>());

void glDraw(GLenum mode, const std::vector<glm::vec2> &vertices, const std::vector<glm::vec2> &uv = std::vector<glm::vec2>());
void glDraw(GLenum mode, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec2> &uv = std::vector<glm::vec2>());

void glDraw(GLenum mode, const std::vector<glm::vec2> &vertices, const std::vector<glm::vec2> &uv);
void glDraw(GLenum mode, const std::vector<glm::vec3> &vertices, const std::vector<glm::vec2> &uv);

void glDrawLines(const std::vector<glm::vec2> &vertices);
void glDrawLines(const std::vector<glm::vec3> &vertices);

void glDrawLine(glm::vec2 a, glm::vec2 b);
void glDrawLine(glm::vec3 a, glm::vec3 b);

void glDrawLineStrip(const std::vector<glm::vec2> &vertices);
void glDrawLineStrip(const std::vector<glm::vec3> &vertices);

void glDrawLineLoop(const std::vector<glm::vec2> &vertices);
void glDrawLineLoop(const std::vector<glm::vec3> &vertices);

void glDrawRect2(float x, float y, float w, float h);
void glDrawRect3(float x, float y, float z, float w, float h, float d);

#endif /* GLDraw_hpp */
