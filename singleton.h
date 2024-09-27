#ifndef SINGLETON_H
#define SINGLETON_H
#include "global.h"

/***********************************************
    * @file     singleton.h
    * @brief    单例基类
    * 
    * @author   wmlatiao
    * @date     2024/09/27
    * @history
***********************************************/

template <typename T>
class Singleton{    // 单例类,作为基类来使用
protected:
    Singleton() = default;  // 防止外部用new创建类的实例，但其子类可以用这个构造函数
    Singleton(const Singleton<T>&) = delete;    // 防止进行拷贝构造
    Singleton& operator=(const Singleton<T>&) = delete; //防止进行赋值操作
    static std::shared_ptr<T> _instance;    // 智能指针用于自动回收资源(类中的static变量一定要先初始化，这里只做了声明)
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag s_flag;   // s_flag只会初始化一次
        std::call_once(s_flag,[&](){    // 只有第一次调用GetInstance时会调用这个初始化函数,保证线程安全
            qDebug()<<"实例化Singleton对象";
            _instance = std::shared_ptr<T>(new T);  // 初始化对象
        });
        return _instance;
    }
    
    void printAddress(){
        std::cout<<_instance.get()<<std::endl;  // 得到instance中的原始指针
    }
    
    
    ~Singleton(){
        std::cout<<"this is Singleton disrtuctor"<<std::endl;
    }
};

template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;   // 给单例实例_instance初始化
#endif // SINGLETON_H
