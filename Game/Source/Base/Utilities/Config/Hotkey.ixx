module;

#include "glaze/glaze.hpp"

export module fbc.configHotkey;

import fbc.config;
import fbc.futil;
import fbc.iserializable;
import sdl;
import std;

export namespace fbc {
	constexpr strv BASE_HOTKEY_FILE = "hotkeys.json";

	export class Hotkey {
	public:
		Hotkey(strv id, int key, int gamepad): ID(id), key(key), keyDefault(key), pad(gamepad), padDefault(gamepad) {}
		virtual ~Hotkey() {}

		const str ID;

		inline bool isKeyJustPressed() { return key > 0 && sdl::keyboardJustPressed(key); }
		inline int getKey() { return key; }
		inline int getPad() { return pad; }
		inline void setKey(int key) { this->key = key; }
		inline void setPad(int pad) { this->pad = pad; }

		void reset();
		void set(int key, int pad);

		static inline str configPath() { return sdl::dirPref(futil::FBC.data(), BASE_HOTKEY_FILE.data()); }

		static Hotkey& add(strv id, int key, int pad);
		static void commit();
		static void reload();
	private:
		int key;
		int keyDefault;
		int pad;
		int padDefault;

		static inline umap<str, Hotkey> keys = umap<str, Hotkey>();

	};

	// Reset keys and pad to defaults
	void Hotkey::reset()
	{
		key = keyDefault;
		pad = padDefault;
	}

	// Set key and pad
	void Hotkey::set(int key, int pad)
	{
		this->key = key;
		this->pad = pad;
	}

	// Register a hotkey to be tracked for saving and reloading
	Hotkey& Hotkey::add(strv id, int key, int pad)
	{
		auto [it, inserted] = keys.try_emplace(str(id), Hotkey(id, key, pad));
		if (!inserted) {
			throw std::logic_error("Duplicate Hotkey with ID: " + str(id));
		}
		return it->second;
	}

	// Save the contents of the hotkeys to an external file
	void Hotkey::commit()
	{
		map<str, pair<int, int>> output = futil::transformMap<str, Hotkey, pair<int, int>>(keys, [](const Hotkey& row) { return pair<int,int>(row.key, row.pad); });
		str configPath = Hotkey::configPath();
		glz::write_error error = glz::write_file_json(output, configPath, str{});
		if (error) {
			sdl::logError("Failed to save hotkeys at path %s", configPath.data());
		}
	}

	// Refresh the value map contents from the external file if it exists
	void Hotkey::reload()
	{
		map<str, pair<int, int>> input;
		str configPath = Hotkey::configPath();
		if (std::filesystem::exists(configPath)) {
			glz::parse_error error = glz::read_file_json(input, configPath, str{});
			if (error) {
				sdl::logError("Failed to read hotkeys at path %s", configPath.data());
				return;
			}

			// If a hotkey is in the map, set it to whatever values were read from the map. Otherwise, reset them to their defaults
			for (const auto& hk : keys) {
				Hotkey key = hk.second;
				auto entry = input.find(hk.first);
				if (entry != input.end()) {
					pair<int, int> found = entry->second;
					key.set(found.first, found.second);
				}
				else {
					key.reset();
				}
			}
		}
	}

}