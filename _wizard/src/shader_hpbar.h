#pragma once
#include "xx2d_shader.h"

namespace Game {

    struct QuadInstanceHPBarData {
        XY pos{}, scale{ 1, 1 };                // float * 4
        xx::RGBA8 color{ 255, 255, 255, 255 };  // u8 * 4
        uint32_t numbers[4];                    // u32 * 4 / u8 * 16   // last one is len
        float value{};                          // float * 1
    };

    struct Shader_QuadInstanceHPBar : xx::Shader {
        using xx::Shader::Shader;
        GLint uTex0{ -1 }, aVert{ -1 }, aPosScale{ -1 }, aColor{ -1 }, aHPBar{ -1 }, aValue{ -1 };
        xx::GLVertexArrays va;
        xx::GLBuffer vb, ib;

        static constexpr int32_t maxQuadNums{ 20000 };
        std::unique_ptr<QuadInstanceHPBarData[]> quadInstanceHPBarDatas = std::make_unique_for_overwrite<QuadInstanceHPBarData[]>(maxQuadNums);
        GLuint texId{};
        int32_t quadCount{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        QuadInstanceHPBarData* Draw(uint32_t v);
        QuadInstanceHPBarData* Draw(uint32_t v1, uint32_t v2);
        QuadInstanceHPBarData* Draw(std::string_view sv);
        void Draw(QuadInstanceHPBarData const& data);
    };

}
