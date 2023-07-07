VENV = venv
PYTHON = $(VENV)/bin/python3
PIP = $(VENV)/bin/pip
ACTIVATE = $(VENV)/bin/activate

DEFINITIONBIN = app/definitions
MODELBIN = app/models
FIRMWARE = ../../Firmware/MaD_Firmware

HEADERS = $(FIRMWARE)/Librarys/JSON/JSON.h $(FIRMWARE)/include/Utility/Monitor.h $(FIRMWARE)/include/Utility/StateMachineDefinition.h $(FIRMWARE)/include/Main/Communication/CommunicationDefinition.h $(FIRMWARE)/include/Utility/Motion.h

DEFINITIONS := $(HEADERS:.h=.py)

.PHONY: run clean

run:
	$(PYTHON) MaD.py
	#. $(ACTIVATE); export FLASK_APP=MaD.py; flask run --host=0.0.0.0

venvc: clean $(VENV)/bin/activate

venv: $(VENV)/bin/activate
	$(PIP) install -r requirements.txt

$(VENV)/bin/activate: requirements.txt
	python3 -m venv $(VENV)
	$(PIP) install -r requirements.txt

clean:
	rm -rf $(VENV)
	find -iname "*.pyc" -delete
	rm -rf __pycache__