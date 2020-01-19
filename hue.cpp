#include "hue.h"

QByteArray createPicture(unsigned char * imageData, int fileSize, int imageWidth, int imageHeight)
{
    QByteArray result;

    BMPheader header;

    header.bfType           = 0x4d42;
    header.bfSize           = fileSize;
    header.bfReserved       = 0;
    header.bfOffBits        = 54;
    header.biSize           = 40;
    header.biWidth          = imageWidth;
    header.biHeight         = imageHeight;
    header.biPlanes         = 1;
    header.biBitCount       = 24;
    header.biCompression    = 0;
    header.biSizeImage      = imageWidth*imageHeight;
    header.biXPelsPerMeter  = 7559;
    header.biYPelsPerMeter  = 7559;
    header.biClrUsed        = 0;
    header.biClrImportant   = 0;

    result.append((char*)&header, sizeof(BMPheader));
    result.append(header.bfOffBits - sizeof(BMPheader), 0);
    result.append((char*)imageData, imageWidth*imageHeight*3);

    return result;
}

float Min( float fR, float fG, float fB )
{
    float fMin = fR;
    if (fG < fMin)
    {
        fMin = fG;
    }
    if (fB < fMin)
    {
        fMin = fB;
    }
    return fMin;
}


float Max( float fR, float fG, float fB)
{
    float fMax = fR;
    if (fG > fMax)
    {
        fMax = fG;
    }
    if (fB > fMax)
    {
        fMax = fB;
    }
    return fMax;
}

void HLSToRGB( unsigned char &R, unsigned char &G, unsigned char &B, double H, double S, double L )
{
    double Q = 0;
    double P = 0;
    double Hk = 0;

    L /= 100.0;
    //H /= 100.0;
    S /= 100.0;

    double Tr, Tg, Tb;

    if(L < 0.5) Q = L * (1.0 + S);
    else Q = L + S - (L * S);

    P = 2.0 * L - Q;
    Hk = H / 360.0;

    Tr = Hk + 1.0 / 3;
    Tg = Hk;
    Tb = Hk - 1.0 / 3;

    if (Tr < 0) Tr += 1.0;
    if (Tr > 1) Tr -= 1.0;

    if (Tg < 0) Tg += 1.0;
    if (Tg > 1) Tg -= 1.0;

    if (Tb < 0) Tb += 1.0;
    if (Tb > 1) Tb -= 1.0;

    if(Tr < 1.0/6.0)                  Tr = P + ((Q - P) * 6.0 * Tr);
    else
    if((Tr >= 1.0/6.0) && (Tr < 1.0/2.0)) Tr = Q;
    else
    if((Tr <= 1.0/2.0) && (Tr < 2.0/3.0)) Tr = P + ((Q - P) * 6.0 * (2.0/3 - Tr));
    else
        Tr = P;


    if(Tg < 1.0/6.0)                  Tg = P + ((Q - P) * 6.0 * Tg);
    else
    if((Tg >= 1.0/6.0) && (Tg < 1.0/2.0)) Tg = Q;
    else
    if((Tg >= 1.0/2.0) && (Tg < 2.0/3.0)) Tg = P + ((Q - P) * 6.0 * (2.0/3 - Tg));
    else
        Tg = P;

    if(Tb < 1.0/6.0)                  Tb = P + ((Q - P) * 6.0 * Tb);
    else
    if((Tb >= 1.0/6.0) && (Tb < 1.0/2.0)) Tb = Q;
    else
    if((Tb >= 1.0/2.0) && (Tb < 2.0/3.0)) Tb = P + ((Q - P) * 6.0 * (2.0/3 - Tb));
    else
        Tb = P;

    R = Tr * 255; G = Tg * 255; B = Tb * 255;
}

void RGBToHSL( int R, int G, int B, double& H, double& S, double& L )
{
    float fR = R / 255.0;
    float fG = G / 255.0;
    float fB = B / 255.0;


    float fCMin = Min(fR, fG, fB);
    float fCMax = Max(fR, fG, fB);


    L = 50 * (fCMin + fCMax);

    if (fCMin == fCMax)
    {
        S = 0;
        H = 0;
        return;

    }
    else if (L < 50)
    {
        S = 100 * (fCMax - fCMin) / (fCMax + fCMin);
    }
    else
    {
        S = 100 * (fCMax - fCMin) / (2.0 - fCMax - fCMin);
    }

    if (fCMax == fR)
    {
        H = 60 * (fG - fB) / (fCMax - fCMin);
    }
    if (fCMax == fG)
    {
        H = 60 * (fB - fR) / (fCMax - fCMin) + 120;
    }
    if (fCMax == fB)
    {
        H = 60 * (fR - fG) / (fCMax - fCMin) + 240;
    }
    if (H < 0)
    {
        H = H + 360;
    }
}

void filter(unsigned char * image, unsigned char * result, int width, int height, float value)
{
    double H, L, S;
    unsigned char R, G, B;

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j+=3)
        {
            RGBToHSL(image[i*width + j],image[i*width + j+1], image[i*width + j+2], H, S, L);
            HLSToRGB(R, G, B, H*value, S, L);

            if(R > 255) R = 255;
            if(G > 255) G = 255;
            if(B > 255) B = 255;

            result[i*width + j + 0] = R;
            result[i*width + j + 1] = G;
            result[i*width + j + 2] = B;
        }
}
