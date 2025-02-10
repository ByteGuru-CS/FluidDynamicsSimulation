
//
//  app_delegate.hpp
//
//  Created by yuancunkuan on 2025/2/6.
//

#ifndef app_delegate_hpp
#define app_delegate_hpp

#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "view.hpp"

class AppDelegate : public NS::ApplicationDelegate
{
public:
    ~AppDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching(NS::Notification* notification) override;
    virtual void applicationDidFinishLaunching(NS::Notification* notification) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;

private:
    NS::Window*         window_;
    MTK::View*          mtk_view_;
    MTL::Device*        device_;
    MTKViewDelegate*    view_delegate_ = nullptr;
};


#endif /* app_delegate_hpp */
