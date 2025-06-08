#include "ntpch.h"
#include "MeshFactory.h"

#include "Nut/Asset/AssetManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Nut {

	AssetHandle MeshFactory::CreateBoxTexture(const glm::vec3& size)
	{
        std::vector<Vertex> vertices(24);
        std::vector<Index> indices(12);

        // 6个面，每面4个顶点
        glm::vec3 positions[6][4] = {
            // +Z
            {
                {-size.x / 2, -size.y / 2,  size.z / 2},
                { size.x / 2, -size.y / 2,  size.z / 2},
                { size.x / 2,  size.y / 2,  size.z / 2},
                {-size.x / 2,  size.y / 2,  size.z / 2}
            },
            // -Z
            {
                { size.x / 2, -size.y / 2, -size.z / 2},
                {-size.x / 2, -size.y / 2, -size.z / 2},
                {-size.x / 2,  size.y / 2, -size.z / 2},
                { size.x / 2,  size.y / 2, -size.z / 2}
            },
            // +X
            {
                { size.x / 2, -size.y / 2,  size.z / 2},
                { size.x / 2, -size.y / 2, -size.z / 2},
                { size.x / 2,  size.y / 2, -size.z / 2},
                { size.x / 2,  size.y / 2,  size.z / 2}
            },
            // -X
            {
                {-size.x / 2, -size.y / 2, -size.z / 2},
                {-size.x / 2, -size.y / 2,  size.z / 2},
                {-size.x / 2,  size.y / 2,  size.z / 2},
                {-size.x / 2,  size.y / 2, -size.z / 2}
            },
            // +Y
            {
                {-size.x / 2,  size.y / 2,  size.z / 2},
                { size.x / 2,  size.y / 2,  size.z / 2},
                { size.x / 2,  size.y / 2, -size.z / 2},
                {-size.x / 2,  size.y / 2, -size.z / 2}
            },
            // -Y
            {
                {-size.x / 2, -size.y / 2, -size.z / 2},
                { size.x / 2, -size.y / 2, -size.z / 2},
                { size.x / 2, -size.y / 2,  size.z / 2},
                {-size.x / 2, -size.y / 2,  size.z / 2}
            }
        };

        glm::vec3 normals[6] = {
            { 0,  0,  1}, // +Z
            { 0,  0, -1}, // -Z
            { 1,  0,  0}, // +X
            {-1,  0,  0}, // -X
            { 0,  1,  0}, // +Y
            { 0, -1,  0}  // -Y
        };

        glm::vec3 tangents[6] = {
            { 1,  0,  0}, // +Z
            {-1,  0,  0}, // -Z
            { 0,  0, -1}, // +X
            { 0,  0,  1}, // -X
            { 1,  0,  0}, // +Y
            { 1,  0,  0}  // -Y
        };

        glm::vec3 bitangents[6] = {
            { 0,  1,  0}, // +Z
            { 0,  1,  0}, // -Z
            { 0,  1,  0}, // +X
            { 0,  1,  0}, // -X
            { 0,  0, -1}, // +Y
            { 0,  0,  1}  // -Y
        };

        glm::vec2 uvs[4] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        // 填充顶点
        for (int face = 0; face < 6; ++face) {
            for (int v = 0; v < 4; ++v) {
                int idx = face * 4 + v;
                vertices[idx].Position = positions[face][v];
                vertices[idx].Normal = normals[face];
                vertices[idx].TexCoord = uvs[v];
                vertices[idx].Tangent = tangents[face];
                vertices[idx].Bitangent = bitangents[face];
            }
        }

        // 填充索引
        for (uint32_t face = 0; face < 6; ++face) {
            uint32_t base = face * 4;
            indices[face * 2 + 0] = { base + 0, base + 1, base + 2 };
            indices[face * 2 + 1] = { base + 2, base + 3, base + 0 };
        }

		AssetHandle meshSource = AssetManager::AddMemoryOnlyAsset(std::make_shared<MeshSource>(vertices, indices, glm::mat4(1.0f)));
		return AssetManager::AddMemoryOnlyAsset(std::make_shared<StaticMesh>(meshSource));
	}

    AssetHandle MeshFactory::CreateBoxGeometry(const glm::vec3& size)
    {
        std::vector<Vertex> vertices;
        vertices.resize(8);
        vertices[0].Position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
        vertices[1].Position = { size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
        vertices[2].Position = { size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
        vertices[3].Position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
        vertices[4].Position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
        vertices[5].Position = { size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
        vertices[6].Position = { size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
        vertices[7].Position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };

        vertices[0].Normal = { -1.0f, -1.0f,  1.0f };
        vertices[1].Normal = { 1.0f, -1.0f,  1.0f };
        vertices[2].Normal = { 1.0f,  1.0f,  1.0f };
        vertices[3].Normal = { -1.0f,  1.0f,  1.0f };
        vertices[4].Normal = { -1.0f, -1.0f, -1.0f };
        vertices[5].Normal = { 1.0f, -1.0f, -1.0f };
        vertices[6].Normal = { 1.0f,  1.0f, -1.0f };
        vertices[7].Normal = { -1.0f,  1.0f, -1.0f };

        std::vector<Index> indices;
        indices.resize(12);
        indices[0] = { 2, 1, 0 };
        indices[1] = { 0, 3, 2 };
        indices[2] = { 6, 5, 1 };
        indices[3] = { 1, 2, 6 };
        indices[4] = { 5, 6, 7 };
        indices[5] = { 7, 4, 5 };
        indices[6] = { 3, 0, 4 };
        indices[7] = { 4, 7, 3 };
        indices[8] = { 1, 5, 4 };
        indices[9] = { 4, 0, 1 };
        indices[10] = { 6, 2, 3 };
        indices[11] = { 3, 7, 6 };

        AssetHandle meshSource = AssetManager::AddMemoryOnlyAsset(std::make_shared<MeshSource>(vertices, indices, glm::mat4(1.0f)));
        return AssetManager::AddMemoryOnlyAsset(std::make_shared<StaticMesh>(meshSource));
    }

	static void CalculateRing(size_t segments, float radius, float y, float dy, float height, float actualRadius, std::vector<Vertex>& vertices)
	{
		float segIncr = 1.0f / (float)(segments - 1);
		for (size_t s = 0; s < segments; s++)
		{
			float x = glm::cos(float(M_PI * 2) * s * segIncr) * radius;
			float z = glm::sin(float(M_PI * 2) * s * segIncr) * radius;

			Vertex& vertex = vertices.emplace_back();
			vertex.Position = glm::vec3(actualRadius * x, actualRadius * y + height * dy, actualRadius * z);
		}
	}

	AssetHandle MeshFactory::CreateCapsule(float radius, float height)
	{
		constexpr size_t subdivisionsHeight = 8;
		constexpr size_t ringsBody = subdivisionsHeight + 1;
		constexpr size_t ringsTotal = subdivisionsHeight + ringsBody;
		constexpr size_t numSegments = 12;
		constexpr float radiusModifier = 0.021f; // Needed to ensure that the wireframe is always visible

		std::vector<Vertex> vertices;
		std::vector<Index> indices;

		vertices.reserve(numSegments * ringsTotal);
		indices.reserve((numSegments - 1) * (ringsTotal - 1) * 2);

		float bodyIncr = 1.0f / (float)(ringsBody - 1);
		float ringIncr = 1.0f / (float)(subdivisionsHeight - 1);

		for (int r = 0; r < subdivisionsHeight / 2; r++)
			CalculateRing(numSegments, glm::sin(float(M_PI) * r * ringIncr), glm::sin(float(M_PI) * (r * ringIncr - 0.5f)), -0.5f, height, radius + radiusModifier, vertices);

		for (int r = 0; r < ringsBody; r++)
			CalculateRing(numSegments, 1.0f, 0.0f, r * bodyIncr - 0.5f, height, radius + radiusModifier, vertices);

		for (int r = subdivisionsHeight / 2; r < subdivisionsHeight; r++)
			CalculateRing(numSegments, glm::sin(float(M_PI) * r * ringIncr), glm::sin(float(M_PI) * (r * ringIncr - 0.5f)), 0.5f, height, radius + radiusModifier, vertices);

		for (int r = 0; r < ringsTotal - 1; r++)
		{
			for (int s = 0; s < numSegments - 1; s++)
			{
				Index& index1 = indices.emplace_back();
				index1.V1 = (uint32_t)(r * numSegments + s + 1);
				index1.V2 = (uint32_t)(r * numSegments + s + 0);
				index1.V3 = (uint32_t)((r + 1) * numSegments + s + 1);

				Index& index2 = indices.emplace_back();
				index2.V1 = (uint32_t)((r + 1) * numSegments + s + 0);
				index2.V2 = (uint32_t)((r + 1) * numSegments + s + 1);
				index2.V3 = (uint32_t)(r * numSegments + s);
			}
		}

		AssetHandle meshSource = AssetManager::AddMemoryOnlyAsset(std::make_shared<MeshSource>(vertices, indices, glm::mat4(1.0f)));
		return AssetManager::AddMemoryOnlyAsset(std::make_shared<StaticMesh>(meshSource));
	}
}