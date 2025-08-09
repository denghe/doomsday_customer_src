#pragma once
#include <string>

namespace UI {

	enum class Languages {
		en,
		cn,
		// ...
		__MAX__
	};

	enum class StringKeys {
		title_Main,
		title_ChooseHero,
		generic_On,
		generic_Off,
		generic_Choose,
		generic_EscBack,
		generic_SpaceChoose,
		options_Blahblah,
		options_VolumeMusic,
		options_VolumeSound,
		mainMenu_Start,
		mainMenu_Options,
		mainMenu_Quit,
		// ...
		__MAX__
	};
	
	// en
	static constexpr std::u32string_view TextsEN[(int32_t)StringKeys::__MAX__] {
		U"pest terminator",
		U"choose character",
		U"on",
		U"off",
		U"choose",
		U"[Q] back",
		U"[SPACE] confirm",
		U"blahblah",
		U"music volume",
		U"sound volume",
		U"start",
		U"options",
		U"quit",
		// ...
	};

	// cn
	static constexpr std::u32string_view TextsCN[(int32_t)StringKeys::__MAX__]{
		U"异虫终结者",
		U"选择角色",
		U"开",
		U"关",
		U"选择",
		U"[Q] 返回",
		U"[SPACE] 确定",
		U"那啥那啥",
		U"音乐音量",
		U"声效音量",
		U"开始游戏",
		U"设置",
		U"退出",
		// ...
	};

	// ...

	static constexpr std::u32string_view const* Textss[(int32_t)Languages::__MAX__]{
		TextsEN,
		TextsCN,
		// ...
	};

	inline static Languages gLanguage{ Languages::en };
	inline static std::u32string_view const* gTexts{ Textss[(int32_t)gLanguage] };

	XX_INLINE void SetLanguage(Languages lang) {
		gLanguage = lang;
		gTexts = Textss[(int32_t)lang];
	}

	XX_INLINE void SetNextLanguage() {
		gLanguage = (Languages)((int32_t)gLanguage + 1);
		if (gLanguage == Languages::__MAX__) {
			gLanguage = {};
		}
		gTexts = Textss[(int32_t)gLanguage];
	}

	XX_INLINE std::u32string_view const& GetText(StringKeys k) {
		assert(!gTexts[(int32_t)k].empty());
		return gTexts[(int32_t)k];
	}

	
#define CODE_TEXTOF(k) XX_INLINE std::u32string_view const& TextOf_##k() { return GetText(StringKeys::k); }

	CODE_TEXTOF(title_Main);
	CODE_TEXTOF(title_ChooseHero);
	CODE_TEXTOF(generic_On);
	CODE_TEXTOF(generic_Off);
	CODE_TEXTOF(generic_Choose);
	CODE_TEXTOF(generic_EscBack);
	CODE_TEXTOF(generic_SpaceChoose);
	CODE_TEXTOF(options_Blahblah);
	CODE_TEXTOF(options_VolumeMusic);
	CODE_TEXTOF(options_VolumeSound);
	CODE_TEXTOF(mainMenu_Start);
	CODE_TEXTOF(mainMenu_Options);
	CODE_TEXTOF(mainMenu_Quit);

	// ...
}
