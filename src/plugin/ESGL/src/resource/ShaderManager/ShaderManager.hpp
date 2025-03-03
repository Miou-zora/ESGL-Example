#pragma once
#include "Loader.hpp"

namespace ESGL {
    class ShaderManager {
        public:
            ShaderManager() = default;
            ~ShaderManager() = default;

            ShaderProgram &add(const std::string &name) {
                m_shaders.try_emplace(name);
                return m_shaders[name];
            }

            void use(const std::string &name) {
                m_shaders[name].use();
            }
            
            void disable(const std::string &name) {
                m_shaders[name].disable();
            }

            ShaderProgram &get(const std::string &name) {
                return m_shaders[name];
            }

        private:
            std::map<std::string, ShaderProgram> m_shaders;
    };
}