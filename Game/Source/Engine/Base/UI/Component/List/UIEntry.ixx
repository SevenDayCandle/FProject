export module fbc.UIEntry;

import fbc.CoreConfig;
import fbc.CoreContent;
import fbc.FFont;
import fbc.TextInfo;
import fbc.IDrawable;
import fbc.UIInteractable;
import fbc.UILabel;
import fbc.FUtil;
import fbc.RelativeHitbox;
import fbc.ScreenManager;
import sdl;
import std;

namespace fbc {
	export template <typename T> class UIEntry : public UIInteractable, public TextInfo {
	public:
		UIEntry(const T& item, int index, const func<void(UIEntry<T>&)>& onClick, fbc::RelativeHitbox* hb, FFont& f, const str& text, IDrawable& image = cct.images.uiCheckboxEmpty, IDrawable& checkImage = cct.images.uiCheckboxFilled, sdl::Color baseColor = sdl::COLOR_WHITE, sdl::Color hoverColor = sdl::COLOR_WHITE) :
			item(item), index(index), onClick(onClick), baseColor(baseColor), hoverColor(hoverColor), UIInteractable(hb, image), TextInfo(f, text), checkImage(checkImage) {
		}

		operator const T*() const { return &item; }
		operator const T&() const { return item; }

		bool active = false;
		bool toggled = false;
		const T& item;
		int index;

		inline virtual float getProjectedWidth() { return image.getWidth() + cfg.renderScale(8) + TextInfo::getTextWidth(); };
		inline virtual void updateActiveStatus(bool val) { active = val; };
		inline virtual void updateSelectStatus(bool selected) { toggled = selected; };

		virtual void onHbHover();
		virtual void onHbUnhover();
		virtual void onSizeUpdated() override;
		virtual void refreshDimensions() override;
		virtual void renderImpl() override;
	protected:
		sdl::Color baseColor;
		sdl::Color hoverColor;
		IDrawable& checkImage;
		func<void(UIEntry<T>&)> onClick;

		inline virtual void clickLeftEvent() override { onClick(*this); }
	};

	template<typename T> void UIEntry<T>::onHbHover()
	{
		TextInfo::setColor(hoverColor);
	}

	template<typename T> void UIEntry<T>::onHbUnhover()
	{
		TextInfo::setColor(baseColor);
	}

	template<typename T> void UIEntry<T>::onSizeUpdated()
	{
		TextInfo::setPos(this->hb->h * 1.25f, 0);
	}

	template<typename T> void UIEntry<T>::refreshDimensions()
	{
		UIInteractable::refreshDimensions();
		refreshCache();
	}

	template<typename T> void UIEntry<T>::renderImpl() {

		sdl::RectF check = { this->hb->x, this->hb->y, this->hb->h, this->hb->h };


		if (active) {
			if (toggled) {
				// TODO combine into one draw call once we move to a different draw API
				checkImage.draw(&check, sdl::COLOR_WHITE);
				checkImage.draw(&check, sdl::BlendMode::SDL_BLENDMODE_ADD, sdl::COLOR_WHITE);
			}
			else {
				image.draw(&check, sdl::COLOR_WHITE);
				image.draw(&check, sdl::BlendMode::SDL_BLENDMODE_ADD, sdl::COLOR_WHITE);
			}
		}
		else {
			if (toggled) {
				checkImage.draw(&check, sdl::COLOR_WHITE);
			}
			else {
				image.draw(&check, sdl::COLOR_WHITE);
			}
		}

		TextInfo::drawText(check.x, check.y);
	}
}