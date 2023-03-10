#pragma once

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Vector2;
class Vector3;
class VertexMesh;

struct MaterialSlot
{
	size_t start_index = 0;
	size_t index_size = 0;
	size_t material_id = 0;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* file_path, ResourceManager* resource_manager);
	Mesh(VertexMesh* vertex_mesh_data, UINT vertex_size, UINT* index_data, UINT index_size, MaterialSlot* material_slot, UINT material_slot_size, ResourceManager* resource_manager);
	virtual ~Mesh();

	const VertexBufferPtr& GetVertexBuffer();
	const IndexBufferPtr& GetIndexBuffer();

	MaterialSlot GetMaterialSlot(UINT slot);
	size_t GetMaterialSlotSize() const;

private:
	void ComputeTangent
	(
		const Vector3 v0, const Vector3 v1, const Vector3 v2,
		const Vector2 t0, const Vector2 t1, const Vector2 t2,
		Vector3& tangent, Vector3 binormal
	);

	VertexBufferPtr m_vertex_buffer = nullptr;
	IndexBufferPtr m_index_buffer = nullptr;

	std::vector<MaterialSlot> m_material_slots;
};

