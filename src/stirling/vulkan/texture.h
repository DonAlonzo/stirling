#pragma once

#include "vulkan/vulkan.h"

namespace stirling {
	namespace vulkan {
		class Device;
	}
}
#include "image.h"
#include "image_view.h"
#include "sampler.h"

namespace stirling {
	namespace vulkan {
		class Texture {
		public:
			Texture(const Device& device, Image&& image);

			const Image& getImage() const;
			const ImageView& getImageView() const;
			const Sampler& getSampler() const;

		private:
			const Device* m_device;
			Image         m_image;
			ImageView     m_image_view;
			Sampler       m_sampler;

			ImageView     initImageView() const;
			Sampler       initSampler() const;
		};
	}
}