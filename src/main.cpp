#include <cassert>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "logger.hpp"
#include "renderer.hpp"
#include "app_delegate.hpp"

Logger LogPrint = Logger();

int main(int argc, char* argv[])
{
    NS::AutoreleasePool* autorelease_pool = NS::AutoreleasePool::alloc()->init();

    AppDelegate del;

    NS::Application* shared_application = NS::Application::sharedApplication();
    shared_application->setDelegate(&del);
    shared_application->run();

    autorelease_pool->release();

    return 0;
}
