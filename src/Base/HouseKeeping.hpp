#ifndef HOUSE_KEEPING_HPP
#define HOUSE_KEEPING_HPP

#include <cstdint>
#include <memory>

#include "MemoryDevice.hpp"
#include "rp_hw-profiles.h"

// Base Housekeeping address
static const int HOUSEKEEPING_BASE_ADDR = 0x00000000;
static const int HOUSEKEEPING_BASE_SIZE = 0x2000;

struct CAN_control {
	uint32_t enable				: 1;
	uint32_t reserved			: 31;
};

struct ExternalTrigger {
	uint32_t enable				: 1;
	uint32_t gpio_adc_dac		: 1;
	uint32_t output_selector	: 1;
	uint32_t reserved			: 29;
};

struct PLL_control {
	uint32_t enable				: 1;
	uint32_t reserved_1			: 3;
	uint32_t reference_detected : 1;
	uint32_t reserved_2			: 3;
	uint32_t locked				: 1;
	uint32_t reserved_3			: 23;
};

// Classic Red Pitaya borad control registers
struct HKRegistersMapV1 {
	uint32_t id;                            // 0x0
	uint32_t dna_lo;                        // 0x4 **DNA part 1**
	uint32_t dna_hi;                        // 0x8 **DNA part 2**
	uint32_t digital_loop;                  // 0xC **Digital Loopback**
	uint32_t ex_cd_p;                       // 0x10 **Expansion connector direction P**
	uint32_t ex_cd_n;                       // 0x14 **Expansion connector direction N**
	uint32_t ex_co_p;                       // 0x18 **Expansion connector output P**
	uint32_t ex_co_n;                       // 0x1c **Expansion connector output N**
	uint32_t ex_ci_p;                       // 0x20 **Expansion connector input P**
	uint32_t ex_ci_n;                       // 0x24 **Expansion connector input N**
	uint32_t reserved_2;                    // 0x28
	uint32_t reserved_3;                    // 0x2C
	uint32_t led_control;                   // 0x30 **LED control**
	CAN_control can_control;                // 0x34 **CAN control**
	uint32_t reserved_4[50];                // 0x38 - 0x100
	uint32_t fpga_ready;                    // 0x100 **FPGA ready**
	uint32_t reserved_5[959];               // 0x104 - 0x1000
	ExternalTrigger ext_trigger;            // 0x1000 **External trigger override**
};

// 124-12-4CH Red Pitaya board control registers
struct HKRegistersMapV2 {
	uint32_t id;                            // 0x0
	uint32_t dna_lo;                        // 0x4 **DNA part 1**
	uint32_t dna_hi;                        // 0x8 **DNA part 2**
	uint32_t digital_loop;                  // 0xC **Digital Loopback**
	uint32_t ex_cd_p;                       // 0x10 **Expansion connector direction P**
	uint32_t ex_cd_n;                       // 0x14 **Expansion connector direction N**
	uint32_t ex_co_p;                       // 0x18 **Expansion connector output P**
	uint32_t ex_co_n;                       // 0x1c **Expansion connector output N**
	uint32_t ex_ci_p;                       // 0x20 **Expansion connector input P**
	uint32_t ex_ci_n;                       // 0x24 **Expansion connector input N**
	uint32_t reserved_2;                    // 0x28
	uint32_t reserved_3;                    // 0x2C
	uint32_t led_control;                   // 0x30 **LED control**
	CAN_control can_control;                // 0x34 **CAN control**
	uint32_t reserved_4;                    // 0x38
	uint32_t reserved_5;                    // 0x3C
	PLL_control pll_control;                // 0x40
	uint32_t idelay_reset;                  // 0x44 **IDELAY reset**
	uint32_t idelay_cha;                    // 0x48 **IDELAY CHA**
	uint32_t idelay_chb;                    // 0x4C **IDELAY CHB**
	uint32_t idelay_chc;                    // 0x50 **IDELAY CHC**
	uint32_t idelay_chd;                    // 0x54 **IDELAY CHD**
	uint32_t reserved_6[42];                // 0x58 - 0x100
	uint32_t fpga_ready;                    // 0x100 **FPGA ready**
	uint32_t reserved_7[959];               // 0x104 - 0x1000
	ExternalTrigger ext_trigger;            // 0x1000 **External trigger override**

};

