#include "ConfigManager.h"
#include "Presenter.h"
#include "InputManager.h"
#include "StateManager.h"
#undef main

InputManager iptm;

int main(int* argc, char* argv[]) {
	ConfigManager cfgm;
	Presenter pres;
	StateManager statMan;

	cfgm.init();
	pres.init("Connect 4 balls");
	iptm.init();
	statMan.init();

	while (statMan.m_running) {
		iptm.update();
		statMan.update();
		if (!statMan.m_running) { break; }
		statMan.draw();
		pres.draw();
	}

	statMan.destroy();
	iptm.destruct();
	pres.destruct();
	cfgm.destruct();
	return 0;
}