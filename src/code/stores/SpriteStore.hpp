#pragma once

#include <string>
#include <unordered_map>

#include "raylib.h"

class SpriteStore {
    public:
        Texture2D* GetTexture(const std::string& path) {
            auto it = textures.find(path);
            if (it != textures.end()) return &it->second;

            auto [inserted, ok] = textures.emplace(path, LoadTexture(path.c_str()));
            return &inserted->second;
        }

        // For textures that don't come from disk (procedural placeholders, etc.)
        // but still want centralized ownership/unload.
        Texture2D* RegisterTexture(const std::string& key, Texture2D texture) {
            auto [inserted, ok] = textures.emplace(key, texture);
            return &inserted->second;
        }

        void UnloadAll() {
            for (auto& [path, texture] : textures) UnloadTexture(texture);
            textures.clear();
        }

    private:
        std::unordered_map<std::string, Texture2D> textures;
};