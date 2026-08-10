# Charon Automated Cross-Compilation Control Matrix
CXX      = x86_64-w64-mingw32-g++
CXXFLAGS = -std=c++17 -O3 -s -mwindows
LIBS     = -lws2_32
TARGET   = agent.exe
SOURCE   = agent.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "[*] Initializing cross-compilation toolchains for Windows platform targets..."
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LIBS)
	@echo "[++] Success: Core executable generated cleanly: $(TARGET)"

clean:
	@echo "[*] Flushing compilation workspace caches..."
	rm -f $(TARGET)
