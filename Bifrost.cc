#include "Bifrost.h"

Bifrost::Bifrost(const Param &api_key, const Param &username)
	: api_key(api_key), username(username), bridge(username, api_key) {
}

// Getters OUTSIDE Bifrost
wXY Bifrost::getWorldBound(const WorldSnapshot &ws) {
	Wc x = ws.get_rows();
	Wc y = ws.get_cols();
	return {x, y};
}
