/***********************************************************************
    说    明： 单例模式的泛型实现。本单元提供两个泛型单例类：

                分别实现了两个语义上的单例， 其中：

                1. 类CiaSingleInstance<T>跟T是is a的关系，应用此模式则说明T类是可继承的，所以T类必需满足如下条件：
                1.1 默认构造函数和析构函数是public或protected的访问级别
                1.2 析构函数必需是virtual的

                2. 类ChaSingleInstance<T>跟T是has a的关系，即关联关系，应用此模式则说明T是ChaSingleInstance<T>里
                的一个静态成员变量类型，所以T类必需满足如下条件：
                2.1 默认构造函数和析构函数是public的访问级别

                建议：1. 优先考虑使用ChaSingleInstance<T>
                2. typedef 将使代码更具可读性，
                如：typedef ChaSingleInstance<T> TSingleInstance;
***********************************************************************/
#pragma once


template <typename T>
class CiaSingleton final : public T
{
public:
    typedef T SingletonType;

public:
    // 获取单例
    static SingletonType * singleton();
    // 释放单例
    static void release();

private:
    // 单例实体指针
    static SingletonType *sm_pSingleInstance;

private:
    CiaSingleton(const CiaSingleton &) = delete;
    const CiaSingleton & operator=(const CiaSingleton &) = delete;

    // 此类不可继承，不可实例化
    CiaSingleton() {};
    ~CiaSingleton() override {};

private:
    struct Init
    {
        Init() {
            CiaSingleton<T>::singleton();
        }
    };

    struct destroy
    {
        ~destroy() {
            if (nullptr == sm_pSingleInstance) {
                return;
            }

            delete sm_pSingleInstance;
            sm_pSingleInstance = nullptr;
        }
    };

private:
    static Init sm_init;
};

template <typename T>
class ChaSingleton final
{
public:
    typedef T SingleInstanceType;

    // 获取单例
    static SingleInstanceType * singleton();
    // 释放单例
    static void release();

private:
    // 单例实体指针
    static SingleInstanceType *sm_pSingleInstance;

private:
    ChaSingleton(const ChaSingleton &) = delete;
    const ChaSingleton & operator=(const ChaSingleton &) = delete;

    // 此类不可继承，不可实例化
    ChaSingleton()  = delete;
    ~ChaSingleton() = delete;

private:
    struct Init
    {
        Init() {
            ChaSingleton<T>::singleton();
        }
    };

    struct Destroy
    {
        ~Destroy() {
            if (nullptr == sm_pSingleInstance) {
                return;
            }

            delete sm_pSingleInstance;
            sm_pSingleInstance = nullptr;
        }
    };

private:
    static Init sm_init;
};

// 类CiaSingleInstance的定义
template <typename T>
typename CiaSingleton<T>::SingletonType * CiaSingleton<T>::sm_pSingleInstance = nullptr;

template <typename T>
typename CiaSingleton<T>::SingletonType * CiaSingleton<T>::singleton() {
    if (nullptr == sm_pSingleInstance) {
        sm_pSingleInstance = new CiaSingleton<T>();
        static destroy s_Des;
    }

    return sm_pSingleInstance;
}

template <typename T>
void CiaSingleton<T>::release() {
    if (nullptr != sm_pSingleInstance) {
        delete sm_pSingleInstance;
        sm_pSingleInstance = nullptr;
    }
}

template <typename T>
typename CiaSingleton<T>::Init CiaSingleton<T>::sm_init;


// 类ChaSingleInstance的定义
template <typename T>
typename ChaSingleton<T>::SingleInstanceType * ChaSingleton<T>::sm_pSingleInstance = nullptr;

template <typename T>
typename ChaSingleton<T>::SingleInstanceType * ChaSingleton<T>::singleton() {
    if (nullptr == sm_pSingleInstance) {
        sm_pSingleInstance = new SingleInstanceType();
        static Destroy s_Des;
    }

    return sm_pSingleInstance;
}

template <typename T>
void ChaSingleton<T>::release() {
    if (nullptr != sm_pSingleInstance) {
        delete sm_pSingleInstance;
        sm_pSingleInstance = nullptr;
    }
}

template <typename T>
typename ChaSingleton<T>::Init ChaSingleton<T>::sm_init;
