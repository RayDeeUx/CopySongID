#include <Geode/modify/AudioAssetsBrowser.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/CustomSFXWidget.hpp>

using namespace geode::prelude;

#define MOVE_FUCKING_STUPID_NOSTALGIA_BAIT_BUTTON\
	fuckingStupidNostalgiaBaitButton->setPosition(this->m_moreBtn->getPositionX() + 50.f, this->m_moreBtn->getPositionY());\
	fuckingStupidNostalgiaBaitButton->setEnabled(this->m_moreBtn->isVisible() && !this->m_isRobtopSong);\
	fuckingStupidNostalgiaBaitButton->setVisible(this->m_moreBtn->isVisible() && !this->m_isRobtopSong);

#define CREATE_SPRITE_WITH_CONDITION(condition)\
	CCLabelBMFont* label = CCLabelBMFont::create("Copy", "bigFont.fnt");\
	CCSprite* sprite = CircleButtonSprite::create(label, CircleBaseColor::Pink);\
	sprite->setScale(condition);\
	label->setScale(.4f);

#define CREATE_AND_ADD_BUTTON(asset, function)\
	CCMenuItemSpriteExtra* copyButton = CCMenuItemSpriteExtra::create(sprite, this, function);\
	copyButton->setID("copy-" asset "-id"_spr);\
	m_buttonMenu->addChild(copyButton);

#define COPY_ID_USING(memory, type)\
	if (!Mod::get()->getSettingValue<bool>("enabled")) return;\
	if (memory < 1) return Notification::create(fmt::format(type" ID {} is not a valid ID, so it was not copied.", memory), NotificationIcon::Error, 2.f)->show();\
	(void) geode::utils::clipboard::write(fmt::format("{}", memory));\
	return Notification::create(fmt::format(type" ID {} was copied.", memory))->show();

#ifdef GEODE_IS_ANDROID
#define SANITY_CHECK_AND_GO_TBH(memory, type)\
	if (!Mod::get()->getSettingValue<bool>("enabled")) return;\
	if (!sender || sender->getTag() != 5062025 || memory.empty()) return;\
	MyAudioAssetsBrowser::composeStringForClipboard(memory, type);
#else
#define SANITY_CHECK_AND_GO_TBH(memory, type)\
	if (!Mod::get()->getSettingValue<bool>("enabled")) return;\
	const auto ids = static_cast<std::vector<int>>(memory);\
	if (!sender || sender->getTag() != 5062025 || ids.empty()) return;\
	MyAudioAssetsBrowser::composeStringForClipboard(ids, type);
#endif

class $modify(MyCustomSongWidget, CustomSongWidget) {
	static void onModify(auto& self) {
		(void) self.setHookPriorityAfterPost("CustomSongWidget::updateWithMultiAssets", "spaghettdev.songpreview");
	}

	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
		if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk)) return false;

		if (!m_downloadBtn || m_customSongID < 1 || m_isRobtopSong) return true;

		if (!Mod::get()->getSettingValue<bool>("enabled")) return true;

		if (Mod::get()->getSettingValue<bool>("stupidNostalgiaBait") && this->m_moreBtn && !isMusicLibrary) {
			ButtonSprite* fuckingStupidNostalgiaBait = ButtonSprite::create("Copy", 220, 0, .6f, false, "bigFont.fnt", "GJ_button_04.png", 25.f);
			fuckingStupidNostalgiaBait->setScale(.7f);
			fuckingStupidNostalgiaBait->setID("copy-song-id-worthless-nostalgia-bait-edition"_spr);
			CCMenuItemSpriteExtra* fuckingStupidNostalgiaBaitButton = CCMenuItemSpriteExtra::create(fuckingStupidNostalgiaBait, this, menu_selector(MyCustomSongWidget::onCopySongID));
			fuckingStupidNostalgiaBaitButton->setTag(20260810);
			this->m_buttonMenu->addChild(fuckingStupidNostalgiaBaitButton);
			MOVE_FUCKING_STUPID_NOSTALGIA_BAIT_BUTTON
			return true;
		}

		CREATE_SPRITE_WITH_CONDITION(m_isMusicLibrary ? 0.375f : 0.5f)
		CREATE_AND_ADD_BUTTON("song", menu_selector(MyCustomSongWidget::onCopySongID))
		copyButton->setPositionX(m_downloadBtn->getPositionX() + (m_isMusicLibrary ? 21.f : 21.5f));
		copyButton->setPositionY(m_downloadBtn->getPositionY() + (m_isMusicLibrary ? 21.f : 61.5f));

		return true;
	}

	void updateWithMultiAssets(gd::string p0, gd::string p1, int p2) {
		CustomSongWidget::updateWithMultiAssets(p0, p1, p2);
		if (!m_infoBtn || (m_sfx.empty() && m_songs.empty()) || m_isMusicLibrary || m_playbackBtn->isVisible() || !Mod::get()->getSettingValue<bool>("enabled")) return;
		m_infoBtn->setPositionX(m_infoBtn->getPositionX() - 5.f);
	}
	
	// code begrudgingly reused from poloak because some people just cant learn to move the fuck on from absolute's dogshit ui design hmmph
	void updateSongInfo() {
		CustomSongWidget::updateSongInfo();
		if (m_isMusicLibrary || !this->m_artistLabel || !this->m_moreBtn || !this->m_buttonMenu || !Mod::get()->getSettingValue<bool>("stupidNostalgiaBait") || !this->m_buttonMenu->getChildByID("copy-song-id-worthless-nostalgia-bait-edition"_spr)) return;
		this->m_artistLabel->setScale(1.f);
		this->m_artistLabel->limitLabelWidth(120.f, .75f, 0.f);
		this->m_moreBtn->setPositionX(this->m_buttonMenu->convertToNodeSpace({this->m_artistLabel->getScaledContentSize().width + 175.f, 0.f}).x);
		CCMenuItemSpriteExtra* fuckingStupidNostalgiaBaitButton = static_cast<CCMenuItemSpriteExtra*>(this->m_buttonMenu->getChildByID("copy-song-id-worthless-nostalgia-bait-edition"_spr));
		MOVE_FUCKING_STUPID_NOSTALGIA_BAIT_BUTTON
	}

	void onCopySongID(CCObject* sender) {
		if (m_isRobtopSong || !sender || sender->getTag() != 20260810) return;
		COPY_ID_USING(m_customSongID, "Song")
	}
};

