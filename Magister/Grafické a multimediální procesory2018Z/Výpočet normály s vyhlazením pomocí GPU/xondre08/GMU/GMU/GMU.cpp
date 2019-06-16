#pragma warning(disable:4996)
/* Precompiled C header */
#include "pch.h"

/* Std C/C++ libs */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <sstream>

/* assimp - No longer used */
#ifdef ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif // ASSIMP




/* Model importer */
#include "ImportModel.h"

/* Windows */
#include <Windows.h>


/* DEFINES */

/* CONSTANTS */

/* GLOBAL VARIABLES */



void CreateNormalsCPU(Model *model,size_t iteration) 
{
	CPUnormals::calcTriangleNormals1(model->indices,model->indicesCount,model->vertices, &model->vertexNormals, &model->triangleNormals);
	CPUnormals::calcVertexNormals(model->indices, model->indicesCount, model->vertices, &model->vertexNormals, model->verticesCount, &model->triangleNormals);
	for (size_t i = 1; i < iteration; i++)
	{
			CPUnormals::calcTriangleNormals(model->indices, model->indicesCount, model->vertices, &model->vertexNormals, &model->triangleNormals);
			CPUnormals::calcVertexNormals(model->indices, model->indicesCount, model->vertices, &model->vertexNormals, model->verticesCount, &model->triangleNormals);
	}
}

void printArray(std::string s, std::vector <unsigned int> arr) 
{
	std::cout << s << std::endl;
	for (size_t i = 0; i < arr.size(); i += 3)
	{
		std::cout << arr[i] << ";" << arr[i + 1] << ";" << arr[i + 2] << std::endl;
	}
}

void printArray(std::string s, std::vector <Vec3> arr) 
{
	std::cout << s << std::endl;
	for (size_t i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i].X << ";" << arr[i].Y << ";" << arr[i].Z << std::endl;
	}
}

/*  */
cl_double *makeFloatPtr(std::vector<Vec3> &in)
{
	cl_double *out = new cl_double[in.size() * 3];
	for (int i = 0; i < in.size(); i++)
	{
		out[i * 3 + 0] = in[i].X;
		out[i * 3 + 1] = in[i].Y;
		out[i * 3 + 2] = in[i].Z;
	}
	return out;
}

/*  */
cl_uint *makeIntPtr(std::vector<unsigned int> &in)
{
	cl_uint *out = new cl_uint[in.size()];
	for (int i = 0; i < in.size(); i += 3)
	{
		out[i + 0] = in[i + 0];
		out[i + 1] = in[i + 1];
		out[i + 2] = in[i + 2];
	}
	return out;
}

