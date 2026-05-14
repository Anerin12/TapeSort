#include <cstdint>
#include <optional>
#include <string>

class IType {
    public:      
        virtual std::optional<int32_t> read() = 0;
        virtual bool write(int32_t value) = 0;
        virtual bool rewind() = 0;
        virtual bool move(bool direction) = 0;
};