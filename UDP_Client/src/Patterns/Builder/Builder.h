#pragma once

namespace dev
{
    class Builder
    {
    public:
        virtual void reset() = 0;
        virtual ~Builder() = default;
    };
}