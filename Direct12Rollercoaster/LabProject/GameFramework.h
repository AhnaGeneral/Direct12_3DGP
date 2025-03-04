#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#include "Timer.h"
#include "Scene.h"
#include "Player.h"

class CGameFramework
{
public:
	CGameFramework(void);
	~CGameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();

	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE					m_hInstance = NULL;
	HWND						m_hWnd = NULL;
    bool						m_bActive = true;          
	CGameTimer					m_GameTimer;
	HDC							m_hDCFrameBuffer = NULL;
    HBITMAP						m_hBitmapFrameBuffer = NULL;
	CPlayer						*m_pPlayer = NULL;
	CAirplaneMesh				*m_pAirplaneMesh = NULL;
	CCubeMesh					*m_pCubeMesh = NULL;
	int							m_nScenes = 0;
	int							m_nScene = 0;
	CScene						*m_pScene = NULL;
	float						m_fTime = 0.f;
	bool                        m_boolforward = false;
	std::vector<XMFLOAT3>       VectorPosition;
	std::vector<float>          AnglePosx;
	std::vector<float>          AnglePosy;
	std::vector<float>          AnglePosz;



public:

	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();
	float GetTimer() {return m_fTime;}
	void  SetTimer(float ZeroTime) { m_fTime = ZeroTime; }
	void BuildObjects();
	void ReleaseObjects();
	void ProcessInput();
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	POINT						m_ptOldCursorPos;

	_TCHAR						m_pszFrameRate[50];
};

