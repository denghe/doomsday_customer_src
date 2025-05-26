#pragma once
#include "xx2d_shader.h"

namespace Game {

    struct QuadInstanceTilesData {
        XY pos{}, scale{};                  // float * 4
        XY tileSize{}, mapSize{};           // float * 4
        XY tiling{}, offset{};              // float * 4
        XYi miniMapSize{};                  // i32 * 2
        // todo: light / color tex ?
    };

    // todo: change texture format to GL_R32I

    struct Shader_QuadInstanceTiles : xx::Shader {
        using xx::Shader::Shader;
        GLint uTex0{ -1 }, uTex1{ -1 }, aVert{ -1 }, aPosScale{ -1 }, aTileSizeMapSize{ -1 }, aTilingOffset{ -1 }, aMiniMapSize{ -1 };
        xx::GLVertexArrays va;
        xx::GLBuffer vb, ib;

        static constexpr int32_t maxQuadNums{ 20000 };
        GLuint lastTextureId{}, lastLightTextureId{};
        std::unique_ptr<QuadInstanceTilesData[]> quadInstanceTilesDatas = std::make_unique_for_overwrite<QuadInstanceTilesData[]>(maxQuadNums);
        int32_t quadCount{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        QuadInstanceTilesData* Draw(GLuint texId_Tiles, GLuint texId_Mini);
        QuadInstanceTilesData* Draw(xx::Ref<xx::GLTexture> const& texTiles, xx::Ref<xx::GLTexture> const& texMini);
    };

}
