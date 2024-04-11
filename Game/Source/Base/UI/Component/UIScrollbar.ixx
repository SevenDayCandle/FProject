export module fbc.uiScrollbar;

import fbc.hitbox;
import fbc.uiHoverable;
import fbc.futil;
import fbc.iDrawable;
import fbc.screenManager;
import sdl;
import std;

export namespace fbc {
	export class UIScrollbar : public UIHoverable {
	public:
		UIScrollbar(Hitbox* hb, IDrawable& imageBar, IDrawable& imageButton): UIHoverable(hb), imageBar(imageBar), imageButton(imageButton) {
		}
		virtual ~UIScrollbar() {}

		inline float getScroll() { return scrollPercent; }
		inline UIScrollbar& setOnScroll(const func<void(float)>& onScroll) { return this->onScroll = onScroll, *this; }

		virtual void renderImpl() override;
		virtual void updateImpl() override;

		void scroll(float percent);
		void setScrollPos(float percent);
		virtual float toPercentage(float x, float y) = 0;
		virtual void updateDropzonePos(float percent) = 0;
		virtual void updateDropzoneSize(float percent) = 0;
	protected:
		float actualRange;
		IDrawable& imageBar;
		IDrawable& imageButton;
		sdl::RectF dropzone;
	private:
		float clickPercent = -1;
		float scrollPercent;
		func<void(float)> onScroll;
	};

	void UIScrollbar::renderImpl() {
		imageBar.draw(hb.get(), { 0.0f, 0.0f }, 0, sdl::RendererFlip::SDL_FLIP_NONE);
		imageButton.draw(&dropzone, { 0.0f, 0.0f }, 0, sdl::RendererFlip::SDL_FLIP_NONE);
	}

	// Updates the scrollbar button position and triggers the scroll event at the new position
	void UIScrollbar::scroll(float percent)
	{
		setScrollPos(percent);
		onScroll(scrollPercent);
	}

	// Updates the position of the scrollbar button without actually triggering the scroll event
	void UIScrollbar::setScrollPos(float percent)
	{
		scrollPercent = std::clamp(percent, 0.0f, 1.0f);
		updateDropzonePos(percent);
	}

	void UIScrollbar::updateImpl() {
		UIHoverable::updateImpl();

		if (hb->isHovered() && sdl::mouseIsLeftJustClicked()) {
			screenManager::activeElement = this;
			float per = std::clamp(toPercentage(sdl::mouseGetX(), sdl::mouseGetY()), 0.0f, 1.0f);
			clickPercent = per;
			if (!sdl::mouseIsHovering(dropzone)) {
				scroll(per);
			}
		}
		else if (screenManager::activeElement == this) {
			if (sdl::mouseIsLeftClicked()) {
				float per = std::clamp(toPercentage(sdl::mouseGetX(), sdl::mouseGetY()), 0.0f, 1.0f);
				if (per != clickPercent) {
					scroll(scrollPercent + per - clickPercent);
					clickPercent = per;
				}
			}
			else {
				screenManager::activeElement = nullptr;
			}
		}
	}
}