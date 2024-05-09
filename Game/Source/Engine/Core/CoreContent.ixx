export module fbc.CoreContent;

import fbc.BaseContent;
import fbc.CoreConfig;
import fbc.CoreImages;
import fbc.CoreStrings;
import fbc.FFont;
import fbc.FUtil;
import sdl;
import std;

export namespace fbc {
	const str BASE_FOLDER = "/Resources";

	export class CoreContent : public fbc::BaseContent {
	public:
		CoreContent(strv ID) : fbc::BaseContent(ID, sdl::dirBase() + BASE_FOLDER) {}

		CoreImages images = CoreImages(*this);
		CoreStrings strings = CoreStrings(*this);

		inline FFont& fontBold() { return *fontBoldData; }
		inline FFont& fontRegular() { return *fontRegularData; }

		void dispose() override;
		void initialize() override;
		void initializeFonts();
		void postInitialize() override;
	private:
		uptr<FFont> fontBoldData;
		uptr<FFont> fontRegularData;
	};

	export CoreContent cct = CoreContent(futil::FBC);

	// TODO
	void CoreContent::dispose()
	{
		images.dispose();
		fontBoldData = nullptr;
		fontRegularData = nullptr;
	}

	// TODO
	void CoreContent::initialize()
	{
		initializeFonts();
		images.initialize();
		strings.initialize();
	}

	void CoreContent::initializeFonts()
	{
		fontBoldData = std::make_unique<FFont>(cfg.textFontBold.get(), cfg.renderScale(48), cfg.renderScale(2), cfg.renderScale(3));
		fontRegularData = std::make_unique<FFont>(cfg.textFont.get(), cfg.renderScale(48), 0, cfg.renderScale(3));
	}

	// TODO
	void CoreContent::postInitialize()
	{
		images.postInitialize();
		strings.postInitialize();
	}
}