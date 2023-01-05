VENV = venv
PYTHON = $(VENV)/bin/python3
PIP = $(VENV)/bin/pip
ACTIVATE = $(VENV)/bin/activate

DEFINITIONBIN = app/definitions
MODELBIN = app/models
FIRMWARE = ../../Firmware/MaD_Firmware

HEADERS = $(FIRMWARE)/Librarys/JSON/JSON.h $(FIRMWARE)/include/Utility/MonitorDefinition.h $(FIRMWARE)/include/Utility/StateMachineDefinition.h $(FIRMWARE)/include/Main/Communication/CommunicationDefinition.h $(FIRMWARE)/Librarys/MotionPlanning/MotionPlanningDefinition.h

DEFINITIONS := $(addprefix $(DEFINITIONBIN)/,$(HEADERS:.h=.py))

$(VENV)/bin/activate: requirements.txt
	python3 -m venv $(VENV)
	$(PIP) install -r requirements.txt

.PHONY: run clean

run:
	$(PYTHON) MaD.py
	#. $(ACTIVATE); export FLASK_APP=MaD.py; flask run --host=0.0.0.0

clean:
	rm -rf $(VENV)
	find -iname "*.pyc" -delete
	rm -rf __pycache__
	
$(DEFINITIONBIN):
	mkdir -p $@

$(DEFINITIONBIN)/__init__.py: $(DEFINITIONBIN)
	touch $@

$(DEFINITIONBIN)/%.py: %.h $(DEFINITIONBIN)/__init__.py
	$(PYTHON) c2py.py $< -o $(DEFINITIONBIN)/$(@F)

convert: $(DEFINITIONS)

venvc: clean $(VENV)/bin/activate
venv: $(VENV)/bin/activate
	$(PIP) install -r requirements.txt

generate: clean venv convert dbmigrate