// 250-12 Red Pitaya board control registers
struct HKRegistersMapV3 {
	uint32_t id;                            // 0x0
	uint32_t dna_lo;                        // 0x4 **DNA part 1**
	uint32_t dna_hi;                        // 0x8 **DNA part 2**
	uint32_t digital_loop;                  // 0xC **Digital Loopback**
	uint32_t ex_cd_p;                       // 0x10 **Expansion connector direction P**
	uint32_t ex_cd_n;                       // 0x14 **Expansion connector direction N**
	uint32_t ex_co_p;                       // 0x18 **Expansion connector output P**
	uint32_t ex_co_n;                       // 0x1c **Expansion connector output N**
	uint32_t ex_ci_p;                       // 0x20 **Expansion connector input P**
	uint32_t ex_ci_n;                       // 0x24 **Expansion connector input N**
	uint32_t reserved_2;                    // 0x28
	uint32_t reserved_3;                    // 0x2C
	uint32_t led_control;                   // 0x30 **LED control**
	CAN_control can_control;                // 0x34 **CAN control**
	uint32_t reserved_4;                    // 0x38
	uint32_t reserved_5;                    // 0x3C
	PLL_control pll_control;                // 0x40
	uint32_t idelay_reset;                  // 0x44 **IDELAY reset**
	uint32_t idelay_cha;                    // 0x48 **IDELAY CHA**
	uint32_t idelay_chb;                    // 0x4C **IDELAY CHB**
	uint32_t adc_spi_cw;                    // 0x50 **ADC SPI Control word**
	uint32_t adc_spi_wd;                    // 0x54 **ADC SPI Write data / start transfer**
	uint32_t adc_spi_rd;                    // 0x58 **ADC SPI Read data / Transfer busy**
	uint32_t reserved_6;                    // 0x5C
	uint32_t dac_spi_cw;                    // 0x60 **DAC SPI Control word**
	uint32_t dac_spi_wd;                    // 0x64 **DAC SPI Write data / start transfer**
	uint32_t dac_spi_rd;                    // 0x68 **DAC SPI Read data / Transfer busy**
	uint32_t reserved_7[37];                // 0x6C - 0x100
	uint32_t fpga_ready;                    // 0x100 **FPGA ready**
	uint32_t reserved_8[959];               // 0x104 - 0x1000
	ExternalTrigger ext_trigger;            // 0x1000 **External trigger override**
};

enum class HKVersion {
	V1,
	V2,
	V3
};

static const uint32_t LED_CONTROL_MASK = 0xFF;
static const uint32_t DIGITAL_LOOP_MASK = 0x1;
static const uint32_t EX_CD_P_MASK = 0xFF;
static const uint32_t EX_CD_N_MASK = 0xFF;
static const uint32_t EX_CO_P_MASK = 0xFF;
static const uint32_t EX_CO_N_MASK = 0xFF;
static const uint32_t EX_CI_P_MASK = 0xFF;
static const uint32_t EX_CI_N_MASK = 0xFF;

class HouseKeeping {
public:
	HouseKeeping(std::shared_ptr<MemoryDevice> memDev);
	~HouseKeeping();

	bool Init(bool reset = true);
	bool Release();
	bool IsInitialized() const;

	bool ResetRegistersMap();

	HKVersion GetVersion() const;
	uint32_t GetId() const;
	uint64_t GetDNA() const;

/* ------------ PLL ------------- */

	void SetPllControlEnable(bool enable);

	bool GetPllControlEnable() const;

	//void SetPllControlLocked(bool lock);

	bool GetPllControlLocked() const;

/* ---------- DAISY CHAIN -------- */

	bool SetEnableDaisyChainSync(bool enable);

	bool GetEnableDaisyChainSync() const;

/* -------- DIGITAL PINS -------- */

	bool SetLEDState(uint32_t state);

	uint32_t GetLEDState() const;

	bool SetGPIOPinDirection(uint32_t direction);

	uint32_t GetGPIOPinDirection() const;

	bool SetGPIOPinState(uint32_t state);

	uint32_t GetGPIOPinState() const;

	bool DigitalPinReset();

	bool SetDigitalLoopEnable(bool enable);

/* ---------- TRIGGER ---------- */

	bool SetDigitalPinEnableTriggerOutput(bool enable);

	bool GetDigitalPinEnableTriggerOutput() const;

	bool SetTriggerSource(RpOutputTriggerMode source);

	RpOutputTriggerMode GetTriggerSource() const;

/* ------------ CAN ------------ */

	bool SetCANModeEnable(bool enable);

	bool GetCANModeEnable() const;

private:
	bool _isInitialized;
	HKVersion _version;

	union {
		volatile HKRegistersMapV1 *v1;
		volatile HKRegistersMapV2 *v2;
		volatile HKRegistersMapV3 *v3;
	} _registersMap;

	std::shared_ptr<MemoryDevice> _memDev;
};

#endif /* HOUSE_KEEPING_HPP */