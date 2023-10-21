#pragma once

class A {
private:
    int a = 0;
public:
    A() = default;
    ~A() = default;

    A(int inA) : a(inA) {}

    virtual void test() = 0;
};

class B : public A {
public:
    B() = default;
    ~B() = default;

    B(int inB) : A(inB) {}

    void test() override;
};

template <typename T>
class C {
private:
    T t;
public:
    C() = delete;
    ~C() = default;
    C(T inT) : t(inT) {}

    void test();
};