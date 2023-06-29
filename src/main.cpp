#include "main.hpp"
#include "SettingsViewController.hpp"
#include "ModConfig.hpp"
#include "ColorManager.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/ColorsOverrideSettingsPanelController.hpp"
#include "GlobalNamespace/MenuEnvironmentManager.hpp"

#include "VRUIControls/VRPointer.hpp"

#include "questui/shared/QuestUI.hpp"

static ModInfo modInfo;

using namespace QonsistentSaberColors;

MAKE_HOOK_MATCH(VRPointer_CreateLaserPointerAndLaserHit, &VRUIControls::VRPointer::CreateLaserPointerAndLaserHit, void, VRUIControls::VRPointer* self)
{
    VRPointer_CreateLaserPointerAndLaserHit(self);
    if(getModConfig().ColoredLasers.GetValue() && getModConfig().Enabled.GetValue())
        UpdateLaserColor();
}

MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool a, bool b, bool c)
{
    MainMenuViewController_DidActivate(self, a, b, c);
    if(getModConfig().Enabled.GetValue())
    {
        UpdateControllerColors();
        if(getModConfig().ColoredLasers.GetValue())
            UpdateLaserColor();
    }
}

MAKE_HOOK_MATCH(ColorsOverrideSettingsPanelController_HandleOverrideColorsToggleValueChanged, &GlobalNamespace::ColorsOverrideSettingsPanelController::HandleOverrideColorsToggleValueChanged, void, GlobalNamespace::ColorsOverrideSettingsPanelController* self, bool isOn)
{
    ColorsOverrideSettingsPanelController_HandleOverrideColorsToggleValueChanged(self, isOn);
    if(getModConfig().Enabled.GetValue())
    {
        UpdateControllerColors();
        if(getModConfig().ColoredLasers.GetValue())
            UpdateLaserColor();
    }
}

MAKE_HOOK_MATCH(ColorsOverrideSettingsPanelController_HandleEditColorSchemeControllerDidChangeColorScheme, &GlobalNamespace::ColorsOverrideSettingsPanelController::HandleEditColorSchemeControllerDidChangeColorScheme, void, GlobalNamespace::ColorsOverrideSettingsPanelController* self, GlobalNamespace::ColorScheme* colorScheme)
{
    ColorsOverrideSettingsPanelController_HandleEditColorSchemeControllerDidChangeColorScheme(self, colorScheme);
    if(getModConfig().Enabled.GetValue())
    {
        UpdateControllerColors();
        if(getModConfig().ColoredLasers.GetValue())
            UpdateLaserColor();
    }
}

MAKE_HOOK_MATCH(ColorsOverrideSettingsPanelController_HandleDropDownDidSelectCellWithIdx, &GlobalNamespace::ColorsOverrideSettingsPanelController::HandleDropDownDidSelectCellWithIdx, void, GlobalNamespace::ColorsOverrideSettingsPanelController* self, HMUI::DropdownWithTableView* dropDownWithTableView, int idx)
{
    ColorsOverrideSettingsPanelController_HandleDropDownDidSelectCellWithIdx(self, dropDownWithTableView, idx);
    if(getModConfig().Enabled.GetValue())
    {
        UpdateControllerColors();
        if(getModConfig().ColoredLasers.GetValue())
            UpdateLaserColor();
    }
}

MAKE_HOOK_MATCH(MenuEnvironmentManager_ShowEnvironmentType, &GlobalNamespace::MenuEnvironmentManager::ShowEnvironmentType, void, GlobalNamespace::MenuEnvironmentManager* self, GlobalNamespace::MenuEnvironmentManager::MenuEnvironmentType type)
{
    MenuEnvironmentManager_ShowEnvironmentType(self, type);
    UpdatePointers();
    if(type == GlobalNamespace::MenuEnvironmentManager::MenuEnvironmentType::None)
        return;

    if(getModConfig().Enabled.GetValue())
    {
        UpdateControllerColors();
        if(getModConfig().ColoredLasers.GetValue())
            UpdateLaserColor();
    }
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getModConfig().Init(modInfo);

    QuestUI::Init();
    QuestUI::Register::RegisterAllModSettingsViewController<QonsistentSaberColors::SettingsViewController*>(modInfo, "QonsistentSaberColors");

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), VRPointer_CreateLaserPointerAndLaserHit);
    INSTALL_HOOK(getLogger(), MainMenuViewController_DidActivate);
    INSTALL_HOOK(getLogger(), ColorsOverrideSettingsPanelController_HandleOverrideColorsToggleValueChanged);
    INSTALL_HOOK(getLogger(), ColorsOverrideSettingsPanelController_HandleEditColorSchemeControllerDidChangeColorScheme);
    INSTALL_HOOK(getLogger(), ColorsOverrideSettingsPanelController_HandleDropDownDidSelectCellWithIdx);
    INSTALL_HOOK(getLogger(), MenuEnvironmentManager_ShowEnvironmentType);
    getLogger().info("Installed all hooks!");
}
