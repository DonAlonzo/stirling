#pragma once

#include "vulkan/vulkan.h"

#include <glm/glm.hpp>

// stirling::vulkan
#include "vulkan/command_pool.h"
#include "vulkan/depth_image.h"
#include "vulkan/descriptor_pool.h"
#include "vulkan/device.h"
#include "vulkan/image.h"
#include "vulkan/index_buffer.h"
#include "vulkan/instance.h"
#include "vulkan/model.h"
#include "vulkan/pipeline.h"
#include "vulkan/physical_device.h"
#include "vulkan/render_pass.h"
#include "vulkan/semaphore.h"
#include "vulkan/swapchain.h"
#include "vulkan/texture.h"
#include "vulkan/vertex.h"
#include "vulkan/vertex_buffer.h"

// stirling
#include "camera.h"
#include "input_handler.h"
#include "entity.h"
#include "model_component.h"
#include "physics_component.h"
#include "world.h"