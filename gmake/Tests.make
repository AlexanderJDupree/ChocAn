# GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild prelink

ifeq ($(config),debug)
  ifeq ($(origin CC), default)
    CC = clang
  endif
  ifeq ($(origin CXX), default)
    CXX = clang++
  endif
  ifeq ($(origin AR), default)
    AR = ar
  endif
  TARGETDIR = ../bin/tests
  TARGET = $(TARGETDIR)/debug_tests
  OBJDIR = obj/debug/Tests
  DEFINES += -DDEBUG
  INCLUDES += -I../third_party -I../include
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Werror -g -Wall -Wextra -Wall -Wextra -Werror -std=c++17 -stdlib=libc++
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Werror -g -Wall -Wextra -Wall -Wextra -Werror -std=c++17 -stdlib=libc++
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += ../lib/debug/libChocAn-Core.so ../lib/debug/libChocAn-Data.so ../lib/debug/libChocAn-App.so ../lib/debug/libChocAn-View.so -lc++ -lc++abi
  LDDEPS += ../lib/debug/libChocAn-Core.so ../lib/debug/libChocAn-Data.so ../lib/debug/libChocAn-App.so ../lib/debug/libChocAn-View.so
  ALL_LDFLAGS += $(LDFLAGS) -Wl,-rpath,'$$ORIGIN/../../lib/debug'
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release)
  ifeq ($(origin CC), default)
    CC = clang
  endif
  ifeq ($(origin CXX), default)
    CXX = clang++
  endif
  ifeq ($(origin AR), default)
    AR = ar
  endif
  TARGETDIR = ../bin/tests
  TARGET = $(TARGETDIR)/release_tests
  OBJDIR = obj/release/Tests
  DEFINES += -DNDEBUG
  INCLUDES += -I../third_party -I../include
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Werror -O2 -Wall -Wextra -Wall -Wextra -Werror -std=c++17 -stdlib=libc++
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Werror -O2 -Wall -Wextra -Wall -Wextra -Werror -std=c++17 -stdlib=libc++
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += ../lib/release/libChocAn-Core.so ../lib/release/libChocAn-Data.so ../lib/release/libChocAn-App.so ../lib/release/libChocAn-View.so -lc++ -lc++abi
  LDDEPS += ../lib/release/libChocAn-Core.so ../lib/release/libChocAn-Data.so ../lib/release/libChocAn-App.so ../lib/release/libChocAn-View.so
  ALL_LDFLAGS += $(LDFLAGS) -Wl,-rpath,'$$ORIGIN/../../lib/release'
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

OBJECTS := \
	$(OBJDIR)/state_controller_tests.o \
	$(OBJDIR)/account_tests.o \
	$(OBJDIR)/address_tests.o \
	$(OBJDIR)/datetime_tests.o \
	$(OBJDIR)/id_generator_tests.o \
	$(OBJDIR)/login_tests.o \
	$(OBJDIR)/name_tests.o \
	$(OBJDIR)/transaction_builder_tests.o \
	$(OBJDIR)/transaction_tests.o \
	$(OBJDIR)/sqlite_db_tests.o \
	$(OBJDIR)/test_config_main.o \
	$(OBJDIR)/terminal_input_controller_tests.o \

RESOURCES := \

CUSTOMFILES := \

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

$(TARGET): $(GCH) ${CUSTOMFILES} $(OBJECTS) $(LDDEPS) $(RESOURCES) | $(TARGETDIR)
	@echo Linking Tests
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(CUSTOMFILES): | $(OBJDIR)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning Tests
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) $(PCH) | $(OBJDIR)
$(GCH): $(PCH) | $(OBJDIR)
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
else
$(OBJECTS): | $(OBJDIR)
endif

$(OBJDIR)/state_controller_tests.o: ../tests/app/state_controller_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/account_tests.o: ../tests/core/account_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/address_tests.o: ../tests/core/address_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/datetime_tests.o: ../tests/core/datetime_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/id_generator_tests.o: ../tests/core/id_generator_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/login_tests.o: ../tests/core/login_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/name_tests.o: ../tests/core/name_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/transaction_builder_tests.o: ../tests/core/transaction_builder_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/transaction_tests.o: ../tests/core/transaction_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/sqlite_db_tests.o: ../tests/data/sqlite_db_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/test_config_main.o: ../tests/test_config_main.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/terminal_input_controller_tests.o: ../tests/view/terminal_input_controller_tests.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif