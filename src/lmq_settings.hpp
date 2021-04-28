#pragma once


#include <oxenmq/oxenmq.h>


constexpr auto LOKINET_RPC_URL = "tcp://127.0.0.1:1190";

extern std::string RPCURL;

extern oxenmq::OxenMQ lmq;
extern std::optional<oxenmq::ConnectionID> lmq_conn;
