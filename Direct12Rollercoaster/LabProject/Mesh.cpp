#include "stdafx.h"
#include "Mesh.h"

CPolygon::CPolygon(int nVertices)
{
	if (nVertices > 0)
	{
		m_nVertices = nVertices;
		m_pVertices = new CVertex[nVertices];
	}
}

CPolygon::~CPolygon()
{
	if (m_pVertices) delete[] m_pVertices;
}

void CPolygon::SetVertex(int nIndex, CVertex& vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

CMesh::CMesh(int nPolygons)
{
	if (nPolygons > 0)
	{
		m_nPolygons = nPolygons;
		m_ppPolygons = new CPolygon*[nPolygons];
		m_nReferences = 0;
	}
}

CMesh::~CMesh(void)
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i]) delete m_ppPolygons[i];
		delete[] m_ppPolygons;
	}
}

void CMesh::SetPolygon(int nIndex, CPolygon *pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons) && m_ppPolygons)
	{
		m_ppPolygons[nIndex] = pPolygon;
	}
}

void CMesh::Render(HDC hDCFrameBuffer, XMFLOAT4X4& xmf4x4World, CCamera *pCamera)
{
	XMFLOAT4X4 xmf4x4Transform = Matrix4x4::Multiply(xmf4x4World, pCamera->m_xmf4x4ViewProject);
	for (int j = 0; j < m_nPolygons; j++)
	{
		int nVertices = m_ppPolygons[j]->m_nVertices;
		CVertex *pVertices = m_ppPolygons[j]->m_pVertices;
		XMFLOAT3 xmf3Previous(-1.0f, 0.0f, 0.0f);
		for (int i = 0; i <= nVertices; i++)
		{
			CVertex vertex = pVertices[i % nVertices];
			//World/View/Projection Transformation(Perspective Divide)
			XMFLOAT3 xmf3Current = Vector3::TransformCoord(vertex.m_xmf3Position, xmf4x4Transform);
			if ((xmf3Current.z >= 0.0f) && (xmf3Current.z <= 1.0f))
			{
				//Screen Transformation
				xmf3Current.x = +xmf3Current.x * (pCamera->m_Viewport.m_nWidth * 0.5f) + pCamera->m_Viewport.m_xStart + (pCamera->m_Viewport.m_nWidth * 0.5f);
				xmf3Current.y = -xmf3Current.y * (pCamera->m_Viewport.m_nHeight * 0.5f) + pCamera->m_Viewport.m_yStart + (pCamera->m_Viewport.m_nHeight * 0.5f);

				if (xmf3Previous.x >= 0.0f)
				{
					::MoveToEx(hDCFrameBuffer, (long)xmf3Previous.x, (long)xmf3Previous.y, NULL);
					::LineTo(hDCFrameBuffer, (long)xmf3Current.x, (long)xmf3Current.y);
				}
				xmf3Previous = xmf3Current;
			}
		}
	}
}

