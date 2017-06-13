#pragma once

#define g_pObjectManager cObjectManager::GetInstance() //

class cObject;
class cSkinnedMesh;

class cObjectManager
{
private:
	SINGLETONE(cObjectManager);

	set<cObject*> m_setObject;
public:
	inline void AddSetObject(cObject* pObjcet)
	{
		m_setObject.insert(pObjcet);
	}

	inline void RemoveSetObject(cObject* pObject)
	{
		m_setObject.erase(pObject);
	}

	inline void Destroy()
	{
		m_setObject.clear();
	}

	
	

};

