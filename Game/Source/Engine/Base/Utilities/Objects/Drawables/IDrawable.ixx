export module fbc.IDrawable;

import fbc.FUtil;
import fbc.ILoadable;
import sdl;

namespace fbc {
	export struct IDrawable : public ILoadable {
	public:
		IDrawable() {}
		virtual ~IDrawable() override = default;

		inline void draw(const sdl::RectF* destRec, const sdl::Point& origin = { 0,0 }, float rotation = 0, sdl::FlipMode flip = sdl::FlipMode::SDL_FLIP_NONE) { drawBase(getBaseRec(), destRec, origin, rotation, flip); }
		inline void draw(const sdl::RectF* destRec, const sdl::Color& tint, const sdl::Point& origin = { 0,0 }, float rotation = 0, sdl::FlipMode flip = sdl::FlipMode::SDL_FLIP_NONE) { draw(getBaseRec(), destRec, tint, origin, rotation, flip); }
		inline void draw(const sdl::RectF* destRec, const sdl::BlendMode blend, const sdl::Color& tint, const sdl::Point& origin = { 0,0 }, float rotation = 0, sdl::FlipMode flip = sdl::FlipMode::SDL_FLIP_NONE) { draw(getBaseRec(), destRec, blend, tint, origin, rotation, flip); }
		inline virtual void dispose() override {}
		inline virtual void reload() const override {}

		void draw(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Color& tint, const sdl::Point& origin = { 0,0 }, float rotation = 0, sdl::FlipMode flip = sdl::FlipMode::SDL_FLIP_NONE);
		void draw(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::BlendMode blend, const sdl::Color& tint, const sdl::Point& origin = { 0,0 }, float rotation = 0, sdl::FlipMode flip = sdl::FlipMode::SDL_FLIP_NONE);

		virtual void drawBase(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Point& origin, float rotation, sdl::FlipMode flip) = 0;
		virtual const sdl::RectF* getBaseRec() const = 0;
		virtual float getHeight() const = 0;
		virtual float getWidth() const = 0;
		virtual void setDrawBlend(const sdl::BlendMode bl) = 0;
		virtual void setDrawColor(const sdl::Color& tint) = 0;
	};

	void IDrawable::draw(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Color& tint, const sdl::Point& origin, float rotation, sdl::FlipMode flip)
	{
		setDrawColor(tint);
		drawBase(sourceRec, destRec, origin, rotation, flip);
	}

	void IDrawable::draw(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::BlendMode blend, const sdl::Color& tint, const sdl::Point& origin, float rotation, sdl::FlipMode flip)
	{
		setDrawBlend(blend);
		setDrawColor(tint);
		drawBase(sourceRec, destRec, origin, rotation, flip);
		setDrawBlend(sdl::BlendMode::SDL_BLENDMODE_BLEND);
	}
}