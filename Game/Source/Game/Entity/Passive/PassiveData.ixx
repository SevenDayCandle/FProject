export module fbc.PassiveData;

import fbc.BaseContent;
import fbc.Effect;
import fbc.FUtil;
import fbc.GameObjectData;
import sdl;
import std;

export namespace fbc {
	export class PassiveData : public GameObjectDataDerived<PassiveData> {
	public:
		PassiveData(BaseContent& source, strv id) : GameObjectDataDerived(source, id) {}
		virtual ~PassiveData() {}

		int maxUpgrades = 0;
		vec<uptr<Effect>> effects;
	};
}