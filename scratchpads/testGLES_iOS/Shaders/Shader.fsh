//
//  Shader.fsh
//  testGL
//
//  Created by Sébastien Métrot on 6/24/10.
//  Copyright MXP4 2010. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
