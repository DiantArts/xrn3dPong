///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/Vulkan/Renderer.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::vulkan::Renderer::Renderer(
    ::xrn::engine::vulkan::Window& window,
    ::xrn::engine::vulkan::Device& device
)
    : m_window{ window }
    , m_device{ device }
{
    this->recreateSwapChain();
    this->createCommandBuffers();
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rule of 5
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::vulkan::Renderer::~Renderer()
{
    this->freeCommandBuffers();
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Getters/setters
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::vulkan::Renderer::getSwapChainRenderPass() const
    -> ::VkRenderPass
{
    return m_pSwapChain->getRenderPass();
}

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::vulkan::Renderer::getAspectRatio() const
    -> float
{
    return m_pSwapChain->extentAspectRatio();
}

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::vulkan::Renderer::isFrameInProgress() const
    -> bool
{
    return m_isFrameStarted;
}

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::vulkan::Renderer::getCurrentCommandBuffer() const
    -> ::VkCommandBuffer
{
    XRN_SASSERT(this->isFrameInProgress(), "Cannot get command buffer when frame is not in progress");
    return m_commandBuffers[m_currentFrameIndex];
}

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] auto ::xrn::engine::vulkan::Renderer::getFrameIndex() const
    -> int
{
    XRN_SASSERT(this->isFrameInProgress(), "Cannot get frame index when frame is not in progress");
    return m_currentFrameIndex;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Frame
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] auto ::xrn::engine::vulkan::Renderer::beginFrame()
    -> ::VkCommandBuffer
{
    XRN_SASSERT(!this->isFrameInProgress(), "Cannot call beginFrame() while frame already in progress");
    switch (m_pSwapChain->acquireNextImage(&m_currentImageIndex)) {
    case VK_SUCCESS:
    case VK_SUBOPTIMAL_KHR:
        break;
    case VK_ERROR_OUT_OF_DATE_KHR:
        XRN_LOG(::xrn::Logger::Level::note, "New swapchain needs to be created.");
        m_window.setResizedFlag();
        this->recreateSwapChain();
        return nullptr;
    default:
        XRN_LOG(::xrn::Logger::fatalError, "Failed to aquire swapChain image");
    }

    m_isFrameStarted = true;
    auto commandBuffer{ this->getCurrentCommandBuffer() };

    ::VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    XRN_SASSERT(::vkBeginCommandBuffer(commandBuffer, &beginInfo) == VK_SUCCESS, "Failed to begin recording command buffer!");
    return commandBuffer;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::vulkan::Renderer::endFrame()
{
    XRN_SASSERT(this->isFrameInProgress(), "Cannot call endFrame() if frame is not in progress");
    auto commandBuffer{ this->getCurrentCommandBuffer() };
    XRN_SASSERT(::vkEndCommandBuffer(commandBuffer) == VK_SUCCESS, "Failed to record command buffer.");
    if (
        auto result{ m_pSwapChain->submitCommandBuffers(&commandBuffer, &m_currentImageIndex) };
        result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.wasResized()
    ) {
        m_window.resetResizedFlag();
        recreateSwapChain();
    } else {
        XRN_SASSERT(result == VK_SUCCESS, "Failed to present swapChain image.");
    }

    m_isFrameStarted = false;
    m_currentFrameIndex = (m_currentFrameIndex + 1) % ::xrn::engine::vulkan::SwapChain::MAX_FRAMES_IN_FLIGHT;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::vulkan::Renderer::beginSwapChainRenderPass(
    ::VkCommandBuffer commandBuffer
)
{
    XRN_SASSERT(this->isFrameInProgress(), "Cannot call beginSwapChainRenderPass() if frame is not in progress");
    XRN_SASSERT(commandBuffer == getCurrentCommandBuffer(), "Cannot begin render pass on command buffer from a different frame");

    ::VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_pSwapChain->getRenderPass();
    renderPassInfo.framebuffer = m_pSwapChain->getFrameBuffer(m_currentImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_pSwapChain->getSwapChainExtent();

    std::array<::VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    ::vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    ::VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_pSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(m_pSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    ::VkRect2D scissor{{0, 0}, m_pSwapChain->getSwapChainExtent()};
    ::vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    ::vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::vulkan::Renderer::endSwapChainRenderPass(
    ::VkCommandBuffer commandBuffer
)
{
    XRN_SASSERT(this->isFrameInProgress(), "Cannot call endSwapChainRenderPass() if frame is not in progress");
    XRN_SASSERT(commandBuffer == getCurrentCommandBuffer(), "Cannot end render pass on command buffer from a different frame");
    ::vkCmdEndRenderPass(commandBuffer);
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::vulkan::Renderer::createCommandBuffers()
{
    m_commandBuffers.resize(::xrn::engine::vulkan::SwapChain::MAX_FRAMES_IN_FLIGHT);

    ::VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    XRN_SASSERT(vkAllocateCommandBuffers(m_device.device(), &allocInfo, m_commandBuffers.data()) == VK_SUCCESS, "Failed to allocate command buffers!");
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::vulkan::Renderer::freeCommandBuffers()
{
    ::vkFreeCommandBuffers(
        m_device.device(),
        m_device.getCommandPool(),
        static_cast<::std::size_t>(m_commandBuffers.size()),
        m_commandBuffers.data()
    );
    m_commandBuffers.clear();
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::vulkan::Renderer::recreateSwapChain()
{
    auto windowSize{ m_window.getSize() };
    while (!windowSize.isValid()) {
        windowSize = m_window.getSize();
        ::glfwWaitEvents();
    }
    ::vkDeviceWaitIdle(m_device.device());

    if (!m_pSwapChain) {
        m_pSwapChain = ::std::make_unique<::xrn::engine::vulkan::SwapChain>(m_device, windowSize);
    } else {
        ::std::shared_ptr<::xrn::engine::vulkan::SwapChain> oldSwapchain{ ::std::move(m_pSwapChain) };
        m_pSwapChain = ::std::make_unique<::xrn::engine::vulkan::SwapChain>(m_device, windowSize, oldSwapchain);
        XRN_SASSERT(oldSwapchain->compareSwapFormats(*m_pSwapChain.get()), "Swap chain image or depth format has changed");
    }
}
