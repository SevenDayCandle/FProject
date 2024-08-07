export module fbc.ContentManifest;

import fbc.FUtil;

namespace fbc {
	export struct ContentManifest {
		str id;
		str name;
		str description;
		str version;
		str fVersion;
		str credits;
		vec<str> dependencies;
	};
}