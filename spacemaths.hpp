#pragma once
#include <iostream>
#include <cmath>

using namespace std;

struct ivec2{
    int x,y;
};

class vec3
{
public:
    float values[3];
    vec3() {};
    vec3(float x, float y, float z)
    {
        values[0] = x;
        values[1] = y;
        values[2] = z;
    }
    float& operator[](int i)
    {
        return values[i];
    }
    vec3 operator+(vec3 v)
    {
        vec3 ret;
        for(int i = 0; i < 3; ++i)
        {
            ret[i] = values[i] + v[i];
        }
        return ret;
    }
    vec3 normalize()
    {
        float mag = sqrt(values[0]*values[0]+values[1]*values[1]+values[2]*values[2]);
        return vec3(values[0]/mag,values[1]/mag,values[2]/mag);
    }
    void print()
    {
        printf("%.2f %.2f %.2f\n", values[0],values[1],values[2]);
    }
};

class vec4
{
public:
    float values[4];
    vec4() {};
    vec4(float x, float y, float z, float w)
    {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = w;
    }
    vec4(float x, float y, float z)
    {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = 1;
    }
    float& operator[](int i)
    {
        return values[i];
    }
    vec4 operator+(vec4 v)
    {
        vec4 ret;
        for(int i = 0; i < 4; ++i)
        {
            ret[i] = values[i] + v[i];
        }
        return ret;
    }
    vec3 xyz()
    {
        return vec3(values[0], values[1], values[2]);
    }
};

class mat4
{
public:
    float values[16];
    mat4()
    {
        for(int i = 0; i < 16; i++)
            values[i] = 0;
    }
    mat4(float m11, float m12, float m13, // I'm sure there's a much better way to do this, but I don't know it yet
         float m21, float m22, float m23,
         float m31, float m32, float m33)
    {
        values[0] = m11; values[1] = m12; values[2] = m13; values[3] = 0;
        values[4] = m21; values[5] = m22; values[6] = m23; values[7] = 0;
        values[8] = m31; values[9] = m32; values[10]= m33; values[11]= 0;
        values[12]=   0; values[13]=   0; values[14]=   0; values[15]= 1;
    }
    float& operator[](ivec2 i) // I should get cpp 23 set up so I can avoid this
    {
        return values[4*i.x + i.y];
    }
    mat4 operator*(mat4 m)
    {
        mat4 ret;
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                for(int k = 0; k < 4; ++k)
                {
                    ret[ivec2{i,j}] += (*this)[ivec2{i,k}] * m[ivec2{k,j}];
                }
            }
        }
        return ret;
    }
    vec4 operator*(vec4 v)
    {
        vec4 ret;
        for(int i = 0; i < 4; ++i)
        {
            ret[i] = 0;
            for(int j = 0; j < 4; ++j)
            {
                ret[i] += (*this)[ivec2{i,j}] * v[j];
            }
        }
        return ret;
    }
    void print()
    {
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
                printf("%.2f ", (*this)[ivec2{i,j}]);
            cout << endl;
        }
    }
};

mat4 yaw(float y)
{
    return mat4(cos(y), 0,  -sin(y),
                0,      1,  0,
                sin(y), 0,  cos(y));
}

mat4 pitch(float p)
{
    return mat4(1, 0, 0,
                0, cos(p), -sin(p),
                0, sin(p), cos(p));
}

mat4 perspective(float fov, float ar, float near, float far)
{
    float rads = fov * M_PI / 180.0;
    mat4 ret;
    ret[ivec2{0,0}] = 1/tan(rads/2);
    ret[ivec2{1,1}] = 1/(ar*tan(rads/2));
    ret[ivec2{2,2}] = (near + far) / (near - far);
    ret[ivec2{2,3}] = 2*near*far / (near - far);
    ret[ivec2{3,2}] = -1;
    return ret;
}

mat4 translate(float x, float y, float z)
{
    mat4 ret;
    for(int i = 0; i < 4; ++i)
        ret[ivec2{i,i}] = 1;
    ret[ivec2{0,3}]=x;
    ret[ivec2{1,3}]=y;
    ret[ivec2{2,3}]=z;
    return ret;
}