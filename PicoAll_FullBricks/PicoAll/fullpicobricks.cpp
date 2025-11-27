#include "fullpicobricks.h"
#include "ADC.h"

/**************************************
 * Button
 **************************************/
Button* Button::instances[MAX_GPIO] = {nullptr};

// Constructor, initializes the button
Button::Button(uint pinNumber, uint32_t edgeType)
    : pin(pinNumber), edge(edgeType),
      pressCount(0), toggleStateValue(false),
      debounceAlarm(0), lastState(false),
      eventFlag(false)
{
    gpio_init(pin);                                             C_Button("Initialize Button GPIO");
    gpio_set_dir(pin, GPIO_IN);                                 C_Button("Configure Button as input");
    gpio_pull_down(pin);                                        C_Button("Button configured with pull-down (active-high)");

    instances[pin] = this;                                      
    C_Button("Store this instance in static array for ISR lookup");
    
    gpio_set_irq_enabled_with_callback(pin, edge, true, &Button::gpio_isr); 
    C_Button("Interrupt enabled on Button pin: selected edge registered");
}

// Fired when button is pressed
void Button::gpio_isr(uint gpio, uint32_t events) {
    if (instances[gpio]) {
        instances[gpio]->startDebounce();                       
        C_Button("Dispatch to correct Button instance startDebounce()");
    }
}

// Starts the debounce process by storing the btn state
void Button::startDebounce() {
    lastState = gpio_get(pin);                                  C_Button("Read current button state for debounce");
    cancel_alarm(debounceAlarm);                                C_Button("Cancel any previous debounce alarm");
    debounceAlarm = add_alarm_in_ms(DEBOUNCE_MS, &Button::debounceTimerCallback, (void*)this, false); 
                                                                C_Button("Start debounce timer");
}


int64_t Button::debounceTimerCallback(alarm_id_t id, void* user_data) {
    Button* self = reinterpret_cast<Button*>(user_data);        
    C_Button("Static timer callback calls handleDebounce() on instance");
    
    return self->handleDebounce();
}

/**
 * The function checks whether the state stays "consistent" even after a small 20ms break
 * This ensures that no V fluctuation caused by the physical structure of the button is mistaken as an actual btn press
 * If the state stays consistent, then the btn is considered pressed: count is increased and its state is toggled
 */
int64_t Button::handleDebounce() {
    bool currentState = gpio_get(pin);                          C_Button("Read button state at debounce timeout");
    if (currentState == lastState && currentState == true) {
        pressCount++;                                           C_Button("Increment press counter");
        toggleStateValue = !toggleStateValue;                   C_Button("Toggle internal state");
        eventFlag = true;                                       C_Button("Set event flag for hasEvent()");
                                                                C_Button("Debounce passed: valid button press");
    } else {
                                                                C_Button("Debounce failed: bounce ignored");
    }
    return 0;
}

int Button::getPressCount() const {
    return pressCount;                                          C_Button("Return press count");
}

bool Button::isPressed() const {
    return gpio_get(pin);                                       C_Button("Return current button level");
}

bool Button::toggleState() const {
    return toggleStateValue;                                    C_Button("Return toggle state");
}

bool Button::hasEvent() {
    if (eventFlag) {
        eventFlag = false;                                      C_Button("Clear event flag after reading");
        return true;
    }
    return false;
}

/**************************************
 * Single color Led 
 **************************************/
Led::Led(uint pinNumber) : pin(pinNumber), state(false) {
    gpio_init(pin);                                         C_Led("Initialize LED GPIO");
    gpio_set_dir(pin, true);                                C_Led("LED configured as output");
    gpio_put(pin, state);                                   C_Led("Ensure LED starts OFF");
}

// Turn LED ON
void Led::on() {
    state = 1;                                              C_Led("Update LED state to ON");
    gpio_put(pin, state);                                   C_Led("Switch LED ON");
}

// Turn LED OFF
void Led::off() {
    state = 0;                                              C_Led("Update LED state to OFF");
    gpio_put(pin, state);                                   C_Led("Switch LED OFF");
}

