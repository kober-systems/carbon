*** Test Cases ***
Datastore Unity tests pass on Teensy 4.1
    Run Unity Firmware    test_datastore

ArduinoUsbIO Unity tests pass on Teensy 4.1
    Run Unity Firmware    test_arduino_usb_io

ArduinoHidIO Unity tests pass on Teensy 4.1
    Run Unity Firmware    test_arduino_hid_io

*** Keywords ***
Run Unity Firmware
    [Arguments]    ${suite}
    Execute Command    include @${CURDIR}/../../.pio/renode/teensy41/${suite}.resc
    Create Terminal Tester    sysbus.lpuart6    timeout=10    defaultPauseEmulation=true
    Register Failing Uart String    FAILURES
    Wait For Line On Uart    OK
