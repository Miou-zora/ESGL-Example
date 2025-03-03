#pragma once
#include <map>
#include <string>
#include "Material.hpp"

class MaterialCache {
    public:
        MaterialCache() = default;
        ~MaterialCache() = default;

        Material &add(const std::string &name) {
            _materials.try_emplace(name);
            return _materials[name];
        }

        Material &get(const std::string &name) {
            return _materials[name];
        }

        void remove(const std::string &name) {
            _materials.erase(name);
        }

    private:
        std::map<std::string, Material> _materials;
};