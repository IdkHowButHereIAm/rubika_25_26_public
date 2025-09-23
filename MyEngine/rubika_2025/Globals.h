#pragma once

// This is a forward declaration :)
// This is VERY useful and you must learn about it and use it as much as possible
class TextureMgr;

class Globals final
{
public:
	static Globals& Instance();
	void Init();
	void Destroy();
	TextureMgr* TextureMgr;
	
private:

	static inline Globals* m_instance = nullptr;
	Globals();
	~Globals();

};