// Toggle LED state
void Led::toggle() {
    state = !state;                                         C_Led("Toggle LED state variable");
    gpio_put(pin, state);                                   C_Led("Write toggled state to GPIO");
}

// Set LED state directly
void Led::setState(bool s) {
    state = s ? 1 : 0;                                      C_Led("Set LED state variable directly");
    gpio_put(pin, state);                                   C_Led("Write new state to GPIO");
}

// Return current LED state
bool Led::isOn() const {
    return state;                                           C_Led("Return current LED state");
}

/**************************************
 * RGB Led 
 **************************************/
// Constructor: Initializes RGBLedDriver with given pin number and number of LEDs
RGBLedDriver::RGBLedDriver(uint8_t  pinNumber, uint16_t numberOfPixels) {
    this->pixelSm = 0;                         // Use state machine 0 of the PIO
    this->pixelPio = pio0;                     // Use PIO0 hardware
    this->Init(pinNumber, numberOfPixels);     // Call initialization function
}

// Initializes the PIO program for controlling WS2812 LEDs (RGBLedDriver)
void RGBLedDriver::Init(uint8_t  pinNumber, uint16_t numberOfPixels) {
    uint offset = pio_add_program(this->pixelPio, &ws2812_program); // Load WS2812 PIO program
    ws2812_program_init(this->pixelPio, this->pixelSm, offset, pinNumber, 800000, false); // Initialize with 800kHz

    this->actual_number_of_pixels = numberOfPixels; // Store number of LEDs

    // Initialize all pixel colors to off (0,0,0)
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++) {
        this->pixelBuffer[i][0] = 0; // Red
        this->pixelBuffer[i][1] = 0; // Green
        this->pixelBuffer[i][2] = 0; // Blue
    }

    this->Show();        // Update LEDs to reflect buffer
    sleep_ms(1000);      // Short delay to stabilize
}

// Sets the RGB color of a specific LED and updates the strip
void RGBLedDriver::setPixelColor(uint16_t pixelNumber, uint8_t r, uint8_t g, uint8_t b) {
    this->pixelBuffer[pixelNumber][0] = r;  // Set red value
    this->pixelBuffer[pixelNumber][1] = g;  // Set green value
    this->pixelBuffer[pixelNumber][2] = b;  // Set blue value
    this->Show();                           // Send updated colors to LED strip
}

// Fills all LEDs with the same RGB color and updates the strip
void RGBLedDriver::Fill(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++) {
        this->pixelBuffer[i][0] = r;  // Set red
        this->pixelBuffer[i][1] = g;  // Set green
        this->pixelBuffer[i][2] = b;  // Set blue
    }
    this->Show(); // Update all pixels at once
}

// Sends the RGB data of each pixel to the RGBLedDriver strip
void RGBLedDriver::Show(void) {
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++) {
        this->putPixel(                  // Send encoded pixel color
            urgb_u32(                    // Convert RGB values to 24-bit GRB format
                pixelBuffer[i][0],       // Red
                pixelBuffer[i][1],       // Green
                pixelBuffer[i][2]        // Blue
            )
        );
    }
}

// Converts individual RGB values into a single 24-bit GRB value
uint32_t RGBLedDriver::urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
        ((uint32_t)(r) << 8) |          // Red in bits 15-8
        ((uint32_t)(g) << 16) |         // Green in bits 23-16
        (uint32_t)(b);                  // Blue in bits 7-0
}

// Sends a single pixel's GRB data to the PIO state machine
void RGBLedDriver::putPixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(this->pixelPio, this->pixelSm,
                        pixel_grb << 8u); // Shift data to match 24-bit WS2812 format
}

/**************************************
 * Buzzer 
 **************************************/
using namespace std;
PWM::PWM(uint gpioPin, bool inverted)
    : _gpioPin(gpioPin), _inverted(inverted),
      _frequency(1000.0f), _duty(50.0f),
      _divider(1.0f), _wrap(0), _enabled(false)
{
    gpio_set_function(_gpioPin, GPIO_FUNC_PWM);
    _sliceNum = pwm_gpio_to_slice_num(_gpioPin);
    configurePWM();
    applyDuty();
    enable(true);
}

