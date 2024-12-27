#include <cassert>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>


#pragma region Declarations {

class Renderer
{
public:
    Renderer(MTL::Device* device);
    ~Renderer();
    void draw(MTK::View* view);

private:
    MTL::Device*        device_;
    MTL::CommandQueue*  command_queue_;
};

class MyMTKViewDelegate : public MTK::ViewDelegate
{
public:
    MyMTKViewDelegate(MTL::Device* device);
    virtual ~MyMTKViewDelegate() override;
    virtual void drawInMTKView(MTK::View* view) override;

private:
    Renderer* renderer_;
};

class MyAppDelegate : public NS::ApplicationDelegate
{
public:
    ~MyAppDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching(NS::Notification* notification) override;
    virtual void applicationDidFinishLaunching(NS::Notification* notification) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;

private:
    NS::Window*         window_;
    MTK::View*          mtk_view_;
    MTL::Device*        device_;
    MyMTKViewDelegate*  view_delegate_ = nullptr;
};

#pragma endregion Declarations }


int main(int argc, char* argv[])
{
    NS::AutoreleasePool* autorelease_pool = NS::AutoreleasePool::alloc()->init();

    MyAppDelegate del;

    NS::Application* shared_application = NS::Application::sharedApplication();
    shared_application->setDelegate(&del);
    shared_application->run();

    autorelease_pool->release();

    return 0;
}


#pragma mark - AppDelegate
#pragma region AppDelegate {

MyAppDelegate::~MyAppDelegate()
{
    mtk_view_->release();
    window_->release();
    device_->release();
    delete view_delegate_;
}

NS::Menu* MyAppDelegate::createMenuBar()
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

void MyAppDelegate::applicationWillFinishLaunching(NS::Notification* notification)
{
    NS::Menu* menu = createMenuBar();
    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->setMainMenu(menu);
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void MyAppDelegate::applicationDidFinishLaunching(NS::Notification* notification)
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

    view_delegate_ = new MyMTKViewDelegate(device_);
    mtk_view_->setDelegate(view_delegate_);

    window_->setContentView(mtk_view_);
    window_->setTitle(NS::String::string("Window", NS::StringEncoding::UTF8StringEncoding));

    window_->makeKeyAndOrderFront(nullptr);

    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->activateIgnoringOtherApps(true);
}

bool MyAppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender)
{
    return true;
}

#pragma endregion AppDelegate }


#pragma mark - ViewDelegate
#pragma region ViewDelegate {

MyMTKViewDelegate::MyMTKViewDelegate(MTL::Device* device)
: MTK::ViewDelegate()
, renderer_(new Renderer(device))
{
}

MyMTKViewDelegate::~MyMTKViewDelegate()
{
    delete renderer_;
}

void MyMTKViewDelegate::drawInMTKView(MTK::View* view)
{
    renderer_->draw(view);
}

#pragma endregion ViewDelegate }


#pragma mark - Renderer
#pragma region Renderer {

Renderer::Renderer(MTL::Device* device)
: device_(device->retain())
{
    command_queue_ = device_->newCommandQueue();
}

Renderer::~Renderer()
{
    command_queue_->release();
    device_->release();
}

void Renderer::draw(MTK::View* view)
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* cmd = command_queue_->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);
    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}

#pragma endregion Renderer }
