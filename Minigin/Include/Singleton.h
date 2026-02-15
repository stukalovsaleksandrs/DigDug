#ifndef SINGLETON_H
#define SINGLETON_H

namespace DAE
{
    template <typename T>
    class Singleton
    {
    public:
        [[nodiscard]] static T& GetInstance()
        {
            static T instance{};
            return instance;
        }

        virtual ~Singleton() = default;
        Singleton(Singleton const& other) = delete;
        Singleton(Singleton&& other) = delete;
        Singleton& operator=(Singleton const& other) = delete;
        Singleton& operator=(Singleton&& other) = delete;

    protected:
        Singleton() = default;

    };
}

#endif
