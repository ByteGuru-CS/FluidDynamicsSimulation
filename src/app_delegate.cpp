#include "app_delegate.hpp"


AppDelegate::~AppDelegate()
{
    mtk_view_->release();
    window_->release();
    device_->release();
    delete view_delegate_;
}

NS::Menu* AppDelegate::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Menu* main_menu = NS::Menu::alloc()->init();
    NS::MenuItem* app_menu_item = NS::MenuItem::alloc()->init();
    NS::Menu* app_menu = NS::Menu::alloc()->init(NS::String::string("FluidDynamicsSimulation", UTF8StringEncoding));

    NS::String* app_name = NS::RunningApplication::currentApplication()->localizedName();
    NS::String* quit_item_name = NS::String::string("Quit", UTF8StringEncoding )->stringByAppendingString(app_name);
    SEL quit_cb = NS::MenuItem::registerActionCallback("AppQuit", [](void*,SEL,const NS::Object* sender) {
        auto app = NS::Application::sharedApplication();
        app->terminate(sender);
    });

    NS::MenuItem* app_quit_item = app_menu->addItem(quit_item_name, quit_cb, NS::String::string("q", UTF8StringEncoding));
    app_quit_item->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);
    app_menu_item->setSubmenu(app_menu);

    NS::MenuItem* window_menu_item = NS::MenuItem::alloc()->init();
    NS::Menu* window_menu = NS::Menu::alloc()->init(NS::String::string("Demo", UTF8StringEncoding));

    SEL close_window_cb = NS::MenuItem::registerActionCallback( "WindowClose", [](void*, SEL, const NS::Object*){
        auto app = NS::Application::sharedApplication();
            app->windows()->object<NS::Window>(0)->close();
    } );
    NS::MenuItem* close_window_item = window_menu->addItem(NS::String::string("Close Window", UTF8StringEncoding), close_window_cb, NS::String::string("w", UTF8StringEncoding));
    close_window_item->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

    window_menu_item->setSubmenu(window_menu);

    main_menu->addItem(app_menu_item);
    main_menu->addItem(window_menu_item);

    app_menu_item->release();
    window_menu_item->release();
    app_menu->release();
    window_menu->release();

    return main_menu->autorelease();
}

void AppDelegate::applicationWillFinishLaunching(NS::Notification* notification)
{
    NS::Menu* menu = createMenuBar();
    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->setMainMenu(menu);
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(NS::Notification* notification)
{
    CGRect frame = (CGRect){ {100.0, 100.0}, {1280.0, 720.0} };

    window_ = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
        NS::BackingStoreBuffered,
        false);

    device_ = MTL::CreateSystemDefaultDevice();

    mtk_view_ = MTK::View::alloc()->init(frame, device_);
    mtk_view_->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    mtk_view_->setClearColor(MTL::ClearColor::Make( 1.0, 0.0, 0.0, 1.0 ));

    view_delegate_ = new MTKViewDelegate(device_);
    mtk_view_->setDelegate(view_delegate_);

    window_->setContentView(mtk_view_);
    window_->setTitle(NS::String::string("Window", NS::StringEncoding::UTF8StringEncoding));

    window_->makeKeyAndOrderFront(nullptr);

    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->activateIgnoringOtherApps(true);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender)
{
    return true;
}
