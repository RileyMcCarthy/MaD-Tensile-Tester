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

venv: clean $(VENV)/bin/activate

dbinit:
	. $(ACTIVATE); export FLASK_APP=MaD.py; flask db init
	. $(ACTIVATE); export FLASK_APP=MaD.py; flask db migrate
	. $(ACTIVATE); export FLASK_APP=MaD.py; flask db upgrade

dbmigrate:
	. $(ACTIVATE); export FLASK_APP=MaD.py; flask db migrate
	. $(ACTIVATE); export FLASK_APP=MaD.py; flask db upgrade
	
$(DEFINITIONBIN):
	mkdir -p $@

$(MODELBIN):
	mkdir -p $@

$(DEFINITIONBIN)/__init__.py: $(DEFINITIONBIN)
	touch $@

$(MODELBIN)/__init__.py: $(MODELBIN)
	touch $@

$(MODELBIN)/%.py: $(DEFINITIONBIN)/%.py $(MODELBIN)/__init__.py
	$(PYTHON) struct2model.py $(addprefix app.definitions.,$(@F:.py=)) -o $(MODELBIN)/$(@F)

$(DEFINITIONBIN)/%.py: %.h $(DEFINITIONBIN)/__init__.py
	$(PYTHON) c2py.py $< -o $(DEFINITIONBIN)/$(@F)

convert: $(DEFINITIONS)

venv: $(VENV)/bin/activate

generate: clean venv convert dbmigrate