
//
//  renderer.hpp
//
//  Created by yuancunkuan on 2025/2/5.
//

#ifndef renderer_hpp
#define renderer_hpp

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <simd/simd.h>

class Renderer
{
public:
    Renderer(MTL::Device* device);
    ~Renderer();
    void buildShaders();
    void buildBuffers();
    void draw(MTK::View* view);

private:
    MTL::Device*        device_;
    MTL::CommandQueue*  command_queue_;
    MTL::RenderPipelineState* pso_;
    MTL::Buffer* vertex_pos_buf_;
    MTL::Buffer* vertex_color_buf_;
};

#endif /* renderer_hpp */
