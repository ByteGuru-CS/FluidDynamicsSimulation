
//
//  view.hpp
//
//  Created by yuancunkuan on 2025/2/6.
//

#ifndef view_hpp
#define view_hpp

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "renderer.hpp"

class MTKViewDelegate : public MTK::ViewDelegate
{
public:
    MTKViewDelegate(MTL::Device* device);
    virtual ~MTKViewDelegate() override;
    virtual void drawInMTKView(MTK::View* view) override;

private:
    Renderer* renderer_;
};

#endif /* view_hpp */
