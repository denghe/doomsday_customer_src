#pragma once

namespace Game {

    inline void Shader_QuadInstanceNumbers::Init() {

        v = xx::LoadGLVertexShader({ R"(#version 300 es
uniform vec2 uCxy;	            // screen center coordinate
uniform vec2 uCharSize;

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aColor;
in mat4 aNumbers;

out vec2 vTexCoord;
out vec4 vColor;
out mat4 vNumbers;

void main() {
    vec2 pos = aPosScale.xy;
    vec2 scale = aPosScale.zw;
    vec2 offset = aVert * scale;
    float len = aNumbers[3][3];
    vec2 size = vec2(uCharSize.x * len, uCharSize.y);
    gl_Position = vec4((pos + size * offset) * uCxy, 0, 1);
    vColor = aColor;
    vTexCoord = vec2(uCharSize.x * len * aVert.x, uCharSize.y - uCharSize.y * aVert.y);
    vNumbers = aNumbers;
})"sv });

        f = xx::LoadGLFragmentShader({ R"(#version 300 es
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;
uniform vec2 uCharSize;
uniform vec2 uNumberUVs[10];	// num char 0 ~ 9's uv

in vec2 vTexCoord;
in vec4 vColor;
in mat4 vNumbers;

out vec4 oColor;

void main() {
    int idx1 = int(vTexCoord.x / uCharSize.x);
    int rIdx = int(idx1 / 4);
    int cIdx = idx1 - rIdx * 4;
    int idx2 = int(vNumbers[rIdx][cIdx]);
    vec2 uv = uNumberUVs[ idx2 ] + mod(vTexCoord, uCharSize);
    oColor = vColor * texture(uTex0, uv / vec2(textureSize(uTex0, 0)));
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uCharSize = glGetUniformLocation(p, "uCharSize");
        uNumberUVs = glGetUniformLocation(p, "uNumberUVs");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aVert = glGetAttribLocation(p, "aVert");
        aPosScale = glGetAttribLocation(p, "aPosScale");
        aColor = glGetAttribLocation(p, "aColor");
        aNumbers = glGetAttribLocation(p, "aNumbers");
        CheckGLError();

        glGenVertexArrays(1, va.GetValuePointer());
        glBindVertexArray(va);

        glGenBuffers(1, (GLuint*)&ib);
        static const XY verts[4] = { { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f } };
        glBindBuffer(GL_ARRAY_BUFFER, ib);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(aVert, 2, GL_FLOAT, GL_FALSE, sizeof(XY), 0);
        glEnableVertexAttribArray(aVert);

        glGenBuffers(1, (GLuint*)&vb);
        glBindBuffer(GL_ARRAY_BUFFER, vb);

        glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(QuadInstanceNumbersData), 0);
        glVertexAttribDivisor(aPosScale, 1);
        glEnableVertexAttribArray(aPosScale);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(QuadInstanceNumbersData), (GLvoid*)offsetof(QuadInstanceNumbersData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        glVertexAttribPointer(aNumbers + 0, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(QuadInstanceNumbersData), (GLvoid*)(0 + offsetof(QuadInstanceNumbersData, numbers)));
        glVertexAttribDivisor(aNumbers + 0, 1);
        glEnableVertexAttribArray(aNumbers + 0);

        glVertexAttribPointer(aNumbers + 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(QuadInstanceNumbersData), (GLvoid*)(4 + offsetof(QuadInstanceNumbersData, numbers)));
        glVertexAttribDivisor(aNumbers + 1, 1);
        glEnableVertexAttribArray(aNumbers + 1);

        glVertexAttribPointer(aNumbers + 2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(QuadInstanceNumbersData), (GLvoid*)(8 + offsetof(QuadInstanceNumbersData, numbers)));
        glVertexAttribDivisor(aNumbers + 2, 1);
        glEnableVertexAttribArray(aNumbers + 2);

        glVertexAttribPointer(aNumbers + 3, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(QuadInstanceNumbersData), (GLvoid*)(12 + offsetof(QuadInstanceNumbersData, numbers)));
        glVertexAttribDivisor(aNumbers + 3, 1);
        glEnableVertexAttribArray(aNumbers + 3);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    inline void Shader_QuadInstanceNumbers::Begin() {
        assert(!gLooper.shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / gLooper.windowSize.x, 2 / gLooper.windowSize.y * gLooper.flipY);
        glUniform2f(uCharSize, ResTpFrames::_size_font_outline_32.x, ResTpFrames::_size_font_outline_32.y);
        XY uvs[10];
        uvs[0] = { ResTpFrames::_uvrect_font_outline_48.x, ResTpFrames::_uvrect_font_outline_48.y };
        uvs[1] = { ResTpFrames::_uvrect_font_outline_49.x, ResTpFrames::_uvrect_font_outline_49.y };
        uvs[2] = { ResTpFrames::_uvrect_font_outline_50.x, ResTpFrames::_uvrect_font_outline_50.y };
        uvs[3] = { ResTpFrames::_uvrect_font_outline_51.x, ResTpFrames::_uvrect_font_outline_51.y };
        uvs[4] = { ResTpFrames::_uvrect_font_outline_52.x, ResTpFrames::_uvrect_font_outline_52.y };
        uvs[5] = { ResTpFrames::_uvrect_font_outline_53.x, ResTpFrames::_uvrect_font_outline_53.y };
        uvs[6] = { ResTpFrames::_uvrect_font_outline_54.x, ResTpFrames::_uvrect_font_outline_54.y };
        uvs[7] = { ResTpFrames::_uvrect_font_outline_55.x, ResTpFrames::_uvrect_font_outline_55.y };
        uvs[8] = { ResTpFrames::_uvrect_font_outline_56.x, ResTpFrames::_uvrect_font_outline_56.y };
        uvs[9] = { ResTpFrames::_uvrect_font_outline_57.x, ResTpFrames::_uvrect_font_outline_57.y };
        glUniform2fv(uNumberUVs, 10, (GLfloat*)uvs);
        glBindVertexArray(va);
    }

    inline void Shader_QuadInstanceNumbers::End() {
        assert(gLooper.shader == this);
        if (quadCount) {
            Commit();
        }
    }

    inline void Shader_QuadInstanceNumbers::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(QuadInstanceNumbersData) * quadCount, quadInstanceNumbersDatas.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, gLooper.res._texid_font_outline_32);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, quadCount);

        CheckGLError();

        drawVerts += quadCount * 6;
        drawCall += 1;

        quadCount = 0;
    }

    inline QuadInstanceNumbersData* Shader_QuadInstanceNumbers::Draw(std::string_view sv) {
        assert(sv.size());
        assert(gLooper.shader == this);
        if (quadCount + 1 > maxQuadNums) {
            Commit();
        }
        auto r = &quadInstanceNumbersDatas[quadCount];
        quadCount += 1;

        r->pos = {};
        r->scale = 1;
        r->color = xx::RGBA8_White;

        auto buf = sv.data();
        auto len = sv.size();
        r->numbers[15] = (uint8_t)len;
        for (size_t i = 0; i < len; ++i) {
            r->numbers[i] = buf[i] - 48;
        }

        return r;
    }

    inline QuadInstanceNumbersData* Shader_QuadInstanceNumbers::Draw(uint32_t v) {
        return Draw(xx::ToString(v));
    }

    inline void Shader_QuadInstanceNumbers::Draw(QuadInstanceNumbersData const& data) {
        assert(gLooper.shader == this);
        if (quadCount + 1 > maxQuadNums) {
            Commit();
        }
        auto r = &quadInstanceNumbersDatas[quadCount];
        quadCount += 1;
        memcpy(r, &data, sizeof(data));
    }
}
