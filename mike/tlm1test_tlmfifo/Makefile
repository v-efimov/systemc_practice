TARGET = out

IDIR = .
SDIR = .
ODIR = .

SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = $(SRC:$(SDIR)/%.c=$(ODIR)/%.o)

# CXX = g++
CXXFLAGS = -I$(IDIR)
CXXFLAGS += -g -O0
CXXFLAGS += -Iinclude
CXXFLAGS += -std=c++17
CFLAGS += -Wall
# SCPATH = /usr/local/systemc-2.3.3
SCPATH = $(SYSTEMC_HOME)

LIBS = -lm

# $(TARGET): $(OBJ)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(SCPATH)/include -L. -L$(SCPATH)/lib-linux64 -Wl,-rpath $(SCPATH)/lib-linux64 $^ $(LIBS) -o $@ -lsystemc

# Workable:
# $(TARGET): $(OBJ)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(SCPATH)/include -L. -Wl,-rpath $(SCPATH)/lib $^ $(LIBS)  $(SCPATH)/lib/libsystemc.dylib -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(SCPATH)/include  $^ $(LIBS)  $(SCPATH)/lib/libsystemc.dylib -o $@



$(ODIR)/%.o: $(SDIR)/%.c
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET)
