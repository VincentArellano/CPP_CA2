//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <valarray>
#include "Image.h"



bool Image::load(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) {
            throw("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3]; // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }
    return true;
}
bool Image::loadRaw(string filename)
{
    return false;
}
bool Image::savePPM(string filename)
{
    std::ofstream ofs;
    ofs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ofs.fail()) {
            throw("Can't open output file");
        }
        ofs << "P6\n" << w << " " << h << "\n255\n";
        unsigned char pix[3]; // convert floats to bytes
        for (int i = 0; i < w * h; ++i) {
            pix[0] = static_cast<unsigned char>(this->pixels[i].r);
            pix[1] = static_cast<unsigned char>(this->pixels[i].g);
            pix[2] = static_cast<unsigned char>(this->pixels[i].b);
            ofs.write(reinterpret_cast<char *>(pix), 3);
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }
    return true;
}


void Image::filterRed()
{
    for (int i = 0; i < w * h; ++i) {
        pixels[i].b = 0;
        pixels[i].g = 0;
    }
}
void Image::filterGreen()
{
    for (int i = 0; i < w * h; ++i) {
        pixels[i].r = 0;
        pixels[i].b = 0;
    }
}
void Image::filterBlue()
{
    for (int i = 0; i < w * h; ++i) {
        pixels[i].r = 0;
        pixels[i].g = 0;
    }
}
void Image::greyScale()
{
    for (int i = 0; i < w * h; ++i) {
        pixels[i].r = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].g = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].b = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
    }
}
void Image::flipHorizontal()
{
    for (int i = 0; i < h; ++i) {
        for(int j = 0; j < w/2; ++j) {
            int temp = pixels[i * w + j].r;
            pixels[i * w + j].r = pixels[i * w + w - j - 1].r;
            pixels[i * w + w - j - 1].r = temp;
            temp = pixels[i * w + j].g;
            pixels[i * w + j].g = pixels[i * w + w - j - 1].g;
            pixels[i * w + w - j - 1].g = temp;
            temp = pixels[i * w + j].b;
            pixels[i * w + j].b = pixels[i * w + w - j - 1].b;
            pixels[i * w + w - j - 1].b = temp;
        }
    }

}
void Image::flipVertically()
{
    for(int j = 0; j < w; ++j) {
        for(int i = 0; i < h/2; ++i) {
            int temp = pixels[i * w + j].r;
            pixels[i * w + j].r = pixels[(h - i - 1) * w + j].r;
            pixels[(h - i - 1) * w + j].r = temp;
            temp = pixels[i * w + j].g;
            pixels[i * w + j].g = pixels[(h - i - 1) * w + j].g;
            pixels[(h - i - 1) * w + j].g = temp;
            temp = pixels[i * w + j].b;
            pixels[i * w + j].b = pixels[(h - i - 1) * w + j].b;
            pixels[(h - i - 1) * w + j].b = temp;
        }
    }

}
void Image::AdditionalFunction2()
{
    //inverted colours
    for (int i = 0; i < w*h; ++i)
    {
        pixels[i].r = 255 - pixels[i].r;
        pixels[i].g = 255 - pixels[i].g;
        pixels[i].b = 255 - pixels[i].b;
    }
}
void Image::AdditionalFunction3()
{
    //image contrast
    for(int i = 0; i< w*h; i++)
    {
        this->pixels[i].r = this->pixels[i].r/-127.5;
        this->pixels[i].g = this->pixels[i].g/-127.5;
        this->pixels[i].b = this->pixels[i].b/-127.5;
    }
}
void Image::AdditionalFunction1()
{
    //darken the image
    for(int i = 0; i< w * h; i++)
    {
        this->pixels[i].r = this->pixels[i].r/2.5;
        this->pixels[i].g =this->pixels[i].g/2.5;
        this->pixels[i].b = this->pixels[i].b/2.5;
    }
}

void Image::GammaEncoding() {
    for(int i = 0; i< w*h; i++)
    {
        float gamma = 1 / 2.5;

        pixels[i].r = pow(pixels[i].r / 255.0f, gamma) * 255;
        pixels[i].g = pow(pixels[i].g / 255.0f, gamma) * 255;
        pixels[i].b = pow(pixels[i].b / 255.0f, gamma) * 255;
    }
}

void Image::OtherAdvancedFeature() {
    //gaussian blur
    for(int i = 0; i < h; ++i)
    {
        for(int f = 0; f < w; ++f)
        {
            float redTotal = 0.0, blueTotal = 0.0, greenTotal = 0.0;
            for(int a = -1; a <= 1; ++a)
            {
                for(int b = -1; b <= 1; ++b)
                {
                    if(i + a >= 0 && i + a < h && f + b >= 0 && f + b < w)
                    {
                        redTotal += pixels[(i + a) * w + (f + b)].r;
                        greenTotal += pixels[(i + a) * w + (f + b)].g;
                        blueTotal += pixels[(i + a) * w + (f + b)].b;
                    }
                }
            }
            pixels[i * w + f].r = redTotal / 10;
            pixels[i * w + f].g = greenTotal / 10;
            pixels[i * w + f].b = blueTotal / 10;
        }
    }
}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

Rgb* Image::getImage()
{
    return pixels;
}