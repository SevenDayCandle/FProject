export module fbc.UINumberInput;

import fbc.CoreConfig;
import fbc.CoreContent;
import fbc.FFont;
import fbc.FWindow;
import fbc.FUtil;
import fbc.Hitbox;
import fbc.IDrawable;
import fbc.TextSupplier;
import fbc.TextInfo;
import fbc.UIButton;
import fbc.UIInteractable;
import fbc.UITitledInteractable;
import sdl;
import std;

namespace fbc {
	export class UINumberInput : public UITitledInteractable, public TextInfo, public TextSupplier {
	public:
		UINumberInput(FWindow& window, Hitbox* hb,
			int limMin = 0,
			int limMax = std::numeric_limits<int>::max(),
			IDrawable& image = cct.images.uiPanel,
			IDrawable& arrow = cct.images.uiArrowIncrement,
			FFont& textFont = cct.fontRegular()) : TextSupplier(window), UITitledInteractable(window, hb, image), TextInfo(textFont), limMin(limMin), limMax(limMax), arrow(arrow) {
			initCaret(this->font, this->hb->x, this->hb->y);
			UINumberInput::onSizeUpdated();
		}

		inline int getValue() const { return val; }
		inline UINumberInput& setOnBufferUpdate(const func<void(int)>& onBufferUpdateCallback) { return this->onBufferUpdateCallback = onBufferUpdateCallback, *this; }
		inline UINumberInput& setOnComplete(const func<void(int)>& onComplete) { return this->onComplete = onComplete, *this; }

		void commit(int num);
		virtual UINumberInput& setLimits(int limMin, int limMax);
		virtual UINumberInput& setValue(int num);
		virtual void onSizeUpdated() override;
		virtual void refreshDimensions() override;
		virtual void renderImpl(sdl::SDLBatchRenderPass& rp) override;
		virtual void start() override;
		virtual void updateImpl() override;
	protected:
		int limMax = std::numeric_limits<int>::max();
		int limMin = 0;

		inline void doOnComplete(int val) { if (onComplete) onComplete(val); }
		inline virtual int getLimitWidth() override { return this->hb->w; }

		virtual void commitInternal();
		void clickLeftEvent() override;
		void onBufferUpdated() override;
		void onKeyPress(int32 c) override;
		void onTextInput(const char* text) override;
		void resetBuffer() override;
		void updateCaretPos() override;
	private:
		func<void(int)> onBufferUpdateCallback;
		func<void(int)> onComplete;
		IDrawable& arrow;
		sdl::RectF lessRect;
		sdl::RectF moreRect;
		int interval = 1;
		int val = 0;
		int valTemp = 0;

		void modifyDuringInput(int val);
	};

	// Commits the number restrained to the limits and invokes the completion callback
	void UINumberInput::commit(int num)
	{
		setValue(num);
		doOnComplete(num);
	}

	// Sets the limits to constrain inputted values
	UINumberInput& UINumberInput::setLimits(int limMin, int limMax)
	{
		this->limMin = limMin;
		this->limMax = limMax;
		if (val < limMin || val > limMax) {
			setValue(val);
		}
		return *this;
	}

	void UINumberInput::onSizeUpdated()
	{
		TextInfo::setPos(cfg.renderScale(24), this->hb->h * 0.25f);
		moreRect.w = moreRect.h = lessRect.w = lessRect.h = hb->h / 2;
	}

	void UINumberInput::refreshDimensions()
	{
		UITitledInteractable::refreshDimensions();
		refreshCache();
	}

	void UINumberInput::renderImpl(sdl::SDLBatchRenderPass& rp)
	{
		UITitledInteractable::renderImpl(rp);
		TextInfo::drawText(rp, hb->x, hb->y, win.getW(), win.getH());
		arrow.draw(rp, lessRect, win.getW(), win.getH());
		arrow.draw(rp, moreRect, win.getW(), win.getH());
		if (sdl::runner::keyboardInputActive(this)) {
			renderCaret(rp);
		}
	}

