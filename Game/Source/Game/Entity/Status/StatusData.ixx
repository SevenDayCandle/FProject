export module fbc.StatusData;

import fbc.BaseContent;
import fbc.Effect;
import fbc.FUtil;
import fbc.GameObjectData;
import fbc.StatusTurnMode;
import std;

namespace fbc {
	export class StatusData : public GameObjectDataDerived<StatusData> {
	public:
		StatusData(BaseContent& source, strv id): GameObjectDataDerived(source, id) {}

		bool removable;
		bool debuff;
		bool stacksIncreaseTurns;
		int maxStacks = futil::INT_MAX;
		int maxTurns = futil::INT_MAX;
		int minStacks = 0;
		int priority = 0;
		StatusTurnMode turnMode = StatusTurnMode::NONE;
		vec<uptr<Effect>> effects;
	};
}