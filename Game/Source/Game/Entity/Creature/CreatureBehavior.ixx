export module fbc.CreatureBehavior;

import fbc.FUtil;
import fbc.KeyedItem;
import std;

export namespace fbc {
	export class CreatureBehavior : KeyedItem<CreatureBehavior> {
	public:
		CreatureBehavior(strv key): KeyedItem<CreatureBehavior>(key) {}
		virtual ~CreatureBehavior() {}
	};
}