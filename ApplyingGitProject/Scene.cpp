//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene() { }
CScene::~CScene() { }

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 1000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(30.0f, 30.0f, 30.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	m_pLights[2].m_bEnable = true;
	m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 600.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(50.0f, 30.0f, 30.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	BuildDefaultLightsAndMaterials();

	XMFLOAT3 xmf3Scale(8.0f, 2.0f, 8.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.5f, 0.05f, 0.0f);
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Image/HeightMap.raw"), 257, 257, 257, 257, xmf3Scale, xmf4Color);

	pWaterMesh = new CSeaWater(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, 500.0f, 0.05f, 500.0f);
	pWaterMesh->SetPosition(XMFLOAT3(1000.f, m_pTerrain->GetHeight(500.f, 500.f)-10.f , 1000.f));
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float fxPitch = 12.0f * 10.0f;
	float fyPitch = 12.0f * 10.0f;
	float fzPitch = 12.0f * 10.0f;

	float fTerrainWidth = m_pTerrain->GetWidth();
	float fTerrainLength = m_pTerrain->GetLength();

	int xObjects = 10;
	int yObjects = 1;
	int zObjects = 8;
	m_nObjects = (xObjects * yObjects * zObjects);
	                                                                                                                    
	CBillboardRectMesh* StartViewMesh = new CBillboardRectMesh(pd3dDevice, pd3dCommandList, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.5f);
	StartView = new CStartView(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature); 
	StartView->SetMesh(StartViewMesh);


	m_ppBillboardObj = new CGameObject*[m_nObjects];
	CBillboardRectMesh* pBillboardMesh = new CBillboardRectMesh(pd3dDevice, pd3dCommandList, 50.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//SetMesh(pBillboardMesh);


	CTexture* pFreeTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pFreeTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Image/flower_white.dds", 0);
	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255); //256의 배수
	CBillboardShader *pBillboardShader = new CBillboardShader();
	pBillboardShader->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	pBillboardShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	pBillboardShader->CreateCbvSrvDescriptorHeaps(pd3dDevice, pd3dCommandList, 1, 1);
	//SetShader(pBillboardShader);
	//pBillboardShader->CreateConstantBufferViews(pd3dDevice, pd3dCommandList, 1, m_pd3dcbGameObjects, ncbElementBytes);
	pBillboardShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pFreeTexture, 6, false);

	//m_ppMaterials[0]->SetTexture(pFreeTexture);

	for (int i = 0, x = 0; x < xObjects; x++)
	{
		for (int z = 0; z < zObjects; z++)
		{
            pBillboard = new CBillboard(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, 50.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	        pBillboard->SetMesh(pBillboardMesh); 
			pBillboard->SetShader(pBillboardShader);
			pBillboard->m_ppMaterials[0]->SetTexture(pFreeTexture);
			float xPosition = x * fxPitch;
			float zPosition = z * fzPitch;
			float fHeight = m_pTerrain->GetHeight(xPosition, zPosition);

			if ((m_pTerrain->GetHeight(500.f, 500.f) - 10.f) < fHeight)
				pBillboard->SetPosition(xPosition, fHeight + 50.0f, zPosition);

			//pRotatingObject->SetCbvGPUDescriptorHandlePtr(m_d3dCbvGPUDescriptorStartHandle.ptr + (::gnCbvSrvDescriptorIncrementSize * i));
			m_ppBillboardObj[i++] = pBillboard;

		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_nGameObjects = 7;
	m_ppGameObjects = new CGameObject*[m_nGameObjects];
	//[질문]알수없음 왜 Child에 들어가 있는거지 

	CGameObject *pApacheModel = 
		CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Mi28.bin");

	CApacheObject* pApacheObject = NULL;

	CTexture *pTextures;
	pTextures = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pTextures->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/DirectTexture.dds", 0);

	CMaterial::m_pIlluminatedShader->CreateShaderResourceViews(pd3dDevice, pd3dCommandList, pTextures, 3, false);
	

	//질문
	//pApacheModel->SetTexture(pTextures);
	pApacheModel->m_pChild->SetTexture(pTextures); 

	pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(XMFLOAT3(1700.0f, m_pTerrain->GetHeight(1700.0f, 400.f) + 50.f, 400.f));
	pApacheObject->SetScale(1.2f, 1.2f, 1.2f);
	m_ppGameObjects[0] = pApacheObject;

	pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(XMFLOAT3(1800.0f, m_pTerrain->GetHeight(1800.0f, 400.f) + 50.f, 400.f));
	pApacheObject->SetScale(1.2f, 1.2f, 1.2f);
	m_ppGameObjects[1] = pApacheObject;

	pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	
	pApacheObject->SetPosition(XMFLOAT3(1900.0f, m_pTerrain->GetHeight(1900.0f, 400.f) + 50.f, 400.f));
	pApacheObject->SetScale(1.2f, 1.2f, 1.2f);
	m_ppGameObjects[2] = pApacheObject;

	pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(XMFLOAT3(1850.0f, m_pTerrain->GetHeight(1850.0f, 400.f) + 50.f, 400.f));
	pApacheObject->SetScale(1.2f, 1.2f, 1.2f);
	m_ppGameObjects[3] = pApacheObject;

	pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(XMFLOAT3(1750.0f, m_pTerrain->GetHeight(1750.0f, 400.f) + 50.f, 400.f));
	pApacheObject->SetScale(1.2f, 1.2f, 1.2f);
	m_ppGameObjects[4] = pApacheObject;

	pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(XMFLOAT3(1650.0f, m_pTerrain->GetHeight(1650.0f, 400.f) + 50.f, 400.f));
	pApacheObject->SetScale(1.2f, 1.2f, 1.2f);
	m_ppGameObjects[5] = pApacheObject;

	pApacheObject = new CApacheObject();
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(XMFLOAT3(1600.0f, m_pTerrain->GetHeight(1600.0f, 400.f) + 50.f, 400.f));
	pApacheObject->SetScale(1.2f, 1.2f, 1.2f);
	m_ppGameObjects[6] = pApacheObject;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}

	if (m_ppBillboardObj)
	{
		for (int i = 0; i < m_nObjects; i++) if (m_ppBillboardObj[i]) m_ppBillboardObj[i]->Release();
		delete[] m_ppBillboardObj;
	}
	//if (m_pTerrain) delete m_pTerrain;

	ReleaseShaderVariables();

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_DESCRIPTOR_RANGE pd3dDescriptorRanges[5];

	pd3dDescriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[0].NumDescriptors = 1;
	pd3dDescriptorRanges[0].BaseShaderRegister = 0; //t0: gtxtTexture
	pd3dDescriptorRanges[0].RegisterSpace = 0;
	pd3dDescriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[1].NumDescriptors = 2; 
	pd3dDescriptorRanges[1].BaseShaderRegister = 1; 
	pd3dDescriptorRanges[1].RegisterSpace = 0;
	pd3dDescriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[2].NumDescriptors = 1;
	pd3dDescriptorRanges[2].BaseShaderRegister = 3;  // t3: 
	pd3dDescriptorRanges[2].RegisterSpace = 0;
	pd3dDescriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[3].NumDescriptors = 1;
	pd3dDescriptorRanges[3].BaseShaderRegister = 4;  // t4: 
	pd3dDescriptorRanges[3].RegisterSpace = 0;
	pd3dDescriptorRanges[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	pd3dDescriptorRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pd3dDescriptorRanges[4].NumDescriptors = 1;
	pd3dDescriptorRanges[4].BaseShaderRegister = 5;  // t4: 
	pd3dDescriptorRanges[4].RegisterSpace = 0;
	pd3dDescriptorRanges[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER pd3dRootParameters[8];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	pd3dRootParameters[3].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[0];
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	pd3dRootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[4].DescriptorTable.NumDescriptorRanges = 1;  
	pd3dRootParameters[4].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[1];  
	//t1: gtxtTerrainBaseTexture, t2: gtxtTerrainDetailTexture
	pd3dRootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[5].DescriptorTable.NumDescriptorRanges = 1;  // water
	pd3dRootParameters[5].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[2];
	pd3dRootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[6].DescriptorTable.NumDescriptorRanges = 1;  // billboard
	pd3dRootParameters[6].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[3];
	pd3dRootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	pd3dRootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	pd3dRootParameters[7].DescriptorTable.NumDescriptorRanges = 1;  // 
	pd3dRootParameters[7].DescriptorTable.pDescriptorRanges = &pd3dDescriptorRanges[4];
	pd3dRootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//샘플러
	D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;
	::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	d3dSamplerDesc.MipLODBias = 0;
	d3dSamplerDesc.MaxAnisotropy = 1;
	d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	d3dSamplerDesc.ShaderRegister = 0;
	d3dSamplerDesc.RegisterSpace = 0;
	d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 1;//
	d3dRootSignatureDesc.pStaticSamplers = &d3dSamplerDesc;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{ //질문
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) 
		m_ppGameObjects[i]->ReleaseUploadBuffers();
	//if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
	for (int i = 0; i < m_nObjects; i++)
		m_ppBillboardObj[i]->ReleaseUploadBuffers();

}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': m_ppGameObjects[0]->MoveForward(+1.0f); break;
		case 'S': m_ppGameObjects[0]->MoveForward(-1.0f); break;
		case 'A': m_ppGameObjects[0]->MoveStrafe(-1.0f); break;
		case 'D': m_ppGameObjects[0]->MoveStrafe(+1.0f); break;
		case 'Q': m_ppGameObjects[0]->MoveUp(+1.0f); break;
		case 'R': m_ppGameObjects[0]->MoveUp(-1.0f); break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);

	if (m_pLights)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights
	if (!PlayOn)
	{
		if (StartView)
			StartView->Render(pd3dCommandList, pCamera);
	}
	if (PlayOn)
	{
		if (m_pTerrain)
			m_pTerrain->Render(pd3dCommandList, pCamera);
		if (pWaterMesh)
			pWaterMesh->Render(pd3dCommandList, pCamera);

		for (int i = 0; i < m_nObjects; i++)
		{
			if (m_ppBillboardObj[i])
			{
				//m_ppBillboardObj[i]->UpdateTransform(NULL);
				m_ppBillboardObj[i]->Render(pd3dCommandList, pCamera);
				m_ppBillboardObj[i]->Animates(m_fElapsedTime, pCamera);
			}
		}
		for (int i = 0; i < m_nGameObjects; i++)
		{
			if (m_ppGameObjects[i])
			{
				m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
				m_ppGameObjects[i]->UpdateTransform(NULL);
				m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
			}
		}
	}
}

