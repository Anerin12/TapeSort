#include <cstdint>
#include <optional>
#include <string>

class IType {

    private:

    public:
        std::optional<int32_t> read();
        std::optional<int32_t> write(int32_t value);

};