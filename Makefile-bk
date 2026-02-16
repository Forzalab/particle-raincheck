# ============================================================================
# COLOR CONFIGURATION
# ============================================================================

RED     := \033[1;31m
CYAN    := \033[0;36m
YELLOW  := \033[1;33m
BLUE    := \033[0;34m
MAGENTA := \033[0;35m
BRIGHT_MAGENTA := \033[1;35m
WHITE   := \033[1;97m
GRAY    := \033[38;5;242m
LIGHT_GRAY := \033[38;5;250m
RESET   := \033[0m

ERROR_BG := \033[41m
ERROR_FG := \033[1;97m
WARN_BG  := \033[43m
WARN_FG  := \033[1;30m

# ============================================================================
# BUILD CONFIGURATION
# ============================================================================

WORKDIR := $(CURDIR)

LINTER := /usr/bin/cppcheck
CC     := /usr/bin/g++
LIBS   := -lcurl -lbridges -lncurses

CCFLAGS := -Wall -Wextra -Wfatal-errors -fcompare-debug-second \
           -pedantic -pedantic-errors -Wno-sign-compare \
           -Wno-unused-variable -Wno-unused-parameter \
           -std=c++26 -g -D_GLIBCXX_DEBUG \
           -fsanitize=undefined -fsanitize=address \
           -fdiagnostics-color=always

LINTFLAGS := --language=c++ --enable=all --suppress=missing
IncludeSystem --suppress=*:*libs/include/* --suppress=unuse
dStructMember

CPP_SRCS    := $(wildcard $(WORKDIR)/*.cpp)
CC_SRCS     := $(wildcard $(WORKDIR)/*.cc)
SRCS        := $(CPP_SRCS) $(CC_SRCS)
H_HEADERS   := $(wildcard $(WORKDIR)/*.h)
HPP_HEADERS := $(wildcard $(WORKDIR)/*.hpp)
HEADERS     := $(H_HEADERS) $(HPP_HEADERS)
OBJS        := $(CPP_SRCS:.cpp=.o) $(CC_SRCS:.cc=.o)
MAIN_FILES := $(shell grep -l "int main" $(SRCS) 2>/dev/null || echo "")
MAIN_COUNT := $(words $(MAIN_FILES))
TARGET := $(WORKDIR)/a.out
ALL_FILES   := $(SRCS) $(HEADERS)

# ============================================================================
# BUILD PIPELINE
# ============================================================================

# Add new target before all
.PHONY: select-main
select-main:
	@if [ $(MAIN_COUNT) -gt 1 ]; then \
		echo ""; \
		echo "$(YELLOW)▐$(RESET) $(WHITE)Multiple main() functions found:$(RESET)"; \
		echo ""; \
		i=1; \
		for file in $(MAIN_FILES); do \
			echo "  $(CYAN)$$i)$(RESET) $$(basename $$file)"; \
			i=$$((i + 1)); \
		done; \
		echo ""; \
		read -p "→ Select file (1-$(MAIN_COUNT)): " choice; \
		if [ $$choice -ge 1 ] && [ $$choice -le $(MAIN_COUNT) ]; then \
			selected=$$(echo $(MAIN_FILES) | cut -d' ' -f$$choice); \
			echo "$$selected" > /tmp/cmake_selected_main.txt; \
			echo "$(CYAN)✓$(RESET) Selected: $$(basename $$selected)"; \
		else \
			echo "$(RED)Invalid selection$(RESET)"; \
			rm -f /tmp/cmake_selected_main.txt; \
			exit 1; \
		fi; \
	else \
		rm -f /tmp/cmake_selected_main.txt; \
	fi

.PHONY: all
all: check-sources check-compiler select-main lint compile link
	@echo ""
	@echo "$(CYAN)╔══════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║$(RESET)  $(WHITE)✓ Build complete$(RESET)                        $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚══════════════════════════════════════════╝$(RESET)"
	@echo "$(YELLOW)→ Run:$(RESET) $(LIGHT_GRAY)cmake run$(RESET)"
	@echo ""

.PHONY: build-pipeline
build-pipeline: check-sources check-compiler lint compile link
	@echo ""
	@echo "$(CYAN)╔══════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)║$(RESET)  $(WHITE)✓ Build complete$(RESET)                        $(CYAN)║$(RESET)"
	@echo "$(CYAN)╚══════════════════════════════════════════╝$(RESET)"
	@echo "$(YELLOW)→ Run:$(RESET) $(LIGHT_GRAY)cmake run$(RESET)"
	@echo ""

# ============================================================================
# VALIDATION
# ============================================================================

.PHONY: check-sources
check-sources:
	@if [ -z "$(SRCS)" ]; then \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)  ❌ No C++ files found                 $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo ""; \
		echo "$(YELLOW)Location:$(RESET) $(LIGHT_GRAY)$(WORKDIR)$(RESET)"; \
		echo "$(YELLOW)→ Try: $(CYAN)cmake help$(RESET)"; \
		exit 1; \
	fi

.PHONY: check-compiler
check-compiler:
	@if ! command -v $(CC) > /dev/null 2>&1; then \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)  ❌ Compiler not found                 $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo ""; \
		echo "$(YELLOW)Path:$(RESET) $(WHITE)$(CC)$(RESET)"; \
		echo "$(YELLOW)→ Try: module load gcc"; \
		exit 1; \
	fi

.PHONY: check-linter
check-linter:
	@if ! command -v $(LINTER) > /dev/null 2>&1; then \
		echo "$(WARN_BG)$(WARN_FG)  ⚠ Linter not found - skipping  $(RESET)"; \
	fi
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# STAGE 1/4: LINTING
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

.PHONY: lint
lint: check-sources check-linter
	@if command -v $(LINTER) > /dev/null 2>&1; then \
		FILE_COUNT=$(words $(ALL_FILES)); \
		if [ $$FILE_COUNT -eq 1 ]; then \
			FILE_WORD="file"; \
		else \
			FILE_WORD="files"; \
		fi; \
		echo ""; \
		echo "$(BRIGHT_MAGENTA)▐$(RESET) $(WHITE)[1/4]$(RESET) $(BRIGHT_MAGENTA)🔍 Linting$(RESET) $(LIGHT_GRAY)$(WORKDIR)$(RESET)"; \
		echo "$(BRIGHT_MAGENTA)▐$(RESET) $(LIGHT_GRAY)Checking $$FILE_COUNT $$FILE_WORD...$(RESET)"; \
		$(LINTER) $(LINTFLAGS) $(ALL_FILES) 2>&1 || true; \
		echo "$(BRIGHT_MAGENTA)▐ $(BRIGHT_MAGENTA)✓$(RESET) Linting of $$FILE_COUNT $$FILE_WORD complete"; \
		echo ""; \
	fi

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# STAGE 2/4: COMPILATION
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: compile
compile: check-sources check-compiler $(OBJS)
	@echo "$(BLUE)▐ $(BLUE)✓$(RESET) Compilation of $(words $(SRCS)) source files complete"
	@echo ""

.PHONY: compile-selected
compile-selected: $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(COMPILE_SRCS)))
	@echo "$(BLUE)▐ $(BLUE)✓$(RESET) Compilation complete"
	@echo ""

%.o: %.cpp $(HEADERS)
	@echo ""
	@echo "$(BLUE)▐$(RESET) $(WHITE)[2/4]$(RESET) $(BLUE)🔨 Compiling$(RESET) $(WHITE)$(notdir $<)$(RESET)"
	@$(CC) $(CCFLAGS) -c $< -o $@ > /tmp/compile_output.txt 2>&1; \
	COMPILE_STATUS=$$?; \
	cat /tmp/compile_output.txt; \
	if [ $$COMPILE_STATUS -eq 0 ]; then \
		echo "$(BLUE)▐ $(BLUE)✓$(RESET) $(notdir $<) $(GRAY)→$(RESET) $(notdir $@)"; \
		rm -f /tmp/compile_output.txt; \
	else \
		echo ""; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)  ❌ COMPILATION FAILED                 $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo ""; \
		echo "$(RED)▐$(RESET) $(WHITE)File:$(RESET) $(LIGHT_GRAY)$<$(RESET)"; \
		echo ""; \
		rm -f /tmp/compile_output.txt; \
		exit 1; \
	fi

%.o: %.cc $(HEADERS)
	@echo ""
	@echo "$(BLUE)▐$(RESET) $(WHITE)[2/4]$(RESET) $(BLUE)🔨 Compiling$(RESET) $(WHITE)$(notdir $<)$(RESET)"
	@$(CC) $(CCFLAGS) -c $< -o $@ > /tmp/compile_output.txt 2>&1; \
	COMPILE_STATUS=$$?; \
	cat /tmp/compile_output.txt; \
	if [ $$COMPILE_STATUS -eq 0 ]; then \
		echo "$(BLUE)▐ $(BLUE)✓$(RESET) $(notdir $<) $(GRAY)→$(RESET) $(notdir $@)"; \
		rm -f /tmp/compile_output.txt; \
	else \
		echo ""; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)  ❌ COMPILATION FAILED                 $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo ""; \
		echo "$(RED)▐$(BLUE)▐$(RESET) $(WHITE)File:$(RESET) $(LIGHT_GRAY)$<$(RESET)"; \
		echo ""; \
		rm -f /tmp/compile_output.txt; \
		exit 1; \
	fi

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# STAGE 3/4: LINKING
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

.PHONY: link
link: check-sources $(OBJS)
	@echo ""
	@echo "$(YELLOW)▐$(RESET) $(WHITE)[3/4]$(RESET) $(YELLOW)🔗 Linking$(RESET) $(WHITE)executable$(RESET)"
	@if [ -f /tmp/cmake_selected_main.txt ]; then \
		SELECTED=$$(cat /tmp/cmake_selected_main.txt); \
		LINK_OBJS=""; \
		for obj in $(OBJS); do \
			src=$${obj%.o}.cpp; \
			if [ ! -f $$src ]; then src=$${obj%.o}.cc; fi; \
			if [ "$$src" = "$$SELECTED" ] || ! grep -q "int main" $$src 2>/dev/null; then \
				LINK_OBJS="$$LINK_OBJS $$obj"; \
			fi; \
		done; \
		$(CC) $(CCFLAGS) $$LINK_OBJS $(LIBS) -o $(TARGET) > /tmp/link_output.txt 2>&1; \
	else \
		$(CC) $(CCFLAGS) $(OBJS) $(LIBS) -o $(TARGET) > /tmp/link_output.txt 2>&1; \
	fi; \
	LINK_STATUS=$$?; \
	cat /tmp/link_output.txt; \
	if [ $$LINK_STATUS -eq 0 ]; then \
		echo "$(YELLOW)▐ $(YELLOW)✓$(RESET) Linking of $(WHITE)$(notdir $(TARGET))$(RESET) complete"; \
		echo ""; \
		rm -f /tmp/link_output.txt; \
	else \
		echo ""; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)  ❌ LINKING FAILED                     $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo ""; \
		if grep -q "undefined reference" /tmp/link_output.txt; then \
			echo "$(YELLOW)→$(RESET) Check function definitions"; \
		fi; \
		echo ""; \
		rm -f /tmp/link_output.txt; \
		exit 1; \
	fi

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# STAGE 4/4: RUN
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

.PHONY: run
run: all
	@EXECUTABLES=$$(find $(WORKDIR) -maxdepth 1 -type f -executable 2>/dev/null | grep -v "\.o$$" | sort); \
	EXEC_COUNT=$$(echo "$$EXECUTABLES" | grep -c .); \
	if [ -z "$$EXECUTABLES" ]; then \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)  ❌ No executables found                $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo ""; \
		exit 1; \
	elif [ $$EXEC_COUNT -eq 1 ]; then \
		SELECTED="$$EXECUTABLES"; \
	else \
		echo ""; \
		echo "$(YELLOW)▐$(RESET) $(WHITE)Multiple executables found:$(RESET)"; \
		echo ""; \
		i=1; \
		echo "$$EXECUTABLES" | while read exe; do \
			echo "  $(CYAN)$$i)$(RESET) $$(basename $$exe)"; \
			i=$$((i + 1)); \
		done; \
		echo ""; \
		read -p "→ Select executable (1-$$EXEC_COUNT): " choice; \
		if [ $$choice -ge 1 ] && [ $$choice -le $$EXEC_COUNT ]; then \
			SELECTED=$$(echo "$$EXECUTABLES" | sed -n "$${choice}p"); \
		else \
			echo "$(RED)Invalid selection$(RESET)"; \
			exit 1; \
		fi; \
	fi; \
	echo "$(MAGENTA)▐$(RESET) $(WHITE)[4/4]$(RESET) $(MAGENTA)🚀 Running$(RESET) $(WHITE)$$(basename $$SELECTED)$(RESET)"; \
	echo ""; \
	cd $(WORKDIR) && $$SELECTED || { \
		echo ""; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)  ❌ Program crashed                    $(RESET)"; \
		echo "$(ERROR_BG)$(ERROR_FG)                                        $(RESET)"; \
		echo ""; \
		exit 1; \
	}; \
	echo ""; \
	echo "$(MAGENTA)▐ $(MAGENTA)✓$(RESET) Execution of $(WHITE)$$(basename $$SELECTED)$(RESET) complete"; \
	echo ""

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# UTILITIES
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

.PHONY: clean
clean:
	@echo "$(YELLOW)→ Cleaning build artifacts..."
	@rm -f $(OBJS) $(TARGET) /tmp/compile_output.txt /tmp/link_output.txt
	@echo "$(CYAN)✓$(RESET) Clean complete"

.PHONY: help
help:
	@echo ""
	@echo "$(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo "$(WHITE)  cmake$(RESET) $(LIGHT_GRAY)- Central Build Tool$(RESET)"
	@echo "$(CYAN)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo ""
	@echo "$(LIGHT_GRAY)Working:$(RESET) $(WORKDIR)"
	@echo ""
	@echo "$(MAGENTA)Sources ($(words $(SRCS))):$(RESET)"
	@if [ -n "$(SRCS)" ]; then \
		for file in $(SRCS); do echo "  $(CYAN)✓$(RESET) $$file"; done; \
	else \
		echo "  $(RED)✗$(RESET) No .cpp or .cc files"; \
	fi
	@echo ""
	@echo "$(MAGENTA)Headers ($(words $(HEADERS))):$(RESET)"
	@if [ -n "$(HEADERS)" ]; then \
		for file in $(HEADERS); do echo "  $(CYAN)✓$(RESET) $$file"; done; \
	else \
		echo "  $(LIGHT_GRAY)—$(RESET) No .h or .hpp files"; \
	fi
	@echo ""
	@echo "$(YELLOW)Commands:$(RESET)"
	@echo "  $(WHITE)cmake run$(RESET)    $(GRAY)Build & run$(RESET)"
	@echo "  $(WHITE)cmake all$(RESET)    $(GRAY)Build only$(RESET)"
	@echo "  $(WHITE)cmake clean$(RESET)  $(GRAY)Remove artifacts$(RESET)"
	@echo ""
