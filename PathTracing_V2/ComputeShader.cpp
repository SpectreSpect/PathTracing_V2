#include "ComputeShader.h"

ComputeShader::ComputeShader()
{
   compiledComputeShader = nullptr;
   pComputeShader = nullptr;
}

ComputeShader::~ComputeShader()
{
    if (compiledComputeShader)
        compiledComputeShader->Release();
    if (pComputeShader)
        pComputeShader->Release();
}
ComputeShader::ComputeShader(ID3D11Device* device, LPCWSTR filePath, LPCSTR pEntryPoint, LPCSTR pTarget)
{
    HRESULT hr = InitShader(device, filePath, pEntryPoint, pTarget);
    if (FAILED(hr)) MessageBox(nullptr, L"InitShader() is failed", L"ComputeShader::ComputeShader()", MB_ICONERROR);
}

HRESULT ComputeShader::InitShader(ID3D11Device* device, LPCWSTR filePath, LPCSTR pEntryPoint, LPCSTR pTarget)
{
    if (compiledComputeShader)
        compiledComputeShader->Release();
    if (pComputeShader)
        pComputeShader->Release();
    HRESULT hr = D3DCompileFromFile(filePath, NULL, NULL, pEntryPoint, pTarget, NULL, NULL, &compiledComputeShader, NULL);
    if (FAILED(hr)) MessageBox(nullptr, L"D3DCompileFromFile() is failed", L"ComputeShader::InitShader()", MB_ICONERROR);
    hr = device->CreateComputeShader(compiledComputeShader->GetBufferPointer(), compiledComputeShader->GetBufferSize(), NULL, &pComputeShader);
    return hr;
}
