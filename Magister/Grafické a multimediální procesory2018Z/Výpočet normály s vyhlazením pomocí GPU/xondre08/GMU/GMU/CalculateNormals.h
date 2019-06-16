#pragma once
#include <vector>
#include "Vector3.h"
#include <iostream>
namespace CPUnormals {

	void calcTriangleNormals(std::vector<unsigned int> indicies, size_t indiciesCount, std::vector < Vec3> vertex, std::vector<Vec3 >*verticiesNormal, std::vector<Vec3 >*triangleNormal);
	void calcTriangleNormals1(std::vector<unsigned int >indicies, size_t indiciesCount, std::vector<Vec3> vertex, std::vector<Vec3 > *verticiesNormal, std::vector<Vec3> *triangleNormal);
	void calcVertexNormalsHARD(size_t indiciesCount, Vec3  *verticiesNormal[], size_t vertexCount, Vec3 *triangleNormal[]);
	void calcVertexNormals(std::vector<unsigned int> indices, size_t indiciesCount, std::vector < Vec3> vertex, std::vector < Vec3> *verticesNormal, size_t vertexCount, std::vector < Vec3> *triangleNormal);
}