#pragma once

class cObject
{
public:
	cObject();

protected:
	// >> :
	ULONG m_ulRefCount;
public:
	virtual ~cObject();
	virtual void AddRef();
	virtual void Release();
	// : <<

};

