export module fbc.ILoadable;

import fbc.FUtil;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass; 
import sdl.SDLProps; 
import sdl.SDLRunner;
import std;

namespace fbc {
	export struct ILoadable {
	public:
		ILoadable() {}

		virtual ~ILoadable() = default;

		virtual void dispose() = 0;
		virtual void reload () const = 0;
	};
}