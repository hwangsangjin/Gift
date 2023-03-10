#pragma once

class VertexShader
{
public:
	VertexShader(const wchar_t* file_path, RenderSystem* render_system);

	ID3D11VertexShader* GetVertexShader() const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertex_shader = nullptr;
	RenderSystem* m_render_system = nullptr;
};

