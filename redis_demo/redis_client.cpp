#include "redis_client.h"
#include <iostream>

CRedisClient::CRedisClient()
	: m_pread_deque(nullptr)
	, m_pwrite_deque(nullptr)
	, m_pcpp_redis_client(nullptr)
{
}

bool CRedisClient::init(
	const std::string & host
	, std::size_t port
	, const std::string &password
	, std::uint32_t timeout_ms
	, std::int32_t max_reconnects
	, std::uint32_t reconnect_interval_ms) 
{
	m_pcpp_redis_client = new(std::nothrow) cpp_redis::client();
	if (nullptr == m_pcpp_redis_client) {
		return false;
	}

	m_pcpp_redis_client->connect(
		host
		, port
		, nullptr
		, timeout_ms
		, max_reconnects
		, reconnect_interval_ms);

	m_pread_deque = &m_a_deque;
	m_pwrite_deque = &m_b_deque;

	if (!this->isConnected()) {
		return false;
	}

	if (!password.empty()) {
		if (!this->verifyPassword(password)) {
			return false;
		}

	}

	return this->isConnected();
}

void CRedisClient::release()
{
	if (this->isConnected()) {
		this->disConnect();
	}

	if (this->isReconnecting()) {
		this->cancelReconnect();
	}

	delete m_pcpp_redis_client;
	m_pcpp_redis_client = nullptr;
}

void CRedisClient::run()
{
	while (!m_pread_deque->empty()) {
		auto &context = m_pread_deque->front();
		context.call_back_fun(context.reply);
		m_pread_deque->pop_front();
	}

	this->swapDeque();
}

bool CRedisClient::isConnected() const 
{
	return m_pcpp_redis_client->is_connected();
}

bool CRedisClient::isReconnecting() const
{
	return m_pcpp_redis_client->is_reconnecting();
}

void CRedisClient::disConnect(bool wait_for_removal)
{
	m_pcpp_redis_client->disconnect(wait_for_removal);
}

void CRedisClient::cancelReconnect()
{
	m_pcpp_redis_client->cancel_reconnect();
}

std::future<cpp_redis::reply> CRedisClient::send(const std::vector<std::string>& redis_cmd)
{
	try
	{
		return m_pcpp_redis_client->send(
			redis_cmd);
	}
	catch (const std::exception &e)
	{
		std::cerr << "catch redis clien throw exception: " << e.what() << std::endl;
	}

	return {};
}

CRedisClient & CRedisClient::send(const std::vector<std::string>& redis_cmd, const cpp_redis::reply_callback_t & callback)
{
	try
	{
		m_pcpp_redis_client->send(
			redis_cmd
			, [callback, this](cpp_redis::reply &reply) {
				// ¼ÓËø
				std::scoped_lock(m_mutex_for_deque);
				m_pwrite_deque->push_back({ callback, reply });
			});
	}
	catch (const std::exception &e)
	{
		std::cerr << "catch redis clien throw exception: " << e.what() << std::endl;
	}

	return *this;
}

CRedisClient & CRedisClient::commit()
{
	m_pcpp_redis_client->commit();

	return *this;
}

CRedisClient & CRedisClient::sync_commit()
{
	m_pcpp_redis_client->sync_commit();

	return *this;
}

void CRedisClient::swapDeque()
{	
	// ¼ÓËø
	std::scoped_lock(m_mutex_for_deque);

	if (m_pread_deque == &m_a_deque) {
		m_pread_deque = &m_b_deque;
		m_pwrite_deque = &m_a_deque;
	} else {
		m_pread_deque = &m_a_deque;
		m_pwrite_deque = &m_b_deque;
	}
}

bool CRedisClient::verifyPassword(const std::string & strPassword)
{
	auto future = this->send({ "AUTH", strPassword });
	
	this->sync_commit();

	return future.get().ok();
}
