#pragma once

namespace Game {

    inline void Shader_QuadInstanceNumbers::Init() {

        v = xx::LoadGLVertexShader({ R"(#version 300 es
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aColor;
in ivec4 aNumbers;

out vec2 vTexCoord;
out vec4 vColor;
flat out ivec4 vNumbers;

void main() {
    vec2 pos = aPosScale.xy;
    vec2 scale = aPosScale.zw;
    vec2 offset = aVert * scale;
    float len = float(aNumbers.w >> 24);
    vec2 size = vec2(9.f * len, 13.f);
    gl_Position = vec4((pos + size * offset) * uCxy, 0.f, 1.f);
    vColor = aColor;
    vTexCoord = vec2(9.f * len * aVert.x, 13.f - 13.f * aVert.y);
    vNumbers = aNumbers;
})"sv });

        f = xx::LoadGLFragmentShader({ R"(#version 300 es
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
in vec4 vColor;
flat in ivec4 vNumbers;

out vec4 oColor;

void main() {
    uint i = uint(vTexCoord.x * 0.1111111f);        // / 9
    uint rIdx = (i >> 2);                           // / 4
    uint cIdx = i - (rIdx << 2);                    // * 4
    uint n4 = uint(vNumbers[rIdx]);
    uint n = (n4 >> (cIdx << 3)) & 0xffu;           // * 8
    vec2 uv = vec2(10.f * float(n) + vTexCoord.x - float(i) * 9.f, vTexCoord.y);
    //vec2 uv = vTexCoord;
    oColor = vColor * texture(uTex0, uv / vec2(120.f, 13.f));
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
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

        // "I" Pointer for int
        glVertexAttribIPointer(aNumbers, 4, GL_INT, sizeof(QuadInstanceNumbersData), (GLvoid*)offsetof(QuadInstanceNumbersData, numbers));
        glVertexAttribDivisor(aNumbers, 1);
        glEnableVertexAttribArray(aNumbers);

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

        glBindTexture(GL_TEXTURE_2D, gLooper.res_nums1->GetValue());
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
        auto tar = (uint8_t*)&r->numbers;
        for (size_t i = 0; i < len; ++i) {
            tar[i] = buf[i] - 48;
        }
        tar[15] = (uint8_t)len;
        return r;
    }

    inline QuadInstanceNumbersData* Shader_QuadInstanceNumbers::Draw(int32_t v, bool includeSignal) {
        std::string s;
        if (includeSignal) {
            if (v >= 0) {
                xx::Append(s, (uint32_t)v);
                s.push_back((char)58);
            }
            else {
                xx::Append(s, (uint32_t)-v);
                s.push_back((char)59);
            }
        }
        else {
            xx::Append(s, std::abs(v));
        }
        return Draw(s);
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
