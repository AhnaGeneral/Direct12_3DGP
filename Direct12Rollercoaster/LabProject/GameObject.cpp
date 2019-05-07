#include "StdAfx.h"
#include "GameObject.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////

inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
	//ex)  fMin = -1.0f, fMax = 1.0f   -1,0f +randfloat / 0x7fff x 2.f
}

XMVECTOR RandomUnitVectorOnSphere()//CExplosiveObject에서 사용된다. 
{
	XMVECTOR xmvOne = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR xmvZero = XMVectorZero();
	// [질문]  xmvZero는 아무대도 참조가 안되어있음
	while (true)
	{
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);
		if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne)) return(XMVector3Normalize(v));
		// 모든 방향에 정규화 시킨다. 
		// XMVector3Greater V1 가 V2 보다 큰 경우 는 true를 돌려 주어 , 그렇지 않은 경우는 false를 돌려줍니다. 
		// [질문]XMVector3LengthSq 여기 안에 가보면 XMVectorDot로 두벡터의 각도를 반환하는데 같은 벡터의 각도를 받는 이유가 뭘까? sq는 제곱
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
CGameObject::CGameObject() 
{ 
	//멤버변수들 초기화 
	m_pMesh = NULL; 
	m_xmf4x4World = Matrix4x4::Identity();

	m_dwColor = RGB(0, 0, 0);

	m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_fMovingSpeed = 0.0f;
	m_fMovingRange = 0.0f;

	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.05f;
}

CGameObject::CGameObject(CMesh *pMesh) : CGameObject()
{
	m_pMesh = pMesh; 
}

CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetPosition(float x, float y, float z) 
{
	//position matrix에 x,y,z를 대입 
	m_xmf4x4World._41 = x; 
	m_xmf4x4World._42 = y; 
	m_xmf4x4World._43 = z; 
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position) 
{ 
	m_xmf4x4World._41 = xmf3Position.x; 
	m_xmf4x4World._42 = xmf3Position.y; 
	m_xmf4x4World._43 = xmf3Position.z; 
}

void CGameObject::SetLook(XMFLOAT3& xmf3Position)
{
	m_xmf4x4World._31 = xmf3Position.x;
	m_xmf4x4World._32 = xmf3Position.y;
	m_xmf4x4World._33 = xmf3Position.z;
}

void CGameObject::SetUp(XMFLOAT3& xmf3Position)
{
	m_xmf4x4World._21 = xmf3Position.x;
	m_xmf4x4World._22 = xmf3Position.y;
	m_xmf4x4World._23 = xmf3Position.z;
}

void CGameObject::SetRight(XMFLOAT3& xmf3Position)
{
	m_xmf4x4World._11 = xmf3Position.x;
	m_xmf4x4World._12 = xmf3Position.y;
	m_xmf4x4World._13 = xmf3Position.z;
}

XMFLOAT3 CGameObject::GetPosition() 
{ 
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43)); 
}

XMFLOAT3 CGameObject::GetLook() 
{ 	
	XMFLOAT3 xmf3Look(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	xmf3Look = Vector3::Normalize(xmf3Look);
	return(xmf3Look);
}

XMFLOAT3 CGameObject::GetUp() 
{ 	
	XMFLOAT3 xmf3Up(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	xmf3Up = Vector3::Normalize(xmf3Up);
	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetRight()
{ 	
	XMFLOAT3 xmf3Right(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	xmf3Right = Vector3::Normalize(xmf3Right);
	return(xmf3Right);
}




// 월드 변환 행렬의 연산이 이루워지는 곳 !!================================================================================

void CGameObject::MoveStrafe(float fDistance)//움직임 처벌? 
{
	// xmf3Position = Right scalar + position  
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Right, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Up, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Look, fDistance));
	CGameObject::SetPosition(xmf3Position);
}

// ===============================================================================================================================================


void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{

	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fPitch, fYaw, fRoll);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);

}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fDistance)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fDistance, m_xmf4x4World._42 + vDirection.y * fDistance, m_xmf4x4World._43 + vDirection.z * fDistance);
}

