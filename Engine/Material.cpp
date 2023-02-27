#include "pch.h"
#include "Material.h"
#include "Engine.h"
#include "ConstantBuffer.h"

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	// ���� ���̴� ����
	void* shader_byte_code = nullptr;
	size_t shader_byte_size = 0;
	Engine::GetInstance()->GetGraphics()->CompileVertexShader(vertex_shader_path, "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_vertex_shader = Engine::GetInstance()->GetGraphics()->CreateVertexShader(shader_byte_code, shader_byte_size);
	assert(m_vertex_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();

	// �ȼ� ���̴� ����
	Engine::GetInstance()->GetGraphics()->CompilePixelShader(pixel_shader_path, "main", &shader_byte_code, &shader_byte_size);
	assert(shader_byte_code);
	assert(shader_byte_size);
	m_pixel_shader = Engine::GetInstance()->GetGraphics()->CreatePixelShader(shader_byte_code, shader_byte_size);
	assert(m_pixel_shader);
	Engine::GetInstance()->GetGraphics()->ReleaseCompiledShader();
}

Material::Material(const MaterialPtr& material)
{
	m_vertex_shader = material->GetVertexShader();
	assert(m_vertex_shader);
	m_pixel_shader = material->GetPixelShader();
	assert(m_pixel_shader);
}

Material::~Material()
{
}

VertexShaderPtr Material::GetVertexShader()
{
	return m_vertex_shader;
}

PixelShaderPtr Material::GetPixelShader()
{
	return m_pixel_shader;
}

ConstantBufferPtr Material::GetConstantBuffer()
{
	return m_constant_buffer;
}

TexturePtr& Material::GetTexture()
{
	return m_textures[0];
}

size_t Material::GetTextureSize()
{
	return m_textures.size();
}

void Material::AddTexture(const TexturePtr& texture)
{
	m_textures.push_back(texture);
}

void Material::RemoveTexture(unsigned int index)
{
	if (index >= m_textures.size())
		return;

	m_textures.erase(m_textures.begin() + index);
}

void Material::SetData(void* data, unsigned int size)
{
	if (!m_constant_buffer)
		m_constant_buffer = Engine::GetInstance()->GetGraphics()->CreateConstantBuffer(&data, size);
	else
		m_constant_buffer->Update(Engine::GetInstance()->GetGraphics()->GetDeviceContext(), data);
}

Material::CullMode Material::GetCullMode()
{
	return m_cull_mode;
}

void Material::SetCullMode(CullMode cull_mode)
{
	m_cull_mode = cull_mode;
}

Material::FillMode Material::GetFillMode()
{
	return m_fill_mode;
}

void Material::SetFillMode(FillMode fill_mode)
{
	m_fill_mode = fill_mode;
}
