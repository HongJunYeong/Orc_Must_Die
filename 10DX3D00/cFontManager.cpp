#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}

LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	//찾았다면 그것을 넘겨줌.
	if (m_mapFont.find(e) != m_mapFont.end())
		return m_mapFont[e];


	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));

	if (e == E_DEFAULT)
	{
		fd.Height = 25;
		fd.Width = 12;
		fd.Weight == FW_BOLD;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		strcpy_s(fd.FaceName, "굴림체");
	}
	else if (e == E_QUEST)
	{
		fd.Height = 50;
		fd.Width = 25;
		fd.Weight == FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		AddFontResource("font/umberto.ttf");
		strcpy_s(fd.FaceName, "umberto");
	}
	else if (e == E_WOOD_DUG)
	{
		fd.Height = 25;
		fd.Width = 12;
		fd.Weight == FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		strcpy_s(fd.FaceName, "HY목각파임B");
	}
	else if (e == E_GODICK)
	{
		fd.Height = 25;
		fd.Width = 12;
		fd.Weight == FW_MEDIUM;
		fd.Italic = false;
		fd.CharSet = DEFAULT_CHARSET;
		fd.OutputPrecision = OUT_DEFAULT_PRECIS;
		fd.PitchAndFamily = FF_DONTCARE;
		strcpy_s(fd.FaceName, "HY견고딕");
	}

	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_mapFont[e]);
	return m_mapFont[e];
}



void cFontManager::Destroy()
{
	for each(auto it in m_mapFont)
	{
		SAFE_RELEASE(it.second);
	}

}