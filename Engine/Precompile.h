#pragma once

// lib
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "DirectXTK")
#pragma comment(lib, "DirectXTex")
#pragma comment(lib, "fmod_vc")

#include <cassert>
#include <exception>
#include <stdexcept>
#include <windows.h>
#include <iostream>
#include <comdef.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "DirectXTex/DirectXTex.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "FMOD/fmod.hpp"

// define
#define GiftError(message)\
{\
std::wclog << "Error: " << message << std::endl;\
throw std::runtime_error("");\
}

#define GiftWarning(message)\
std::wclog << "Warning: " << message << std::endl;

#define GiftInformation(message)\
std::wclog << "Information: " << message << std::endl;

// class
class Engine;
class Timer;
class World;
class Entity;
class InputSystem;
class SoundSystem;
class RenderSystem;
class Window;
class Display;
class SwapChain;
class DeviceContext;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;
class ResourceManager;
class Resource;
class Mesh;
class Texture;
class Texture2D;
class Material;
class Audio;
class Component;
class TransformComponent;
class MeshComponent;
class SpriteComponent;
class CameraComponent;
class LightComponent;
class TerrainComponent;
class WaterComponent;
class AudioComponent;

// typedef
typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Texture2D> Texture2DPtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Audio> AudioPtr;

// enum
enum class CameraType
{
	Orthorthographic,
	Perspective
};

enum class CullMode
{
	None,
	Front,
	Back
};

enum class FillMode
{
	Wireframe,
	Solid
};

enum class Key
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	Escape,
	Shift,
	Space,
	Enter,
	LeftMouseButton,
	MiddleMouseButton,
	RightMouseButton
};