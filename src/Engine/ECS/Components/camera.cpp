#include "camera.h"
#include "camera.h"
#include "camera.h"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include "transform.h"
#include "../../smath.h"
#include "../../Modules/render_module.h"

namespace sorbengine::ecs::components
{
	glm::mat4 camera::get_view_matrix(const transform& tform) const
	{
		const glm::mat4 inverse_rotation_matrix = glm::mat4_cast(glm::conjugate(tform.get_local_rotation()));
		const auto translation_matrix =
			glm::translate(smath::identity_matrix, -tform.local_position);
		return inverse_rotation_matrix * translation_matrix;
	}

	glm::mat4 camera::get_perspective_matrix(const float_t aspect) const
	{
		auto projection_matrix = glm::perspective(
			glm::radians(fov),
			aspect,
			0.1f,
			100.0f);
		smath::apply_projection_matrix_scaling(projection_matrix);
		return projection_matrix;
	}

	glm::quat camera::get_pitch_yaw_rotation() const
	{
		// Create quaternions for each rotation
		const glm::quat yaw_quaternion = glm::angleAxis(glm::radians(yaw), smath::up);
		const glm::quat pitch_quaternion = glm::angleAxis(glm::radians(pitch), smath::right);
		return yaw_quaternion * pitch_quaternion;
	}
}
