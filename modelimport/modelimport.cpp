#include <assimp/Importer.hpp>      
#include <assimp/scene.h>       
#include <assimp/postprocess.h>
#include <iostream>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include <string>
#include <fstream>

char* ToChar(float value, rapidxml::xml_document<>& doc) {
	char tmpval[64];
	sprintf(tmpval,"%f",value);
	return doc.allocate_string(tmpval);
}

bool ConvertToLCM(const aiScene* scene, std::string outputFile) 
{

	rapidxml::xml_document<> doc;

	rapidxml::xml_node<>* lcmMeshes = doc.allocate_node(rapidxml::node_element, "LCMMeshes");
	doc.append_node(lcmMeshes);
	rapidxml::xml_attribute<>* lcmMeshCount = doc.allocate_attribute("NumMeshes", ToChar(scene->mNumMeshes, doc));
	lcmMeshes->append_attribute(lcmMeshCount);

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];

		rapidxml::xml_node<>* lcmMesh = doc.allocate_node(rapidxml::node_element, "Mesh");
		rapidxml::xml_attribute<>* lcmHasNormals = doc.allocate_attribute("HasNormals", (mesh->HasNormals()) ? "1" : "0");
		lcmMesh->append_attribute(lcmHasNormals);
		rapidxml::xml_attribute<>* lcmVertexCount = doc.allocate_attribute("NumVertices", ToChar(mesh->mNumVertices, doc));
		lcmMesh->append_attribute(lcmVertexCount);
		lcmMeshes->append_node(lcmMesh);

		rapidxml::xml_node<>* lcmVertices = doc.allocate_node(rapidxml::node_element, "Vertices");

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			rapidxml::xml_node<>* lcmVertex = doc.allocate_node(rapidxml::node_element, "Vertex");
			rapidxml::xml_attribute<>* lcmHasColors = doc.allocate_attribute("HasColors", (mesh->HasVertexColors(j)) ? "1" : "0");
			lcmVertex->append_attribute(lcmHasColors);
			rapidxml::xml_attribute<>* lcmHasTexCoords = doc.allocate_attribute("HasTexCoords", (mesh->HasTextureCoords(j)) ? "1" : "0");
			lcmVertex->append_attribute(lcmHasTexCoords);

			rapidxml::xml_node<>* lcmCoords = doc.allocate_node(rapidxml::node_element, "Coords");

			rapidxml::xml_attribute<>* lcmX = doc.allocate_attribute("x", ToChar(mesh->mVertices[j].x, doc));
			lcmCoords->append_attribute(lcmX);
			rapidxml::xml_attribute<>* lcmY = doc.allocate_attribute("y", ToChar(mesh->mVertices[j].y, doc));
			lcmCoords->append_attribute(lcmY);
			rapidxml::xml_attribute<>* lcmZ = doc.allocate_attribute("z", ToChar(mesh->mVertices[j].z, doc));
			lcmCoords->append_attribute(lcmZ);

			lcmVertex->append_node(lcmCoords);

			if (mesh->HasVertexColors(j)) {

				rapidxml::xml_node<>* lcmColors = doc.allocate_node(rapidxml::node_element, "Colors");

				rapidxml::xml_attribute<>* lcmR = doc.allocate_attribute("r", ToChar(mesh->mColors[j]->r, doc));
				lcmColors->append_attribute(lcmR);
				rapidxml::xml_attribute<>* lcmB = doc.allocate_attribute("b", ToChar(mesh->mColors[j]->b, doc));
				lcmColors->append_attribute(lcmB);
				rapidxml::xml_attribute<>* lcmG = doc.allocate_attribute("g", ToChar(mesh->mColors[j]->g, doc));
				lcmColors->append_attribute(lcmG);

				lcmVertex->append_node(lcmColors);
			}

			if (mesh->HasTextureCoords(j))
			{
				rapidxml::xml_node<>* lcmCoords = doc.allocate_node(rapidxml::node_element, "TexCoords");

				rapidxml::xml_attribute<>* lcmU = doc.allocate_attribute("u", ToChar(mesh->mTextureCoords[j]->x, doc));
				lcmCoords->append_attribute(lcmU);
				rapidxml::xml_attribute<>* lcmV = doc.allocate_attribute("v", ToChar(mesh->mTextureCoords[j]->y, doc));
				lcmCoords->append_attribute(lcmV);

				lcmVertex->append_node(lcmCoords);
			}

			if (mesh->HasNormals()) {
				rapidxml::xml_node<>* lcmNormals = doc.allocate_node(rapidxml::node_element, "Normals");

				rapidxml::xml_attribute<>* lcmNX = doc.allocate_attribute("x", ToChar(mesh->mNormals[j].x, doc));
				lcmNormals->append_attribute(lcmNX);
				rapidxml::xml_attribute<>* lcmNY = doc.allocate_attribute("y", ToChar(mesh->mNormals[j].y, doc));
				lcmNormals->append_attribute(lcmNY);
				rapidxml::xml_attribute<>* lcmNZ = doc.allocate_attribute("z", ToChar(mesh->mNormals[j].z, doc));
				lcmNormals->append_attribute(lcmNZ);

				lcmVertex->append_node(lcmNormals);
			}

			lcmVertices->append_node(lcmVertex);
		}

		lcmMesh->append_node(lcmVertices);

		rapidxml::xml_node<>* lcmIndices = doc.allocate_node(rapidxml::node_element, "Indices");
		int numIndices = 0;
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices == 3) 
			{
				for (int k = 0; k < 3; k++)
				{
					rapidxml::xml_node<>* lcmIndice = doc.allocate_node(rapidxml::node_element, "Indice", ToChar(mesh->mFaces[j].mIndices[k], doc));
					lcmIndices->append_node(lcmIndice);
					numIndices++;
				}
			}
		}
		rapidxml::xml_attribute<>* lcmNumIndices = doc.allocate_attribute("NumIndices", ToChar(numIndices, doc));
		lcmIndices->append_attribute(lcmNumIndices);
		lcmMesh->append_node(lcmIndices);

	}


	std::ofstream lcmFile;
	lcmFile.open(outputFile);
	lcmFile << doc;

	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		return 1;

	Assimp::Importer importer;

	std::string input(argv[1]);
	//std::string input = "C:\\Users\\Janic\\Documents\\LoveCraft\\Debug\\monkey.obj";
	std::string output;
	std::string::size_type pos = input.rfind('.');
	if(pos != std::string::npos)
		output = input.substr(0, pos) + ".lcm";
	else
		output = input + ".lcm";

	const aiScene* scene = importer.ReadFile( input, aiProcess_CalcTangentSpace       | 
		aiProcess_GenNormals |
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType |
		aiProcess_GenUVCoords);

	// If the import failed
	if( !scene)
	{
		std::cout << "failed to load model : " << input << std::endl;
		return false;
	}

	ConvertToLCM(scene, output);
}