export module fbc.Card;

import fbc.CardData;
import fbc.CombatSquare;
import fbc.Effect;
import fbc.FUtil;
import fbc.GameObject;
import fbc.PileType;
import sdl;
import std;

namespace fbc {
	export class Card : public GameObjectDerived<CardData> {
	public:
		Card(CardData& data) : GameObjectDerived<CardData>(data) {}

		int upgrades;
		opt<vec<uptr<Effect>>> effectOverride;

		inline int cost() const { return data.costBase + data.costUp * upgrades; };
		inline vec<uptr<Effect>>& getEffects() { return effectOverride ? effectOverride.value() : data.effects; }

		bool isPlayable();
		Card& setUpgrade(int upgrades);
		const PileType& getPileAfterUse();
		
		void use(GameObject* source, CombatSquare& square);
	};
}