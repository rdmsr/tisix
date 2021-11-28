#include <abi/layer.hpp>
#include <abi/syscalls.hpp>
#include <tisix/log.hpp>

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

enum cmos_regs
{
    CMOS_RTC_SECOND = 0x00,
    CMOS_RTC_MINUTE = 0x02,
    CMOS_RTC_HOUR = 0x04,
    CMOS_RTC_DAY = 0x07,
    CMOS_RTC_MONTH = 0x08,
    CMOS_RTC_YEAR = 0x09,

    CMOS_STATUS_A = 0x0A,
};

static uint8_t from_binary_coded_decimal(uint8_t value)
{
    return (value / 16) * 10 + (value & 0xf);
}

uint8_t cmos_read(enum cmos_regs reg)
{
    tisix::out(CMOS_ADDRESS, reg, 1);

    return tisix::in(CMOS_DATA, 1);
}

bool cmos_is_update(void)
{
    return cmos_read(CMOS_STATUS_A) & 0x80;
}

#define read_cmos_data(name) from_binary_coded_decimal(cmos_read(CMOS_RTC_##name))

#define time_pad(val) (val < 10) ? '0' : '\0'

int main(void)
{

    while (cmos_is_update())
        ;

    auto hour = read_cmos_data(HOUR);
    auto minute = read_cmos_data(MINUTE);
    auto second = read_cmos_data(SECOND);

    log("Time is {}{}:{}{}:{}{}", time_pad(hour), hour, time_pad(minute), minute, time_pad(second), second);

    return 0;
}