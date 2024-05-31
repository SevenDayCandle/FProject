export module fbc.SettingsDialogPage;

import fbc.ConfigItem;
import fbc.FUtil;
import fbc.Hitbox;
import fbc.RelativeHitbox;
import fbc.UICanvas;
import fbc.UIDropdown;
import fbc.UINumberInput;
import fbc.UITextInput;
import fbc.UISlider;
import fbc.UIToggle;
import sdl;
import std;

export namespace fbc {
	export class SettingsDialogPage : public UICanvas {
		class SettingsDialogBaseCache {
		public:
			SettingsDialogBaseCache() {}
			virtual ~SettingsDialogBaseCache() {}

			func<void()> onReset;

			inline void setOnReset(func<void()> onReset) { this->onReset = onReset; }

			virtual void commit() = 0;
		};

		template<typename T> class SettingsDialogCache : public SettingsDialogBaseCache {
		public:
			SettingsDialogCache(ConfigItem<T>& conf) : conf(conf), value(conf.get()) {}
			virtual ~SettingsDialogCache() override {}

			ConfigItem<T>& conf;
			T value;

			inline void commit() override { conf.set(value); }
		};

	public:
		SettingsDialogPage(Hitbox* hb, strv name): UICanvas(hb), name(name) {}
		virtual ~SettingsDialogPage() {}

		const str name;

		friend std::ostream& operator<<(std::ostream& os, const SettingsDialogPage& obj) { return os << obj.name; }

		template <typename T, c_itr<T> Iterable> inline UIDropdown<T>& addDropdown(ConfigItem<T>& conf, strv name, Iterable& items, func<str(const T&)> labelFunc = [](const T& item) { return futil::toString(item); }) { return addDropdownImpl<T, Iterable>(conf, name, items, labelFunc); }
		template <typename T, c_itr<T*> Iterable> inline UIDropdown<T>& addDropdown(ConfigItem<T>& conf, strv name, Iterable& items, func<str(const T&)> labelFunc = [](const T& item) { return futil::toString(item); }) { return addDropdownImpl<T, Iterable>(conf, name, items, labelFunc); }
		template <typename T, typename U, c_itr<U> Iterable> inline UIDropdown<U>& addDropdownMapped(ConfigItem<T>& conf, strv name, Iterable& items, func<T(const U*)> convFunc, func<str(const T&)> labelFunc = [](const T& item) { return futil::toString(item); }) { return addDropdownMappedImpl<T, U, Iterable>(conf, name, items, convFunc, labelFunc); }
		template <typename T, typename U, c_itr<U*> Iterable> inline UIDropdown<U>& addDropdownMapped(ConfigItem<T>& conf, strv name, Iterable& items, func<T(const U*)> convFunc, func<str(const T&)> labelFunc = [](const T& item) { return futil::toString(item); }) { return addDropdownMappedImpl<T, U, Iterable>(conf, name, items, convFunc, labelFunc); }

		UINumberInput& addInputNum(ConfigItem<int>& conf, strv name);
		UITextInput& addInputText(ConfigItem<str>& conf, strv name);
		UISlider& addSlider(ConfigItem<int>& conf, strv name, int min = 0, int max = std::numeric_limits<int>::max());
		UIToggle& addToggle(ConfigItem<bool>& conf, strv name);
		void reset();
	private:
		vec<uptr<SettingsDialogBaseCache>> confs;

		template <typename T, typename Iterable> UIDropdown<T>& addDropdownImpl(ConfigItem<T>& conf, strv name, Iterable& items, func<str(const T&)> labelFunc = [](const T& item) { return futil::toString(item); });
		template <typename T, typename U, typename Iterable> UIDropdown<U>& addDropdownMappedImpl(ConfigItem<T>& conf, strv name, Iterable& items, func<T(const U*)> convFunc, func<str(const T&)> labelFunc = [](const T& item) { return futil::toString(item); });
	};


	UISlider& SettingsDialogPage::addSlider(ConfigItem<int>& conf, strv name, int min, int max)
	{
		SettingsDialogCache<int>& cache = static_cast<SettingsDialogCache<int>&>(*confs.emplace_back(std::make_unique<SettingsDialogCache<int>>(conf)));
		UISlider& slider = stackElementYDir(std::make_unique<UISlider>(new RelativeHitbox(*hb, 150, 100), min, max));
		slider
			.setValue(conf.get())
			.setOnComplete([&cache](int val) {cache.value = val; })
			.withLabel(name);
		cache.setOnReset([&slider, &conf]() {slider.setValue(conf.get()); });
		return slider;
	}

	UIToggle& SettingsDialogPage::addToggle(ConfigItem<bool>& conf, strv name)
	{
		SettingsDialogCache<bool>& cache = static_cast<SettingsDialogCache<bool>&>(*confs.emplace_back(std::make_unique<SettingsDialogCache<bool>>(conf)));
		UIToggle& toggle = stackElementYDir(std::make_unique<UIToggle>(new RelativeHitbox(*hb, 100, 100), name));
		toggle.setToggleState(conf.get())
			.setOnClick([&cache](const UIToggle& toggle) {cache.value = toggle.toggled; });
		cache.setOnReset([&toggle, &conf]() {toggle.toggle(conf.get()); });
		return toggle;
	}

	template<typename T, typename Iterable> UIDropdown<T>& SettingsDialogPage::addDropdownImpl(ConfigItem<T>& conf, strv name, Iterable& items, func<str(const T&)> labelFunc)
	{
		SettingsDialogCache<T>& cache = static_cast<SettingsDialogCache<T>&>(*confs.emplace_back(std::make_unique<SettingsDialogCache<T>>(conf)));
		UIDropdown<T>& dr = stackElementYDir(UIDropdown<T>::singleMenu(new RelativeHitbox(*hb, 200, 100), labelFunc));
		dr.setItems(items)
			.setOnChange([&cache](vec<const T*> res) { if (res.size() > 0) cache.value = *res[0]; })
			.withLabel(name);
		dr.updateSingle(conf.get());
		cache.setOnReset([&dr, &conf]() {dr.selectSingle(conf.get()); });
		return dr;
	}

	template<typename T, typename U, typename Iterable> UIDropdown<U>& SettingsDialogPage::addDropdownMappedImpl(ConfigItem<T>& conf, strv name, Iterable& items, func<T(const U*)> convFunc, func<str(const T&)> labelFunc)
	{
		SettingsDialogCache<T>& cache = static_cast<SettingsDialogCache<T>&>(*confs.emplace_back(std::make_unique<SettingsDialogCache<T>>(conf)));
		UIDropdown<U>& dr = stackElementYDir(UIDropdown<U>::singleMenu(new RelativeHitbox(*hb, 200, 100), labelFunc));
		dr.setItems(items)
			.setOnChange([&cache, convFunc](vec<const U*> res) { if (res.size() > 0) cache.value = convFunc(res[0]); })
			.withLabel(name);
		dr.updateSingle(conf.get());
		cache.setOnReset([&dr, &conf]() {dr.selectSingle(conf.get()); });
		return dr;
	}

	void SettingsDialogPage::reset()
	{
		for (const uptr<SettingsDialogBaseCache>& conf : confs) {
			conf->onReset();
		}
	}
}