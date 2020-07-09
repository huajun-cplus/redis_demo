#include "game_module.h"

#include "redis_client.h"


bool CGameMoudle::init()
{
	return true;
}

void CGameMoudle::release()
{
}

void CGameMoudle::run()
{
	static bool bExcute = true;

	if (!bExcute) {
		return;
	}

	// get k1
	RedisClientInst::singleton()->send(
		std::vector<std::string>({"GET", "k1"})
		, [](cpp_redis::reply &reply) {
			if (reply.ok()) {
				std::cout << "这里做业务 在主线程" << std::endl;
				std::cout << "get ok" << std::endl;
				if (reply.is_string()) {
					std::cout << "k1: "<< reply.as_string() << std::endl;
				}
			} else {
				std::cout << "get ko" << std::endl;
			}
		});

	RedisClientInst::singleton()->commit();

	bExcute = false;
}
