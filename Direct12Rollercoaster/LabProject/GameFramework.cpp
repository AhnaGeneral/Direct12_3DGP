//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
    ::srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	BuildFrameBuffer(); 

	BuildObjects(); 

	return(true);
}

void CGameFramework::BuildFrameBuffer()
{
    HDC hDC = ::GetDC(m_hWnd);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

    m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, (rcClient.right - rcClient.left) + 1, (rcClient.bottom - rcClient.top) + 1);
    ::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
    ::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
    HBRUSH hBrush = ::CreateSolidBrush(dwColor);
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, m_pPlayer->m_pCamera->m_Viewport.m_nWidth, m_pPlayer->m_pCamera->m_Viewport.m_nHeight);
    ::SelectObject(m_hDCFrameBuffer, hOldBrush);
    ::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{    
    HDC hDC = ::GetDC(m_hWnd);
    ::BitBlt(hDC, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, m_pPlayer->m_pCamera->m_Viewport.m_nWidth, m_pPlayer->m_pCamera->m_Viewport.m_nHeight, m_hDCFrameBuffer, m_pPlayer->m_pCamera->m_Viewport.m_xStart, m_pPlayer->m_pCamera->m_Viewport.m_yStart, SRCCOPY);
    ::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			::SetCapture(hWnd);//어떤 좌표인지 Set해주고
			::GetCursorPos(&m_ptOldCursorPos); //내 마우스 좌표가 무엇인지 Get해준다
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			::ReleaseCapture(); //다시 해제한다.
			break;
		case WM_MOUSEMOVE:
			break;
		default:
			break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					::PostQuitMessage(0);
					break;
				case 'S':
				{
					if (m_pScene)
					{
						m_pScene->ReleaseObjects();//원래 있던것을 지우고
						delete m_pScene;
					}
					m_nScene = (m_nScene + 1) % m_nScenes;
					switch (m_nScene)
					{
						case 0: 
							m_pScene = new CPlayerScene(); 
							break;
						case 1: 
							m_pScene = new CCollisionScene(); 
							break;
						case 2: 
							m_pScene = new CExplosionScene(); 
							break;
						case 3: 
							m_pScene = new CRunAwayScene(); 	
							break;
					}

					if (m_pPlayer) delete m_pPlayer;
					if (m_nScene == 3)
					{
						CAirplaneMesh* pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);

						m_pPlayer = new CTerrainPlayer();
						m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
						m_pPlayer->SetMesh(pAirplaneMesh);
						m_pPlayer->SetColor(RGB(0, 0, 255));
						m_pPlayer->SetMesh(pAirplaneMesh);
						m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 10.0f, -15.0f));
					}
					else
					{
						CAirplaneMesh *pAirplaneMesh = new CAirplaneMesh(8.0f, 8.0f, 2.0f);
						
						m_pPlayer = new CAirplanePlayer();
						m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
						m_pPlayer->SetMesh(pAirplaneMesh);
						m_pPlayer->SetColor(RGB(0, 0, 255));
						m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
					}

					m_pScene->m_pPlayer = m_pPlayer;

					m_pScene->BuildObjects();
					break;
				}
				//열차의 메쉬가 바뀌는 기능 있으면 재미있을거 같아서 추가함
				
				case 'C':
				{

					int m_nCharacters = 2;
					int m_nCharacter = 0; 

					m_nCharacter = (m_nCharacter + 1) % m_nCharacters;
					switch (m_nCharacter)
					{
					case 0:
					
						break;
					case 1:
					
						break;
					}

					if (m_nCharacter == 1)
					{
						CCubeMesh *pCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
						m_pPlayer->SetMesh(pCubeMesh);
					}
					else
					{
						CAirplaneMesh *pAirplaneMesh = new CAirplaneMesh(8.0f, 8.0f, 2.0f);
						m_pPlayer->SetMesh(pAirplaneMesh);
					}

				}
				default:
					m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
					break;
			}
			break;
		default:
			break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
				m_GameTimer.Stop();
			else
				m_GameTimer.Start();
			break;
		}
		case WM_SIZE:
			break;
		case WM_LBUTTONDOWN://왼쪽 마우스 단추가 아래로 있습니다.
		case WM_RBUTTONDOWN://오른쪽 마우스 단추가 아래로 있습니다.
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
			break;


		case WM_KEYDOWN:
		case WM_KEYUP:
			OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
			break;
		}
		return(0);
}

void CGameFramework::BuildObjects()
{
	
	CAirplaneMesh *pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);
	m_pPlayer = new CTerrainPlayer();
	m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);

	
	pAirplaneMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(6.0f, 6.0f, 6.0f * 0.3f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	//m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 8.0f, -20.0f));//플레이어랑 카메라의 거리
	m_nScenes = 4;
	m_nScene = 0;
	m_pScene = new CRunAwayScene();

	m_pScene->BuildObjects();

	m_pScene->m_pPlayer = m_pPlayer;

}

