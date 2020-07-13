#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3dCompiler.lib")
class ComputeShader
{
public:
	ComputeShader();
	~ComputeShader();
	ComputeShader(ID3D11Device* device, LPCWSTR filePath, LPCSTR pEntryPoint, LPCSTR pTarget);
	HRESULT InitShader(ID3D11Device* device, LPCWSTR filePath, LPCSTR pEntryPoint = "main", LPCSTR pTarget = "cs_5_0");
	ID3D10Blob* compiledComputeShader;
	ID3D11ComputeShader* pComputeShader;
};