class $modify(MyCustomSFXWidget, CustomSFXWidget) {
	bool init(SFXInfoObject* sfxInfo, CustomSFXDelegate* sfxDelegate, bool p2, bool p3, bool p4, bool p5, bool p6) {
		if (!CustomSFXWidget::init(sfxInfo, sfxDelegate, p2, p3, p4, p5, p6)) return false;

		if (!Mod::get()->getSettingValue<bool>("enabled")) return true;

		if (!m_sfxObject || m_sfxObject->m_folder || !m_downloadButton || m_sfxID < 1 || m_compactMode) return true;

		CREATE_SPRITE_WITH_CONDITION(0.375f)
		CREATE_AND_ADD_BUTTON("sfx", menu_selector(MyCustomSFXWidget::onCopySFXID))
		copyButton->setPosition(m_downloadButton->getPosition() + ccp(20.f, 20.f));

		return true;
	}

	void onCopySFXID(CCObject* sender) {
		COPY_ID_USING(m_sfxID, "SFX")
	}
};

class $modify(MyAudioAssetsBrowser, AudioAssetsBrowser) {
	bool init(gd::vector<int>& songIDs, gd::vector<int>& sfxIDs) {
		if (!AudioAssetsBrowser::init(songIDs, sfxIDs)) return false;

		if (!Mod::get()->getSettingValue<bool>("enabled") || (songIDs.empty() && sfxIDs.empty()) || !CCScene::get()->getChildByType<LevelInfoLayer>(0) || !m_buttonMenu) return true;

		if (!sfxIDs.empty()) MyAudioAssetsBrowser::makeButton("SFX", menu_selector(MyAudioAssetsBrowser::onCopyAllSFXIDs));
		if (!songIDs.empty()) MyAudioAssetsBrowser::makeButton("Song", menu_selector(MyAudioAssetsBrowser::onCopyAllSongIDs));

		return true;
	}
	void makeButton(const std::string& type, const cocos2d::SEL_MenuHandler function) {
		if (!Mod::get()->getSettingValue<bool>("enabled")) return;

		CCLabelBMFont* label = CCLabelBMFont::create(fmt::format("Copy\n{}s", type).c_str(), "bigFont.fnt");
		label->setAlignment(kCCTextAlignmentCenter);
		CircleButtonSprite* pinkSprite = CircleButtonSprite::create(label, CircleBaseColor::Pink);
		pinkSprite->setScale(.6f);

		CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(pinkSprite, this, function);
		button->setID(fmt::format("copy-all-{}-ids"_spr, utils::string::toLower(type)));
		button->setTag(5062025);

		m_buttonMenu->addChild(button);

		// "oH NOoOOOo HArCOdED poSiTiONS!1!!1"
		// it's relative to a buttonMenu, let go of your mother pearls already. jeez
		button->setPosition({157.f, 120.f});
		if (type != "SFX" && !m_sfxIds.empty()) button->setPositionX(button->getPositionX() - 30.f);
	}
	void onCopyAllSongIDs(CCObject* sender) {
		SANITY_CHECK_AND_GO_TBH(m_songsIds, "Song")
	}
	void onCopyAllSFXIDs(CCObject* sender) {
		SANITY_CHECK_AND_GO_TBH(m_sfxIds, "SFX")
	}
	#ifdef GEODE_IS_ANDROID
	static void composeStringForClipboard(const gd::vector<int>& ids, const std::string& type) {
	#else
	static void composeStringForClipboard(const std::vector<int>& ids, const std::string& type) {
	#endif
		if (!Mod::get()->getSettingValue<bool>("enabled")) return;
		std::string stringToReturn = fmt::format("{} IDs: ", type);
		for (const int id : ids) {
			if (id < 1) continue;
			stringToReturn = stringToReturn.append(fmt::format("{}, ", id));
		}
		if (stringToReturn.find_last_of(", ")) {
			stringToReturn.pop_back();
			stringToReturn.pop_back();
		}
		if (stringToReturn.empty()) return Notification::create(fmt::format("There were no {} IDs to copy.", type), NotificationIcon::Error, 2.f)->show();
		geode::utils::clipboard::write(stringToReturn);
		return Notification::create(fmt::format("{} {} ID{} copied to clipboard!", ids.size(), type, ids.size() == 1 ? "" : "s"), NotificationIcon::None, 3.f)->show();
	}
};