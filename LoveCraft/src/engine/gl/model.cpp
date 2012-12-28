#include "model.h"
#include <iostream>
#include "external/rapidxml-1.13/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <cassert>
#include "util/tool.h"
#include "util/matrix4.h"

Model::Model() : m_scale(Vector3f(1,1,1)), m_rot(1,0,0,0), m_pos(Vector3f(0,0,0)) {}

Model::~Model() {}

bool Model::Load( const std::string filePath )
{
	std::ifstream lcmFile;
	lcmFile.open(filePath);

	rapidxml::xml_document<> doc;

	// Buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(lcmFile)), std::istreambuf_iterator<char>( ));

	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]); 

	rapidxml::xml_node<>* meshes = doc.first_node();
	int meshCount = atoi(meshes->first_attribute()->value());
	rapidxml::xml_node<>* mesh = meshes->first_node();
	for (int i = 0; i < meshCount; i++)
	{
		int hasNormals = atoi(mesh->first_attribute()->value());
		int verticesCount = atoi(mesh->first_attribute()->next_attribute()->value());

		Model::VertexData* vd1 = new VertexData[verticesCount];

		rapidxml::xml_node<>* vertices = mesh->first_node();
		rapidxml::xml_node<>* vertex = vertices->first_node();
		for (int j = 0; j < verticesCount; j++)
		{
			int hasColors = atoi(vertex->first_attribute()->value());
			int hasTexCoords = atoi(vertex->first_attribute()->next_attribute()->value());

			rapidxml::xml_node<>* prop = vertex->first_node();
			vd1[j].x = atof(prop->first_attribute()->value());
			vd1[j].y = atof(prop->first_attribute()->next_attribute()->value());
			vd1[j].z = atof(prop->first_attribute()->next_attribute()->next_attribute()->value());

			prop = prop->next_sibling();
			if (hasColors)
			{
				vd1[j].r = atof(prop->first_attribute()->value());
				vd1[j].b = atof(prop->first_attribute()->next_attribute()->value());
				vd1[j].g = atof(prop->first_attribute()->next_attribute()->next_attribute()->value());

				prop = prop->next_sibling();
			}

			if (hasTexCoords)
			{
				vd1[j].u = atof(prop->first_attribute()->value());
				vd1[j].v = atof(prop->first_attribute()->next_attribute()->value());

				prop = prop->next_sibling();
			}

			if (hasNormals)
			{
				vd1[j].n1 = atof(prop->first_attribute()->value());
				vd1[j].n2 = atof(prop->first_attribute()->next_attribute()->value());
				vd1[j].n3 = atof(prop->first_attribute()->next_attribute()->next_attribute()->value());

				prop = prop->next_sibling();
			}

			vertex = vertex->next_sibling();
		}

		// indices
		rapidxml::xml_node<>* lcmIndices = mesh->last_node();
		int numIndices = atoi(lcmIndices->first_attribute()->value());
		uint16* indices = new uint16[numIndices];

		rapidxml::xml_node<>* lcmIndice = lcmIndices->first_node();

		for (int k = 0; k < numIndices; k++)
		{
			indices[k] = (uint16)atoi(lcmIndice->value());
			lcmIndice = lcmIndice->next_sibling();
		}

		mesh = mesh->next_sibling();

		std::cout << vd1[verticesCount - 1].x << std::endl;
		std::cout << indices[numIndices - 1] << std::endl;

		SetMeshData(vd1, verticesCount, indices, numIndices);

		delete [] indices;
		delete [] vd1;

	}

	return true;

}

void Model::SetMeshData(VertexData* vd, int vertexCount, uint16* indexData, int indexCount) {
	glewInit();

	assert(vertexCount <= USHRT_MAX);
	if(vertexCount == 0)
		return;

	if(!m_isValid)
	{
		glGenBuffers(1, &m_vertexVboId);
		glGenBuffers(1, &m_indexVboId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vd, GL_STATIC_DRAW);

	m_indicesCount = indexCount;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * indexCount, indexData, GL_STATIC_DRAW);

	m_isValid = true;
}

void Model::Render(bool wireFrame) const
{
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glMultMatrixf(m_rot.RotationMatrix().GetInternalValues());
	glScalef(m_scale.x, m_scale.y, m_scale.z);

	if(IsValid())
	{
		glClientActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (char*)0);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, sizeof(VertexData), (char*)12);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (char*)24);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(VertexData), (char*)32);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
		glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_SHORT, (char*)0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		CHECK_GL_ERROR();
	}

	glPopMatrix();
}

void Model::Rotate( const Quaternion& rot )
{
	m_rot = m_rot * rot;
}

void Model::Translate( float x, float y, float z )
{
	Translate(Vector3f(x, y, z));
}

void Model::Translate( const Vector3f& trans )
{
	m_pos += trans;
}

void Model::Scale( float x, float y, float z )
{
	Scale(Vector3f(x, y, z));
}

void Model::Scale( const Vector3f& scale )
{
	m_scale.x *= scale.x;
	m_scale.y *= scale.y;
	m_scale.z *= scale.z;
}

void Model::SetPosition(const Vector3f& pos) {
	m_pos = pos;
}

Vector3f Model::GetPosition() const
{
	return m_pos;
}

void Model::SetRotation(const Vector3f& rot) {
	Vector3f rotRad = -rot * PII / 180;
	Quaternion q;
	q.FromAxis(rotRad.Lenght(), rotRad);
	q.Normalise();
	m_rot = q;
}

void Model::SetRotation( const Quaternion& rot )
{
	m_rot = rot;
}

Quaternion Model::GetRotation() const
{
	return m_rot;
}


