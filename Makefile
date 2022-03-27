VPATH = .
CPPD = .
SOD = .

RDS := $(wildcard *.cpp)
CPPP := $(wildcard $(CPPD)/*.cpp)
SOS = $(RDS:.cpp=.so)
SOP = $(subst .cpp,.so,$(subst $(CPPD),$(SOD),$(CPPP)))
ODP = $(subst .cpp,.o.d,$(subst $(CPPD),$(SOD),$(CPPP)))

DMTOOL = ecell3-dmc

.PHONY: clean 
.PHONY: allclean

all: $(SOS)

src: $(SRCS)

$(SOS) : %.so : %.cpp
	$(DMTOOL) $<
	rm -f $(ODP) 

clean: 
	rm -f $(SOP) 
	rm -f $(ODP) 