void CGameObject::Animate(float fElapsedTime)
{
	if (m_fRotationSpeed != 0.0f) Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
	//move player가 아닌 다른 객체들의 위치 정보들을 저장한 곳에서 움직임을의 크기를 정해주는거 ?  m_xmf3MovingDirection
	if (m_pMesh)
	{
		m_pMesh->m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (m_pMesh)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		m_pMesh->Render(hDCFrameBuffer, m_xmf4x4World, pCamera);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

CWallsObject::CWallsObject()
{
}

CWallsObject::~CWallsObject()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//CExplosiveObject의 파티클의 위치 정보를 넣어줄 배열
XMFLOAT3 CExplosiveObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];

CMesh *CExplosiveObject::m_pExplosionMesh = NULL;

CExplosiveObject::CExplosiveObject()
{
}

CExplosiveObject::~CExplosiveObject()
{
}

void CExplosiveObject::PrepareExplosion()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) 
	XMStoreFloat3(&m_pxmf3SphereVectors[i], ::RandomUnitVectorOnSphere());
	//랜덤한 벡터의 포지션을 받아와서 vector를 float변수로 변경해준다.

	m_pExplosionMesh = new CCubeMesh(0.3f, 0.3f, 0.3f);
}

void CExplosiveObject::Animate(float fElapsedTime)
{
	if (m_bBlowingUp)
	{
		m_fElapsedTimes += fElapsedTime;
		if (m_fElapsedTimes <= m_fDuration)
		{
			XMFLOAT3 xmf3Position = GetPosition();
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
				m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);
			}
		}
		else
		{
			m_bBlowingUp = false;
			m_fElapsedTimes = 0.0f;
		}
	}
	else
	{
		CGameObject::Animate(fElapsedTime);
	}
}

void CExplosiveObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (m_bBlowingUp)//A키를 누르면 m_bBlowingUp는 true
	{
		for (int i = 0; i < EXPLOSION_DEBRISES; i++)
		{
			if (m_pExplosionMesh)
			{
				HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
				HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
				m_pExplosionMesh->Render(hDCFrameBuffer, m_pxmf4x4Transforms[i], pCamera);
				::SelectObject(hDCFrameBuffer, hOldPen);
				::DeleteObject(hPen);
			}
		}
	}
	else
	{
		CGameObject::Render(hDCFrameBuffer, pCamera);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTerrainObject::CTerrainObject()
{
}

CTerrainObject::~CTerrainObject()
{
}


CFollowerObject::CFollowerObject(CGameObject *pTarget)
{
	m_pTarget = pTarget;
}

CFollowerObject::~CFollowerObject()
{
}
//[질문] 따라다니는 큐브의 animate인데 .. 언제 업데이트가 되는걸까? 
void CFollowerObject::Animate(float fElapsedTime)
{
	XMFLOAT3 xmf3TargetPosition = m_pTarget->GetPosition(); //play의 포지션
	XMFLOAT3 xmf3Position = GetPosition(); //followerobject **ppobject에서 동적할당되어있는 객체들의  포지션 정보

	XMFLOAT3 xmf3Look = GetLook(); //followerobject **ppobject에서 동적할당되어있는 객체들의 바라보고 있는 시점
	XMFLOAT3 xmf3ToTarget = Vector3::Subtract(xmf3TargetPosition, xmf3Position, true);
	//[질문]XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2))); true,false 정규화와 정규화 아닌것의 차이?
	// A-B  B--->A로 가는 방향을 구하는 식 !!! 

	float fDotProduct = Vector3::DotProduct(xmf3Look, xmf3ToTarget); 
	//바라보는쪽, 내가 움직이여야하는 방향벡터의 내적

	// 90도 이면 같다는뜻 ! 0도 이니?  응 ! 0.0f이고 아니면 예각이니 둔각이니? 예각이면 0보다 크겠지? 
	float fAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : ((fDotProduct > 0.0f) ? XMConvertToDegrees(acos(fDotProduct)) : 90.0f);

	XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3Look, xmf3ToTarget);
//	if (fAngle != 0.0f) Rotate(0.0f, fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), 0.0f);
	Rotate(0.0f, fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), 0.0f);

	//[질문 레알 질문]
	if (Vector3::Distance(xmf3TargetPosition, xmf3Position) > DISTANCE_TO_TARGET_OBJECT)
	{
		XMFLOAT3 xmf3Look = GetLook();
		SetPosition(Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Look, m_fMovingSpeed * fElapsedTime, false)));
	}

//	CGameObject::Animate(fElapsedTime);
}

