#pragma once


#include <lokimq/lokimq.h>


constexpr auto LOKINET_RPC_URL = "tcp://127.0.0.1:1190";

extern std::string RPCURL;

extern lokimq::LokiMQ lmq;
extern lokimq::ConnectionID lmq_conn;
