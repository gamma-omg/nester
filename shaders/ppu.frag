precision highp float;

uniform sampler2D texture;
varying vec2 tx;

void main(void)
{
    gl_FragColor = texture2D(texture, tx);
}
