import fbc.CoreConfig;
import fbc.CoreContent;
import fbc.ConfigHotkey;
import fbc.ScreenManager;
import fbc.TitleScreen;
import sdl;
import std;

using namespace fbc;

void dispose() {
	// TODO save game state as necessary
	cct.dispose();
	sdl::quit();
}

void draw() {
	sdl::renderSetDrawColor(sdl::COLOR_BLACK);
	sdl::renderClear();

	screenManager::render();

	sdl::renderPresent();
}

void update() {
	screenManager::update();
}

bool initialize() {
	if (!sdl::initSDL()) {
		return false;
	}

	cfg.initialize();
	fbc::Hotkey::reload();
	cfg.postInitialize();
	cct.initialize();

	sdl::initWindow(cfg.graphicsResolutionX.get(), cfg.graphicsResolutionY.get(), cfg.graphicsWindowMode.get(), cfg.graphicsVSync.get());

	screenManager::openScreen(std::make_unique<fbc::TitleScreen>());
	return true;
}

int main()
{
	if (!initialize()) {
		return -1;
	}

	while(sdl::poll())
	{
		update();
		draw();
		
		// Only limit frame if vsync is off
		if (!cfg.graphicsVSync.get()) {
			sdl::capFrame(cfg.graphicsFPS.get());
		}
	}

	dispose();
	return 0;
}
