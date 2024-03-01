export module fbc;

import fbc.coreContent;
import fbc.futil;
import fbc.screenManager;
import raylib;

float renderScalePrivate = 1;
constexpr float BASE_WIDTH = 1920;

export namespace fbc {
	const str ID = "fbc";
	core::CoreContent cct = core::CoreContent(ID);

	// TODO
	void dispose() {
		fbc::screenManager::dispose();
		cct.dispose();
	}

	// TODO
	void draw() {
		raylib::beginDrawing();
		raylib::clearBackground(raylib::White);


		raylib::endDrawing();
	}

	// Update the window from the config
	void refreshWindow() {
		raylib::setWindowSize(cct.cfg.graphicsResolutionX.get(), cct.cfg.graphicsResolutionY.get());
		if (raylib::isWindowFullscreen() != cct.cfg.graphicsIsFullScreen.get()) {
			raylib::toggleFullscreen();
		}
		renderScalePrivate = cct.cfg.graphicsResolutionX.get() / BASE_WIDTH;
	}

	// Update graphics settings from the config
	void refreshGraphics() {
		raylib::setTargetFPS(cct.cfg.graphicsFPS.get());
		refreshWindow();
	}

	// Return the current render scale for the window
	float renderScale() { return renderScalePrivate; }

	// TODO
	void initialize() {
		cct.initialize();

		raylib::initWindow(cct.cfg.graphicsResolutionX.get(), cct.cfg.graphicsResolutionY.get(), "Fabricate");
		refreshGraphics();
	}

	// TODO
	void update() {

	}
}