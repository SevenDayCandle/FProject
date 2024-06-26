export module fbc.KeyedItem;

import std;

/*
 * A struct intended to mimic Java enums that can be expanded.
 * Every instance of a derivative class of KeyedItem must have a unique id
 */
namespace fbc {
	export template <typename C> struct KeyedItem {
	public:
		KeyedItem(const KeyedItem&) = delete;
		virtual ~KeyedItem() = default;

		const std::string_view id;

		bool operator<(const KeyedItem<C>& other) const { return this->id < other.id; }
		bool operator>(const KeyedItem<C>& other) const { return this->id > other.id; }
		bool operator==(const KeyedItem<C>& other) const { return this->id == other.id; }
		std::filesystem::path operator/(const std::filesystem::path& other) const {return std::filesystem::path(id) / other;}
		friend std::ostream& operator<<(std::ostream& os, const KeyedItem& obj) { return os << obj.id; }
		operator std::string_view() const { return id; }

		static auto all();
		static std::vector<C*> allSorted();
		static C& get(std::string_view name);
		static C* optGet(std::string_view name);
	protected:
		constexpr KeyedItem(std::string_view name) : id(name) {
			auto& values = registered();
			if (!values.emplace(this->id, static_cast<C*>(this)).second) {
				throw std::logic_error("Duplicate KeyedItem");
			}
		}
		static constexpr std::unordered_map<std::string_view, C*>& registered() {
			static std::unordered_map<std::string_view, C*> values;
			return values;
		}
	};

	// Get every single instantiation of this class
	template<typename C> auto KeyedItem<C>::all()
	{
		return registered() | std::views::values;
	}

	// Get every single instantiation of this class as a list
	template<typename C> std::vector<C*> KeyedItem<C>::allSorted()
	{
		const auto& values = registered();
		std::vector<C*> result;
		for (const auto& pair : values) {
			result.emplace_back(static_cast<C*>(pair.second));
		}
		std::ranges::sort(result, std::less<C*>());
		return result;
	}

	// Get a particular instantation of this class matching the given id
	template<typename C> C& KeyedItem<C>::get(std::string_view name)
	{
		auto& values = registered();
		auto it = values.find(name);
		if (it == values.end()) {
			throw std::out_of_range("No KeyedItem with id: " + std::string(name));
		}
		return *(it->second);
	}

	// Get a particular instantation of this class matching the given id. Does not throw on a missing key
	template<typename C> C* KeyedItem<C>::optGet(std::string_view name)
	{
		auto& values = registered();
		auto it = values.find(name);
		if (it == values.end()) {
			return nullptr;
		}
		return it->second;
	}
}