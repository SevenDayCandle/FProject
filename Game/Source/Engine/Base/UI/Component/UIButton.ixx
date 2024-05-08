export module fbc.uiButton;

import fbc.futil;
import fbc.iDrawable;
import fbc.hitbox;
import fbc.screenManager;
import fbc.uiInteractable;
import sdl;

export namespace fbc {
	export class UIButton : public UIInteractable {
	public:

		UIButton(Hitbox* hb, IDrawable& image) : UIInteractable(hb, image) {}
		UIButton(Hitbox* hb, IDrawable& image, sdl::Point origin) : UIInteractable(hb, image, origin) {}
		~UIButton() override {}

		inline UIButton& setOnClick(const func<void(UIButton&)>& onClick) { return this->onLeftClick = onClick, *this; }
		inline UIButton& setOnRightClick(const func<void(UIButton&)>& onRightClick) { return this->onRightClick = onRightClick, *this; }

		virtual void renderImpl() override;
	private:
		func<void(UIButton&)> onLeftClick;
		func<void(UIButton&)> onRightClick;

		virtual void clickLeftEvent() override;
		virtual void clickRightEvent() override;
	};

	void UIButton::renderImpl() {
		UIInteractable::renderImpl();
		if (hb->isHovered()) {
			image.draw(hb.get(), sdl::BlendMode::SDL_BLENDMODE_ADD, color, origin, rotation, flip);
		}
	}

	void UIButton::clickLeftEvent()
	{
		if (this->onLeftClick) {
			this->onLeftClick(*this);
		}
	}

	void UIButton::clickRightEvent()
	{
		if (this->onRightClick) {
			this->onRightClick(*this);
		}
	}
}
