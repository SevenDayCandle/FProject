export module fbc.RBordered;

import fbc.FUtil;
import fbc.IDrawable;
import sdl;
import std;

namespace fbc {
	export class RBordered : public IDrawable {
	public:
		RBordered(IDrawable& base) : base(base), patchSize(base.getWidth() / 2.0f) {}
		RBordered(IDrawable& base, float patchSize): base(base), patchSize(patchSize) {}

		inline const sdl::RectF* getBaseRec() const override { return base.getBaseRec(); }
		inline float getHeight() const override { return base.getHeight();}
		inline float getWidth() const override { return base.getWidth(); }

		void drawBase(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Point& origin, float rotation, sdl::FlipMode flip) override;
		void reload() const override;
		void setDrawBlend(const sdl::BlendMode bl) override;
		void setDrawColor(const sdl::Color& tint) override;
	private:
		IDrawable& base;
		mutable float patchSize;
	};

	/* Assuming that the underlying base drawable consists of four seamless corners of size patchSize x patchSize, we infer the following:
		- The top edge (st) is the left-most column of the top-right corner
		- The left edge (scl) is the top-most row of the bottom-left corner
		- The right edge (scr) is the top-most row of the bottom-right corner
		- The bottom edge (sb) is the left-most column of the bottom-right corner
		- The center (sc) is the top-left pixel of the bottom-right corner
	*/
	void RBordered::drawBase(const sdl::RectF* sourceRec, const sdl::RectF* destRec, const sdl::Point& origin, float rotation, sdl::FlipMode flip) {
		float px = sourceRec->x + patchSize;
		float py = sourceRec->y + patchSize;

		sdl::RectF stl = { sourceRec->x,  sourceRec->y, patchSize, patchSize };
		sdl::RectF st = { px, sourceRec->y, 1, patchSize };
		sdl::RectF str = { px,  sourceRec->y, patchSize, patchSize };
		sdl::RectF scl = { sourceRec->x, py, patchSize, 1 };
		sdl::RectF sc = { px, py, 1,1 };
		sdl::RectF scr = { px, py, patchSize, 1 };
		sdl::RectF sbl = { sourceRec->x, py, patchSize, patchSize };
		sdl::RectF sb = { px, py, 1, patchSize };
		sdl::RectF sbr = { px, py, patchSize, patchSize };


		sdl::RectF dc = {destRec->x + patchSize, destRec->y + patchSize, destRec->w - (patchSize * 2), destRec->h - (patchSize * 2)};
		float right = dc.x + dc.w;
		float bottom = dc.y + dc.h;

		sdl::RectF dtl = { destRec->x,  destRec->y, patchSize, patchSize };
		sdl::RectF dt = { dc.x,  destRec->y, dc.w, patchSize };
		sdl::RectF dtr = { right,  destRec->y, patchSize, patchSize };
		sdl::RectF dcl = { destRec->x, dc.y, patchSize, dc.h };
		sdl::RectF dcr = { right, dc.y, patchSize, dc.h };
		sdl::RectF dbl = { destRec->x, bottom, patchSize, patchSize };
		sdl::RectF db = { dc.x, bottom, dc.w, patchSize };
		sdl::RectF dbr = { right, bottom, patchSize, patchSize };

		base.drawBase(&stl, &dtl, origin, rotation, flip);
		base.drawBase(&st, &dt, origin, rotation, flip);
		base.drawBase(&str, &dtr, origin, rotation, flip);
		base.drawBase(&scl, &dcl, origin, rotation, flip);
		base.drawBase(&sc, &dc, origin, rotation, flip);
		base.drawBase(&scr, &dcr, origin, rotation, flip);
		base.drawBase(&sbl, &dbl, origin, rotation, flip);
		base.drawBase(&sb, &db, origin, rotation, flip);
		base.drawBase(&sbr, &dbr, origin, rotation, flip);
	}

	// If the patch size was not manually set, automatically adjust it based on the source texture
	// Note that this working relies on the original texture being reloaded before this; this should normally be enforced when creating drawables in StaticLoadables through its helper methods
	void RBordered::reload() const
	{
		if (patchSize <= 0) {
			patchSize = base.getWidth() / 2.0f;
		}
	}

	void RBordered::setDrawBlend(const sdl::BlendMode bl)
	{
		base.setDrawBlend(bl);
	}

	void RBordered::setDrawColor(const sdl::Color& tint)
	{
		base.setDrawColor(tint);
	}
}
