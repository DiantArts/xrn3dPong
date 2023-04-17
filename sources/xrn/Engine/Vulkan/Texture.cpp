///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/Vulkan/Texture.hpp>
#include <xrn/Engine/Configuration.hpp>

///////////////////////////////////////////////////////////////////////////
// Implem
///////////////////////////////////////////////////////////////////////////
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::vulkan::Texture::Texture(
    const ::std::string_view filename
    , ::xrn::engine::vulkan::Device &device
)
{
    // load image
    int textureWidth, textureHeight, textureChannels;

    ::std::string filepath;
    filepath.reserve(
        ::xrn::engine::configuration.filepath.textureDirectory.size() + filename.size()
    );
    filepath += ::xrn::engine::configuration.filepath.textureDirectory;
    filepath += filename;

    ::stbi_uc* pixels{ ::stbi_load(
        filepath.c_str()
        , &textureWidth
        , &textureHeight
        , &textureChannels
        , STBI_rgb_alpha
    ) };
    ::VkDeviceSize imageSize{ textureWidth * textureHeight * 4 };

    XRN_FATAL_ASSERT(pixels, "Failed to load texture image");


    // init pixel buffer
    ::VkBuffer stagingBuffer;
    ::VkDeviceMemory stagingBufferMemory;
    device.createBuffer(
        imageSize
        , VK_BUFFER_USAGE_TRANSFER_SRC_BIT
        , VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        , stagingBuffer
        , stagingBufferMemory
    );

    // copy pixels to vulkan buffer
    void* data;
    ::vkMapMemory(device.device(), stagingBufferMemory, 0, imageSize, 0, &data);
    ::std::memcpy(data, pixels, static_cast<::std::size_t>(imageSize));
    ::vkUnmapMemory(device.device(), stagingBufferMemory);

    // unload image
    ::stbi_image_free(pixels);

    // add it to a vulkan image
    ::VkImage textureImage;
    ::VkDeviceMemory textureImageMemory;
    this->createImage(
        device
        , textureHeight
        , textureHeight
        , VK_FORMAT_R8G8B8A8_SRGB
        , VK_IMAGE_TILING_OPTIMAL
        , VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
        , VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        , textureImage
        , textureImageMemory
    );
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::vulkan::Texture::createImage(
    ::xrn::engine::vulkan::Device &device
    , ::std::uint32_t width
    , ::std::uint32_t height
    , ::VkFormat format
    , ::VkImageTiling tiling
    , ::VkImageUsageFlags usage
    , ::VkMemoryPropertyFlags properties
    , ::VkImage& image
    , ::VkDeviceMemory& imageMemory
)
{
    ::VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    XRN_FATAL_ASSERT(
        ::vkCreateImage(device.device(), &imageInfo, nullptr, &image) == VK_SUCCESS
        , "Failed to create image"
    );

    ::VkMemoryRequirements memRequirements;
    ::vkGetImageMemoryRequirements(device.device(), image, &memRequirements);

    ::VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties);

    XRN_FATAL_ASSERT(
        ::vkAllocateMemory(device.device(), &allocInfo, nullptr, &imageMemory) == VK_SUCCESS
        , "Failed to allocate image memory"
    );
    ::vkBindImageMemory(device.device(), image, imageMemory, 0);
}
