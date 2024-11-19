from ctypes import CDLL, POINTER, c_uint8

import pytest

@pytest.fixture
def utils():
    yield CDLL("./utils.so")



def test_temp(utils):
    for i in range(10000):
        result = utils.get_temperature()
        assert 0 <= result <= 40
        if i % 1000 == 0:  # Log progress every 1000 iterations
            print(f"Iteration {i}: Passed")

def test_circular_buffer(utils):
    counter = 0

    # Define the function prototype
    utils.get_temperature_log.restype = POINTER(c_uint8 * 10)  # 10 is the size of the array
    # Get a pointer to the temperature_log array
    temperature_log = utils.get_temperature_log()

    for i in range(100):
        for counter in range(10):
            current_value = temperature_log[counter]
            utils.add_new_temp_value()
            new_value = temperature_log[counter]
            assert current_value != new_value
            counter+=1
        counter = 0
        if i % 10 == 0:  # Log progress every 100 iterations
            print(f"Iteration {i}: Passed")