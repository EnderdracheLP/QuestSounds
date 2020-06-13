# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


LOCAL_PATH := $(call my-dir)

TARGET_ARCH_ABI := arm64-v8a

include $(CLEAR_VARS)
LOCAL_MODULE := hook

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Build the modloader shared library
include $(CLEAR_VARS)
LOCAL_MODULE	        := modloader
LOCAL_SRC_FILES         := ./extern/beatsaber-hook/include/libs/libmodloader.so
LOCAL_EXPORT_C_INCLUDES := ./extern/beatsaber-hook/include
include $(PREBUILT_SHARED_LIBRARY)

# Build the beatsaber-hook shared library, SPECIFICALLY VERSIONED!
include $(CLEAR_VARS)
LOCAL_MODULE	        := beatsaber-hook_2019_3_0f6_0_1_0
LOCAL_SRC_FILES         := ./extern/beatsaber-hook/libs/arm64-v8a/libbeatsaber-hook_2019_3_0f6_0_1_0.so
LOCAL_EXPORT_C_INCLUDES := ./extern/beatsaber-hook/
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
# Include the two libraries
LOCAL_SHARED_LIBRARIES := modloader
LOCAL_SHARED_LIBRARIES += beatsaber-hook_2019_3_0f6_0_1_0
LOCAL_LDLIBS     := -llog
LOCAL_CFLAGS     := -I"C:\Program Files\Unity\Hub\Editor\2019.3.0f6\Editor\Data\il2cpp\libil2cpp"
LOCAL_MODULE     := questsounds
LOCAL_CPPFLAGS   := -std=c++2a
LOCAL_C_INCLUDES := ./include ./src
LOCAL_SRC_FILES  += $(call rwildcard,src/,*.cpp)
include $(BUILD_SHARED_LIBRARY)

# In order to make this mod work with BMBF, you must provide a zip file with the specific libbeatsaber-hook.so (file copied to the libs directory)
# and the mod itself (installed as a hook mod)