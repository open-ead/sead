#include <cmath>
#include <sead.h>

namespace sead { namespace PrimitiveRendererUtil {

void setQuadVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[4] = {
        Vertex( Vector3f(-0.5f,  0.5f, 0.0f), Vector2f(0.0f, 1.0f), Color4f(0.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f( 0.5f,  0.5f, 0.0f), Vector2f(1.0f, 1.0f), Color4f(0.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f(-0.5f, -0.5f, 0.0f), Vector2f(0.0f, 0.0f), Color4f(1.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f( 0.5f, -0.5f, 0.0f), Vector2f(1.0f, 0.0f), Color4f(1.0f, 0.0f, 0.0f, 0.0f) )
    };

    static const u16 cIdx[6] = {
        0, 2, 1,
        1, 2, 3
    };

    if (vtx != NULL)
        memcpy(vtx, cVtx, sizeof(cVtx));

    if (idx != NULL)
        memcpy(idx, cIdx, sizeof(cIdx));
}

void setLineVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[2] = {
        Vertex( Vector3f(-0.5f,  0.0f, 0.0f), Vector2f(0.0f, 0.5f), Color4f(0.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f( 0.5f,  0.0f, 0.0f), Vector2f(1.0f, 0.5f), Color4f(1.0f, 0.0f, 0.0f, 0.0f) )
    };

    static const u16 cIdx[2] = {
        0, 1
    };

    if (vtx != NULL)
        memcpy(vtx, cVtx, sizeof(cVtx));

    if (idx != NULL)
        memcpy(idx, cIdx, sizeof(cIdx));
}

void setCubeVertex(Vertex* vtx, u16* idx)
{
    static const Vertex cVtx[8] = {
        Vertex( Vector3f(-0.5f, -0.5f, -0.5f), Vector2f(0.0f, 0.0f), Color4f(1.0f / 3.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f(-0.5f,  0.5f, -0.5f), Vector2f(0.0f, 1.0f), Color4f(       0.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f(-0.5f,  0.5f,  0.5f), Vector2f(1.0f, 1.0f), Color4f(1.0f / 3.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f(-0.5f, -0.5f,  0.5f), Vector2f(1.0f, 0.0f), Color4f(2.0f / 3.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f( 0.5f, -0.5f,  0.5f), Vector2f(0.0f, 0.0f), Color4f(       1.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f( 0.5f,  0.5f,  0.5f), Vector2f(0.0f, 1.0f), Color4f(2.0f / 3.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f( 0.5f,  0.5f, -0.5f), Vector2f(1.0f, 1.0f), Color4f(1.0f / 3.0f, 0.0f, 0.0f, 0.0f) ),
        Vertex( Vector3f( 0.5f, -0.5f, -0.5f), Vector2f(1.0f, 0.0f), Color4f(2.0f / 3.0f, 0.0f, 0.0f, 0.0f) )
    };

    static const u16 cIdx[36] = {
        2, 1, 0, 3,
        2, 0, 5, 2,
        3, 4, 5, 3,
        6, 5, 7, 7,
        5, 4, 1, 6,
        0, 6, 7, 0,
        0, 7, 3, 3,
        7, 4, 1, 2,
        6, 2, 5, 6
    };

    if (vtx != NULL)
        memcpy(vtx, cVtx, sizeof(cVtx));

    if (idx != NULL)
        memcpy(idx, cIdx, sizeof(cIdx));
}

void setWireCubeVertex(Vertex* vtx, u16* idx)
{
    setCubeVertex(vtx, NULL);

    static const u16 cIdx[17] = {
        0, 1, 2,
        3, 0, 7,
        6, 1, 2,
        5, 6, 7,
        4, 5, 4,
        3, 0

    };

    if (idx != NULL)
        memcpy(idx, cIdx, sizeof(cIdx));
}

void setSphereVertex(Vertex* vtx, u16* idx, s32 x, s32 y)
{
    if (vtx != NULL) {
        for (int i = 0; i < y; i++)
        {
            f32 angle_y = ((i + 1) / (y + 1.0f) - 0.5f) * M_PI;

            f32 sy_2 = sinf(angle_y) * 0.5f;
            f32 cy_2 = cosf(angle_y) * 0.5f;

            for (int j = 0; j < x; j++)
            {
                f32 angle_x = (j * M_PI * 2.0f) / x;
                if ((i & 1) == 0)
                    angle_x = angle_x - ((M_PI * 2.0f / x) / 2.0f);

                vtx[i * x + j].pos.x = cosf(angle_x) * cy_2;
                vtx[i * x + j].pos.y = sy_2;
                vtx[i * x + j].pos.z = sinf(angle_x) * cy_2;
                vtx[i * x + j].uv.x = sy_2 + 0.5f;
                vtx[i * x + j].uv.y = static_cast<f32>(j) / x;
                vtx[i * x + j].color.r = 0.5f - sy_2;
            }
        }

        vtx[x * y].pos.x = 0.0f;
        vtx[x * y].pos.y = -0.5f;
        vtx[x * y].pos.z = 0.0f;
        vtx[x * y].uv.x = 0.0f;
        vtx[x * y].uv.y = 0.5f;
        vtx[x * y].color.r = 1.0f;

        vtx[x * y + 1].pos.x = 0.0f;
        vtx[x * y + 1].pos.y = 0.5f;
        vtx[x * y + 1].pos.z = 0.0f;
        vtx[x * y + 1].uv.x = 1.0f;
        vtx[x * y + 1].uv.y = 0.5f;
        vtx[x * y + 1].color.r = 0.0f;
    }

    if (idx != NULL)
    {
        for (int i = 0; i < x; i++)
        {
          idx[i * 3 + 0] = x * y;
          idx[i * 3 + 1] = i;
          idx[i * 3 + 2] = (i + 1) - ((i + 1) / x) * x;
        }

        for (int i = 0; i < y - 1; i++)
        {
            for (int j = 0; j < x; j++)
            {
                int v = i - (i - (i >> 31) & (u32)~1);
                idx[((i * x + j) * 2 + x) * 3 + 0] = i * x + j;
                idx[((i * x + j) * 2 + x) * 3 + 1] = (i + 1) * x + ((j + v) - ((j + v) / x) * x);
                idx[((i * x + j) * 2 + x) * 3 + 2] = i * x + ((j + 1) - ((j + 1) / x) * x);
                idx[((i * x + j) * 2 + x) * 3 + 3] = (i + 1) * x + ((j + v) - ((j + v) / x) * x);
                v += j + 1;
                idx[((i * x + j) * 2 + x) * 3 + 4] = (i + 1) * x + (v - (v / x) * x);
                idx[((i * x + j) * 2 + x) * 3 + 5] = i * x + ((j + 1) - ((j + 1) / x) * x);
            }
        }

        for (int i = 0; i < x; i++)
        {
            idx[i * 3 + 0 + (x * 3) * ((y - 1) * 2 + 1)] = x * y + 1;
            idx[i * 3 + 1 + (x * 3) * ((y - 1) * 2 + 1)] = x * (y - 1) + ((i + 1) - ((i + 1) / x) * x);
            idx[i * 3 + 2 + (x * 3) * ((y - 1) * 2 + 1)] = x * (y - 1) + i;
        }
    }
}

void setDiskVertex(Vertex* vtx, u16* idx, s32 div)
{
    if (vtx != NULL)
    {
        for (int i = 0; i < div; i++)
        {
            f32 angle = (i * M_PI * 2.0f) / div;

            vtx[i].pos.x = cosf(angle) * 0.5f;
            vtx[i].pos.y = sinf(angle) * 0.5f;
            vtx[i].pos.z = 0.0f;
            vtx[i].uv.x = vtx[i].pos.x;
            vtx[i].uv.y = 1.0f - vtx[i].pos.y;
            vtx[i].color.r = 1.0f;
        }

        vtx[div].pos.x = 0.0f;
        vtx[div].pos.y = 0.0f;
        vtx[div].pos.z = 0.0f;
        vtx[div].uv.x = 0.5f;
        vtx[div].uv.y = 0.5f;
        vtx[div].color.r = 0.0f;
    }

    if (idx != NULL)
        for (int i = 0; i < div; i++)
        {
            idx[i * 3 + 0] = i;
            idx[i * 3 + 1] = (i + 1) - ((i + 1) / div) * div;
            idx[i * 3 + 2] = div;
        }
}

void setCylinderVertex(Vertex* vtx, u16* idx, s32 div)
{
    if (vtx != NULL)
    {
        for (int i = 0; i < div; i++)
        {
            f32 angle = (i * M_PI * 2.0f) / div;

            vtx[i].pos.x = cosf(angle) * 0.5f;
            vtx[i].pos.z = -sinf(angle) * 0.5f;
            vtx[i].pos.y = 0.5f;
            vtx[i].uv.x = vtx[i].pos.x;
            vtx[i].uv.y = 1.0f - vtx[i].pos.z;
            vtx[i].color.r = 0.0f;
            vtx[i + div + 1].pos.x = cosf(angle) * 0.5f;
            vtx[i + div + 1].pos.z = -sinf(angle) * 0.5f;
            vtx[i + div + 1].pos.y = -0.5f;
            vtx[i + div + 1].uv.x = vtx[i].pos.x;
            vtx[i + div + 1].uv.y = 1.0f - vtx[i].pos.z;
            vtx[i + div + 1].color.r = 1.0f;
        }

        vtx[div].pos.x = 0.0f;
        vtx[div].pos.y = 0.5f;
        vtx[div].pos.z = 0.0f;
        vtx[div].uv.x = 0.5f;
        vtx[div].uv.y = 0.5f;
        vtx[div].color.r = 0.0f;
        vtx[div + div + 1].pos.x = 0.0f;
        vtx[div + div + 1].pos.y = -0.5f;
        vtx[div + div + 1].pos.z = 0.0f;
        vtx[div + div + 1].uv.x = 0.5f;
        vtx[div + div + 1].uv.y = 0.5f;
        vtx[div + div + 1].color.r = 1.0f;
    }

    if (idx != NULL)
    {
        for (int i = 0; i < div; i++)
        {
            idx[i * 3 + 0] = i;
            idx[i * 3 + 1] = (i + 1) - ((i + 1) / div) * div;
            idx[i * 3 + 2] = div;
            idx[i * 3 + div * 3 + 0] = i + (div + 1);
            idx[i * 3 + div * 3 + 1] = div + (div + 1);
            idx[i * 3 + div * 3 + 2] = ((i + 1) - ((i + 1) / div) * div) + (div + 1);
        }

        for (int i = 0; i < div; i++)
        {
            idx[i * 6 + div * 6 + 0] = i;
            idx[i * 6 + div * 6 + 1] = i + (div + 1);
            idx[i * 6 + div * 6 + 2] = (i + 1) - ((i + 1) / div) * div;
            idx[i * 6 + div * 6 + 3] = (i + 1) - ((i + 1) / div) * div;
            idx[i * 6 + div * 6 + 4] = i + (div + 1);
            idx[i * 6 + div * 6 + 5] = ((i + 1) - ((i + 1) / div) * div) + (div + 1);
        }
    }
}

} } // namespace sead::PrimitiveRendererUtil
