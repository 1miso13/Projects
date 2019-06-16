#pragma once
#pragma warning(disable:4996)
//#define UV
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "CalculateNormals.h"
class Model
{
	void loadModel(std::string path);
	void ClearVecs();
public:
	std::vector<unsigned int> indices;
	std::vector < Vec3>vertices;
	std::vector < Vec3>uvs; //only X and Y ! Z = 0
	std::vector<unsigned int> uv_indices;
	std::vector < Vec3>vertexNormals;
	std::vector < Vec3>triangleNormals;
	size_t indicesCount=0;
	size_t verticesCount=0;
	bool loadObj(std::string path);
	void exportModel(std::vector < Vec3> verticesNormal);
	void exportModel();
	Model(std::string path);
	~Model();
};

#ifdef ASSIMP
inline void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_GenNormals| aiProcess_RemoveComponent | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	auto directory = path.substr(0, path.find_last_of('/'));

	for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
	{
		const aiMesh* mesh = scene->mMeshes[n];

		//indices = (unsigned int *)realloc(indices, sizeof(unsigned int) * indicesCount + sizeof(unsigned int) * mesh->mNumFaces * 3);
		//unsigned int faceIndex = 0;
		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t]; 
			//memcpy(&indices[indicesCount+faceIndex], face->mIndices, 3 * sizeof(unsigned int));
			for (size_t i = 0; i < 3; i++)
			{
				indices.push_back(face->mIndices[i]);
			}
			//faceIndex += 3;
		}
		indicesCount += scene->mMeshes[n]->mNumFaces*3;

		//vertices = (Vec3 *)realloc(vertices, sizeof(Vec3) * verticesCount + sizeof(Vec3) * mesh->mNumVertices);
		//memcpy(&vertices[verticesCount], mesh->mVertices, sizeof(Vec3)*mesh->mNumVertices);
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back(Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		}
		verticesCount += mesh->mNumVertices;
	}
	//vertexNormals =(Vec3*) calloc(sizeof(Vec3) * verticesCount, sizeof(Vec3) * verticesCount);
	//triangleNormals = (Vec3*)calloc(sizeof(Vec3) * verticesCount, sizeof(Vec3) * verticesCount);
	for (size_t i = 0; i < verticesCount; i++)
	{
		vertexNormals.push_back(Vec3(0, 0, 0));
	}
	for (size_t i = 0; i < indicesCount /3; i++)
	{
		triangleNormals.push_back(Vec3(0, 0, 0));
	}

}
#endif //ASSIMP


