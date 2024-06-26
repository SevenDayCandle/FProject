export module fbc.FRegion;

import fbc.FTexture;
import fbc.FUtil;
import fbc.IDrawable;
import sdl;

namespace fbc {
	export class FRegion : public IDrawable {
	public:
		FRegion(FTexture& base, sdl::RectF dim): base(base), dim(dim) {}

		inline const sdl::RectF* getBaseRec() const override { return &dim; }
		inline float getHeight() const override { return dim.h; }
		inline float getWidth() const override { return dim.w; }

		void drawBase(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Point& origin, float rotation, sdl::FlipMode flip) override;
		void setDrawBlend(const sdl::BlendMode bl) override;
		void setDrawColor(const sdl::Color& tint) override;
	private:
		FTexture& base;
		sdl::RectF dim;
	};

	void FRegion::drawBase(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Point& origin, float rotation, sdl::FlipMode flip)
	{
		base.draw(sourceRec, origin, rotation, flip);
	}

	void FRegion::setDrawBlend(const sdl::BlendMode bl)
	{
		base.setDrawBlend(bl);
	}

	void FRegion::setDrawColor(const sdl::Color& tint)
	{
		base.setDrawColor(tint);
	}
}