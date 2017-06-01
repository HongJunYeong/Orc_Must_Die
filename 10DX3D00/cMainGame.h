#pragma once

// >> : 
class cCamera; 
class cGrid; 
// << :

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update(); 
	void Render(); 
	
private : 
	cCamera*			m_pCamera; 
	cGrid*				m_pGrid; 

	LPD3DXSPRITE		m_pSprite;

	LPDIRECT3DTEXTURE9 m_cursor;
	LPDIRECT3DSURFACE9 m_surf;
public :
	void WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

