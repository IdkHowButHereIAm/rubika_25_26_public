#pragma once

// This is a forward declaration :)
// This is VERY useful and you must learn about it and use it as much as possible
class TextureMgr;

class Globals final
{
public:

	/*
	* Constructor & Destructor are here for a reason
	*/
	Globals();
	~Globals();

	/*
	* Init & Destroy of the class
	* They must be implemented in the right way to (de)initialize members
	*/
	void Init();
	void Destroy();
private:
	TextureMgr* TextureMgr;
};