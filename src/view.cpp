
//
//  logger.hpp
//
//  Created by yuancunkuan on 2025/2/5.
//

#include "view.hpp"

MTKViewDelegate::MTKViewDelegate(MTL::Device* device)
: MTK::ViewDelegate()
, renderer_(new Renderer(device))
{
}

MTKViewDelegate::~MTKViewDelegate()
{
    delete renderer_;
}

void MTKViewDelegate::drawInMTKView(MTK::View* view)
{
    renderer_->draw(view);
}
