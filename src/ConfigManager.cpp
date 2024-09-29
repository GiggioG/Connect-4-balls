#include "ConfigManager.h"

#include <fstream>
#include <filesystem>
#include <string>

ConfigManager* ConfigManager::m_instance = nullptr;

ConfigManager::ConfigManager() {}
ConfigManager::~ConfigManager() {}

void ConfigManager::init() {
	try {
		for (std::filesystem::directory_entry const& entry : std::filesystem::directory_iterator(std::filesystem::absolute(CONFIG_FOLDER))) {
			enterConfigFile(entry.path().string());
		}
	}
	catch (std::filesystem::filesystem_error e) {
		std::cerr << e.what() << '\n';
	}
	m_instance = this;
}
void ConfigManager::destruct() {
	m_configList.clear();
}

ConfigItem& ConfigManager::cfg(string name) {
	if (m_configList.count(name) == 0) {
		cerr << "error: requested config item \"" << name << "\" does not exist\n";
		throw;
	}
	return m_configList[name];
}

void ConfigManager::enterConfigFile(string path) {
	ifstream fs(path);
	string name, sval;
	ConfigItem item;

	std::cout << "LOADING CONFIG FILE " << path << '\n';

	fs.peek();
	while (!fs.eof()) {
		fs >> item.type >> name;
		getline(fs, sval, '\n');
		sval = sval.substr(1);
		item.s = sval;
		if (item.type == 'i') { item.i = stoi(sval); }
		else if (item.type == 'f') { item.f = stof(sval); }
		m_configList.insert({ name, item });

		std::cout << "\t" << item.type << ' ' << name << "\t= ";
		if (item.type == 's') {
			std::cout << '\"' << item.s << "\"\n";
		} else {
			std::cout << sval << '\n';
		}
	}
	fs.close();
}	