void CGameFramework::ReleaseObjects()
{
	if (m_pScene)
	{
		m_pScene->ReleaseObjects();
		delete m_pScene;
	}

	if (m_pPlayer) delete m_pPlayer;
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
    if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);

    if (m_hWnd) DestroyWindow(m_hWnd);
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	DWORD dwDirection = 0;
	XMFLOAT3 getposition;
	if (GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer[0x57] & 0xF0||pKeyBuffer[VK_UP]&0xF0)
		{
			m_pScene->SetBoolforward(true);
			dwDirection |= DIR_FORWARD;
			getposition = m_pPlayer->GetPosition();
			FORWARDAirPlaneMesh* pAirplaneMesh = new FORWARDAirPlaneMesh(8.0f, 8.0f, 2.0f);

			m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
			m_pPlayer->SetMesh(pAirplaneMesh);
			m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
	      	m_pScene->m_pPlayer = m_pPlayer;
		}
		else
		{
			m_pScene->SetBoolforward(false);
			getposition = m_pPlayer->GetPosition();
			CAirplaneMesh* pAirplaneMesh = new CAirplaneMesh(8.0f, 8.0f, 2.0f);
			m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
			m_pPlayer->SetMesh(pAirplaneMesh);
			m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
			m_pScene->m_pPlayer = m_pPlayer;
		}

	}
	
	float cxDelta = 0.0f, cyDelta = 0.0f;

	POINT ptCursorPos;

	//이게 뭐한걸까 ? 
	if (GetCapture() == m_hWnd) //m_hwnd핸들:iD capture 어떤 응용 프로그램.
	{
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);//나의 마우스의 좌표값을 넣어줘 

		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f; 
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}

	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
	{
		if (cxDelta || cyDelta) //[왜? ||이걸 했지]움직임이 감지 되었다면
		{
			//VK_RBUTTON 마우스 오른쪽 버튼 
			//플레이어의 회전값을 준다.

			if (pKeyBuffer[VK_RBUTTON] & 0xF0)
			{
				m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
			}

			else //여기가 VK_LBUTTON임 
			{
					m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);		

					XMFLOAT3 getposition;
					XMFLOAT3 NormalLook = Vector3::Normalize(m_pPlayer->GetLook());
					XMFLOAT3 Normalpos = Vector3::Normalize(m_pPlayer->GetPosition());
					XMFLOAT3 NormalRight = Vector3::Normalize(m_pPlayer->GetRight());

					XMFLOAT3 Lookposcross =  Vector3::CrossProduct(NormalLook, Normalpos);

					XMFLOAT3 Rightlookcross = Vector3::CrossProduct(NormalRight, NormalLook);

					float dotproduct = Vector3::DotProduct(NormalLook, Normalpos);
					float dotRightlook = Vector3::DotProduct(Normalpos, NormalRight);
					float angle = (dotproduct > 0.0f) ? -1.0f : 1.0f;
					float angle2 = (dotRightlook > 0.0f) ? -1.0f : 1.0f; 
					
					if (angle < 0.0f)
					{
						getposition = m_pPlayer->GetPosition();
						UPAirPlaneMesh* pAirplaneMesh = new UPAirPlaneMesh(8.0f, 8.0f, 2.0f);
					
						m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
						m_pPlayer->SetMesh(pAirplaneMesh);
						m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
						m_pScene->m_pPlayer = m_pPlayer;
						m_pPlayer->Move(dwDirection, 0.05f);

						if (angle2 < 0.0f)
						{
							getposition = m_pPlayer->GetPosition();
							RIGHTAirPlaneMesh* pAirplaneMesh = new RIGHTAirPlaneMesh(8.0f, 8.0f, 2.0f);

							m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
							m_pPlayer->SetMesh(pAirplaneMesh);
							m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
							m_pScene->m_pPlayer = m_pPlayer;
							m_pPlayer->Move(dwDirection, 0.3f);
						}

						if (angle2 > 0.0f)
						{
							getposition = m_pPlayer->GetPosition();
							LEFTAirPlaneMesh* pAirplaneMesh = new LEFTAirPlaneMesh(8.0f, 8.0f, 2.0f);

							m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
							m_pPlayer->SetMesh(pAirplaneMesh);
							m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
							m_pScene->m_pPlayer = m_pPlayer;
							m_pPlayer->Move(dwDirection, 0.3f);

						}
					}

					if (angle > 0.0f)
					{
						getposition = m_pPlayer->GetPosition();
						DOWNAirPlaneMesh* pAirplaneMesh = new DOWNAirPlaneMesh(8.0f, 8.0f, 2.0f);

						m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
						m_pPlayer->SetMesh(pAirplaneMesh);
						m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
						m_pScene->m_pPlayer = m_pPlayer;
						m_pPlayer->Move(dwDirection, 1.5f);

						if (angle2 < 0.0f)
						{
							getposition = m_pPlayer->GetPosition();
							RIGHTAirPlaneMesh* pAirplaneMesh = new RIGHTAirPlaneMesh(8.0f, 8.0f, 2.0f);

							m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
							m_pPlayer->SetMesh(pAirplaneMesh);
							m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
							m_pScene->m_pPlayer = m_pPlayer;
							m_pPlayer->Move(dwDirection, 0.3f);
						}

						if (angle2 > 0.0f)
						{
							getposition = m_pPlayer->GetPosition();
							LEFTAirPlaneMesh* pAirplaneMesh = new LEFTAirPlaneMesh(8.0f, 8.0f, 2.0f);

							m_pPlayer->SetPosition(getposition.x, getposition.y, getposition.z);
							m_pPlayer->SetMesh(pAirplaneMesh);
							m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));
							m_pScene->m_pPlayer = m_pPlayer;
							m_pPlayer->Move(dwDirection, 0.3f);

						}

					}
					
			}
		}

		//플레이어를 움직여주는 곳 플레이어 속도 조절 가능함
		if (dwDirection)
			m_pPlayer->Move(dwDirection, 0.3f);
	}
	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::FrameAdvance()
{    
    if (!m_bActive) return;

	m_GameTimer.Tick(0.0f);
	m_fTime += m_GameTimer.GetTimeElapsed();
	m_pScene->SetTime(m_fTime);

	ProcessInput();


	m_pScene->Animate(m_GameTimer.GetTimeElapsed());

	ClearFrameBuffer(RGB(255, 255, 255));

	m_pScene->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);
	m_pPlayer->Render(m_hDCFrameBuffer, m_pPlayer->m_pCamera);

	PresentFrameBuffer();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}


