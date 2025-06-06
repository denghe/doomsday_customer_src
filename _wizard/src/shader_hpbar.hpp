﻿#pragma once

namespace Game {

    inline void Shader_QuadInstanceHPBar::Init() {

        v = xx::LoadGLVertexShader({ R"(#version 300 es
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aColor;
in ivec4 aHPBar;
in float aValue;

out vec2 vTexCoord;
flat out vec4 vColor;
flat out ivec4 vHPBar;
flat out float vNumLen;
flat out float vValueLen;

void main() {
    vec2 pos = aPosScale.xy;
    vec2 scale = aPosScale.zw;
    vec2 offset = aVert * scale;
    vNumLen = float(aHPBar.w >> 24) * 9.f;
    gl_Position = vec4((pos + vec2(110.f, -15.f) * offset) * uCxy, 0.f, 1.f);
    vColor = aColor;
    vTexCoord = vec2(110.f * aVert.x, 15.f * aVert.y);
    vHPBar = aHPBar;
    vValueLen = aValue * 110.f;
})"sv });

        f = xx::LoadGLFragmentShader({ R"(#version 300 es
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;

in vec2 vTexCoord;
flat in vec4 vColor;
flat in ivec4 vHPBar;
flat in float vNumLen;
flat in float vValueLen;

out vec4 oColor;

void main() {
    float uvY = vTexCoord.y / 47.f;
    // oColor = texture(uTex0, vec2(vTexCoord.x / 110.f, uvY + (16.f + step(vTexCoord.x, vValueLen) * 16.f) / 47.f));
    vec4 c;
    if (vTexCoord.x > 2.f && vTexCoord.x < vNumLen + 2.f) {         // left margin: 2px
        float x = vTexCoord.x - 2.f;
        uint i = uint(x * 0.1111111f);                  // / 9
        uint rIdx = (i >> 2);                           // / 4
        uint cIdx = i - (rIdx << 2);                    // * 4
        uint n4 = uint(vHPBar[rIdx]);
        uint n = (n4 >> (cIdx << 3)) & 0xffu;           // * 8
        vec2 uv = vec2((10.f * float(n) + x - float(i) * 9.f) / 110.f, uvY);
        c = texture(uTex0, uv);
    }
    if (c.w == 0.f) {
        if (vTexCoord.x < vValueLen) {
            c = vColor * texture(uTex0, vec2(vTexCoord.x / 110.f, uvY + 32.f / 47.f));
            if (c.w == 0.f) {
                c = texture(uTex0, vec2(vTexCoord.x / 110.f, uvY + 16.f / 47.f));
            }
        } else {
            c = texture(uTex0, vec2(vTexCoord.x / 110.f, uvY + 16.f / 47.f));
        }
    }
    oColor = c;
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");

        aVert = glGetAttribLocation(p, "aVert");
        aPosScale = glGetAttribLocation(p, "aPosScale");
        aColor = glGetAttribLocation(p, "aColor");
        aHPBar = glGetAttribLocation(p, "aHPBar");
        aValue = glGetAttribLocation(p, "aValue");
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

        glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(QuadInstanceHPBarData), 0);
        glVertexAttribDivisor(aPosScale, 1);
        glEnableVertexAttribArray(aPosScale);

        glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(QuadInstanceHPBarData), (GLvoid*)offsetof(QuadInstanceHPBarData, color));
        glVertexAttribDivisor(aColor, 1);
        glEnableVertexAttribArray(aColor);

        // "I" Pointer for int
        glVertexAttribIPointer(aHPBar, 4, GL_INT, sizeof(QuadInstanceHPBarData), (GLvoid*)offsetof(QuadInstanceHPBarData, numbers));
        glVertexAttribDivisor(aHPBar, 1);
        glEnableVertexAttribArray(aHPBar);

        glVertexAttribPointer(aValue, 1, GL_FLOAT, GL_FALSE, sizeof(QuadInstanceHPBarData), (GLvoid*)offsetof(QuadInstanceHPBarData, value));
        glVertexAttribDivisor(aValue, 1);
        glEnableVertexAttribArray(aValue);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    inline void Shader_QuadInstanceHPBar::Begin() {
        assert(!gLooper.shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0/* + textureUnit*/);
        glUniform1i(uTex0, 0);
        glUniform2f(uCxy, 2 / gLooper.windowSize.x, 2 / gLooper.windowSize.y * gLooper.flipY);
        glBindVertexArray(va);
    }

    inline void Shader_QuadInstanceHPBar::End() {
        assert(gLooper.shader == this);
        if (quadCount) {
            Commit();
        }
    }

    inline void Shader_QuadInstanceHPBar::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(QuadInstanceHPBarData) * quadCount, quadInstanceHPBarDatas.get(), GL_STREAM_DRAW);

        glBindTexture(GL_TEXTURE_2D, gLooper.res_hpbar->GetValue());
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, quadCount);

        CheckGLError();

        drawVerts += quadCount * 6;
        drawCall += 1;

        quadCount = 0;
    }

    inline QuadInstanceHPBarData* Shader_QuadInstanceHPBar::Draw(std::string_view sv) {
        assert(sv.size());
        assert(gLooper.shader == this);
        if (quadCount + 1 > maxQuadNums) {
            Commit();
        }
        auto r = &quadInstanceHPBarDatas[quadCount];
        quadCount += 1;

        r->pos = {};
        r->scale = 1;
        r->color = xx::RGBA8_White;
        r->value = 1;

        auto buf = sv.data();
        auto len = sv.size();
        auto tar = (uint8_t*)&r->numbers;
        for (size_t i = 0; i < len; ++i) {
            tar[i] = buf[i] - 48;
        }
        tar[15] = (uint8_t)len;
        return r;
    }

    inline QuadInstanceHPBarData* Shader_QuadInstanceHPBar::Draw(uint32_t v) {
        return Draw(xx::ToString(v));
    }

    inline QuadInstanceHPBarData* Shader_QuadInstanceHPBar::Draw(uint32_t v1, uint32_t v2) {
        auto s = xx::ToString(v1);
        s.push_back((char)58);
        xx::Append(s, v2);
        auto r = Draw(s);
        r->value = (float)v1 / v2;
        return r;
    }

    inline void Shader_QuadInstanceHPBar::Draw(QuadInstanceHPBarData const& data) {
        assert(gLooper.shader == this);
        if (quadCount + 1 > maxQuadNums) {
            Commit();
        }
        auto r = &quadInstanceHPBarDatas[quadCount];
        quadCount += 1;
        memcpy(r, &data, sizeof(data));
    }
}