std::vector <std::string>  splitBySpace(std::string s) {
	std::vector <std::string> vec;
	std::string a="";
	for (size_t i = 0, j = 0; i < s.length(); i++)
	{
		if (isspace(s[i]))
		{
			if (j == 0)
			{//empty
				continue;
			}
			vec.push_back(a);
			j = 0;
			a = "";
		}
		else
		{
			a += s[i];
			j++;
		}
	}
	return vec;
}
void parseFaceItem(std::string s, int*vertexIndex, int * uvIndex) {
	*vertexIndex = -1;
	*uvIndex = -1;
	size_t pos = 0;
	std::string token;
	//vertex
	if ((pos = s.find('/')) == std::string::npos) {
		*vertexIndex = std::stoi(s) - 1;//zly index
		return;
	}
	token = s.substr(0, pos);
	*vertexIndex = std::stoi(token) - 1; //zly index
	s.erase(0, pos + 1);



	//uv
	if ((pos = s.find('/')) == std::string::npos) {
		if (s != "")
		{
			*uvIndex = std::stoi(s) - 1;
		}
		return;
	}
	token = s.substr(0, pos);
	if (token!= "")
	{
		*uvIndex = std::stoi(token) - 1;
	}
	
	//normal
}
inline bool Model::loadObj(std::string path) {
	ClearVecs();
	FILE * file;
	if ((file= fopen(path.c_str(), "r")) == 0)
	{
		printf("Impossible to open the file ! 1\n");
		return false;
	}
	if (file == NULL) {
		printf("Impossible to open the file ! 2\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			Vec3 vertex;
			fscanf(file, "%lf %lf %lf\n", &vertex.X, &vertex.Y, &vertex.Z);
			vertices.push_back(vertex);

		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vec3 uv;
			fscanf(file, "%lf %lf\n", &uv.X, &uv.Y);
			uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			int vertexIndex[3]={-1,-1,-1}, uvIndex[3] = { -1,-1,-1 };

			//read line 
			char line[1024];
			fgets(line, 1024, file);
			std::string str(line);
			//parse by space char
			auto vec = splitBySpace(str);
			//normaly zahodime
			parseFaceItem(vec[0],&vertexIndex[0], &uvIndex[0]);
			parseFaceItem(vec[1], &vertexIndex[1], &uvIndex[1]);
			for (size_t i = 2; i < vec.size(); i++)
			{
				parseFaceItem(vec[i], &vertexIndex[2], &uvIndex[2]);

				indices.push_back(vertexIndex[0]);
				indices.push_back(vertexIndex[1]);
				indices.push_back(vertexIndex[2]);
#ifdef UV
				if (uvIndex[2]!=-1 && uvIndex[1] != -1 && uvIndex[0] != -1)
				{
					uv_indices.push_back(uvIndex[0]);
					uv_indices.push_back(uvIndex[1]);
					uv_indices.push_back(uvIndex[2]);
				}
#endif // UV
				vertexIndex[1] = vertexIndex[2];
				uvIndex[1] = uvIndex[2];
			}




		}
	}
	indicesCount = indices.size();
	verticesCount = vertices.size(); 
	for (size_t i = 0; i < verticesCount; i++)
	{
		vertexNormals.push_back(Vec3(0, 0, 0));
	}
	for (size_t i = 0; i < indicesCount / 3; i++)
	{
		triangleNormals.push_back(Vec3(0, 0, 0));
	}
	return true;
}
inline void Model::ClearVecs() {
	indices.clear();
	vertices.clear();
	uvs.clear();
	uv_indices.clear();
	vertexNormals.clear();
	triangleNormals.clear();
}
inline Model::Model(std::string path)
{
	//loadModel(path); //Assimp
	if (!loadObj(path))
	{
		ClearVecs();
	}
}


inline Model::~Model()
{
}
/// <summary>
/// 
/// </summary>
/// <param name="verticesNormal">verticesNormal.size() == vertices.size()</param>
void Model::exportModel(std::vector <Vec3> verticesNormal) {
	for (size_t i = 0; i < verticesNormal.size(); i++)
	{
		vertexNormals[i] = verticesNormal[i];
	}
	exportModel();
}
/// <summary>
/// 
/// </summary>
/// <param name="verticesNormal">verticesNormal.size() == vertices.size()</param>
void Model::exportModel() {

	std::ofstream myfile;
	myfile.open("out.obj");
	for (size_t i = 0; i < vertices.size(); i++)
	{
		myfile << "v "  << vertices[i].X << " " << vertices[i].Y << " " << vertices[i].Z << std::endl;
	}
	for (size_t i = 0; i < uvs.size(); i++)
	{
		myfile << "vt " << uvs[i].X << " " << uvs[i].Y << std::endl;
	}
	for (size_t i = 0; i < vertexNormals.size(); i++)
	{
		myfile << "vn " << vertexNormals[i].X << " " << vertexNormals[i].Y << " " << vertexNormals[i].Z << std::endl;
	}
	for (size_t i = 0; i < indices.size(); i+=3)
	{
	//	if (uvs.empty())
		{
			myfile
				<< "f " << indices[i] +1 << "//" << indices[i]+1
				<< " " << indices[i + 1] +1 << "//" << indices[i + 1]+1
				<< " " << indices[i + 2] +1 << "//" << indices[i + 2] +1<< std::endl;
		}
	/*	else
		{
			myfile
				<< "f " << indices[i] +1 << "/" << uv_indices[i] << "/" << indices[i]
				<< " " << indices[i + 1] +1<< "/" << uv_indices[i + 1] << "/" << indices[i + 1]
				<< " " << indices[i + 2] +1<< "/" << uv_indices[i + 2] << "/" << indices[i + 2] << std::endl;
		}*/
	}
	myfile.close();
}