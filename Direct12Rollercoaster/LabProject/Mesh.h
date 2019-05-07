#pragma once

#include "Camera.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CVertex
{
public:
	CVertex() { }
	CVertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3(x, y, z); }

	XMFLOAT3					m_xmf3Position;
};

class CPolygon
{
public:
	CPolygon(int nVertices);
	virtual ~CPolygon();

	int							m_nVertices;
	CVertex						*m_pVertices;

	void SetVertex(int nIndex, CVertex& vertex);
};

class CMesh
{
public:
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int							m_nReferences;

	int							m_nPolygons;
	CPolygon					**m_ppPolygons;

public:
	BoundingOrientedBox			m_xmOOBB;

public:
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

public:
	void SetPolygon(int nIndex, CPolygon *pPolygon);
	virtual void Render(HDC hDCFrameBuffer, XMFLOAT4X4& xmf4x4World, CCamera *pCamera);

	// OOBB (Oriented Bounding Box )
	// center, extent, orientation 객체가 움직이면 bounding box도 같이 움직여서 모양이 변하는걸 신경쓰지 않아도 괜찮아
	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CCubeMesh();
};


class CRail : public CMesh
{
public:
	CRail(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CRail();
};

class CWallMesh : public CMesh
{
public:
	CWallMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f, int nSubRects = 20);
	virtual ~CWallMesh();
};

class CAirplaneMesh : public CMesh
{
public:
	CAirplaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~CAirplaneMesh();
};

class CTerrainMesh : public CMesh
{
public:
	CTerrainMesh(float fWidth = 100.0f, float fHeight = 4.0f, float fDepth = 100.0f, int nSubRects = 20);
	virtual ~CTerrainMesh();
};


class FORWARDAirPlaneMesh : public CMesh
{
public:
	FORWARDAirPlaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~FORWARDAirPlaneMesh();
};



class UPAirPlaneMesh : public CMesh
{
public:
	UPAirPlaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~UPAirPlaneMesh();
};



class DOWNAirPlaneMesh : public CMesh
{
public:
	DOWNAirPlaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~DOWNAirPlaneMesh();
};



class RIGHTAirPlaneMesh : public CMesh
{
public:
	RIGHTAirPlaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~RIGHTAirPlaneMesh();
};



class LEFTAirPlaneMesh : public CMesh
{
public:
	LEFTAirPlaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~LEFTAirPlaneMesh();
};

