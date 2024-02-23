#include "EngineApp.h"

int main(int argc, char* argv[])
{
	Engine::init();
	Engine::mainLoop();
	Engine::terminate();
	return 0;
}