#pragma once

namespace ECS {

    struct RectF {
        float x1, y1, h1, w1;
    };

    inline bool rectIntersects(const RectF &a, const RectF &b)
    {
        return !(a.x1+a.w1<b.x1 || b.x1+b.w1<a.x1 || a.y1+a.h1<b.y1 || b.y1+b.h1<a.y1);
    }

};