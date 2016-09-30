#pragma once
#include "DecorateLayer.h"
#include <string>
#include <vector>

class CTextInfoDecorator :
	public CDecorateLayer
{
public:
	CTextInfoDecorator();
	virtual ~CTextInfoDecorator();

	int GetCount() { return (int)m_vecMessage.size(); }
	void AppendMessage(std::string& msg) { m_vecMessage.push_back(msg); }
	void UpdateMessage(int index, std::string& msg) { m_vecMessage[index] = msg; }
	void Clear() { m_vecMessage.clear(); }

protected:
	void OnDecorate(HDC hMem);

private:
	std::vector<std::string> m_vecMessage;
};

