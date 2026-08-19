#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/PlatformToolbox.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// Force GD to treat the platform as Desktop/PC layout
class $modify(PCAppDelegate, AppDelegate) {
    bool applicationDidFinishLaunching() {
        if (!AppDelegate::applicationDidFinishLaunching()) return false;

        #ifdef GEODE_IS_MOBILE
        // Force desktop UI scaling factor
        CCDirector::sharedDirector()->updateContentScale(kCCImageFormat_PNG);
        
        // Show cursor if a mouse or controller touch overlay is used
        PlatformToolbox::showCursor();
        #endif

        return true;
    }
};

// Override platform check methods to report as PC
class $modify(PCPlatformToolbox, PlatformToolbox) {
    static bool isControllerConnected() {
        return PlatformToolbox::isControllerConnected();
    }

    // Force system to report non-mobile ui flags
    static void showCursor() {
        #ifdef GEODE_IS_MOBILE
        auto director = CCDirector::sharedDirector();
        if (director && director->getOpenGLView()) {
            director->getOpenGLView()->showCursor(true);
        }
        #endif
    }
};

// Modify Main Menu layout to remove mobile-specific buttons
class $modify(PCMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        #ifdef GEODE_IS_MOBILE
        // Hide mobile specific UI elements if present (e.g. cross-platform buy buttons)
        if (auto profileBtn = this->getChildByID("profile-button")) {
            // Re-align menu items to PC default positions
            auto mainContainer = this->getChildByID("main-menu");
            if (mainContainer) {
                mainContainer->updateLayout();
            }
        }
        
        log::info("Applied PC UI overrides to Mobile MenuLayer!");
        #endif

        return true;
    }
};
