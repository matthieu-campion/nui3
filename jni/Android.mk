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
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libnui3
LOCAL_CFLAGS    := -x c++ -fmessage-length=0 -pipe -Wno-trigraphs -O0 -DNGL_STATIC_BUILD -DHAVE_BCOPY -DFT2_BUILD_LIBRARY -DDARWIN_NO_CARBON -DFT_CONFIG_OPTION_SYSTEM_ZLIB -DHAVE_BCOPY -DFEATURE_NANOJIT -DJS_TRACER -DXP_UNIX -DAVMPLUS_IA32 -D_DEBUG -DDEBUG -D_ANDROID_ -fexceptions -fvisibility=hidden -fno-threadsafe-statics -Wno-deprecated-declarations -I. -Iinclude -Ideps/libpng -Ideps/gameswf -Ideps/zlib -Ideps/ungif -Ideps/ucdata -Ideps/libjpeg -Isrc/Application/Carbon -Isrc/Stream/Zip -Ideps/freetype2/include -Ideps/expat/include -Ideps/harfbuzz -Ideps/expat/lib -Ideps/tracemonkey -Ideps/libcss/src -Ideps/libcss/src/libwapcaplet -Ideps/libcss/src/libparserutils -Ideps/libcss/include 
LOCAL_SRC_FILES := ../src/Application/Win/ngl.cpp
LOCAL_LDLIBS    := -llog -lGLESv1_CM

include $(BUILD_SHARED_LIBRARY)
