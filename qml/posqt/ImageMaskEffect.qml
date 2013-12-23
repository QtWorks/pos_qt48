import QtQuick 1.1

ShaderEffect {
    id: maskEffect;
    property variant image: null;
    property variant mask: null;

    fragmentShader:
        "
            variantying highp vec2 qt_TexCoord0;
            uniform sampler2D image;
            uniform sampler2D mask;
            void main(void)
            {
                gl_FragColor = texture2D(image, qt_TexCoord0.st) * (texture2D(mask, qt_TexCoord0.st).a);
            }
        "
}
