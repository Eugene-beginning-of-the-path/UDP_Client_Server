#pragma once

//CRTP Singleton
namespace dev::patrn
{
    template <typename T>
    class Singleton
    {
    public:
        template <typename ... Args>
        static T& Instance(Args&&... args)
        {
            static T once(std::forward<Args>(args)...);
            return once;
        }

    protected:
        Singleton() = default;
        ~Singleton() = default;

        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;

        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&) = delete;
    };
}
