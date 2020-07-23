// redis_demo.cpp: 定义应用程序的入口点。
//
#ifdef _WIN32
#include <Winsock2.h>
#endif /* _WIN32 */

#include <iostream>
#include <thread>
#include <chrono>

#include "redis_client.h"
#include "game_module.h"

static const char * DEFAULT_REDIS_SERVER_IP = "127.0.0.1";
static const std::size_t DEFAULT_REDIS_PORT = 6379;
static const char * DEFAULT_REDIS_PASSWORD = "a1234567";

#ifdef _WIN32
bool initWinSock()
{
	//! Windows netword DLL init
	WORD version = MAKEWORD(2, 2);
	WSADATA data;

	if (WSAStartup(version, &data) != 0) {
		std::cerr << "WSAStartup() failure" << std::endl;
		return false;
	}

	return true;
}
#endif /* _WIN32 */

bool init() {
#ifdef _WIN32
	if (!initWinSock()) {
		return false;
	}
#endif /* _WIN32 */

	if (!RedisClientInst::singleton()->init(
		DEFAULT_REDIS_SERVER_IP
		, DEFAULT_REDIS_PORT
		, DEFAULT_REDIS_PASSWORD)) {
		return false;
	}
	std::cout << "redis connected" << std::endl;

	if (!GameMoudleInst::singleton()->init()) {
		return false;
	}

	return true;
}

void run() {
	while (true) {
		RedisClientInst::singleton()->run();
		GameMoudleInst::singleton()->run();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

int main(int argc, char **argv)
{
	if (!init()) {
		return -1;
	}

	run();

	return 0;
}
