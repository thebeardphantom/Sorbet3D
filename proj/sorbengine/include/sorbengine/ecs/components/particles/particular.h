#pragma once
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>

namespace sorbengine::ecs
{
	struct particular
	{
		bool world_space;
		float lifetime;

		particular(const size_t count = 0)
		{
			data = std::make_unique<particle_data>(count);
		}

		int get_count() const
		{
			return data->count_;
		}

		struct particle_data
		{
			particle_data(const size_t count = 0)
			{
				count_ = count;
				color_ = std::vector<glm::vec3>(count);
				position_ = std::vector<glm::vec3>(count);
				size_ = std::vector<glm::vec3>(count);
				birth_time_ = std::vector<float>(count);
				lifetime_ = std::vector<float>(count);
				is_alive_ = std::vector<bool>(count);
			}

		private:
			size_t count_;
			std::vector<glm::vec3> color_;
			std::vector<glm::vec3> position_;
			std::vector<glm::vec3> size_;
			std::vector<float> birth_time_;
			std::vector<float> lifetime_;
			std::vector<bool> is_alive_;

			friend particular;
		};

		std::unique_ptr<particle_data> data;
	};
}