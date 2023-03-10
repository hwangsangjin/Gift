#include "pch.h"
#include "Mesh.h"
#include "RenderSystem.h"
#include "VertexMesh.h"
#include "ResourceManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Engine.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader/tiny_obj_loader.h"

Mesh::Mesh(const wchar_t* file_path, ResourceManager* resource_manager)
	: Resource(file_path, resource_manager)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	auto input_file = std::filesystem::path(file_path).string();

	std::string mtl_dir = input_file.substr(0, input_file.find_last_of("\\/"));

	bool result = tinyobj::LoadObj(&attribs, &shapes, &materials, &warning, &error, input_file.c_str(), mtl_dir.c_str());
	if (!error.empty())
		GiftError("Mesh not created successfully");
	if (!result)
		GiftError("Mesh not created successfully");

	std::vector<VertexMesh> vertex_mesh_list;
	std::vector<UINT> index_list;

	size_t vertex_buffer_size = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		vertex_buffer_size += shapes[s].mesh.indices.size();
	}

	vertex_mesh_list.reserve(vertex_buffer_size);
	index_list.reserve(vertex_buffer_size);

	m_material_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_material_slots[m].start_index = index_global_offset;
		m_material_slots[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				if (shapes[s].mesh.material_ids[f] != m)
				{
					UCHAR num_face_vertices = shapes[s].mesh.num_face_vertices[f];
					index_offset += num_face_vertices;
					continue;
				}

				UCHAR num_face_vertices = shapes[s].mesh.num_face_vertices[f];

				Vector3 vertices_face[3];
				Vector2 texcoords_face[3];

				for (UCHAR v = 0; v < num_face_vertices; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					vertices_face[v] = Vector3(vx, vy, vz);
					texcoords_face[v] = Vector2(tx, ty);
				}

				Vector3 tangent, binormal;

				ComputeTangent
				(
					vertices_face[0], vertices_face[1], vertices_face[2],
					texcoords_face[0], texcoords_face[1], texcoords_face[2],
					tangent, binormal
				);

				for (UCHAR v = 0; v < num_face_vertices; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					tinyobj::real_t nx = 0;
					tinyobj::real_t ny = 0;
					tinyobj::real_t nz = 0;
					if (attribs.texcoords.size())
					{
						nx = attribs.normals[index.normal_index * 3 + 0];
						ny = attribs.normals[index.normal_index * 3 + 1];
						nz = attribs.normals[index.normal_index * 3 + 2];
					}

					Vector3 v_tangent, v_binormal;
					v_binormal = Vector3::Cross(Vector3(nx, ny, nz), tangent);
					v_tangent = Vector3::Cross(v_binormal, Vector3(nx, ny, nz));

					VertexMesh vertex_mesh(Vector3(vx, vy, vz), Vector2(tx, ty), Vector3(nx, ny, nz), v_tangent, v_binormal);
					vertex_mesh_list.push_back(vertex_mesh);
					index_list.push_back((UINT)index_global_offset + v);
				}

				index_offset += num_face_vertices;
				index_global_offset += num_face_vertices;
			}
		}

		m_material_slots[m].index_size = index_global_offset - m_material_slots[m].start_index;
	}

	m_vertex_buffer = m_resource_manager->GetEngine()->GetRenderSystem()->CreateVertexBuffer(&vertex_mesh_list[0], sizeof(VertexMesh), static_cast<UINT>(index_list.size()));
	assert(m_vertex_buffer);

	m_index_buffer = m_resource_manager->GetEngine()->GetRenderSystem()->CreateIndexBuffer(&index_list[0], static_cast<UINT>(index_list.size()));
	assert(m_index_buffer);
}

Mesh::Mesh(VertexMesh* vertex_mesh_data, UINT vertex_size, UINT* index_data, UINT index_size, MaterialSlot* material_slot, UINT material_slot_size, ResourceManager* resource_manager)
	: Resource(L"", resource_manager)
{
	m_vertex_buffer = m_resource_manager->GetEngine()->GetRenderSystem()->CreateVertexBuffer(vertex_mesh_data, sizeof(VertexMesh), vertex_size);
	assert(m_vertex_buffer);

	m_index_buffer = m_resource_manager->GetEngine()->GetRenderSystem()->CreateIndexBuffer(index_data, index_size);
	assert(m_index_buffer);

	m_material_slots.resize(material_slot_size);

	for (UINT i = 0; i < material_slot_size; i++)
	{
		m_material_slots[i] = material_slot[i];
	}
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::GetVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::GetIndexBuffer()
{
	return m_index_buffer;
}

MaterialSlot Mesh::GetMaterialSlot(UINT slot)
{
	return m_material_slots[slot];
}

size_t Mesh::GetMaterialSlotSize() const
{
	return m_material_slots.size();
}

void Mesh::ComputeTangent(const Vector3 v0, const Vector3 v1, const Vector3 v2, const Vector2 t0, const Vector2 t1, const Vector2 t2, Vector3& tangent, Vector3 binormal)
{
	Vector3 delta_position1 = v1 - v0;
	Vector3 delta_position2 = v2 - v0;

	Vector2 delta_uv1 = t1 - t0;
	Vector2 delta_uv2 = t2 - t0;

	float r = 1.0f / (delta_uv1.m_x * delta_uv2.m_y - delta_uv1.m_y * delta_uv2.m_x);
	tangent = (delta_position1 * delta_uv2.m_y - delta_position2 * delta_uv1.m_y);
	tangent = Vector3::Normalize(tangent);
	binormal = (delta_position2 * delta_uv1.m_x - delta_position1 * delta_uv2.m_x);
	binormal = Vector3::Normalize(binormal);
}
