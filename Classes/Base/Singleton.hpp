//
//  Singleton.hpp
//  plane_demo
//
//  Created by dummy on 13-7-3.
//
//

#ifndef SINGLETON_H
#define SINGLETON_H

#ifndef NULL
#define NULL 0
#endif

template <class T>
class Singleton
{
public:
    static T* NewInstance()
    {
        if ( NULL == m_instance )
        {
            m_instance = new T;
        }
        return m_instance;
    }
    static T* GetInstance()
    {
        return m_instance;
    }
    
    static void ReleaseInstance()
    {
        if ( NULL != m_instance )
        {
            delete m_instance;
            m_instance = NULL;
        }
    }
    
protected:
    Singleton() {}
    virtual ~Singleton() {}
    
private:
    Singleton(const Singleton&){};
    Singleton& operator=(const Singleton&){};
    
private:
    static T* m_instance;
};

template <class T>
T* Singleton<T>::m_instance = NULL;

#endif