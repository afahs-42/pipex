# ================================
#           CONFIGURATION
# ================================

NAME        = pipex
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror
DEBUG_FLAGS = -g -DDEBUG -fsanitize=address
SRCDIR      = .
OBJDIR      = obj

# ================================
#             SOURCES
# ================================

SOURCES = main.c \
          utils.c \
          error.c

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
DEPS    = $(OBJECTS:.o=.d)

# ================================
#              RULES
# ================================

all: $(NAME)

# Main target
$(NAME): $(OBJECTS)
	@echo "🔗 Linking $(NAME)..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)
	@echo "✅ $(NAME) compiled successfully!"

# Object files with dependency tracking
$(OBJDIR)/%.o: %.c pipex.h | $(OBJDIR)
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Create object directory
$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@echo "📁 Created $(OBJDIR) directory"

# ================================
#            DEBUGGING
# ================================

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)
	@echo "🐛 Debug version compiled with AddressSanitizer"

valgrind: $(NAME)
	@echo "🔍 Running with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

# ================================
#             TESTING
# ================================

test: $(NAME)
	@echo "🧪 Running basic tests..."
	@./run_tests.sh

test-simple: $(NAME)
	@echo "🧪 Simple test..."
	@echo "hello world" > test_input.txt
	@./pipex test_input.txt "cat" "wc -w" test_output.txt
	@echo -n "Expected: 2, Got: "
	@cat test_output.txt
	@rm -f test_input.txt test_output.txt

test-comprehensive: $(NAME)
	@echo "🧪 Comprehensive testing..."
	@echo "Running test suite..."
	@bash -c 'for cmd in "cat|wc -l" "grep test|wc -w" "sort|uniq"; do \
		IFS="|" read -r cmd1 cmd2 <<< "$$cmd"; \
		echo "Testing: $$cmd1 | $$cmd2"; \
		echo -e "test\nline\ntest\nanother" | $$cmd1 | $$cmd2 > expected.txt; \
		echo -e "test\nline\ntest\nanother" > input.txt; \
		./pipex input.txt "$$cmd1" "$$cmd2" output.txt; \
		if diff expected.txt output.txt > /dev/null; then \
			echo "✅ PASS"; \
		else \
			echo "❌ FAIL"; \
		fi; \
		rm -f input.txt output.txt expected.txt; \
	done'

# ================================
#            BENCHMARKING
# ================================

benchmark: $(NAME)
	@echo "⏱️ Benchmarking..."
	@time ./pipex /dev/urandom "head -c 1000000" "wc -c" /dev/null
	@echo "Benchmark complete"

# ================================
#             CLEANUP
# ================================

clean:
	@echo "🧹 Cleaning object files..."
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "🧹 Cleaning executable..."
	@rm -f $(NAME)
	@rm -f test_*.txt *.tmp

re: fclean all

# ================================
#            UTILITIES
# ================================

# Show compilation commands (for debugging Makefile)
verbose: CFLAGS += -v
verbose: re

# Check for common issues
check:
	@echo "🔍 Checking for common issues..."
	@echo "Checking for forbidden functions..."
	@if grep -n "printf\|scanf" *.c; then \
		echo "❌ Found forbidden functions!"; \
	else \
		echo "✅ No forbidden functions found"; \
	fi
	@echo "Checking file permissions..."
	@ls -la $(NAME) 2>/dev/null || echo "No executable found"

# Install dependencies (if needed)
install-deps:
	@echo "📦 Installing dependencies..."
	@which valgrind > /dev/null || echo "Consider installing valgrind for memory checking"
	@which norminette > /dev/null || echo "Consider installing norminette for style checking"

# 42 specific: check norm
norm:
	@echo "📏 Checking 42 norm..."
	@norminette *.c *.h || echo "Norminette not installed"

# Help target
help:
	@echo "🚀 Pipex Makefile Help"
	@echo "======================"
	@echo "make          - Compile the project"
	@echo "make debug    - Compile with debug flags and AddressSanitizer"
	@echo "make test     - Run test suite"
	@echo "make clean    - Remove object files"
	@echo "make fclean   - Remove all generated files"
	@echo "make re       - Rebuild everything"
	@echo "make valgrind - Run with memory checker"
	@echo "make norm     - Check 42 coding standard"
	@echo "make help     - Show this help"

# ================================
#            ADVANCED FEATURES
# ================================

# Include dependency files
-include $(DEPS)

# Prevent make from deleting intermediate files
.PRECIOUS: $(OBJDIR)/%.o

# Declare phony targets
.PHONY: all clean fclean re debug test test-simple test-comprehensive \
        benchmark valgrind norm check install-deps help verbose

# Default goal
.DEFAULT_GOAL := all

# ================================
#              NOTES
# ================================
# 
# Advanced Features Explained:
# 
# 1. Automatic Dependency Tracking:
#    -MMD -MP flags generate .d files that track header dependencies
#    If you change a header, only affected .c files recompile
# 
# 2. Organized Build:
#    Object files go in obj/ directory for cleanliness
# 
# 3. Multiple Build Types:
#    - Normal: Optimized for submission
#    - Debug: With debug symbols and AddressSanitizer
#    - Verbose: Shows all compiler commands
# 
# 4. Testing Integration:
#    Multiple levels of testing built into the Makefile
# 
# 5. Tool Integration:
#    - Valgrind for memory checking
#    - Norminette for 42 style checking
#    - Time for benchmarking
# 
# 6. Error Prevention:
#    - Checks for forbidden functions
#    - Validates file permissions
#    - Provides helpful error messages
# 
# ================================