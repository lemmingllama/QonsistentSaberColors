#include "main.hpp"
#include "SettingsViewController.hpp"
#include "ColorManager.hpp"
#include "ModConfig.hpp"

#include "bsml/shared/BSML-Lite.hpp"

DEFINE_TYPE(QonsistentSaberColors, SettingsViewController)

int getf() { return 5; }
__declspec(property(get=getf)) int f;

void QonsistentSaberColors::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if(!firstActivation)
        return;

    auto container = BSML::Lite::CreateScrollableSettingsContainer(transform)->transform;

    BSML::Lite::CreateToggle(container, "Enabled", getModConfig().Enabled.GetValue(), [](bool value)
    {
        getModConfig().Enabled.SetValue(value);
        QonsistentSaberColors::UpdateControllerColors();
        QonsistentSaberColors::UpdateLaserColor();
    });

    BSML::Lite::CreateToggle(container, "Colored lasers", getModConfig().ColoredLasers.GetValue(), [](bool value)
    {
        getModConfig().ColoredLasers.SetValue(value);
        QonsistentSaberColors::UpdateLaserColor();
        StringW
    });
}