#pragma once

#include "GameObject.h"
#include "Player.h"
#include <iostream>
#include <deque>


class CScene
{
public:
	CScene();
	virtual ~CScene();

	CPlayer						*m_pPlayer = NULL;
	int							m_nObjects = 0;
	CGameObject					**m_ppObjects = NULL;
	float                       m_fTimeCS;
	float                       m_fpitch;
	float                       m_fYaw;
	float                       m_fRoll;

	std::deque<CGameObject*> m_Raildeque;


	void  SetTime(float Time) { m_fTimeCS = Time; }

	bool  m_boolforward = false;
	void  SetBoolforward(bool boolforward) { m_boolforward = boolforward; }
	bool  GetBoolforward() { return m_boolforward; }

	
	void  SetRotate(float fPitch, float fYaw, float fRoll)
	{
		m_fpitch = fPitch;
		m_fYaw = fYaw; 
		m_fRoll = fRoll;
	}



	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void CheckObjectByObjectCollisions();
		
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

class CPlayerScene : public CScene
{
public:
	CPlayerScene();
	virtual ~CPlayerScene();

	virtual void BuildObjects();
};

class CCollisionScene : public CScene
{
public:
	CCollisionScene();
	virtual ~CCollisionScene();

	CWallsObject				*m_pWallsObject = NULL;

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByWallCollisions();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

class CExplosionScene : public CCollisionScene
{
public:
	CExplosionScene();
	virtual ~CExplosionScene();

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

class CRunAwayScene : public CScene
{
public:
	CRunAwayScene();
	virtual ~CRunAwayScene();



	CTerrainObject				*m_pTerrainObject = NULL;

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	
};
