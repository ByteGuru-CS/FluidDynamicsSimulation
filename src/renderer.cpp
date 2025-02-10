#include "renderer.hpp"
#include "base.hpp"
#include "logger.hpp"

Renderer::Renderer(MTL::Device* device)
: device_(device->retain())
{
    command_queue_ = device_->newCommandQueue();
    buildShaders();
    buildBuffers();
}

Renderer::~Renderer()
{
    vertex_pos_buf_->release();
    vertex_color_buf_->release();
    pso_->release();
    command_queue_->release();
    device_->release();
}

void Renderer::buildShaders()
{
    LogPrint.log(Logger::INFO, "Build Shader, Seleted device:", device_->name()->utf8String());
    NS::Error* error = nullptr;
    MTL::Library* library = device_->newDefaultLibrary();
    if (!library) {
        LogPrint.log(Logger::ERROR, "Metal library error.");
        assert(false);
    }
    //render pipeline
    {
        MTL::Function* vertex_fn = library->newFunction(STR2NS("vertexMain"));
        MTL::Function* fragment_fn = library->newFunction(STR2NS("fragmentMain"));
        MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
        desc->setLabel(STR2NS("render pipeline"));
        desc->setVertexFunction(vertex_fn);
        desc->setFragmentFunction(fragment_fn);
        desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);
        pso_ = device_->newRenderPipelineState(desc, &error);
        if (!pso_) {
            LogPrint.log(Logger::ERROR, error->localizedDescription()->utf8String());
            assert(false);
        }
        vertex_fn->release();
        fragment_fn->release();
        desc->release();
    }
}

void Renderer::buildBuffers()
{
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] =
    {
        { -0.8f,  0.8f, 0.0f },
        {  0.0f, -0.8f, 0.0f },
        { +0.8f,  0.8f, 0.0f }
    };

    simd::float3 colors[NumVertices] =
    {
        {  1.0, 0.3f, 0.2f },
        {  0.8f, 1.0, 0.0f },
        {  0.8f, 0.0f, 1.0 }
    };

    const size_t pos_data_size = NumVertices * sizeof(simd::float3);
    const size_t color_data_size = NumVertices * sizeof(simd::float3);

    vertex_pos_buf_ = device_->newBuffer(pos_data_size, MTL::ResourceStorageModeManaged);
    vertex_color_buf_ = device_->newBuffer(color_data_size, MTL::ResourceStorageModeManaged);

    memcpy(vertex_pos_buf_->contents(), positions, pos_data_size);
    memcpy(vertex_color_buf_->contents(), colors, color_data_size);

    vertex_pos_buf_->didModifyRange(NS::Range::Make(0, vertex_pos_buf_->length()));
    vertex_color_buf_->didModifyRange(NS::Range::Make(0, vertex_color_buf_->length()));
}
void Renderer::draw(MTK::View* view)
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* cmd = command_queue_->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);
    
    enc->setRenderPipelineState(pso_);
    enc->setVertexBuffer(vertex_pos_buf_, 0, 0);
    enc->setVertexBuffer(vertex_color_buf_, 0, 1);
    enc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    
    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}

