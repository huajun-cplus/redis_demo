#pragma once
#include "toolkit/singleton_template.h"

class CGameMoudle final {
public:
	bool init();
	void release();

	void run();
};

template ChaSingleton<CGameMoudle>;
typedef ChaSingleton<CGameMoudle> GameMoudleInst;
