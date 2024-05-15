export module fbc.HorizontalDrawable;

import fbc.FTexture;
import fbc.FUtil;
import fbc.IDrawable;
import sdl;

export namespace fbc {
	export class HorizontalDrawable : public IDrawable {
	public:
		HorizontalDrawable(IDrawable& base) : base(base), patchSize(base.getHeight()) {}
		HorizontalDrawable(IDrawable& base, float patchSize) : base(base), patchSize(patchSize) {}

		inline sdl::RectF* getBaseRec() override { return base.getBaseRec(); }
		inline float getHeight() override { return base.getHeight(); }
		inline float getWidth() override { return base.getWidth(); }

		void drawBase(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Point& origin, float rotation, sdl::FlipMode flip) override;
		void setDrawBlend(const sdl::BlendMode bl) override;
		void setDrawColor(const sdl::Color& tint) override;
	private:
		IDrawable& base;
		float patchSize;
	};

	void HorizontalDrawable::drawBase(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Point& origin, float rotation, sdl::FlipMode flip)
	{
		float p1x = sourceRec->x + patchSize;
		float p2x = p1x + patchSize;
		float p1y = sourceRec->y + patchSize;
		float p2y = p1y + patchSize;

		sdl::RectF scl = { sourceRec->x, p1y, patchSize, patchSize };
		sdl::RectF sc = { p1x, p1y, patchSize,patchSize };
		sdl::RectF scr = { p2x, p1y, patchSize, patchSize };

		sdl::RectF dc = { destRec->x + patchSize, destRec->y, destRec->w - (patchSize * 2), destRec->h };
		float right = dc.x + dc.w;

		sdl::RectF dcl = { destRec->x, dc.y, patchSize, dc.h };
		sdl::RectF dcr = { right, dc.y, patchSize, dc.h };

		base.drawBase(&scl, &dcl, origin, rotation, flip);
		base.drawBase(&sc, &dc, origin, rotation, flip);
		base.drawBase(&scr, &dcr, origin, rotation, flip);
	}

	void HorizontalDrawable::setDrawBlend(const sdl::BlendMode bl)
	{
		base.setDrawBlend(bl);
	}

	void HorizontalDrawable::setDrawColor(const sdl::Color& tint)
	{
		base.setDrawColor(tint);
	}
}
