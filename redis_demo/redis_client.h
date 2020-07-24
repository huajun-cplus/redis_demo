#pragma once
#include <deque>
#include <mutex>

#include "toolkit/singleton_template.h"
#include "cpp_redis/cpp_redis"


class CRedisClient final {
	struct CRedisActionContext {
		cpp_redis::reply_callback_t call_back_fun;
		cpp_redis::reply			reply;
	};

public:
	CRedisClient();

	bool init(
		const std::string &host = "127.0.0.1",
		std::size_t port = 6379,
		const std::string &password = "",
		std::uint32_t timeout_ms = 0,
		std::int32_t max_reconnects = 0,
		std::uint32_t reconnect_interval_ms = 0);
	void release();

	void run();

	bool isConnected() const;
	bool isReconnecting() const;

	void disConnect(bool wait_for_removal = false);
	void cancelReconnect();

public:
	std::future<cpp_redis::reply> send(const std::vector<std::string> &redis_cmd);
	CRedisClient & send(const std::vector<std::string> &redis_cmd, const cpp_redis::reply_callback_t &callback);

	CRedisClient & commit();
	CRedisClient & sync_commit();

	template<class Rep, class Period>
	CRedisClient & sync_commit(const std::chrono::duration<Rep, Period> &timeout) {
		m_pcpp_redis_client->sync_commit(timeout);
		return *this;
	}

private:
	void swapDeque();
	bool verifyPassword(const std::string &strPassword);

private:
	std::mutex						m_mutex_for_deque;
	std::deque<CRedisActionContext> m_a_deque;
	std::deque<CRedisActionContext> m_b_deque;
	decltype(m_a_deque) *			m_pread_deque;
	decltype(m_a_deque) *			m_pwrite_deque;
	cpp_redis::client *				m_pcpp_redis_client;
};

template class ChaSingleton<CRedisClient>;
typedef ChaSingleton<CRedisClient> RedisClientInst;
