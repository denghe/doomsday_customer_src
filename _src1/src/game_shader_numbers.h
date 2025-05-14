#pragma once
#include "xx2d_shader.h"

namespace Game {

    struct QuadInstanceNumbersData {
        XY pos{}, scale{ 1, 1 };                // float * 4
        xx::RGBA8 color{ 255, 255, 255, 255 };  // u8 * 4
        uint32_t numbers[4];                    // u32 * 4      // last one is len
    };

    struct Shader_QuadInstanceNumbers : xx::Shader {
        using xx::Shader::Shader;
        GLint uTex0{ -1 }, aVert{ -1 }, aPosScale{ -1 }, aColor{ -1 }, aNumbers{ -1 };
        xx::GLVertexArrays va;
        xx::GLBuffer vb, ib;

        static constexpr int32_t maxQuadNums{ 20000 };
        std::unique_ptr<QuadInstanceNumbersData[]> quadInstanceNumbersDatas = std::make_unique_for_overwrite<QuadInstanceNumbersData[]>(maxQuadNums);
        int32_t quadCount{};

        void Init();
        virtual void Begin() override;
        virtual void End() override;
        void Commit();
        QuadInstanceNumbersData* Draw(uint32_t v);
        QuadInstanceNumbersData* Draw(std::string_view sv);
        void Draw(QuadInstanceNumbersData const& data);
    };

}
