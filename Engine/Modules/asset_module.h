#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include "engine_module.h"

namespace modules
{
	class asset_module : public engine_module
	{
	public:
		SDL_AppResult init() override;
		void cleanup() override;
		std::string get_name() override;

		ENGINE_API const aiScene* load_model(std::string path);
		ENGINE_API const aiScene* get_scene() const;

	private:
		Assimp::Importer importer_;
	};
}
