#pragma once

#include <unordered_map>
#include <filesystem>
#include "util.h"

struct ConfigItem {
	char type = '\0';
	int i = 0;
	string s = "";
	float f = 0.0f;
};


class ConfigManager {
public:
	ConfigManager();
	~ConfigManager();
	static ConfigManager* m_instance;

	void init();
	void destruct();
	ConfigItem& cfg(string name);
private:
	unordered_map<string, ConfigItem> m_configList;
	void enterConfigFile(string path);
};

namespace {
	ConfigItem& cfg(string name) {
		return ConfigManager::m_instance->cfg(name);
	}
}