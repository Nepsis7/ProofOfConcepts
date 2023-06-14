#include <cstdio>
#include <vector>

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

bool GetBit(const uint8& _index, void* const& _addr)
{
    const uint64 _comparator = 0b1ui64 << _index;
    return (*((uint64*)_addr)) & _comparator;
}

#define BEGIN_FLAGS_DECLARATION()
#define DECLARE_FLAG(name)\
    uint8 name : 1;\
    static inline uint8 F_##name = 0b1ui8 << __COUNTER__;
#define END_FLAGS_DECLARATION() uint8& Flags = *(uint8*)this;

struct Animal
{
    BEGIN_FLAGS_DECLARATION()

    DECLARE_FLAG(CanFly)
    DECLARE_FLAG(CanWalk)
    DECLARE_FLAG(CanSwim)

    END_FLAGS_DECLARATION()

    Animal()
    {
        Flags = 0;
        CanFly = CanSwim = 1;
        CanWalk = 0;
    }
};

template <typename T = uint8>
bool HasFlag(const T& _container, const T& _flag) { return (_container & _flag) > 0; }

template <typename T = uint8>
bool HasFlags(const T& _container, const std::vector<T>& _flags)
{
    for (const T& _flag : _flags)
        if (!HasFlag(_container, _flag))
            return false;
    return true;
}

void main()
{
#define PRINT_BITS_MODE 0
#if PRINT_BITS_MODE
    Animal t{};
    uintptr_t _address = (uintptr_t)&t;
    printf("-----------------------------------------\n");
    for (size_t iByte = 0; iByte < sizeof(Animal); ++iByte)
    {
        uint8 _byte = *(uint8*)(_address);
        for (int iBit = 0; iBit < 8; ++iBit)
        {
            printf("%d\n", (_byte & (0x1 << iBit)) > 0);
        }
        printf("-----------------------------------------\n");
        ++_address;
    }
#else
    Animal _animal = Animal();
    printf("F_CanFly ? %d\n", (_animal.Flags & Animal::F_CanFly) > 0); //same as HasFlag(_animal.Flags, Animal::F_CanFly)
    printf("F_CanWalk ? %d\n", HasFlag(_animal.Flags, Animal::F_CanWalk));
    printf("F_CanSwim ? %d\n", HasFlag(_animal.Flags, Animal::F_CanSwim));
    printf("F_CanSwim && F_CanFly ? %d\n", HasFlags(_animal.Flags, {Animal::F_CanSwim, Animal::F_CanFly}));
    printf("F_CanWalk && F_CanFly ? %d\n", HasFlags(_animal.Flags, {Animal::F_CanWalk, Animal::F_CanFly}));
    _animal.Flags = _animal.Flags | Animal::F_CanWalk;
    printf("set flag F_CanWalk true using flags\n");
    printf("F_CanWalk && F_CanFly ? %d\n", HasFlags(_animal.Flags, {Animal::F_CanWalk, Animal::F_CanFly}));
    _animal.CanWalk = false;
    printf("set flag F_CanWalk false using members\n");
    printf("F_CanWalk && F_CanFly ? %d\n", HasFlags(_animal.Flags, {Animal::F_CanWalk, Animal::F_CanFly}));
#endif
}