CCubeMesh::CCubeMesh(float fWidth, float fHeight, float fDepth) : CMesh(6)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(0, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pTopFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	SetPolygon(1, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	SetPolygon(2, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBottomFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(3, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pLeftFace->SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(4, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pRightFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(5, pRightFace);
}

CCubeMesh::~CCubeMesh(void)
{
}

CWallMesh::CWallMesh(float fWidth, float fHeight, float fDepth, int nSubRects) : CMesh((4 * nSubRects * nSubRects) + 2)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	int k = 0;
	CPolygon *pLeftFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pLeftFace = new CPolygon(4);
			pLeftFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight+(i*fCellHeight), -fHalfDepth+(j*fCellDepth)));
			pLeftFace->SetVertex(1, CVertex(-fHalfWidth, -fHalfHeight+((i+1)*fCellHeight), -fHalfDepth+(j*fCellDepth)));
			pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight+((i+1)*fCellHeight), -fHalfDepth+((j+1)*fCellDepth)));
			pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight+(i*fCellHeight), -fHalfDepth+((j+1)*fCellDepth)));
			SetPolygon(k++, pLeftFace);
		}
	}

	CPolygon *pRightFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pRightFace = new CPolygon(4);
			pRightFace->SetVertex(0, CVertex(+fHalfWidth, -fHalfHeight + (i*fCellHeight), -fHalfDepth + (j*fCellDepth)));
			pRightFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight + ((i + 1)*fCellHeight), -fHalfDepth + (j*fCellDepth)));
			pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight + ((i + 1)*fCellHeight), -fHalfDepth + ((j + 1)*fCellDepth)));
			pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight + (i*fCellHeight), -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pRightFace);
		}
	}

	CPolygon *pTopFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pTopFace = new CPolygon(4);
			pTopFace->SetVertex(0, CVertex(-fHalfWidth + (i*fCellWidth), +fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pTopFace->SetVertex(1, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), +fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pTopFace->SetVertex(2, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			pTopFace->SetVertex(3, CVertex(-fHalfWidth + (i*fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pTopFace);
		}
	}

	CPolygon *pBottomFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pBottomFace = new CPolygon(4);
			pBottomFace->SetVertex(0, CVertex(-fHalfWidth + (i*fCellWidth), -fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pBottomFace->SetVertex(1, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), -fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pBottomFace->SetVertex(2, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			pBottomFace->SetVertex(3, CVertex(-fHalfWidth + (i*fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pBottomFace);
		}
	}

	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(k++, pFrontFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	SetPolygon(k++, pBackFace);
}

CWallMesh::~CWallMesh(void)
{
}

CAirplaneMesh::CAirplaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(24)
{
	float fx = fWidth*0.2, fy = fHeight*0.2, fz = fDepth*0.2;

	float x1 = fx * 0.5f, y1 = fy * 0.5f, x2 = fx * 0.3f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	CPolygon *pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Rail

}

CAirplaneMesh::~CAirplaneMesh()
{
}

CTerrainMesh::CTerrainMesh(float fWidth, float fHeight, float fDepth, int nSubRects) : CMesh((nSubRects * nSubRects))
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	for (int i = 0, k = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			CPolygon *pPolygon = new CPolygon(4);
			pPolygon->SetVertex(0, CVertex(-fHalfWidth + (i*fCellWidth), 0.0f, -fHalfDepth + (j*fCellDepth)));
			pPolygon->SetVertex(1, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), 0.0f, -fHalfDepth + (j*fCellDepth)));
			pPolygon->SetVertex(2, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), 0.0f, -fHalfDepth + ((j + 1)*fCellDepth)));
			pPolygon->SetVertex(3, CVertex(-fHalfWidth + (i*fCellWidth), 0.0f, -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pPolygon);
		}
	}
}

CTerrainMesh::~CTerrainMesh(void)
{
}

CRail::CRail(float fWidth, float fHeight, float fDepth): CMesh(1)
{

	float fx = fWidth *0.2 , fy = fHeight * 0.2, fz = fDepth * 0.2;

	float x1 = fx * 0.5f, y1 = fy * 0.5f, x2 = fx * 0.3f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;

	CPolygon* pFace = new CPolygon(3);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, -fx, +fx*0.5f));
	pFace->SetVertex(1, CVertex(+fx, -fx, +fx*0.5f));
	pFace->SetVertex(2, CVertex(+fx, -fx, -x1 *0.05f));
	pFace->SetVertex(3, CVertex(-fx, -fx, -x1 *0.05f));
	SetPolygon(i++, pFace);			


}

CRail::~CRail()
{
}

FORWARDAirPlaneMesh::FORWARDAirPlaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(35)
{
	float fx = fWidth * 0.2, fy = fHeight * 0.2, fz = fDepth * 0.2;
	float x1 = fx * 0.5f, y1 = fy * 0.5f, x2 = fx * 0.3f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;
	//Upper Plane
	CPolygon * pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Rail

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx , +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 1.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 1.5f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 1.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 1.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 2.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 2.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 2.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 2.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx *2.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx *2.5f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 2.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 2.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.5f, +fx ));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.5f, +fx ));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 4.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 4.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 4.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 4.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 4.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 4.5f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 4.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 4.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 5.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 5.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 5.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 5.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 5.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 5.5f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 5.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 5.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 6.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 6.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 6.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 6.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 6.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 6.5f, +fx));
	SetPolygon(i++, pFace);
}