/* Compare results from cpu vs GPU */
int compareResults(std::vector<Vec3> &in1, cl_double* in2)
{
	for (int i = 0; i < in1.size(); i++)
	{
		if (std::abs(in1[i].X - in2[i * 3 + 0]) < 0.0000001)
		{
			return 0;
		}
		if (std::abs(in1[i].Y - in2[i * 3 + 1]) < 0.0000001)
		{
			return 0;
		}
		if (std::abs(in1[i].Z - in2[i * 3 + 2]) < 0.0000001)
		{
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[])
{
	std::cout << "GMU - Project: Calculate smooth normals using GPU" << std::endl;
	std::cout << "Authors: xondre08, xverny00" << std::endl;

	std::string filePath;
	int iterations;
	if (argc>1)
	{//file path
		filePath = argv[1];
		if (argc > 2) //iterations
		{
			iterations = atoi(argv[2]);
		}
		else
		{
			std::cin >> iterations;
		}
	}
	else
	{
		//path from input
		std::cout << "Path to file:";
		std::cin >> filePath;
		std::cout << "Number of iterations:";
		std::cin >> iterations;
	}
	std::ifstream f(filePath.c_str()); 
	if (!f.good())
	{
		return 200;
	}
	Model model(filePath);
	double cpu_start = getTime(), cpu_end;
		
	CreateNormalsCPU(&model, iterations);
	cpu_end = getTime();
		//printArray("indices:", model.indices);
		//printArray("vertices:", model.vertices);
		//printArray("vertices normal:", model.vertexNormals);
		//std::cout << "Iteration:" << i << std::endl;
	model.exportModel(model.vertexNormals);
	//return 0;

	/* OpenCL begin */
	cl_int err_msg = 0, err_msg2;

	/* Create host buffers */
	cl_double *nrl_data = new cl_double[model.vertices.size() * 3];
	std::memset(nrl_data, 0, model.vertices.size() * 3 * sizeof(cl_double));
	cl_double *out_data = new cl_double[model.vertices.size() * 3];
	std::memset(out_data, 0, model.vertices.size() * 3 * sizeof(cl_double));
	cl_double *vtx_data = makeFloatPtr(model.vertices);
	cl_uint  *idx_data = makeIntPtr(model.indices);

	/* Get gpu device	*/
	std::vector<cl::Platform> platforms;
	clPrintErrorExit(cl::Platform::get(&platforms), "cl::Platform::get");
	std::vector<cl::Device> devices;
	for (int i = 0; i < platforms.size(); i++) 
	{
		platforms[i].getDevices(CL_DEVICE_TYPE_GPU, &devices);
		if (devices.size() == 0)
			continue;
		break;
	}
	cl::Device gpu_device = devices[0];


	/*  Create context and queue with profiling enabled */
	cl::Program::Sources sources;
	cl::Context context = cl::Context(gpu_device);
	cl::CommandQueue queue = cl::CommandQueue(context, gpu_device, CL_QUEUE_PROFILING_ENABLE);

	/* Load source */
	std::stringstream progSource;
	std::fstream kernelFile("kernel.cl");
	std::string kernelContent(
		(std::istreambuf_iterator<char>(kernelFile)),
		std::istreambuf_iterator<char>()
	);
	progSource << kernelFile.rdbuf();
	//char * program_source = readFile("GMU/kernel.cl");

	#ifdef CL_VERSION_2_0
	/* AMD branch */
	sources.push_back(kernelContent);
	#else
	/* NVidia branch */
	const char * program_source = kernelContent.c_str();
	sources.push_back(std::pair<const char *, size_t>(program_source, strlen(program_source)));
	#endif

	/* Get program */
	cl::Program program(context, sources);
	clPrintErrorExit(err_msg, "clCreateProgramWithSource");

	/* Build program */
	if ((err_msg = program.build(std::vector<cl::Device>(1, gpu_device), "", NULL, NULL)) == CL_BUILD_PROGRAM_FAILURE)
	{
		printf("Build log:\n %s", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_device, &err_msg2).c_str());
		clPrintErrorExit(err_msg2, "cl::Program::getBuildInfo<CL_PROGRAM_BUILD_LOG>");
	}
	clPrintErrorExit(err_msg, "clBuildProgram");

	/* Create kernel */
	cl::Kernel kernel(program, "smoothNormals", &err_msg);
	clPrintErrorExit(err_msg, "cl::Kernel");

	/* Create buffers */
	cl::Buffer nrl_buffer = cl::Buffer(context, CL_MEM_READ_WRITE, model.verticesCount * 3 * sizeof(cl_double));
	cl::Buffer vtx_buffer = cl::Buffer(context, CL_MEM_READ_WRITE, model.verticesCount * 3 * sizeof(cl_double));
	cl::Buffer idx_buffer = cl::Buffer(context, CL_MEM_READ_WRITE, model.indices.size() * sizeof(cl_uint));

	/* Set kernel arguments */
	kernel.setArg((cl_uint) 0, nrl_buffer);
	kernel.setArg((cl_uint) 1, vtx_buffer);
	kernel.setArg((cl_uint) 2, idx_buffer);
	kernel.setArg((cl_uint) 3, (cl_uint) iterations);
	kernel.setArg((cl_uint) 4, (cl_uint) model.indices.size() / 3);
	kernel.setArg((cl_uint) 5, (cl_uint) model.verticesCount);

	/* Eventy pro kopirovani na zarizeni */
	cl::UserEvent nrl_write_buffer_event(context, &err_msg);
	clPrintErrorExit(err_msg, "clCreateUserEvent nrl_write_buffer_event");
	cl::UserEvent vtx_write_buffer_event(context, &err_msg);
	clPrintErrorExit(err_msg, "clCreateUserEvent vtx_write_buffer_event");
	cl::UserEvent idx_write_buffer_event(context, &err_msg);
	clPrintErrorExit(err_msg, "clCreateUserEvent idx_write_buffer_event");

	/* Event pro kernel */
	cl::UserEvent ndrange_kernel_event(context, &err_msg);
	clPrintErrorExit(err_msg, "clCreateUserEvent ndrange_kernel_event");

	/* Event pro kopirovani ze zarizeni */
	cl::UserEvent nrl_read_buffer_event(context, &err_msg);
	clPrintErrorExit(err_msg, "clCreateUserEvent nrl_read_buffer_event");

	/* Create range */
	cl::NDRange local = cl::NDRange(256);
	cl::NDRange global = cl::NDRange(1024 * 1024);

	/* Start measuring GPU time */
	double gpu_start = getTime();

	/* Write to buffers */
	queue.enqueueWriteBuffer(nrl_buffer, CL_FALSE, 0u, sizeof(cl_double) * model.verticesCount * 3, nrl_data, NULL, &nrl_write_buffer_event);
	queue.enqueueWriteBuffer(vtx_buffer, CL_FALSE, 0u, sizeof(cl_double) * model.verticesCount * 3, vtx_data, NULL, &vtx_write_buffer_event);
	queue.enqueueWriteBuffer(idx_buffer, CL_FALSE, 0u, sizeof(cl_uint) * model.indices.size(), idx_data, NULL, &idx_write_buffer_event);

	/* Execute */
	err_msg = queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local, NULL, &ndrange_kernel_event);
	if (err_msg != CL_SUCCESS) {
		clPrintErrorExit(err_msg, "kernel");
	}

	/* Read from buffers */
	err_msg = queue.enqueueReadBuffer(nrl_buffer, CL_FALSE, 0u, sizeof(cl_double) * model.verticesCount * 3, out_data, NULL, &nrl_read_buffer_event);
	if (err_msg != CL_SUCCESS) {
		clPrintErrorExit(err_msg, "read buffer");
	}

	/* Synchronize queue */
	clPrintErrorExit(queue.finish(), "clFinish");

	/* Stop measuring GPU time */
	double gpu_end = getTime();

	/* Check data */
	if (compareResults(model.vertices, out_data))
	{
		printf("\nResult: Correct\n");
	}
	else
	{
		printf("\nResult: Incorrect\n");
		/* Print results */
		printf("\nCPU results:\n");
		printArray("vertices normal:", model.vertexNormals);
		printf("\nGPU results:\n");
		for (int i = 0; i < model.verticesCount; i++)
		{
			printf(" N[%d]: %f, %f, %f\n", i, out_data[i * 3], out_data[i * 3 + 1], out_data[i * 3 + 2]);
		}
	}

	/* Print performance info */
	printf("\nHost timers:\n");
	printf(" OpenCL processing time: %fs\n", gpu_end - gpu_start);
	printf(" CPU    processing time: %fs\n", cpu_end - cpu_start);
	printf("\nDevice timers:\n");
	printf(" OpenCL copy time: %fs\n", getEventTime(nrl_write_buffer_event) + getEventTime(vtx_write_buffer_event) + getEventTime(idx_write_buffer_event) + getEventTime(nrl_read_buffer_event));
	printf(" OpenCL processing time: Smooth normals %fs\n", getEventTime(ndrange_kernel_event));

	/* Release allocated memory */
	delete nrl_data;
	delete out_data;
	delete vtx_data;
	delete idx_data;

	//std::system("pause");

	return 0;
}