// -------------------------------------------------------------
// Set frequency and keep duty cycle consistent
// -------------------------------------------------------------
void PWM::setFrequency(float freqHz) {
    if (freqHz < 1) freqHz = 1;
    _frequency = freqHz;

    const float clockFreq = 125000000.0f;
    const uint16_t maxWrap = 65535;

    for (_divider = 1.0f; _divider <= 255.0f; _divider += 0.5f) {
        float wrapCalc = (clockFreq / (_frequency * _divider)) - 1.0f;
        if (wrapCalc <= maxWrap) {
            _wrap = (uint16_t)wrapCalc;
            break;
        }
    }

    configurePWM();
    applyDuty();  // Reapply duty automatically after frequency change
}

// -------------------------------------------------------------
// Set duty cycle independently (no need to re-set frequency)
// -------------------------------------------------------------
void PWM::setDutyCycle(float dutyPercent) {
    if (dutyPercent < 0) dutyPercent = 0;
    if (dutyPercent > 100) dutyPercent = 100;

    _duty = dutyPercent;
    applyDuty();
}

// -------------------------------------------------------------
// Apply duty to hardware
// -------------------------------------------------------------
void PWM::applyDuty() {
    float duty = _duty;
    if (_inverted) duty = 100.0f - duty;

    uint16_t level = (uint16_t)((duty / 100.0f) * _wrap);
    pwm_set_gpio_level(_gpioPin, level);
}

void PWM::enable(bool on) {
    _enabled = on;
    pwm_set_enabled(_sliceNum, _enabled);
}

void PWM::configurePWM() {
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, _divider);
    pwm_config_set_wrap(&config, _wrap);
    pwm_init(_sliceNum, &config, _enabled);
}

/**************************************
 * Relay 
 **************************************/
RelayDriver::RelayDriver(uint8_t pinNumber): pinNumber(pinNumber) {
    gpio_init(pinNumber);               // Initializes relay gpio 
    gpio_set_dir(pinNumber, true);      // Initializes relay as output  
    gpio_put(pinNumber, false);         // Initializes as closed   
}

// Open Switch
void RelayDriver::activate() {                                         
    gpio_put(pinNumber, true);                                  
}

// Close Switch
void RelayDriver::deactivate() {                                        
    gpio_put(pinNumber, false);                                   
}

/**************************************
 * Potentiometer 
 **************************************/
Potentiometer::Potentiometer()
    : ADC(26) { };    

float Potentiometer::readPercent() {
    int raw = readAvg(5);
    float normVal = static_cast<float>(raw) * 100.0f / 4095.0f;

    return normVal;
}

/**************************************
 * Temperature and humidity sensor 
 **************************************/
TempHumDriver::TempHumDriver(i2c_inst_t* i2c, uint8_t addr) 
    : _i2c(i2c), _addr(addr) {}

/**
 * Sends a command to the TempHumDriver sensor.
 * @param high The high byte of the command.
 * @param low The low byte of the command.
 */
void TempHumDriver::sendCommand(uint8_t high, uint8_t low) {
    uint8_t buf[2] = { high, low }; 
    i2c_write_blocking(_i2c, _addr, buf, 2, false);
}   

/**
 * Reads data from the TempHumDriver sensor.
 * @param data Pointer to the buffer where the data will be stored.
 * @param len The number of bytes to read.
 */
void TempHumDriver::readData(uint8_t *data, uint8_t len) {
    i2c_read_blocking(_i2c, _addr, data, len, false);
}

/**
 * Wakes up the TempHumDriver sensor to begin measurement.
 * Sends the appropriate wake-up command and waits for the sensor to be ready.
 */
void TempHumDriver::wakeUp() {
    sendCommand(0x35, 0x17);    // Wake up command (start measurement)
    sleep_ms(500);              // Delay to allow sensor to wake up
}

/**
 * Puts the TempHumDriver sensor to sleep to conserve power.
 * Sends the sleep command to turn off the sensor's measurement functions.
 */
