#ifndef SE_SINGLETON_H
#define SE_SINGLETON_H

namespace Engine
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

#endif// SE_SINGLETON_H
