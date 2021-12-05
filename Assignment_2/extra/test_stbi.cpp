#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int main() {
    std::cout << "Hello, STB_Image" << std::endl;

    string inputPath = "../data/night_negx.png";
    int iw, ih, n;
    
    // 加载图片获取宽、高、颜色通道信息
    unsigned char *idata = stbi_load(inputPath.c_str(), &iw, &ih, &n, 0);

    int ow = iw;
    int oh = ih;
    //auto *odata = (unsigned char *) malloc(ow * oh * n);
    
    // 改变图片尺寸


    string outputPath = "../data/output.png";
    // 写入图片
    stbi_write_png(outputPath.c_str(), ow, oh, n, idata, 0);

    stbi_image_free(idata);
    //stbi_image_free(odata);
    return 0;
}