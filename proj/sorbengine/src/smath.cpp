#include "sorbengine/smath.h"

namespace sorbengine::smath
{
	void apply_projection_matrix_scaling(glm::mat4& projection_matrix)
	{
#ifdef SORBET_FLIP_Z
		projection_matrix *= glm::scale(identity_matrix, glm::vec3(1.0f, 1.0f, -1.0f));
#endif
	}
}
