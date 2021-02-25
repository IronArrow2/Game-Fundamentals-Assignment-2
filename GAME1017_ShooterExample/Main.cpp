#include "Engine.h"

int main(int argc, char* args[]) // Main MUST have these parameters for SDL.
{
	Engine *mainEngine = mainEngine->getInstance(); // Creates a single instance of Engine
	return mainEngine->Run();
}