export module fbc.ConfigNumeric;

import fbc.Config;
import fbc.ConfigItem;
import fbc.FUtil;
import sdl;
import std;

export namespace fbc {
	export class ConfigNumeric : public ConfigItem<int> {
	public:
		ConfigNumeric(fbc::Config& config, strv ID, const int& defaultValue, int min = 0, int max = std::numeric_limits<int>::max()): ConfigItem<int>(config, ID, defaultValue), min(min), max(max) {}
		virtual ~ConfigNumeric() {}

		inline int getMax() const { return max; }
		inline int getMin() const { return min; }
	protected:
		void assignValue(const int& res) override;
	private:
		int max;
		int min;
	};

	void ConfigNumeric::assignValue(const int& res) {
		ConfigItem<int>::assignValue(std::clamp(res, min, max));
	}
}