#include "model.h"
#include <iostream>
#include "rapidxml-1.13/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <cassert>
#include "util/tool.h"
#include "util/matrix4.h"

Model::Model() : m_scale(Vector3f(1,1,1)), m_rot(1,0,0,0), m_pos(Vector3f(0,0,0)), m_translation(Vector3f(0,0,0)), m_meshes(0), m_meshCount(0) {

}

Model::~Model() {

	for (int i = 0; i < m_meshCount; ++i){
		glDeleteBuffers(1, &m_meshes[i].verticesVboId);
		glDeleteBuffers(1, &m_meshes[i].indicesVboId);
	}
	delete[] m_meshes;
}

bool Model::Load( const std::string filePath )
{
	std::ifstream lcmFile;
	lcmFile.open(filePath);

	rapidxml::xml_document<> doc;

	// Buffer
	std::vector<char> buffer((std::istreambuf_iterator<char>(lcmFile)), std::istreambuf_iterator<char>( ));
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]); 

	rapidxml::xml_node<>* lcmMeshes = doc.first_node();
	m_meshCount = atoi(lcmMeshes->first_attribute()->value());
	Mesh* m_meshes = new Mesh[m_meshCount];
	rapidxml::xml_node<>* lcmMesh = lcmMeshes->first_node();
	for (int i = 0; i < m_meshCount; i++)
	{
		Mesh& mesh = m_meshes[i];
		int hasNormals = atoi(lcmMesh->first_attribute()->value());
		mesh.verticesCount = atoi(lcmMesh->first_attribute()->next_attribute()->value());

		mesh.vertices= new VertexData[mesh.verticesCount];
		VertexData* vd = mesh.vertices;

		rapidxml::xml_node<>* vertices = lcmMesh->first_node();
		rapidxml::xml_node<>* vertex = vertices->first_node();
		for (int j = 0; j < mesh.verticesCount; j++)
		{
			int hasColors = atoi(vertex->first_attribute()->value());
			int hasTexCoords = atoi(vertex->first_attribute()->next_attribute()->value());

			rapidxml::xml_node<>* prop = vertex->first_node();
			vd[j].x = atof(prop->first_attribute()->value());
			vd[j].y = atof(prop->first_attribute()->next_attribute()->value());
			vd[j].z = atof(prop->first_attribute()->next_attribute()->next_attribute()->value());

			prop = prop->next_sibling();
			if (hasColors)
			{
				vd[j].r = atof(prop->first_attribute()->value());
				vd[j].b = atof(prop->first_attribute()->next_attribute()->value());
				vd[j].g = atof(prop->first_attribute()->next_attribute()->next_attribute()->value());

				prop = prop->next_sibling();
			}

			if (hasTexCoords)
			{
				vd[j].u = atof(prop->first_attribute()->value());
				vd[j].v = atof(prop->first_attribute()->next_attribute()->value());

				prop = prop->next_sibling();
			}

			if (hasNormals)
			{
				vd[j].n1 = atof(prop->first_attribute()->value());
				vd[j].n2 = atof(prop->first_attribute()->next_attribute()->value());
				vd[j].n3 = atof(prop->first_attribute()->next_attribute()->next_attribute()->value());

				prop = prop->next_sibling();
			}

			vertex = vertex->next_sibling();
		}

		// indices
		rapidxml::xml_node<>* lcmIndices = lcmMesh->last_node();
		mesh.indicesCount = atoi(lcmIndices->first_attribute()->value());
		mesh.indices = new uint16[mesh.indicesCount];
		uint16* indices = mesh.indices;

		rapidxml::xml_node<>* lcmIndice = lcmIndices->first_node();

		for (int k = 0; k < mesh.indicesCount; k++)
		{
			indices[k] = (uint16)atoi(lcmIndice->value());
			lcmIndice = lcmIndice->next_sibling();
		}

		lcmMesh = lcmMesh->next_sibling();
	}

	SetMeshData(m_meshes, m_meshCount);

	for (int i = 0; i < m_meshCount; ++i)
	{
		delete [] m_meshes[i].indices;
		delete [] m_meshes[i].vertices;
	}

	return true;
}

void Model::SetMeshData(Mesh* meshes, uint32 meshesCount) {
	glewInit();

	for (int i = 0; i < meshesCount; ++i)
	{
		Mesh& mesh = meshes[i];

		assert(mesh.verticesCount <= USHRT_MAX);

		glGenBuffers(1, &mesh.verticesVboId);
		glGenBuffers(1, &mesh.indicesVboId);
		//glGenBuffers(1, &m_bonesVboId);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.verticesVboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * mesh.verticesCount, mesh.vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indicesVboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * mesh.indicesCount, mesh.indices, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bonesVboId);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * vertexCount, bones, GL_STATIC_DRAW);
	}

	m_meshes = meshes;
	m_meshCount = meshesCount;
}

void Model::Render(bool wireFrame) const
{
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glMultMatrixf(m_rot.RotationMatrix().GetInternalValues());
	glTranslatef(m_translation.x, m_translation.y, m_translation.z);
	glScalef(m_scale.x, m_scale.y, m_scale.z);

	for (int i = 0; i < m_meshCount; ++i)
	{
		glClientActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[i].verticesVboId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (char*)0);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, sizeof(VertexData), (char*)12);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (char*)24);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(VertexData), (char*)32);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshes[i].indicesVboId);
		glDrawElements(GL_TRIANGLES, m_meshes[i].indicesCount, GL_UNSIGNED_SHORT, (char*)0);

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
	m_translation += trans;
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