void TempHumDriver::sleep() {
    sendCommand(0xB0, 0x64); // Sleep command
}

/**
 * Reads the temperature value from the TempHumDriver sensor.
 * The temperature is in Celsius and is calculated based on the sensor's raw data.
 * @return The temperature in Celsius.
 */
float TempHumDriver::readTemperature() {
    uint8_t data[6];
    sendCommand(0x78, 0x66);    // Start measurement for temperature
    sleep_ms(500);              // Wait for the measurement
    readData(data, 6);          // Read the 6 bytes of data (2 for temperature, 2 for humidity)
    
    uint16_t rawTemp = (data[0] << 8) | data[1];        // Combine the 2 bytes
    rawTemp = rawTemp & 0xFFFC;                         // Mask the last two bits (for the CRC check)
    return (((4375 * rawTemp) >> 14) - 4500) / 100.0;   // Convert raw data to temperature in Celsius based on sensor's formula
}

/**
 * Reads the humidity value from the TempHumDriver sensor.
 * The humidity is in percentage and is calculated based on the sensor's raw data.
 * @return The humidity in percentage.
 */
float TempHumDriver::readHumidity() {
    uint8_t data[6];
    sendCommand(0x78, 0x66);    // Start measurement for humidity
    sleep_ms(100);              // Wait for the measurement
    readData(data, 6);          // Read the 6 bytes of data (2 for temperature, 2 for humidity)

    uint16_t rawHumidity = (data[3] << 8) | data[4];    // Combine the 2 bytes for humidity
    rawHumidity = rawHumidity & 0xFFFC;                 // Mask the last two bits (for the CRC check)
    return ((625 * rawHumidity) >> 12) / 100.0;         // Convert raw humidity data to percentage based on sensor's formula
}

/**************************************
 * Light Sensor
 **************************************/
LightSensorLDR::LightSensorLDR() 
    : ADC(27) {}                               // GPIO27 → ADC1

float LightSensorLDR::readLux() {
    int raw = readAvg(5);                                                  // Average of 5 samples
    if (raw <= 0) raw = 1;                                                 // Avoid division by zero

    const float Rfixed = 10000.0f;                                         // Fixed resistor value (Ω)
    float rLDR = Rfixed * (static_cast<float>(raw) / (4095.0f - raw));     // Compute LDR resistance

    const float A = 2.0e6f;                                                // Empirical constant (sensor-specific)
    const float gamma = 1.4f;                                              // Exponent defining response curve

    float lux = powf((A / rLDR), 1.0f / gamma);                            // Compute light intensity (lux)
    if (lux < 0.01f) lux = 0.01f;                                          // Clamp to avoid negatives
    return lux;                                                            // Return computed lux
}

/**************************************
 * Continuous rotation servo motor  
 **************************************/
MotorDriver::MotorDriver(i2c_inst_t* i2c, uint8_t addr) 
  : _i2c(i2c), _addr(addr) {}

/**
 * Function to control a DC motor (1 or 2) with a given speed (0-255) and direction (0 or 1)
 * num: 1 or 2 (selects motor)
 * speed: 0 to 255 (controls the motor speed)
 * direction: 0 for one direction, 1 for the opposite direction
 */
void MotorDriver::dc(int num, int speed, int direction) { 
  uint8_t buf[4];
  buf[0] = num;
  buf[1] = speed, 
  buf[2] = direction, 
  buf[3] = num ^ speed ^ direction;
  i2c_write_blocking(_i2c, _addr, buf, 4, false);
}

/**
 * Function to control a servo motor (1 to 4) with a given angle (0-180)
 * num: 1, 2, 3, or 4 (selects servo motor)
 * angle: 0 to 180 (controls the servo angle)
 */
void MotorDriver::servo(int num, int angle) { 
  uint8_t buf[5];
  buf[0] = 0x26;
  buf[1] = num + 2;
  buf[2] = 0x00;
  buf[3] = angle;
  buf[4] = (num + 2) ^ angle;
  i2c_write_blocking(_i2c, _addr, buf, 5, false);
}
