#version 100
precision mediump float;

attribute vec4 position;
attribute vec2 texCoord;

varying vec2 v_TexCoord;

void main() {
    gl_Position = position;
    v_TexCoord = texCoord;
}
