#pragma once

namespace Game {

    inline void Shader_QuadInstanceTiles::Init() {

        v = xx::LoadGLVertexShader({ R"(#version 300 es
uniform vec2 uCxy;	            // screen center coordinate

in vec2 aVert;	// fans index { 0, 0 }, { 0, 1.f }, { 1.f, 0 }, { 1.f, 1.f }

in vec4 aPosScale;
in vec4 aTileSizeMapSize;
in vec4 aTilingOffset;
in vec2 aMiniMapSize;

out vec2 vTexCoord;
flat out vec4 vTileSizeOffset;
flat out ivec2 vMiniMapSize;

void main() {
    gl_Position = vec4((aPosScale.xy + aTileSizeMapSize.zw * aVert * aPosScale.zw) * uCxy, 0.f, 1.f);
    vTexCoord = vec2(aTileSizeMapSize.z * aVert.x, aTileSizeMapSize.w - aTileSizeMapSize.w * aVert.y) * aTilingOffset.xy;
    vTileSizeOffset = vec4( aTileSizeMapSize.xy, aTilingOffset.zw );
    vMiniMapSize = ivec2(aMiniMapSize);
})"sv });

        f = xx::LoadGLFragmentShader({ R"(#version 300 es
precision highp float;          // mediump draw border has issue
uniform sampler2D uTex0;        // tiles
uniform sampler2D uTex1;        // minimap indexs[]     .xy: u  .zw: v

in vec2 vTexCoord;
flat in vec4 vTileSizeOffset;
flat in ivec2 vMiniMapSize;

out vec4 oColor;

void main() {
    vec2 tileTexSize = vec2(textureSize(uTex0, 0));
    vec2 miniTexSize = vec2(textureSize(uTex1, 0));
    vec2 tileSize = vTileSizeOffset.xy;
    vec2 offset = vTileSizeOffset.zw;
    vec2 p = vTexCoord + offset;
    ivec2 n = ivec2(p / tileSize);
    vec2 miniIdx = vec2(n - (n / vMiniMapSize) * vMiniMapSize);
    vec4 c = texture(uTex1, miniIdx / miniTexSize);
    ivec4 i = ivec4(c * 256.f);
    vec2 uv = vec2(float(i.x + i.y * 256), float(i.z + i.w * 256)) * tileSize;
    vec2 uvOffset = p - tileSize * miniIdx;
    oColor = texture(uTex0, (uv + uvOffset) / tileTexSize);
})"sv });

        p = LinkGLProgram(v, f);

        uCxy = glGetUniformLocation(p, "uCxy");
        uTex0 = glGetUniformLocation(p, "uTex0");
        uTex1 = glGetUniformLocation(p, "uTex1");

        aVert = glGetAttribLocation(p, "aVert");
        aPosScale = glGetAttribLocation(p, "aPosScale");
        aTileSizeMapSize = glGetAttribLocation(p, "aTileSizeMapSize");
        aTilingOffset = glGetAttribLocation(p, "aTilingOffset");
        aMiniMapSize = glGetAttribLocation(p, "aMiniMapSize");
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

        glVertexAttribPointer(aPosScale, 4, GL_FLOAT, GL_FALSE, sizeof(QuadInstanceTilesData), 0);
        glVertexAttribDivisor(aPosScale, 1);
        glEnableVertexAttribArray(aPosScale);

        glVertexAttribPointer(aTileSizeMapSize, 4, GL_FLOAT, GL_FALSE, sizeof(QuadInstanceTilesData), (GLvoid*)offsetof(QuadInstanceTilesData, tileSize));
        glVertexAttribDivisor(aTileSizeMapSize, 1);
        glEnableVertexAttribArray(aTileSizeMapSize);

        glVertexAttribPointer(aTilingOffset, 4, GL_FLOAT, GL_FALSE, sizeof(QuadInstanceTilesData), (GLvoid*)offsetof(QuadInstanceTilesData, tiling));
        glVertexAttribDivisor(aTilingOffset, 1);
        glEnableVertexAttribArray(aTilingOffset);

        glVertexAttribPointer(aMiniMapSize, 2, GL_FLOAT, GL_FALSE, sizeof(QuadInstanceTilesData), (GLvoid*)offsetof(QuadInstanceTilesData, miniMapSize));
        glVertexAttribDivisor(aMiniMapSize, 1);
        glEnableVertexAttribArray(aMiniMapSize);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        CheckGLError();
    }

    inline void Shader_QuadInstanceTiles::Begin() {
        assert(!gLooper.shader);
        glUseProgram(p);
        glActiveTexture(GL_TEXTURE0 + 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glUniform1i(uTex0, 0);
        glUniform1i(uTex1, 1);
        glUniform2f(uCxy, 2 / gLooper.windowSize.x, 2 / gLooper.windowSize.y * gLooper.flipY);
        glBindVertexArray(va);
    }

    inline void Shader_QuadInstanceTiles::End() {
        assert(gLooper.shader == this);
        if (quadCount) {
            Commit();
        }
    }

    inline void Shader_QuadInstanceTiles::Commit() {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(QuadInstanceTilesData) * quadCount, quadInstanceTilesDatas.get(), GL_STREAM_DRAW);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, lastTextureId);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, lastLightTextureId);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, quadCount);

        CheckGLError();

        drawVerts += quadCount * 6;
        drawCall += 1;

        quadCount = 0;
    }

    inline QuadInstanceTilesData* Shader_QuadInstanceTiles::Draw(GLuint texId_Tiles, GLuint texId_Mini) {
        assert(gLooper.shader == this);
        if (quadCount + 1 > maxQuadNums
            || (lastTextureId && lastTextureId != texId_Tiles)
            || (lastLightTextureId && lastLightTextureId != texId_Mini)) {
            Commit();
        }
        lastTextureId = texId_Tiles;
        lastLightTextureId = texId_Mini;
        auto r = &quadInstanceTilesDatas[quadCount];
        quadCount += 1;
        return r;
    }
    inline QuadInstanceTilesData* Shader_QuadInstanceTiles::Draw(xx::Ref<xx::GLTexture> const& texTiles, xx::Ref<xx::GLTexture> const& texMini) {
        return Draw(texTiles->GetValue(), texMini->GetValue());
    }
}
