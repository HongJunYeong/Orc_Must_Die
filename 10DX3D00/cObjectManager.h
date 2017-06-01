#pragma once

#define g_pObjectManager cObjectManager::GetInstance() //

class cObject;

class cObjectManager
{
private:
	SINGLETONE(cObjectManager);

	set<cObject*> m_setObject;

public:
	inline void AddObject(cObject* pObjcet)
	{
		m_setObject.insert(pObjcet);
	}

	inline void RemoveObject(cObject* pObject)
	{
		m_setObject.erase(pObject);
	}

	inline void Destroy()
	{
		m_setObject.clear();
	}

};