	// Sets the number restrained to the limits WITHOUT invoking the completion callback
	UINumberInput& UINumberInput::setValue(int num)
	{
		valTemp = val = std::clamp(num, limMin, limMax);
		setText(std::to_string(num));
		return *this;
	}

	void UINumberInput::start() {
		buffer = getText();
		TextSupplier::start();
		this->updateCache(buffer, sdl::COLOR_LIME);
	}

	// When clicking outside of the text input, commit whatever is in the input
	void UINumberInput::updateImpl()
	{
		UIInteractable::updateImpl();
		moreRect.x = lessRect.x = hb->x + hb->w - moreRect.w;
		moreRect.y = hb->y;
		lessRect.y = hb->y + moreRect.h;
		if (sdl::runner::keyboardInputActive(this) && sdl::runner::mouseIsLeftJustClicked() && !isHovered()) {
			commitInternal();
			releaseBuffer();
		}
	}

	// When clicked while not focused, start the text input
	void UINumberInput::clickLeftEvent()
	{
		if (!sdl::runner::keyboardInputActive()) {
			if (sdl::runner::mouseIsHovering(lessRect)) {
				commit(val - interval);
			}
			else if (sdl::runner::mouseIsHovering(moreRect)) {
				commit(val + interval);
			}
			else {
				start();
			}
		}
		else if (sdl::runner::mouseIsHovering(lessRect)) {
			modifyDuringInput(valTemp - interval);
		}
		else if (sdl::runner::mouseIsHovering(moreRect)) {
			modifyDuringInput(valTemp + interval);
		}
	}

	// Updates the display text and invokes the buffer change callback
	void UINumberInput::onBufferUpdated()
	{
		try {
			if (buffer.empty()) {
				valTemp = std::max(0, limMin);
			}
			else {
				valTemp = std::stoi(buffer);
				valTemp = std::clamp(valTemp, limMin, limMax);
			}
			this->updateCache(buffer, sdl::COLOR_LIME);
			if (onBufferUpdateCallback) {
				onBufferUpdateCallback(valTemp);
			}
		}
		catch (exception e) {
			sdl::logError("UINumberInput failed to parse: %s", e.what());
		}
	}

	/* Enter will commit the current text
		Esc will revert any changes
		Arrow up increments by interval
		Arrow down decrements by interval
	*/
	void UINumberInput::onKeyPress(int32 c)
	{
		switch (c) {
		case sdl::KEY_ENTER:
		case sdl::KEY_RETURN:
			commitInternal();
			releaseBuffer();
			return;
		case sdl::KEY_ESC:
			this->refreshCache();
			releaseBuffer();
			return;
		case sdl::KEY_DOWN:
			modifyDuringInput(valTemp - interval);
			return;
		case sdl::KEY_UP:
			modifyDuringInput(valTemp + interval);
			return;
		default:
			TextSupplier::onKeyPress(c);
		}
	}

	// Typing only adds numeric characters to the buffer
	void UINumberInput::onTextInput(const char* text)
	{
		if (bufferPos <= buffer.size() && futil::isNumeric(text)) {
			buffer.insert(bufferPos, text);
			bufferPos += std::strlen(text);
			updateCaretPos();
			onBufferUpdated();
		}
	}

	void UINumberInput::resetBuffer()
	{
		this->refreshCache();
		buffer = getText();
	}

	void UINumberInput::updateCaretPos()
	{
		caretPosX = this->hb->x + cfg.renderScale(9) + this->font.measureW(buffer.substr(0, bufferPos));
		caretPosY = this->hb->y;
	}

	void UINumberInput::commitInternal()
	{
		val = valTemp;
		setText(buffer);
		if (onComplete) {
			onComplete(val);
		}
	}

	void UINumberInput::modifyDuringInput(int val)
	{
		valTemp = std::clamp(val, limMin, limMax);
		buffer = std::to_string(valTemp);
		this->updateCache(buffer, sdl::COLOR_LIME);
		if (onBufferUpdateCallback) {
			onBufferUpdateCallback(valTemp);
		}
	}
}