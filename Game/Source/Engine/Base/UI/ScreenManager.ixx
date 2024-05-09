export module fbc.ScreenManager;

import fbc.FUtil;
import fbc.Hitbox;
import fbc.IOverlay;
import fbc.UIBase;
import sdl;
import std;

namespace fbc {
	fbc::deque<fbc::uptr<fbc::UIBase>> screens;
	fbc::deque<fbc::uptr<fbc::IOverlay>> overlays;
	fbc::IOverlay* queuedCloseOverlay;
	bool queuedCloseScreen;
}

export namespace fbc::screenManager {
	fbc::UIBase* activeElement;

	// Close the currently opened screen and reopen the last screen opened.
	// The screen's close method should save any screen state as necessary since the screen will get disposed of
	// Note that the screen will close at the END of the update cycle to ensure that all screen logic for the loop is executed
	void closeCurrentScreen() {
		queuedCloseScreen = true;
	}

	// Close a specific overlay and all overlays above it
	// Note that the overlay will close at the END of the update cycle to ensure that all overlay logic for the loop is executed
	void closeOverlay(fbc::IOverlay* target) {
		queuedCloseOverlay = target;
	}

	// Get the active overlay
	IOverlay* getActiveOverlay() {
		if (overlays.empty()) {
			return nullptr;
		}
		return overlays.front().get();
	}

	// Add an overlay to the top of the screen
	void openOverlay(uptr<fbc::IOverlay>&& target) {
		overlays.push_front(std::move(target));
	}

	// Add a screen to the history of opened screens and open that screen
	void openScreen(uptr<fbc::UIBase>&& screen) {
		screens.push_front(std::move(screen));
		screens.front()->open();
		if (!overlays.empty()) {
			closeOverlay(overlays.back().get());
		}
		activeElement = nullptr;
		sdl::keyboardInputStop();
	}

	// Render the last opened screen, as well as all overlays
	void render() {
		if (!screens.empty()) {
			screens.front()->renderImpl();
		}
		for (const uptr<IOverlay>& overlay : overlays) {
			overlay->render();
		}
	}

	// Close the current screen and switch the specified screen
	void swapScreen(uptr<fbc::UIBase>&& screen) {
		if (!screens.empty()) {
			fbc::UIBase& screenFront = *screens.front();
			screenFront.close();
		}
		screens.push_front(std::move(screen));
		screens.front()->open();
		if (!overlays.empty()) {
			closeOverlay(overlays.back().get());
		}
		activeElement = nullptr;
		sdl::keyboardInputStop();
	}

	// Update the last opened overlay if it exists, otherwise update the last opened screen
	void update() {
		if (!overlays.empty()) {
			overlays.front()->update();
		}
		else if (!screens.empty()) {
			screens.front()->updateImpl();
		}

		// Check if currently opened should close. If so, close all overlays and the screen, then reopen the last screen
		if (queuedCloseScreen) {
			if (!screens.empty()) {
				fbc::UIBase& screen = *screens.front();
				screen.close();
				screens.pop_front();
				for (const uptr<IOverlay>& overlay : overlays) {
					overlay->close();
				}
				overlays.clear();
				if (!screens.empty()) {
					screens.front()->open();
				}
			}
			activeElement = nullptr;
			sdl::keyboardInputStop();
			queuedCloseOverlay = nullptr;
			queuedCloseScreen = false;
		}
		// Otherwise check if there is an overlay to be closed
		else if (queuedCloseOverlay) {
			while (!overlays.empty()) {
				fbc::IOverlay* overlay = overlays.front().get();
				overlay->close();
				overlays.pop_front();
				if (overlay == queuedCloseOverlay) {
					break;
				}
			}
			queuedCloseOverlay = nullptr;
		}
	}
}