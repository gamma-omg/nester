attribute vec4 vertex;
attribute vec4 texcoord;
uniform mat4 mvp;
varying vec2 tx;

void main(void)
{
    gl_Position = mvp * vertex;
    tx = texcoord.xy;
}
