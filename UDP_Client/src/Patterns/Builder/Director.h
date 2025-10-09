#pragma once

#include <memory>

#include "Patterns/Builder/Builder.h"

namespace dev
{
    class Director
    {
    protected:
        std::unique_ptr<Builder> m_builder;

    public:
        inline void setBuilder(std::unique_ptr<Builder>&& ptr)
        {
            m_builder = std::move(ptr);
        }

        virtual void buildProduct() const = 0;
    };
}