# File for test cases
# Here you can add platform-specific files (main file, test, etc.)

CFLAGS = -DTEST -DUNITY_INCLUDE_CONFIG_H
TESTS_DIR = Tests
UNITY_DIR = External/Unity-2.5.2
JDY23_DRIVER = External/jdy23_driver
FOR_TARGET = Y

C_INCLUDES += \
-I$(JDY23_DRIVER)/Inc \
-I$(UNITY_DIR)/src \
-I$(UNITY_DIR)/extras/fixture/src \
-I$(UNITY_DIR)/extras/memory/src \
-I$(TESTS_DIR)

C_SOURCES += \
$(JDY23_DRIVER)/Src/jdy23_driver.c \
$(UNITY_DIR)/src/unity.c \
$(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
$(UNITY_DIR)/extras/memory/src/unity_memory.c

ifeq ($(FOR_TARGET), Y)
C_SOURCES += \
$(TESTS_DIR)/target_tests.c \
$(TESTS_DIR)/target_tests/jdy23_driver/jdy23_test_runner.c \
$(TESTS_DIR)/target_tests/jdy23_driver/jdy23_test.c \
$(JDY23_DRIVER)/Src/jdy23_io.c
# $(JDY23_DRIVER)/Src/jdy23_time.c

include MakefileTestTarget.mk
else
C_INCLUDES += \
-I$(TESTS_DIR)/mocks/Inc \
-I$(TESTS_DIR)/fakes/Inc
C_SOURCES += \
$(TESTS_DIR)/host_tests.c \
$(TESTS_DIR)/host_tests/jdy23_driver/jdy23_test_runner.c \
$(TESTS_DIR)/host_tests/jdy23_driver/jdy23_test.c \
$(TESTS_DIR)/mocks/Src/mock_jdy23_io.c
# $(TESTS_DIR)/fakes/Src/fake_jdy23_time.c

include MakefileTestHost.mk
endif