FORWARDAirPlaneMesh::~FORWARDAirPlaneMesh()
{
}

UPAirPlaneMesh::UPAirPlaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(33)
{
	float fx = fWidth * 0.2, fy = fHeight * 0.2, fz = fDepth * 0.2;

	float x1 = fx * 0.5f, y1 = fy * 0.5f, x2 = fx * 0.3f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	CPolygon * pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Rail


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 1.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 1.5f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 1.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 1.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 2.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 2.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 2.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 2.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 2.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 2.5f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 2.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 2.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.5f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 4.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 4.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 4.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 4.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 4.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 4.5f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 4.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 4.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 5.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 5.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 5.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 5.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 5.5f, +fx*-0.8));
	pFace->SetVertex(3, CVertex(-fx, +fx * 5.5f, +fx*-0.8));
	SetPolygon(i++, pFace);





}

UPAirPlaneMesh::~UPAirPlaneMesh()
{

}

DOWNAirPlaneMesh::DOWNAirPlaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(33)
{
	float fx = fWidth * 0.2, fy = fHeight * 0.2, fz = fDepth * 0.2;

	float x1 = fx * 0.5f, y1 = fy * 0.5f, x2 = fx * 0.3f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	CPolygon * pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Rail


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 1.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 1.5f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 1.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 1.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 2.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 2.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 2.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 2.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 2.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 2.5f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 2.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 2.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.5f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.5f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 4.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 4.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 4.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 4.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 4.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 4.5f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 4.5f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 4.5f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 5.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 5.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 5.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 5.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 5.5f, +fx *2.0));
	pFace->SetVertex(3, CVertex(-fx, +fx * 5.5f, +fx *2.0));
	SetPolygon(i++, pFace);


}

DOWNAirPlaneMesh::~DOWNAirPlaneMesh()
{

}

RIGHTAirPlaneMesh::RIGHTAirPlaneMesh(float fWidth, float fHeight, float fDepth):CMesh(31) 
{
	float fx = fWidth * 0.2, fy = fHeight * 0.2, fz = fDepth * 0.2;

	float x1 = fx * 0.5f, y1 = fy * 0.5f, x2 = fx * 0.3f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	CPolygon * pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Rail

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 6.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 6.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 6.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 6.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 9.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 9.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 9.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 9.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 12.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 12.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 12.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 12.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 15.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 15.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 15.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 15.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx * 0.5f, +fx * 18.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx * 2.0f, +fx * 18.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx * 2.0f, +fx * 18.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx * 0.5f, +fx * 18.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx * -0.5f, +fx * 24.0f, fx));
	pFace->SetVertex(3, CVertex(-fx * 4.0f, +fx * 24.0f, fx));
	SetPolygon(i++, pFace);
}

RIGHTAirPlaneMesh::~RIGHTAirPlaneMesh() {}

LEFTAirPlaneMesh::LEFTAirPlaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(31)
{
	float fx = fWidth * 0.2, fy = fHeight * 0.2, fz = fDepth * 0.2;

	float x1 = fx * 0.5f, y1 = fy * 0.5f, x2 = fx * 0.3f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1) * x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	CPolygon * pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Rail

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 3.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 3.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 3.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 6.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 6.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 6.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 6.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 9.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 9.0f, +fx));
	SetPolygon(i++, pFace);


	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 9.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 9.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 12.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 12.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 12.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 12.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx, +fx * 15.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx, +fx * 15.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx, +fx * 15.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx, +fx * 15.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx * 2.0f, +fx * 18.0f, +fx));
	pFace->SetVertex(3, CVertex(-fx * 0.5f, +fx * 18.0f, +fx));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(4);
	pFace->SetVertex(0, CVertex(-fx * 0.5f, +fx * 18.0f, +fx));
	pFace->SetVertex(1, CVertex(+fx * 2.0f, +fx * 18.0f, +fx));
	pFace->SetVertex(2, CVertex(+fx * 4.0f, +fx * 24.0f, fx));
	pFace->SetVertex(3, CVertex(-fx * -0.5f, +fx * 24.0f, fx));
	SetPolygon(i++, pFace);


}

LEFTAirPlaneMesh::~LEFTAirPlaneMesh()
{

}