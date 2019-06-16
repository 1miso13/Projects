#include "pch.h"
#include "CalculateNormals.h"
namespace CPUnormals {
	Vec3 cross(Vec3 A, Vec3 B, Vec3 C) {
		Vec3 BA = C - B;
		Vec3 BC = A - B;
		Vec3 ret;
		ret.X = BA.Y * BC.Z - BA.Z * BC.Y;
		ret.Y = BA.Z * BC.X - BA.X * BC.Z;
		ret.Z = BA.X * BC.Y - BA.Y * BC.X;
		return ret;
	}

	double vecLength(Vec3 A) {
		return sqrt(A.X * A.X + A.Y * A.Y + A.Z * A.Z);
	}

	Vec3 normalize(Vec3 A) {
		double dist = vecLength(A);
		if (dist == 0)
		{
			dist = 1;
		}
		A.X = A.X / dist;
		A.Y = A.Y / dist;
		A.Z = A.Z / dist;
		return A;
	}
	double VectorAngle(Vec3 A, Vec3 B) {
		//dotproduct
		double angle = A.X * B.X + A.Y * B.Y + A.Z * B.Z;
		angle = acos(angle / (vecLength(A) * vecLength(B)));
		return angle;
	}
	void calcVertexNormals(std::vector<unsigned int> indices, size_t indiciesCount, std::vector < Vec3> vertex, std::vector < Vec3> *verticesNormal, size_t vertexCount, std::vector < Vec3> *triangleNormal)
	{
		size_t count = indiciesCount / 3;
		//vertex
		for (size_t i = 0; i < count; i++)
		{
			(*verticesNormal)[indices[i * 3]] = (*verticesNormal)[indices[i * 3]] + 
					(*triangleNormal)[i] 
											* VectorAngle(
												vertex[indices[i * 3+1]]	-	vertex[indices[i * 3]], 
												vertex[indices[i * 3+2]]	-	vertex[indices[i * 3]]);
			(*verticesNormal)[indices[i*3 + 1]] = (*verticesNormal)[indices[i * 3 + 1]] + 
					(*triangleNormal)[i] 
											* VectorAngle(
												vertex[indices[i * 3+2]]	-	vertex[indices[i * 3+1]], 
												vertex[indices[i * 3]]		-	vertex[indices[i * 3+1]]);
			(*verticesNormal)[indices[i*3 + 2]] = (*verticesNormal)[indices[i * 3 + 2]] + 
					(*triangleNormal)[i] 
											* VectorAngle(
												vertex[indices[i * 3]]		-	vertex[indices[i * 3+2]], 
												vertex[indices[i * 3+1]]	-	vertex[indices[i * 3+2]]);
		}
		//normalize
		for (size_t i = 0; i < vertexCount; i++)
		{
			(*verticesNormal)[i]= normalize((*verticesNormal)[i]);
		}
	}/*
	void calcVertexNormalsHARD(int *indicies, size_t indiciesCount, Vec3 * verticiesNormal, size_t vertexCount, Vec3 * triangleNormal)
	{
		size_t count = indiciesCount / 3;
		//vertex
		for (size_t i = 0; i < count; i++)
		{
			verticiesNormal[i] = triangleNormal[i];
			verticiesNormal[i + 1] = triangleNormal[i];
			verticiesNormal[i + 2] = triangleNormal[i];
		}
	}*/
	void calcTriangleNormals1(std::vector<unsigned int >indicies, size_t indiciesCount, std::vector<Vec3> vertex, std::vector<Vec3 > *verticiesNormal, std::vector<Vec3> *triangleNormal) {
		size_t count = indiciesCount / 3;
		for (size_t i = 0, j; i < count; i++)
		{
			(*triangleNormal)[i] = normalize(cross(vertex[indicies[i*3]], vertex[indicies[i*3 + 1]], vertex[indicies[i*3 + 2]]));
		}
		/*for (size_t i = 0; i < count; i ++)
		{
			std::cout << (*triangleNormal)[i].X << ";" << (*triangleNormal)[i].Y << ";" << (*triangleNormal)[i].Z << std::endl;
		}*/
	}
	void calcTriangleNormals(std::vector<unsigned int> indicies, size_t indiciesCount, std::vector < Vec3> vertex, std::vector<Vec3 >*verticiesNormal, std::vector<Vec3 >*triangleNormal) {


		//verticies	
		size_t count = indiciesCount / 3;
		for (size_t i = 0; i < count; i++)
		{
			//priemer bodov
			(*triangleNormal)[i] = normalize((*verticiesNormal)[indicies[i * 3]] + (*verticiesNormal)[indicies[i * 3+1]] + (*verticiesNormal)[indicies[i * 3+2]]);
		}
	}
}